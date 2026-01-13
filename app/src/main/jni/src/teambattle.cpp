#include "stdafx.h"
#include "dragon.h"
#include "teambattle.h"
#include "menu.h"
#include "smallmenu.h"
#include "CharDataTable.h"
#include "smallmenuset.h"
#include "MenuSet.h"
#include "menunetwork.h"
#include "gameproc.h"
#include "hangul.h"
#include "hong_sprite.h"
#include "teambattle.h"
#include "char.h"
#include "effect.h"
#include "hong_sub.h"
#include "Convert565to555.h"
#include "item.h"
#include "TextOutMgr.h"
#include "QuestFunction.h"		// 021025 kyo
#include "ArenaManager.h"
#include "DualMgr.h"
CBattleZone			g_BattleZone;
bool				g_FightMapStart;
CFightMapCharInfo	g_FightMapCharInfo;
int fight_map_acc_num[4] = { 116, 108, 106, 118 };	// ����...

char FightTeam[5][25];
DWORD FightTeamColor[5] = { RED_COLOR, BLUE_COLOR, GREEN_COLOR, GRAY_COLOR, YELLOW_COLOR };

////////////// class battleteam :: member function /////////////////// 
CBattleTeam::CBattleTeam()
{
	ReSetClass();
}

void CBattleTeam::ReSetClass()
{
	count = 0;
	memset( member, 0, sizeof( k_team_member_info_unit ) * MAX_TEAM_MEMBER );
}

int CBattleTeam::AddMember( k_team_member_info_unit *info )		// ���� ����.
{
	if( count >= MAX_TEAM_MEMBER ) return -1;		// �� á��.

	for( int i=0; i<MAX_TEAM_MEMBER; i++ )
	{
		if( member[i].name[0] == NULL )
		{
			member[i] = *info;
		}
	}	
	count++;
	return 1;
}


int CBattleTeam::DeleteMember( char *name )
{
	for( int i=0; i< MAX_TEAM_MEMBER; i++ )
	{
		if( strcmp( name, member[i].name ) == 0 )
		{
			memset( &member[i], 0, sizeof( k_team_member_info_unit ) );
			count--;
			return 1;
		}
	}
	return 0;		// Ż���ų ���� ����.
}

//////////////////////////////////////////////////////////////////////
////////////// class battlezone :: member function /////////////////// 

CBattleZone::CBattleZone()
{
	ReSetClass();
}

void CBattleZone::ReSetClass()
{
	for( int i=0; i<MAX_TEAM; i++ )
	{
		m_team[i].ReSetClass();
	}
}

int CBattleZone::SetAllInfo( k_team_member_info_all *info )
{
	for( int i=0; i<MAX_TEAM; i++ )
	{
		for( int j=0; j<MAX_TEAM_MEMBER; j++ )
		{
			if( info->unit[i][j].lv )
				m_team[i].AddMember( &info->unit[i][j] );
		}
	}	
	return 1;
}

int CBattleZone::GetCount( int team )
{
	return m_team[team].count;
}

int CBattleZone::AddMember( int team, k_team_member_info_unit *info )
{
	return m_team[team].AddMember( info );
}


