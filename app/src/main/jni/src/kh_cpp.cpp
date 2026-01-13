#include "stdafx.h"
#include "Hangul.h"
#include "dragon.h"
#include "SmallMenuSet.h"
#include "Menu.h"
#include "SmallMenu.h"
#include "Hong_Sub.h"
#include "Hong_Sprite.h"
#include "stdio.h"
#include "directsound.h"
#include "Char.h"
#include "Effect.h"
#include "map.h"
#include "tool.h"
#include "MenuSet.h"
#include "convert565to555.h"
#include "Math.h"
#include "MenuNetwork.h"
#include "GameProc.h"
#include "CharDataTable.h"
#include "object.h"
#include "Item.h"
#include "linecommand.h"
#include "word.h"
#include "kh_cpp.h"



//�⺻���� 
//Ŭ���� 
#define LOW			200
#define C_GENERAL	300
#define HIGH		400
#define SUPERIOR	500
#define C_GREAT		600
#define CRUSADER	700

//���� 
#define J_GENERAL	200
#define PRECISE		300
#define GOOD		400
#define TRUST		500
#define HONORABLE	600
#define J_GREAT		700

//�Լ� 
void	Func_Division(void);	//����¿����� Division ������ ���� �����Ͽ� ����Ű ����... 
//���� 
int		Class_Division;				//Ŭ���� ���ġ ����¿����� ���������� ���� 	
int		Job_Division;				//���� ���ġ ����¿����� ���������� ���� 	
int		QUEST_GIVE;
int		QUEST_COUNT;

int		quest_num[QUEST_MAX] ; 
//SCharacterData.nCharacterData[SPELL]
void IncreaseFamous( int class_plus, int job_plus )
{
	SCharacterData.class_famous += class_plus;
	//
	// Ŭ���� ����� ������� �� � ó���� �ִٸ�...
	//

	SCharacterData.job_famous += job_plus;
	Func_Division();
	//
	// ���� ����� ���������...
	//
}

void DecreaseFamous( int class_plus, int job_plus )
{
	SCharacterData.class_famous -= class_plus;
	if( SCharacterData.class_famous < 0 ) SCharacterData.class_famous = 0;
	//dik�϶����� ��, ó���ؾ��� �κ��� �ִٸ� ���⿡...
	//
	
	SCharacterData.job_famous -= job_plus;
	if( SCharacterData.job_famous < 0 ) SCharacterData.job_famous = 0;
	Func_Division();
}

void Func_Division(){
	//Ŭ���� ���ġ 
	if(SCharacterData.class_famous > 0 && SCharacterData.class_famous < LOW)					Class_Division = 1;
	else if(SCharacterData.class_famous > LOW && SCharacterData.class_famous < C_GENERAL)			Class_Division = 2;
	else if(SCharacterData.class_famous > C_GENERAL && SCharacterData.class_famous < HIGH)		Class_Division = 3;
	else if(SCharacterData.class_famous > HIGH && SCharacterData.class_famous < SUPERIOR)		Class_Division = 4;
	else if(SCharacterData.class_famous > SUPERIOR && SCharacterData.class_famous < C_GREAT)	Class_Division = 5;
	else if(SCharacterData.class_famous > C_GREAT && SCharacterData.class_famous < CRUSADER)	Class_Division = 6;
	//���� ���ġ 
	if(SCharacterData.job_famous > 0 && SCharacterData.job_famous < J_GENERAL)					Job_Division = 1;
	else if(SCharacterData.job_famous > J_GENERAL && SCharacterData.job_famous < PRECISE)			Job_Division = 2;
	else if(SCharacterData.job_famous > PRECISE && SCharacterData.job_famous < GOOD)			Job_Division = 3;
	else if(SCharacterData.job_famous > GOOD && SCharacterData.job_famous < TRUST)				Job_Division = 4;
	else if(SCharacterData.job_famous > TRUST && SCharacterData.job_famous < HONORABLE)			Job_Division = 5;
	else if(SCharacterData.job_famous > HONORABLE && SCharacterData.job_famous < J_GREAT)		Job_Division = 6;
	
}


