#include "stdafx.h"
#include "stdio.h"
#include "Resource.h"
#include "SkillTool.h"
#include "Map.h"
#include "gameproc.h"
#include "Char.h"
#include "Tool.h"
#include "Hong_Sub.h"

#include "Effect.h"
#include "Hong_sprite.h"
#include "dragon.h"
#include "object.h"

HWND			InputSkillTableHwnd;
HWND			CheckHouseObjectHwnd;
//< CSD-030324	
char subFarmType[20][25] = {{""},};
char subMineType[20][25] = {{""},};
char subHubType[20][25] = {{""},};
//> CSD-030324
int             radio_Statue = 0;

MAPSKILLTABLE	g_MapSkillTable;
lpMAPSKILLTABLE Header[8][8];
MYHOUSETOOL		g_MyhouseTool;
DRAGMOUSE		g_DragMouse;
BOOL			g_MyhouseDlgOpen;

extern NPC_INFO g_infNpc[MAX_CHARACTER_SPRITE_]; // CSD-030419
extern ITEM_INFO g_itemref[MAX_ITEM_MUTANT_KIND]; // CSD-030419
//--------------------------------------------------------------------------------------------------------------
//  ��  �� :  NPC��ġ�� �����. 
//  ��  �� : 0 :��� NPC��ġ�� ���ش�. 
//           n : n�� NPC��ġ�� ���ش�. 
//  ��  �� : ����.
//--------------------------------------------------------------------------------------------------------------
//////////////////////// 0613 lkh �߰� /////////////////////////
void DeleteNPCGenerate(int npc_Num)
{
	for( int y=0; y<g_Map.file.wHeight; y++)
	{
		for(int x=0; x<g_Map.file.wWidth; x++)
		{
			lpMAPSKILLTABLE result;
			
			if(TileMap[x][y].attr_skill == 1)
			{
				result=FindSkill(&Header[(int)(x/(int)((g_Map.file.wWidth+7)/8))][(int)(y/(int)((g_Map.file.wHeight+7)/8))], x, y);
				
				if(result==NULL)
					TileMap[x][y].attr_skill = 0;
				else if(result!= NULL && result->skillno == 6)		// npc���� ����Ʈ ���� �������� ���
				{
					if(!npc_Num)
					{
						TileMap[x][y].attr_skill=0;
						DeleteSkill(&Header[(int)(x/(int)((g_Map.file.wWidth+7)/8))][(int)(y/(int)((g_Map.file.wHeight+7)/8))], result);
					}
					else if(result->type_Num == npc_Num)
					{
						TileMap[x][y].attr_skill=0;
						DeleteSkill(&Header[(int)(x/(int)((g_Map.file.wWidth+7)/8))][(int)(y/(int)((g_Map.file.wHeight+7)/8))], result);
					}
				}
			}
		}
	}
}




void ChangeNPCGenerate(int npc_from, int npc_to )
{
	for( int y=0; y<g_Map.file.wHeight; y++)
	{
		for(int x=0; x<g_Map.file.wWidth; x++)
		{
			lpMAPSKILLTABLE result;
			
			if(TileMap[x][y].attr_skill == 1)
			{
				result=FindSkill(&Header[(int)(x/(int)((g_Map.file.wWidth+7)/8))][(int)(y/(int)((g_Map.file.wHeight+7)/8))], x, y);
				
				if(result==NULL)
					TileMap[x][y].attr_skill = 0;
				else if(result!= NULL && result->skillno == 6)		// npc���� ����Ʈ ���� �������� ���
				{
					if( result->type_Num == npc_from )
					{
						result->type_Num = npc_to;
					}
				}
			}
		}
	}
}



void DeleteAllSkillData( void )
{
	for( int i = 0 ; i < 8 ; i ++ )
	{
		for( int j = 0 ; j < 8 ; j ++ )
		{
			while( Header[i][j] )
			{
				DeleteSkill( &Header[i][j], Header[i][j] );
			}
		}
	}
}

void DrawSkillBox(int mox, int moy)
{
	int sx=(mox-Mapx)/TILE_SIZE*32;
	int sy=(moy-Mapy)/TILE_SIZE*32;
	Box( sx+1, sy+1, sx+TILE_SIZE-1, sy+TILE_SIZE-1, RGB(29,184,12));
}

/**
 * @brief 【重構版】從 .skb 檔案讀取技能地圖資料。
 * 此版本修復了 sprintf 帶來的緩衝區溢位安全隱患。
 * @return 成功返回 TRUE，失敗返回 FALSE。
 */