// �̹� ��ϵ� ����� �ִ°�?
int CBattleZone::IsExist( char *name )
{
	for( int i=0; i<MAX_TEAM; i++ )
	{
		int try_count = 0;
		for( int j=0; j<MAX_TEAM_MEMBER; j++ )
		{
			if( m_team[i].count >= try_count ) break;
			if( m_team[i].member[j].lv )
			{
				try_count ++;
				if( strcmp( name ,m_team[i].member[j].name ) == 0 )
				{
					return 1;
				}
			}
		}
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////


//////////////////////////// protocal function ////////////////////////
void RecvAllTeamInfo( t_packet *p )		// ��Ʋ���� ������ �޴´�.
{
	k_team_member_info_all *info = &p->u.kein.team_member_info_all;
	g_BattleZone.ReSetClass();		// �ʱ�ȭ ���Ŀ�
	g_BattleZone.SetAllInfo( info );	// ��� ������� ������ �����Ѵ�.
}

void SendAddMeOfTeamBattle( int team )		// ���� ����ϰڴ�.
{
	t_packet p;			// ������ ����
	p.h.header.type = CMD_ADD_TEAM_MEMBER;
		p.u.kein.default_char = team ;
	p.h.header.size = sizeof( char );
	QueuePacket( &p, 1 );	
}

void RecvAddMemberInfo( t_packet *p )		// ������ ���� �߰� �ƴ�.
{
	k_team_member_info_one *member_info = &p->u.kein.team_member_info_one;
	g_BattleZone.AddMember( member_info->team_num, &member_info->unit);
}

void StartTembattleWaitMenu(int nGame)
{	//< CSD-030520
	if (SMenu[MN_ARENA_LOBBY].bActive)
	{
		return;
	}
	
	g_pArenaManager->Reset();
	g_pArenaManager->SendEnterLobby(nGame);
	Kein_PutMessage(KM_INFO, kein_GetMenuString(154));
}	//> CSD-030520

///////////////////////////////////////////////////////////////////////
// CFightMapCharInfo member function
void CFightMapCharInfo::AddCharInfo( DWORD total_id, DWORD win_defeat, DWORD ladder, int level, int Class, int type, int dual_cls, int cls_step )
{
	CFightMapCharInfo *temp = this;
	CFightMapCharInfo *temp_prev;
	
	if( !temp->m_TotalId )		// ó���� �ƴϸ�
	{
		while( temp )
		{
			if( temp->m_TotalId == total_id )
			{
				memcpy( &temp->m_WinDefeat, &win_defeat, sizeof( DWORD ) );
				temp->m_dwLadderScore = ladder;
				temp->type = type;

        const string strName = g_mgrDual.GetClassName(Class, dual_cls, cls_step, level);
        strcpy(temp->m_szCallLevel, strName.c_str());
				temp->m_wColor= Lv_Color[level-1].GetRGB16();
				return;
			}
			else 
			{
				temp_prev = temp;
				temp = temp->next;
			}
		}
		temp = new CFightMapCharInfo;
		temp_prev->next = temp;
	}
	
	temp->m_TotalId = total_id;
	memcpy( &temp->m_WinDefeat, &win_defeat, sizeof( DWORD ) );
	temp->m_dwLadderScore = ladder;
  const string strName = g_mgrDual.GetClassName(Class, 
                                                Hero->aStepInfo[DUAL_CLS], 
                                                Hero->aStepInfo[CLS_STEP], 
                                                level);
  strcpy(temp->m_szCallLevel, strName.c_str());
	temp->m_wColor= Lv_Color[level-1].GetRGB16();
	temp->type = type;
	
	return;
}

CFightMapCharInfo *CFightMapCharInfo::GetFightMapCharInfo( DWORD total_id )// ���� ������ ������
{
	CFightMapCharInfo *temp = this;
	while( temp )
	{
		if( temp->m_TotalId == total_id ) return temp;
		temp = temp->next;
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////
//global functions
void SetChangeMapOfMenu()
{
	FighMapReset();
	CloseAllMenu();
	bCallGodMeetingMenu = 0;

	g_cQuest.DeleteScriptTimer();		// 021025 kyo ���̵��Ҷ�  Ÿ�̸�,ī���� �̾��� �����. (������ ���̵��� ������..
	g_cQuest.DeleteScriptCounter();		// 021025 kyo 
  g_cQuest.EmptySpellWord();			// 021206 kyo �α׾ƿ��Ҷ� �ֹ����� ���� 

}

void RecvFightMapAddOk( t_packet *p )
{
	int team_num = p->u.kein.default_char;
	if( team_num < 0 || team_num >= 4 ) return;
	AddCurrentStatusMessage( 220, 220, 0, lan->OutputMessage(3,42), FightTeam[team_num] ); //010215 lsw

	Hero->fight_map_team_num = team_num+1;
}

void RecvFightMapAddNot( t_packet *p )
{
	int team_num = p->u.kein.default_char;
	if( team_num < 0 || team_num >= 4 ) return;
	AddCurrentStatusMessage( 220, 0, 0, lan->OutputMessage(3,43), FightTeam[team_num] ); //010215 lsw
}

void RecvFightMapStart( )
{	//< CSD-030522
	if (g_pArenaManager->GetMemberTeam(Hero->id) || g_pArenaManager->GetObserveTeam(Hero->id))
	{
		SmallMenuClose();
		MainMenuClose();
		man_to_man_start = 85;
		g_FightMapStart = true;
	}

	AddCurrentStatusMessage( 255, 255, 255, lan->OutputMessage(3,46) ); //010215 lsw
	g_BattleStatusView = 0;

	ChangeEquipForFightMapStart();
}	//> CSD-030522

// 011018 KHS ���� ; �̰����� ���Ǹ� �־�� �ϴ°�? 
void ChangeEquipForFightMapStart()
{	//< CSD-030521
	CArenaManager::VECTOR_TEAM vtTeam = g_pArenaManager->GetTeamInfo();

	for (CArenaManager::ITOR_TEAM i = vtTeam.begin(); i != vtTeam.end(); ++i)
	{
		const int nTeam = (*i)->GetTeamNumber();

		CArenaTeam::LIST_MEMBER ltMember = (*i)->GetMemberSet();
		
		for (CArenaTeam::ITOR_MEMBER j = ltMember.begin(); j != ltMember.end(); ++j)
		{
			LPCHARACTER pMember = ::ReturnCharacterPoint(*j);
	
			if (pMember == NULL)
			{
				continue;
			}

			int equip[2];
			equip[AT_ARMOR] = fight_map_acc_num[nTeam];
			equip[AT_HELMET] = 1;
			
			if	(pMember->accessory[0] != equip[0])
			{ 
				FreeHeroClothAccessoryDataOne(pMember->sprno, pMember->accessory[0]);
				FreeHeroClothAccessoryDataOne(pMember->sprno, pMember->accessory[0] + 1);
			}
			
			pMember->accessory[0] = equip[AT_ARMOR];
			LoadHeroClothAccessoryDataOne(pMember->sprno, equip[AT_ARMOR]);
			LoadHeroClothAccessoryDataOne(pMember->sprno, equip[AT_ARMOR] + 1);

			if (pMember->accessory[1] != equip[AT_HELMET])
			{
				FreeHeroClothAccessoryDataOne(pMember->sprno, pMember->accessory[1]);
			}

			pMember->accessory[1] = equip[AT_HELMET];
			LoadHeroClothAccessoryDataOne(pMember->sprno, equip[AT_HELMET]);
		}
	}
}	//> CSD-030521

void FighMapReset()
{	//< CSD-030520
	g_BattleStatusView = 0;
	g_FightMapStart = false;
	
	LPCHARACTER ch = Hero;
	while( ch )
	{
		if( ch->fight_map_team_num )
		{
			ResetMagicFlag( ch );
		}
		ch->fight_map_team_num = 0;		
		ch = ch->lpNext;
	}

	memset( &fight_map_live_man, 0, sizeof( int )*4 );
	/*MenuSoundClose( MN_FIGHT_MAP_SIGN );
	SmallMenuClose();
	MainMenuClose();*/
	g_FightMapCharInfo.DeleteAll();		// ������ �����ϱ� ���ؼ�
}	//> CSD-030520

void RecvFightMapEnd( )
{	//< CSD-030522
	if (g_pArenaManager->GetMemberTeam(Hero->id) || g_pArenaManager->GetObserveTeam(Hero->id))
	{
		fight_map_stop = 85;
		if( g_fight_win ) g_fight_win = 1;
		if( g_fight_lose ) g_fight_lose = 1;
	}

	AddCurrentStatusMessage( 255, 255, 255,lan->OutputMessage(3,47) );
	g_pArenaManager->Reset();
	FighMapReset();
}   //> CSD-030522

void RecvFightMapCharInfo( t_packet *p )		// colossus ����� �� ����� ������ �����´�.
{
	DWORD	win_defeat = p->u.kein.fight_map_char_info.win_defeat;
	DWORD	ladderscore = p->u.kein.fight_map_char_info.ladderscore ;
	DWORD	total_id = p->u.kein.fight_map_char_info.total_id;
	int		level = p->u.kein.fight_map_char_info.level;
	int		cls = p->u.kein.fight_map_char_info.Class;
	int		dual_cls = p->u.kein.fight_map_char_info.dual_cls;
	int		cls_step = p->u.kein.fight_map_char_info.cls_step;

	g_FightMapCharInfo.AddCharInfo( total_id, win_defeat, ladderscore, level, cls, 1, dual_cls, cls_step );
}

////////////////////////////////////////////////////////////////////////
void ViewFightMapCharInfo( char *name, DWORD total_id, int x, int y )
{
	if(!name || !::strlen(name))
	{
		return;
	}
	CFightMapCharInfo *pCharInfo= g_FightMapCharInfo.GetFightMapCharInfo( total_id );
	if( !pCharInfo )
	{
		SendPacketDefault( CMD_FIGHT_MAP_CHAR_INFO, &total_id, sizeof( DWORD ) );
		g_FightMapCharInfo.AddCharInfo( total_id, 0, 0, 0, 0, 0, 0, 0 );		// �Ͻ������� ����Ѵ�.
		return;
	}

	const int iLG = TxtOut.GetLineGab();
	const int iBlankGabSize = TxtOut.GetBlankGabSize();
	const int iWidth = TXT_BG_SHORT_WIDTH;//021001 lsw	
	const int iX = x, iY = y;
	int iBlkG = 0;
	int iCt = 0;
	int iReultLineCount = 0;	

	SetHangulAlign( TA_LEFT);
	Hcolor( FONT_COLOR_NAME );

	TxtOut.Init(COLOR_BLACK,iWidth,9);
	TxtOut.PutTextHeaderBGImg(iX,iY+iBlkG+iLG*iCt,iWidth);
	

	iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlkG+iLG*iCt,iWidth,1,"%s", name);
	iCt += iReultLineCount;

	if( pCharInfo->type )		// �������� ���� �����Ÿ� �� �����ش�.
	{
		Hcolor( pCharInfo->m_wColor );
		iReultLineCount = TxtOut.RcTXTOutB(iX,iY+iBlkG+iLG*iCt,iWidth,1,"%s", pCharInfo->m_szCallLevel);
		iCt += iReultLineCount;

		Hcolor( FONT_COLOR_NUMBER );
		iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlkG+iLG*iCt,iWidth,1,"Score:    %5d", pCharInfo->m_dwLadderScore);
		iCt += iReultLineCount;


		iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlkG+iLG*iCt,iWidth,1,"Victory:  %5d", pCharInfo->m_WinDefeat.win);
		iCt += iReultLineCount;

		iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlkG+iLG*iCt,iWidth,1,"Defeat:   %5d", pCharInfo->m_WinDefeat.defeat);
		iCt += iReultLineCount;

		iReultLineCount = TxtOut.RcTXTOut(iX,iY+iBlkG+iLG*iCt,iWidth,1,"Draw:     %5d", pCharInfo->m_WinDefeat.disconnect);
		iCt += iReultLineCount;
	}
	TxtOut.PutTextTailBGImg(iX,iY+iBlkG+iLG*iCt,iWidth);
}