QuestTable  quest_text[QUEST_MAX];

bool ReadAllText(const char* filename, std::string& outContent) {
    std::string path = filename;
    if (path.find("./") == 0) path = path.substr(2);

    SDL_RWops* fp = SDL_RWFromFile(path.c_str(), "rb");
    if (!fp) return false;

    Sint64 size = SDL_RWsize(fp);
    outContent.resize(size);
    SDL_RWread(fp, &outContent[0], size, 1);
    SDL_RWclose(fp);
    return true;
}

void LoadQuestTable(const char* filename, const char* filename2, const char* filename_bin)
{
    // 緩衝區 (保持原樣)
    // char text[1024]; // 在 C++ stringstream 中不需要這個 raw buffer，但為了邏輯對應先留著概念

    // 1. 嘗試讀取第一個文字檔
    std::string file1_content;
    if (ReadAllText(filename, file1_content))
    {
        // --- 進入文本解析與打包模式 (Text -> Bin) ---

        int count = 0;
        std::stringstream ss1(file1_content);
        std::string line;

        // 解析第一個檔
        while (std::getline(ss1, line)) {
            // 處理 Windows/Linux 換行符差異
            if (!line.empty() && line.back() == '\r') line.pop_back();

            if (line.empty()) continue;
            if (line[0] == '#' || line[0] == ';') continue;

            // 注意：LoadTable 接受 char*，我們傳入 c_str()
            // 如果 LoadTable 會修改字串內容，則需要拷貝到 temp buffer
            char temp_buf[1024];
            strncpy(temp_buf, line.c_str(), 1023);
            temp_buf[1023] = '\0';

            quest_text[count].LoadTable(temp_buf);
            count++;
        }

        // 解析第二個檔 (LoadPicture)
        count = 0;
        std::string file2_content;
        if (ReadAllText(filename2, file2_content)) {
            std::stringstream ss2(file2_content);
            while (std::getline(ss2, line)) {
                if (!line.empty() && line.back() == '\r') line.pop_back();
                if (line.empty()) continue;
                if (line[0] == '#' || line[0] == ';') continue;

                char temp_buf[1024];
                strncpy(temp_buf, line.c_str(), 1023);
                temp_buf[1023] = '\0';

                quest_text[count].LoadPicture(temp_buf);
                count++;
            }
        }

        // --- 寫入二進位檔 (.bin) ---
        // 注意：Android Assets 是唯讀的。
        // 如果 filename_bin 指向 assets (./xxx.bin)，這裡的 open 會失敗。
        // 但為了邏輯轉換，我們還是寫出來。

        std::string path_bin = filename_bin;
        if (path_bin.find("./") == 0) path_bin = path_bin.substr(2);

        SDL_RWops* fp_bin = SDL_RWFromFile(path_bin.c_str(), "wb");
        if (fp_bin)
        {
            char* tt = (char*)quest_text;
            char crc = 0;

            // ★修正原版邏輯錯誤★
            // 原代碼在 "wb" 模式下用了 fgets 讀取 fp，這是必死無疑的 Bug。
            // 這裡修正為：計算記憶體中 quest_text 的 CRC
            size_t total_size = sizeof(QuestTable) * QUEST_MAX;
            for(size_t i = 0; i < total_size; i++) {
                crc += tt[i];
            }

            SDL_RWwrite(fp_bin, &crc, 1, 1);
            SDL_RWwrite(fp_bin, quest_text, sizeof(QuestTable), QUEST_MAX);
            SDL_RWwrite(fp_bin, &crc, 1, 1); // 寫入兩次 CRC (依照原邏輯)

            SDL_RWclose(fp_bin);
            SDL_Log("Saved Quest Bin: %s", path_bin.c_str());
        }
        else {
            SDL_Log("Cannot write binary file (expected on Android Assets): %s", path_bin.c_str());
        }
    }
    else
    {
        // --- 進入二進位讀取模式 (Binary Mode) ---
        // 這是 Android 運行時最常進入的分支

        std::string path_bin = filename_bin;
        if (path_bin.find("./") == 0) path_bin = path_bin.substr(2);

        SDL_RWops* fp = SDL_RWFromFile(path_bin.c_str(), "rb");
        if (fp)
        {
            char crc1 = 0, crc2 = 0;

            // 讀取頭部 CRC
            SDL_RWread(fp, &crc1, 1, 1);

            // 讀取主要資料
            SDL_RWread(fp, quest_text, sizeof(QuestTable), QUEST_MAX);

            // 讀取尾部 CRC
            SDL_RWread(fp, &crc2, 1, 1);

            SDL_RWclose(fp);

            // 執行後處理 (原本的邏輯)
            for( int i=0; i<QUEST_MAX; i++ )
            {
                quest_text[i].SaveTable();
            }
            SDL_Log("Loaded Quest Bin: %s", path_bin.c_str());
        }
        else
        {
            SDL_Log("Error: Failed to load Quest data (%s)", path_bin.c_str());
            return;
        }
    }
    return;
}