bool LoadSkillMapTable_Refactored(void)
{
    char temp[FILENAME_MAX];
    snprintf(temp, sizeof(temp), "./skill/%s.skb", MapName);

    // [Android] 1. 路徑處理：移除開頭的 ./
    std::string path = temp;
    if (path.find("./") == 0) path = path.substr(2);

    // [Android] 2. 開啟檔案
    SDL_RWops* fp = SDL_RWFromFile(path.c_str(), "rb");
    if (!fp) {
        // 找不到檔案是正常的，回傳 false 或 true 視您的邏輯而定
        // SDL_Log("LoadSkillMapTable: Not Found %s", path.c_str());
        return false;
    }

    // [Android] 3. 取得檔案大小
    Sint64 fileSize = SDL_RWsize(fp);

    // 計算技能數量
    // ★注意★：這裡假設 MAPSKILLTABLE 在 Android 和 Windows 大小一致
    // 如果讀取錯位，請檢查 sizeof(MAPSKILLTABLE)
    size_t skill_count = fileSize / sizeof(MAPSKILLTABLE);

    // Debug Log: 確認結構體大小 (請對照 Windows 輸出)
    // SDL_Log("LoadSkillMapTable: FileSize=%lld, StructSize=%d, Count=%d",
    //         fileSize, (int)sizeof(MAPSKILLTABLE), (int)skill_count);

    if (skill_count == 0) {
        SDL_RWclose(fp);
        return true; // 空檔案
    }

    // [Android] 4. 建立緩衝區並一次讀取
    std::vector<MAPSKILLTABLE> skill_buffer(skill_count);

    // 參數: (context, ptr, object_size, object_count)
    size_t read_count = SDL_RWread(fp, skill_buffer.data(), sizeof(MAPSKILLTABLE), skill_count);

    // [Android] 5. 關閉檔案
    SDL_RWclose(fp);

    if (read_count != skill_count) {
        SDL_Log("LoadSkillMapTable Error: Expected %zu, Read %zu", skill_count, read_count);
        return false;
    }

    // 6. 遍歷並建立鏈結串列 (邏輯保持不變)
    int dx = (g_Map.file.wWidth + 7) / 8;
    int dy = (g_Map.file.wHeight + 7) / 8;

    for (const auto& st : skill_buffer)
    {
        // 安全檢查
        if (st.x >= 0 && st.x < g_Map.file.wWidth && st.y >= 0 && st.y < g_Map.file.wHeight) {
            int x = (int)(st.x / dx);
            int y = (int)(st.y / dy);

            // 注意：確保 TileMap 有被正確初始化
            TileMap[st.x][st.y].attr_skill = 1;

            // 加入鏈結串列
            // 這裡假設 AddSkill 內部會進行 deep copy (複製內容而非指標)
            AddSkill(&Header[x][y], const_cast<MAPSKILLTABLE*>(&st));
        }
    }

    return true;
}

/*bool LoadSkillMapTable(void)
{
	int		i=0;
	char	temp[FILENAME_MAX];
	FILE *fp ;
	MAPSKILLTABLE st;
	int c=0;
	int ret;
	
 	sprintf( temp, "./skill/%s.skb", MapName );
	fp = Fopen( temp, "rb" );
	if(fp == NULL) 		return FALSE;

	int dx = (g_Map.file.wWidth+7)/8;
	int dy = (g_Map.file.wHeight+7)/8;

	while( !feof( fp ) )
	{
		ret = fread( &st, sizeof( MAPSKILLTABLE), 1, fp );
		if( ret < 1 ) break;

		int x = (int)(st.x / dx);
		int y = (int)(st.y / dy);
		TileMap[ st.x ][ st.y ].attr_skill = 1;
		AddSkill( &Header[ x][y], &st );
		c++;
	}

	fclose(fp);

	return TRUE;
}
*/