void RecvFightRemainTime( t_packet *p )
{
	int time_type = p->u.kein.default_char;
	g_RemainTimeType = time_type; 
	g_RemainTimeDelay = 80;
}
void ResetMagicFlag( LPCHARACTER ch )
{
	EndCastMagic( ch );

	if( ch->lostSightTime )
	{
		ch->sight = NORMAL_SIGHT;
		ch->lostSight = 0;
//		ch->lightness		= 0;//010530 lsw
		ch->lostSightTime = 0;
	}

	MAGICLIST	*next, *Temp;
	Temp = g_lpML;
	while( Temp )
	{
		next = Temp->Next;
		DeleteList( &g_lpML, Temp );
		Temp = next;
	}
}

//////////////////////////////////////////////////////////////////////////////
// ��� ����
///////////////////////////////////////////////////////////////////////////
// ��� �ý���
int g_save_load_flag;
std::unordered_map<int, CGuild> g_GuildMgr;  // CSD-030324

char *CallGuildDegreeName( int guild_num, int degree_num )
{	//< CSD-030324
	if( guild_num < 0 ) return NULL;
	if (degree_num > 0) degree_num--;
	else if (degree_num == 0) degree_num = 4;
	return g_GuildMgr[guild_num].CallGuildDegreeName( degree_num );
}	//> CSD-030324