// ����Ʈ ���̺��̶� �ֿ��� �Ǵµ� ���̺��� ������ �Ǿ����� �ʾƼ� 
// ���� �������� ������ �ҷ��´�. 
void QuestTable::LoadPicture( char *text )	{
	
	char *token = strtok(text,",\t\n");
	if( !token ) return;
	picnum = atoi(token);
		
	token= strtok(NULL, ",\t\n") ;
	if( !token ) return;
	strcpy( picture, token );
}



void QuestTable::SaveTable()
{
	return;
	FILE *fp;
	fp = fopen( "quest.txt", "at+" );
	if( !fp ) return;

	fprintf( fp, "%d	%d	%s	%d	%s	", num, code, name, place, ch );
	int i;
	for( i=0; i<QUEST_IF_COUNT; i++ )
	{
		fprintf(fp, "%d	", quest_if[i] );
	}
	for( i=0; i<QUEST_TAKE_COUNT; i++ )
	{
		fprintf(fp, "%d	", quest_take[i] );		
	}
	fprintf( fp, "\n" );
	fclose( fp );

	fp = fopen( "questpic.txt", "at+" );
	if( !fp ) return;

	fprintf( fp, "%d	%s\n", picnum, picture );
	fclose( fp );
	return;

}

void QuestTable::LoadTable( char *text )	// ���ڿ����� ����Ÿ�� �и�
{
	char *token = strtok(text,",\t\n");
	if( !token ) return;
	num = atoi( token );

	token= strtok(NULL, ",\t\n") ;
	if( !token ) return;
	code = atoi(token);
	
	token = strtok(NULL,",\t\n");
	if( !token ) return;
	strcpy( name, token );
	
	token= strtok(NULL, ",\t\n") ;
	if( !token ) return;
	place = atoi(token);
	
	token = strtok(NULL,",\t\n");
	if( !token ) return;
	strcpy( ch, token );
	
	int i;
	for(i = 0 ; i < QUEST_IF_COUNT; i++){
		token= strtok(NULL, ",\t\n") ;
		if( !token ) return;
		quest_if[i] = atoi(token);
	}

	for(i = 0 ; i < QUEST_TAKE_COUNT; i++){
		token= strtok(NULL, ",\t\n") ;
		if( !token ) return;
		quest_take[i] = atoi(token);
	}
	
	///////////////////////////
	// �׸� �߰�
}

//////////////////////////////////////////
// ó�� ���� �θ���
void kh_InitMenu()
{
	//LoadQuestTable("./quest/quest.txt");
}

int end_quest[200];

void SetEndQuest(int num )
{
	//for( int i=0; i<3; i++ )
		end_quest[num] = 1;
		return ;
}