void MapSkillTool_Attr( int mx, int my )		//�ش� Ÿ���� ��� �Ӽ��� Ȱ�� ���� �����Լ�
{
	if( mx < 0 ) return;
	if( my < 0 ) return;

	if( g_Map.file.wWidth <= mx ) return;
	if( g_Map.file.wHeight <= my ) return;
	
	LPTILE t = &TileMap[ mx][my];

	if( g_MapSkillTable.skillno!=0 )
	{
		if( t->attr_skill==FALSE )		//�ش� Ÿ���� ��� �Ӽ��� ���ʷ� �����ϴ� ���
		{
			t->attr_skill = 1;
			AddSkill( &Header[(int)(mx/(int)((g_Map.file.wWidth+7)/8))][(int)(my/(int)((g_Map.file.wHeight+7)/8))], &g_MapSkillTable);
		}
		else							//�ش� Ÿ���� ��� �Ӽ��� �̹� �Ӽ��� ���õǾ� �ִ� ���
		{
			lpMAPSKILLTABLE	result;
			result=FindSkill( &Header[(int)(mx/(int)((g_Map.file.wWidth+7)/8))][(int)(my/(int)((g_Map.file.wHeight+7)/8))], mx, my);
			if(result==NULL)		return;		//�׷� ���� �������� �ش���ǥ���� ���� ��ũ����Ʈ ���� ���
	
			result->x = g_MapSkillTable.x;
			result->y = g_MapSkillTable.y;
			result->type = g_MapSkillTable.type;
			result->skillno = g_MapSkillTable.skillno;

			if(result->skillno == TOOL_DONTSKILL )
			{
				result->tile_Range	= 0;
				result->probability	= 0;
				result->type_Num	= 0;
				result->subType		= 0;
			}
			else if(result->skillno == TOOL_BUILDHOUSE)
			{
				result->tile_Range	= g_MapSkillTable.tile_Range;
				result->probability	= 0;
				result->type_Num	= 0;
				result->subType		= 0;
			}
			else
			{
				result->tile_Range	= g_MapSkillTable.tile_Range;
				result->probability	= g_MapSkillTable.probability;
				if(result->skillno == TOOL_NPC_GENER )
				{
					result->type_Num= g_MapSkillTable.type_Num;
					result->subType = 0;
				}
				else
				{
					result->type_Num= 0;
					result->subType	= g_MapSkillTable.subType;
				}
			}
		}
	}
	return;
}


void AddSkill( lpMAPSKILLTABLE *Header, lpMAPSKILLTABLE	lpST )		//��ũ�� ����Ʈ�� �Ӹ��� �ٿ� �ֱ�
{		
	lpMAPSKILLTABLE t, temp; 
	
	if( *Header == NULL )		//��ũ�� ����Ʈ ���� ����
	{	
		MemAlloc( *Header, sizeof( MAPSKILLTABLE ));
		(*Header)->type			= lpST->type;
		(*Header)->x			= lpST->x; 
		(*Header)->y			= lpST->y; 
		(*Header)->skillno		= lpST->skillno;
		
		(*Header)->tile_Range	= lpST->tile_Range;
		(*Header)->probability	= lpST->probability;
		(*Header)->type_Num		= lpST->type_Num;
		(*Header)->subType		= lpST->subType;
		(*Header)->prev			= NULL;
		(*Header)->next			= NULL;
	}		
	else						//�̹� ������ ����Ʈ�� ÷�� ����/�ε��ؿ� �����Ͱ� ���� ���
	{	
		t = NULL;
		MemAlloc( t, sizeof( MAPSKILLTABLE ));

		t->type			= lpST->type;
		t->x			= lpST->x; 
		t->y			= lpST->y; 
		t->skillno		= lpST->skillno;
		
		t->tile_Range	= lpST->tile_Range;
		t->probability	= lpST->probability;
		t->type_Num		= lpST->type_Num;
		t->subType		= lpST->subType;

		temp = *Header ;
		temp->prev = t;
		t->next = temp;
		t->prev = NULL;
		*Header = t;
	}
	/*	
		if(lpST->skillno == TOOL_FARMING || lpST->skillno == TOOL_MINING || lpST->skillno == TOOL_FISHING ||
			lpST->skillno == TOOL_CHOPPING || lpST->skillno == TOOL_HUB )
		{
			t->tile_Range	= lpST->tile_Range;
			t->probability	= lpST->probability;
			t->type_Num		= 0;
			t->subType		= lpST->subType;
		}

		else if(lpST->skillno == TOOL_NPC_GENER )
		{
			t->tile_Range	= lpST->tile_Range;
			t->probability	= lpST->probability;
			t->type_Num		= lpST->type_Num;
			t->subType		= 0;
		}

		else if(lpST->skillno == TOOL_BUILDHOUSE )
		{
			t->tile_Range	= lpST->tile_Range;
			t->probability	= 0;
			t->type_Num		= 0;
			t->subType		= 0;
		}
		
		temp = *Header ;
		temp->prev = t;
		t->next = temp;
		t->prev = NULL;
		*Header = t;
	}	
	*/
}	
	
	
void DeleteSkill( lpMAPSKILLTABLE *Header, lpMAPSKILLTABLE f)
{	
	lpMAPSKILLTABLE  t = *Header;//g_lpMapSkillTable;
	
	while( t != NULL )
	{		
		if( t == f )
		{
			if( f == *Header )		//header�� ���ŵǴ� ���
			{
				t = (*Header)->next;

				if(*Header!=NULL)
					MemFree( *Header );

				if(t!=NULL)
				{
					*Header = t;
					(*Header)->prev = NULL;
				}
				return;
			}
			else 
			{
				if ( t->prev != NULL )
				{
					t->prev->next = t->next;
				}
				if( t->next != NULL )
				{
					t->next->prev = t->prev;
				}

				MemFree( t );
				return;
			}
		}

		t= t->next;
	}
}	
	