char *GetGuildName( int guild_num )
{	//< CSD-030324
	if( guild_num < 0 ) return NULL;
	return g_GuildMgr[guild_num].m_szName;
}	//> CSD-030324

int GetGuildCode( LPCHARACTER ch )
{	//< CSD-030324
	return ch->GetGuildCode();
}	//> CSD-030324

// thai2 YGI
void SaveGuildInfoEx( )		// ��� ���� �߰� ����
{	//< CSD-030326
	if( !g_save_load_flag ) return;		// ������ �ٲ����� �־�߸� ���� �����Ѵ�.
	g_save_load_flag = 0;

	char filename[128];
	char filename2[128];
	sprintf( filename, "./data/guild_info_%02d.txt", g_SelectServerIndex +1 );
	
	FILE *fp, *fp2;
	fp = Fopen( filename, "wt" );
	if( !fp ) return;

	sprintf( filename2, "./data/mark_ex_%02d.dat",g_SelectServerIndex+1  );
	fp2 = Fopen( filename2, "wb" );
	if( !fp2 ) 
	{
		fclose( fp );
		return;
	}

	for( auto i =g_GuildMgr.begin(); i != g_GuildMgr.end(); i++ )
	{
          CGuild *curr_guild = &i->second;
          if (curr_guild->m_nCode) {
            fprintf(fp, "%d ,%s ,%s ,%s ,%s ,%s ,%s ,%d ,%d ,%d ,%d\n",
                    curr_guild->m_nCode, curr_guild->m_szName,
                    curr_guild->m_aDegree[0], curr_guild->m_aDegree[1],
                    curr_guild->m_aDegree[2], curr_guild->m_aDegree[3],
                    curr_guild->m_aDegree[4], curr_guild->first,
                    curr_guild->mark_num, curr_guild->make,
                    curr_guild->info_num);

            if (curr_guild->m_Spr_save.img) {
              fwrite(&curr_guild->m_nCode, sizeof(int), 1, fp2);
              fwrite(&curr_guild->m_Spr_save.size, sizeof(DWORD), 1, fp2);
              fwrite(&curr_guild->m_Spr_save.xl, sizeof(short int), 1, fp2);
              fwrite(&curr_guild->m_Spr_save.yl, sizeof(short int), 1, fp2);
              fwrite(curr_guild->m_Spr_save.img, sizeof(char),
                     curr_guild->m_Spr_save.size, fp2);
            }
          }
	}

	fclose( fp );
	fclose( fp2 );
}	//> CSD-030326