void SetStartQuest(int num){
	QUEST_GIVE = rand()%4;
	quest_num[QUEST_COUNT++] = QUEST_GIVE;
	
	if(QUEST_COUNT > 200 ) QUEST_COUNT = 200;
	return;
}
//if( end_quest[quest_num] ) --> ������ ����Ʈ..

extern ItemAttr itemMallAttr[MAX_BUY_ITEM*3*5];
extern int itemMallMax;

void kh_KeyProc()
{
	/*if (g_aCurrentKeys[DIK_R] & 0x80)
	{
		itemMallMax = 0;
		for (int i = 0; i < 30; ++i)
		{
			itemMallAttr[i].item_no = 10275 + i;
			++itemMallMax;
		}
		CallSmallMenu(MN_ITEM_MALL);		// ����Ʈ �޴�
	}
	//if(g_aCurrentKeys[DIK_M] & 0x80)
	{
		if(!SMenu[MN_CHARACTER_COLOR_CHANGE].key){
			SMenu[MN_CHARACTER_COLOR_CHANGE].key = 1;		// ����Ʈ �޴�
		}
		else {
			SMenu[MN_CHARACTER_COLOR_CHANGE].key = 0;
		}
	}// ������������ 
	//FAME
	
	//if(g_aCurrentKeys[DIK_B] & 0x80)
	{
		
		//SetStartQuest(1);
		//CallSmallMenu(MN_FAME);		// ����Ʈ �޴�
		SendReqQuestIndex( 1 );
	}

	//if(g_aCurrentKeys[DIK_X] & 0x80 )
	{	
		kein_kh_key_on = true;
	}*/
}


void FreeQuest()
{
	for( int i=0; i<QUEST_MAX;i++ )
		if( quest_text[i].sq.img ) FreeSpriteOnlyOne( &quest_text[i].sq );
}

void QestDataFree( )
{
	memset( quest_num, 0, sizeof( int ) *QUEST_MAX );
	memset( end_quest, 0, sizeof( int ) *QUEST_MAX );
	QUEST_COUNT = 0;
}








//////////////////////////////////////1010 kkh �߰� //////////////////////////////////////
void CharacterWorkingWater( LPCHARACTER ch )
{
	//1010 kkh �߰� 
	static bool FIRSTONWATER = TRUE; // 1010 kkh �� �ѹ��� ��� ��� �׸� ���� 
	if( TileMap[ch->x / TILE_SIZE][ch->y / TILE_SIZE].attr_river == 1) {// ���࿡ ���ִ� ���� ���Ӽ��� ���̶��.. 
		
		static unsigned int tempCHtileX = 0,tempCHtileY = 0;
		if( tempCHtileX  > ch->x / TILE_SIZE || tempCHtileX < ch->x / TILE_SIZE \
			|| tempCHtileY > ch->y / TILE_SIZE || tempCHtileY < ch->y / TILE_SIZE){
			{ //
				if(abs((long)tempCHtileX - (ch->x / TILE_SIZE))+abs((long)tempCHtileY - (ch->y / TILE_SIZE)) >= 1){
					switch(ch->nCurrentAction){
					case ACTION_BASIC_WALK			:{
							InsertMagic(ch, ch, 490, 0, 0,0,0,0);
							InsertMagic(ch, ch, 491, 0, 0,0,0,0);
							PlayListAutoSounds( rand()%2+1148, 0, 0,0 );
							break;
													 }
					case ACTION_RUN					:{
							InsertMagic(ch, ch, 492, 0, 0,0,0,0);
							InsertMagic(ch, ch, 493, 0, 0,0,0,0);
							PlayListAutoSounds( rand()%2+1145, 0, 0,0 );
							break;
													 }
	 				
					}
					tempCHtileX = ch->x / TILE_SIZE;  // ���� ��ǥ�� ��� ���� ���´�.
					tempCHtileY = ch->y / TILE_SIZE;  // ������ ��� ������ �� �ϱ� ����
				}
				
			}	
				//FIRSTONWATER = TRUE;
		}
	}
	
	//////////////////////////////////////1010 kkh �߰� �� //////////////////////////////////////
}
						   