//	���� ������ǥ...
// 010314 KHS  
lpMAPSKILLTABLE FindSkill( lpMAPSKILLTABLE *H, int x, int y, int order )
{	
	int c = 0;
	lpMAPSKILLTABLE t;
	t = *H;
	while( t != NULL )
	{	
		if( t->x == x && t->y == y )
		{
			if( order == c ) return t;
			c++;
		}
		t = t->next;
	}		
	return NULL;
}
/*
void PutMyhouse(int x, int y)		// �� ���� x,y�� ���� Ÿ����ǥ��� �Ѿ�;� ��
{
	char			temp[FILENAME_MAX];
	static int		map_X, map_Y;
///	RECT			rect, grect;
	static bool		lButtonDown=false;
	int				a,b;
	FILE*			fp;
	TILE			temp_Tile;
	int				object_Count=0;
//	MYHOUSETOOL		Myhouse; //TileMap
	MAPOBJECT		temp_Object;

	sprintf( temp, "./map/%s.mhb", MapName );	//Ȯ����->My House Binery
	fp = Fopen( temp, "rb" );
	if(fp)
	{
		//////////////////////////////// ����κ� //////////////////////////////
		int length_X;//=g_MyhouseTool.ex-g_MyhouseTool.sx;	//����Ʈ�Ͽ� �ڸ� ����Ÿ���� ��Ÿ�� ����
		fread(&length_X, sizeof(int), 1, fp);
		int length_Y;//=g_MyhouseTool.ey-g_MyhouseTool.sy; //����Ʈ�Ͽ� �ڸ� ����Ÿ���� ȾŸ�� ����
		fread(&length_Y, sizeof(int), 1, fp);
		int tile_Num;//=length_X*length_Y;					//����Ʈ�� Ÿ���� �Ѱ���
		fread(&tile_Num, sizeof(int), 1, fp);
				
		fread(&object_Count, sizeof(int), 1, fp);		//����Ʈ�� Ÿ�� ���� ��ġ�� �� ������Ʈ�� 
		////////////////////////////////////////////////////////////////////////

		//////////////////////////// Ÿ���� ���� ���� //////////////////////////
		for( a=x; a<=x+length_X; a++)
			for( b=y; b<=y+length_Y; b++)
			{
				fread(&temp_Tile, sizeof(TILE), 1, fp);
				TileMap[a][b]=temp_Tile;
			}
		////////////////////////////////////////////////////////////////////////

		/////////////////////////// ������Ʈ�� ���� ���� ///////////////////////////
		for(int i = 0 ; i < object_Count ; i++ )
		{
			fread(&temp_Object, sizeof(MAPOBJECT), 1, fp);
			temp_Object.x += x;		//offset ��ǥ ������->32(TILE_SIZE)�� ���ؾ���
			temp_Object.x *= TILE_SIZE;
			temp_Object.y += y;
			temp_Object.y *= TILE_SIZE;
			//temp_Object.id += MapObjectLevel;			//�ش� ������ ������Ʈ ������ȣ �ɼ¸�ŭ ����(1000����)
			Mo[TotalMapObject+i]=temp_Object;
		}
	}
}*/
//------------------------------------------------






//--------------------------------------------------
		
void DeleteAllType(int delete_Type)
{
	lpMAPSKILLTABLE	temp;
	for( int i=0; i<8; i++)
	{
		for( int j=0; j<8; j++)
		{
			lpMAPSKILLTABLE temp_Table=Header[i][j];
			while(temp_Table!=NULL)
			{
				if(temp_Table->skillno == delete_Type)
				{
					TileMap[i][j].attr_skill=0;
					//temp_Skill = FindSkill(g_MapSkillTable, mx, my);
					temp=temp_Table->next;
					DeleteSkill( &Header[i][j], temp_Table);
					temp_Table = temp;
				}
				else temp_Table = temp_Table->next;

			}
		}
	}
}