// thai2 YGI
int LoadGuildTableEx()
{	//< CSD-030324
	char filename[50];
	sprintf( filename, "./data/guild_info_%02d.txt", g_SelectServerIndex +1 );
	
	FILE *fp = Fopen( filename, "rt" );
	if( fp )
	{
		char temp[512];
		int count = 0;
		int code;
		int ct;
		while( fgets( temp, 512, fp ) != NULL )
		{
			if( temp == NULL|| temp[0] == ';'  || temp[0] == '#' ) continue;
			char *token;
			token = strtok( temp, ",\n" );
			if( !token ) continue;
			code = atoi( token );
			if( code < 0 ) continue;
			ct = g_GuildMgr[code].m_nCode = code;

			token = strtok( NULL, ",\n" );
			strcpy( g_GuildMgr[ct].m_szName, token );
			EatRearWhiteChar( g_GuildMgr[ct].m_szName );

			memset( g_GuildMgr[ct].m_aDegree, 0, sizeof( char)*20*5 );
			for( int i=0; i<5; i++ )
			{
				token = strtok( NULL, ",\n" );
				if( !token ) break;
				strcpy( g_GuildMgr[ct].m_aDegree[i], token );
				EatRearWhiteChar( g_GuildMgr[ct].m_aDegree[i] );
			}

			token = strtok( NULL, ",\n" );
			if( !token ) continue;
			code = atoi( token );
			g_GuildMgr[ct].first = code;

			token = strtok( NULL, ",\n" );
			if( !token ) continue;
			code = atoi( token );
			g_GuildMgr[ct].mark_num= code;

			token = strtok( NULL, ",\n" );
			if( !token ) continue;
			code = atoi( token );
			g_GuildMgr[ct].make = code;

			token = strtok( NULL, ",\n" );
			if( !token ) continue;
			code = atoi( token );
			g_GuildMgr[ct].info_num = code;
		}
		fclose( fp );
	}
	return 1;
}	//> CSD-030324

// thai2 YGI
void LoadGuildMarkEx()
{
    // [Android] 1. 路徑與檔名處理
    char filename[128];
    // 移除 ./，直接從 data/ 開始
    sprintf( filename, "data/mark_ex_%02d.dat", g_SelectServerIndex+1 );

    // [Android] 2. 開啟檔案
    SDL_RWops *fp = SDL_RWFromFile( filename, "rb" );
    if( !fp ) {
        // 建議加個 Log，方便排查缺檔
        SDL_Log("LoadGuildMarkEx: Not found %s", filename);
        return;
    }

    int guildCode;
    unsigned int sprSize; //用來替代 DWORD，確保是 4 bytes

    // [Android] 3. 迴圈控制
    // 利用 SDL_RWread 的回傳值判斷是否讀到結尾 (回傳 0 代表 EOF 或錯誤)
    while( SDL_RWread(fp, &guildCode, sizeof(int), 1) == 1 )
    {
        // 安全檢查：防止 guildCode 越界導致崩潰
        // 假設 MAX_GUILD 是您公會陣列的最大值
        // if (guildCode < 0 || guildCode >= MAX_GUILD) {
        //     SDL_Log("LoadGuildMarkEx Error: Invalid GuildCode %d", guildCode);
        //     continue; // 或 break
        // }

        CGuild *curr_guild = &g_GuildMgr[guildCode];

        // [Android] 4. 讀取 Spr Size
        // 讀取時使用 unsigned int (4 bytes)，避免 64-bit long 問題
        SDL_RWread(fp, &sprSize, sizeof(unsigned int), 1);
        curr_guild->m_Spr.size = sprSize; // 賦值回去

        // 讀取尺寸
        SDL_RWread(fp, &curr_guild->m_Spr.xl, sizeof(short int), 1);
        SDL_RWread(fp, &curr_guild->m_Spr.yl, sizeof(short int), 1);

        // 計算中心點
        curr_guild->m_Spr.ox = curr_guild->m_Spr.xl >> 1;
        curr_guild->m_Spr.oy = curr_guild->m_Spr.yl >> 1;

        // 分配記憶體
        // 建議檢查 size 是否合理，避免惡意檔案導致分配過大記憶體
        if (curr_guild->m_Spr.size > 0) {
            curr_guild->m_Spr.img = new char[curr_guild->m_Spr.size];

            // [Android] 5. 讀取圖片數據
            SDL_RWread(fp, curr_guild->m_Spr.img, 1, curr_guild->m_Spr.size);

            CopySprToSpr( &curr_guild->m_Spr_save, &curr_guild->m_Spr );
            convert565to555( &curr_guild->m_Spr ); //Todo finished
            g_SDLInfo.CreateTextureFromSingleSpr_CPP(filename, &curr_guild->m_Spr);
        }
        else {
            curr_guild->m_Spr.img = nullptr;
        }
    }

    // [Android] 6. 關閉檔案
    SDL_RWclose( fp );
}

////////////////////////////////////////////////////////////////////////////////
//class CGuildBasic
int CGuildBasic::InitData()
{
	m_bInfo = 0;
	m_nCode = 0;
	m_bActive = 1;
	memset( m_szName, 0, sizeof( char)*20 );
	memset( m_szGuldMasterName, 0, sizeof( char)*20*5 );
	memset( m_aDegree, 0, sizeof( char)*20*5 );
	return 1;
}

int CGuild::InitData()
{
	save_flag = 0; 
	m_szExplain = NULL; 
	m_Spr.img = NULL; 
	m_Spr_save.img = NULL;

	first = 0;
	mark_num = 0;
	make = 0;
	info_num = 0;


	return 1;
}

////////////////////////////////////////////////////////////////////////////////
// �ݷμ��� ���濡 ���� �Լ� ����
int GetTeamByStone( DWORD attr )
{
	if( GetAttr2( attr, IA2_NOTMOVE )  ) return -1;		// ���� ���� ����.
	return ( /*GetAttr2( attr, STONE_SCORE_2 )?2:0 +*/ GetAttr2( attr, STONE_SCORE_1 )?1:0 );
}

// �� ������ ���� �� �ִ°� ���°�?
bool IsPossibleAttackColossusStone( LPITEMGROUND item )
{
	if( !Hero->fight_map_team_num ) return false;		// ����� ������ �ؾ� �ϰ�
	if( Hero->fight_map_team_num == ( GetTeamByStone( item->attr[IATTR_ATTR] )+1 ) ) return false;	// ���� �ٸ� ��輮�̾� �ϸ�
	if( EquipItemAttr[WT_WEAPON].item_no ) return false;	// �ƹ��͵� �����ϰ� ������ �ȵȴ�.
	return true;
}

// ������ ���ȴ�. ��� ����� ������ ȣ��
void SendAttackColossusItem( LPITEMGROUND item )
{
	t_packet packet;
	packet.h.header.type = CMD_ATTACK_COLOSSUS_STONE;
	packet.u.kein.default_int = item->server_id;
	packet.h.header.size = sizeof( int );
	QueuePacket( &packet, 1 );	
}

// ���ȴ�. �Ӽ��� �ٲٶ�...
void SendAttackColossusItemResult( LPITEMGROUND item )
{

}
////////////////////////////////////////////////////////////////////////////////

WORD GetRGBByTeam( int team, int &r, int &g, int &b )
{
	WORD color=0; // 020620 YGI
	switch( team )
	{
		case 0 : r = 230; g = 40; b = 40; break;
		case 1 : r = 40; g = 40; b = 230; break;
		case 2 : r = 0; g = 0; b = 0; break;
		case 3 : r = 0; g = 0; b = 0; break;
	}
	return color;
}

//< soto-030313
bool	CheckGuardianGuildWear()// �þ��� ������� ����üũ �Ͽ� ��庹���� ���� ������.
{
	return true;
}
//> soto-030313
void SetArenaObserverTarget(const int iTeamType)
{	//< CSD-030520
COMMENT	::AddCurrentStatusMessage( 250, 100, 100, "Set Observer Target '%d'",iTeamType);
	g_pArenaManager->SetObserveTeam(iTeamType);
}	//> CSD-030520

int GetArenaObserverTarget()
{	//< CSD-030520
	return g_pArenaManager->GetObserveTeam();
}	//> CSD-030520	

void SendObserverTeamSelect()//���� �Ǿ��ִ� �� ������ �����ݴϴ�.
{	//< CSD-030520
COMMENT	::AddCurrentStatusMessage( 250, 100, 100, "Send Observer Target" );
	const int nTeam = GetArenaObserverTarget();
	g_pArenaManager->SendEnterObserve(nTeam);
}	//> CSD-030520

void SetArenaBettingTarget(const int iTeamType)
{	//< CSD-030520
COMMENT	::AddCurrentStatusMessage( 250, 100, 100, "Set Betting Target '%d'",iTeamType);
	if (g_pArenaManager->GetBattingMoney() > 0)
	{
		return;
	}

	g_pArenaManager->SetGamblingTeam(iTeamType);
}	//> CSD-030520

int GetArenaBettingTarget()
{	//< CSD-030521
	return g_pArenaManager->GetGamblingTeam(Hero->id);
}	//> CSD-030521

void SendMyArenaBettingMoney(const DWORD dwMoney)
{	//< CSD-030520
COMMENT	::AddCurrentStatusMessage( 250, 100, 100, "Send SendMyArenaBettingMoney" );
	const int nTeam = GetArenaBettingTarget();
	g_pArenaManager->SendEnterGambling(nTeam, dwMoney);
}	//> CSD-030520

DWORD GetTeamBettingMoney(const int iTeamType)//���� �ɸ� ��
{	//< CSD-030520 : ATI_RED_TEAM, ATI_BLUE_TEAM
	return g_pArenaManager->GetBattingMoney(iTeamType);
}	//> CSD-030520

float GetTeamWinRate(const int iTeamType)//�ش� ���� �·�
{	//< CSD-030520 : ATI_RED_TEAM, ATI_BLUE_TEAM
	return g_pArenaManager->GetTeamProbability(iTeamType);
}	//> CSD-030520

DWORD GetMyBettingMoney()//���ñ�
{	//< CSD-030520
	return g_pArenaManager->GetBattingMoney();
}	//> CSD-030520

DWORD GetMyBettingResult()//����
{	//< CSD-030520
	return g_pArenaManager->GetDividendMoney();
}	//> CSD-030520

DWORD GetAreanWaitTime()
{	//< CSD-030521
	return g_pArenaManager->GetReadyTime();
	//> CSD-030521
}

bool CanCallBettingMenu()
{
	// BBD 031214	�±����� �׺��ϸ� ū�ϳ���, ���� ���ϰ� �ϳ��δ� -_-;
#ifdef	THAI_LOCALIZING_
	return false;
#endif

	//< CSD-030522
	if (g_pArenaManager->IsReadyGame())
	{
		return false;
	}

	return true;
}	//> CSD-030522

bool CanCallObserverMenu()
{	//< CSD-030522
	if (g_pArenaManager->IsReadyGame())
	{
		return false;
	}

	if (!g_pArenaManager->IsEnableObserve(Hero->id))
	{
		return false;
	}

	return true;
}	//> CSD-030522