#include "stdafx.h"
#include "Hangul.h"
#include "dragon.h"
#include "SmallMenuSet.h"
#include "Menu.h"
#include "SmallMenu.h"
#include "Hong_Sub.h"
#include "Hong_Sprite.h"
#include "Char.h"
#include "MenuSet.h"
#include "MenuNetwork.h"
#include "object.h"
#include "linecommand.h"
#include "directsound.h"
#include "menudef2.h"
#include "menufunctions.h"
//021030 YGI
#include "gameproc.h"
#include "map.h"
#include "DualMgr.h"

extern LPCHARACTER Hero;
// ���ΰ� ĳ���Ͱ� ��带 ��û�� �� �ִ��� �Ǵ�
// type �� 1�̸� ���� ��� type �� 2 �̸� ���� ��� 
// ���н� ���� ��Ʈ�� ��ȣ �ݳ�
extern int CheckOtherCharacterGuild();
int CanRegistGuild( int type )
{	//< CSD-030324
	int nStr = -1;
	switch( type )
	{
		case 1:			// �ӽ� ��� ��� ���� üũ
			{
				nStr = 0;
				if( Hero->GetGuildCode() )  nStr = 50;		// �̹� ������ �ִ�.
				else if( Hero->lv < 40 ) nStr = 51;
				else if( GetItemMuchByMuchItem( IN_BLUE_SIGNET ) < 40 ) nStr = 52;
				else if( CheckOtherCharacterGuild() ) 
				{
					nStr = 50;
					Kein_PutMessage( KM_FAIL, kein_GetMenuString(129) ); 
				}
				break;
			}
		case 2 :		// ���� ��� ��� ���� üũ
			{
				nStr = 0;
				if( !Hero->GetGuildCode() ) nStr = 56;		// ���� ���� ��忡 �����ؾ� �Ѵ�.
				else if( g_GuildMgr[Hero->GetGuildCode()].first != 1 ) nStr = 50;	// �̹� �ٸ� ��忡 ������ �ִ�.
				else if( Hero->name_status.guild_master != 1 ) nStr = 57;  // ��� ������ �� ���� ��� ������ �� �� �ִ�.
				else if( GetItemMuchByMuchItem( IN_RED_SIGNET ) < 100 ) nStr = 52;
				break;
			}
	}

	return nStr;
}	//> CSD-030324

int CallDualInterFace(int type)
{
	for (int i = 0; i < 3; ++i)
	{
		const string strName = g_mgrDual.GetClassName(Hero->class_type,
														i, 
														Hero->aStepInfo[CLS_STEP] + 1,
														Hero->lv);
		strcpy(SMenu[MN_DUAL_MAIN].nField[i].temp, strName.c_str());
	}

	const string strNone = lan->OutputMessage(1, 38);

	switch (SCharacterData.nCharacterData[SPELL])
	{
	case WIZARD_SPELL: // ������ ������ ������ �ִ� ���
		{ 
			const string strName = g_mgrDual.GetClassName(Hero->class_type,
															WIZARD, 
															Hero->aStepInfo[CLS_STEP] + 1,
															Hero->lv);
			strcpy(SMenu[MN_DUAL_MAIN].nField[WIZARD].temp, strName.c_str());
			strcpy(SMenu[MN_DUAL_MAIN].nField[PRIEST].temp, strNone.c_str());
			break;
		}
	case PRIEST_SPELL: // ������ ������ ������ �ִ� ���
		{       
			const string strName = g_mgrDual.GetClassName(Hero->class_type,
															PRIEST, 
															Hero->aStepInfo[CLS_STEP] + 1,
															Hero->lv);
			strcpy(SMenu[MN_DUAL_MAIN].nField[PRIEST].temp, strName.c_str());
			strcpy(SMenu[MN_DUAL_MAIN].nField[WIZARD].temp, strNone.c_str());
			break;
		}
	}

	const string strExplain = g_mgrDual.GetDulaExplain(Hero->class_type, 0);

	strcpy(SMenu[MN_DUAL_MAIN].nField[6].temp,  strExplain.c_str());  

	SMenu[MN_DUAL_MAIN].work = 0;

	switch (type)
	{
	case MT_DUAL_CHANGE: // 1轉,轉職
		{
			SMenu[MN_DUAL_MAIN].nImageNumber = 2;
			SMenu[MN_DUAL_MAIN].nField[5].nRectImage = 7;
			SMenu[MN_DUAL_MAIN].nField[5].nImageNumber = 6;
			break;
		}
	case MT_DUAL_DIVIDE: // 2轉或以上
		{
			SMenu[MN_DUAL_MAIN].nImageNumber = 1;
			SMenu[MN_DUAL_MAIN].nField[5].nRectImage = 10;
			SMenu[MN_DUAL_MAIN].nField[5].nImageNumber = 9;
			SMenu[MN_DUAL_MAIN].work = Hero->aStepInfo[DUAL_CLS];//030611 lsw
			break;
		}
	case MT_DUAL_RESET_SYMBOL:
	{
		SMenu[MN_DUAL_MAIN].nImageNumber = 2;
		SMenu[MN_DUAL_MAIN].nField[5].nRectImage = 7;
		SMenu[MN_DUAL_MAIN].nField[5].nImageNumber = 6;
		break;
	}
	}

	SMenu[MN_DUAL_MAIN].nTemp = type;
	CallSmallMenu(MN_DUAL_MAIN);
	return 1;
}

void DoDualOKButton(int type, int select)
{
  MP3(SN_DUAL_SELECT);

	switch (type)
	{	//< CSD-021003
	case MT_DUAL_CHANGE: // ���	Ŭ������ ��ȯ
		{
			MP3(SN_DUAL_OK);
			g_mgrDual.SendDualChange(Hero, select);
			break;
		}
	case MT_DUAL_DIVIDE: // ��� �ܰ� ���
		{
			MP3(SN_DUAL_OK);
			g_mgrDual.SendDualDivide(Hero, select);
			break;
		}
	case MT_DUAL_RESET_SYMBOL:
	{
		MP3(SN_DUAL_OK);
		g_mgrDual.SendResetSymbol(Hero, select);
	}break;
	} //> CSD-021003
}

int CallInitAbility()
{
	CallSmallMenu(MN_INIT_ABILITY);
	return 1;
}

void DoInitAbility()
{
  MP3(SN_DUAL_OK);
  // ����Ƽ ��й�
  CallServer(CMD_RESET_ABILITY);
  // ����Ƽ �������̽� ȣ��
  CallMenu(MN_ABILITY);
//	CallServer(CMD_OPEN_ABILITY);	
}

void CheckGuildActive( int guild_code )
{	//< CSD-031001
	if( !guild_code ) return;
	LPCHARACTER ch = Hero;
	while( ch )
	{
		if( ch->GetGuildCode() == guild_code )
		{
			ch->SetGuildCode(0);
			ch->name_status.guild_master = 0;
		}
		ch = ch->lpNext;
	}
}	//> CSD-031001

bool CheckGuildCode( int guild_code )
{	//< CSD-030324
	if( !guild_code ) return 0;
	
	if( !g_GuildMgr[guild_code].m_bInfo ) 
	{
		SendReqGuildInfo1( guild_code );		// �ѹ� ��û�Ѵ�.
	}
	
	if( !g_GuildMgr[guild_code].m_bActive ) return 0;
	return 1;
}	//> CSD-030324

bool CheckGuildMaster( LPCHARACTER ch )
{	//< CSD-030324
	if( !ch ) return 0;
	if( ch->name_status.guild_master != 1 ) return 0;
	return CheckGuildCode( ch->GetGuildCode() );
}	//> CSD-030324

bool IsSubMaster( LPCHARACTER ch )
{	//< CSD-030324
	if( !ch ) return 0;
	int code = ch->GetGuildCode();
	if( !code ) return 0;
	return IsSubMaster( code, ch->name );
}	//> CSD-030324

bool IsSubMaster( int guild_code, char *name )
{	//< CSD-030324
	if( strcmp( g_GuildMgr[guild_code].m_szSubGuldMasterName, name ) == 0 ) return 1;
	return 0;
}	//> CSD-030324

int CallGuildChangeInfo()
{	//< CSD-030324
	if( !CheckGuildCode( Hero->GetGuildCode() ) ) 
	{
		CallOkCancelMessageBox( y_MenuFocus, 0, 0, kein_GetMenuString( 75 ), 0 );
		return 0;
	}

	//SmallMenuClose();

	// ���� �������� �̸��� �����´�.
	SendGetGuildMasterAndSubMasterName( Hero->GetGuildCode(), 2 );

	if( CheckGuildMaster( Hero ) )		// ��� ������ ���
	{
		SMenu[MN_GUILD_CHANGE_INFO].nField[1].nType=FT_PUT_BUTTON;
		SMenu[MN_GUILD_CHANGE_INFO].nField[2].nType=FT_PUT_BUTTON;		
		SMenu[MN_GUILD_CHANGE_INFO].nField[4].nType=FT_PUT_BUTTON;
		SMenu[MN_GUILD_CHANGE_INFO].nField[3].nType=FT_PUT_BUTTON;
		//SMenu[MN_GUILD_CHANGE_INFO].nField[5].nType=FT_NOMAL_PUT;		// �� �渶 ���� ����
		{
			SMenu[MN_GUILD_CHANGE_INFO].nField[5].nType=FT_PUT_BUTTON;
			SMenu[MN_GUILD_CHANGE_INFO].nField[5].nSpecialWillDo=SWD_SET_SUB_MASTER;
			SMenu[MN_GUILD_CHANGE_INFO].nField[5].nSHideNomalCount=6;
			SMenu[MN_GUILD_CHANGE_INFO].nField[5].nRectImage=7;
			SMenu[MN_GUILD_CHANGE_INFO].nField[5].nImageNumber=8;
			SMenu[MN_GUILD_CHANGE_INFO].nField[5].nImageType=IIT_GUILD_CHANGE_INFO;
		}

		if( g_GuildMgr[Hero->GetGuildCode()].first )
		{
			SMenu[MN_GUILD_CHANGE_INFO].nField[1].nType=FT_NOMAL_PUT;
			SMenu[MN_GUILD_CHANGE_INFO].nField[2].nType=FT_NOMAL_PUT;		
			SMenu[MN_GUILD_CHANGE_INFO].nField[4].nType=FT_NOMAL_PUT;
		}
	}
	else
	{
		SMenu[MN_GUILD_CHANGE_INFO].nField[1].nType=FT_NOMAL_PUT;
		SMenu[MN_GUILD_CHANGE_INFO].nField[2].nType=FT_NOMAL_PUT;
		SMenu[MN_GUILD_CHANGE_INFO].nField[3].nType=FT_NOMAL_PUT;
		SMenu[MN_GUILD_CHANGE_INFO].nField[4].nType=FT_NOMAL_PUT;
		//SMenu[MN_GUILD_CHANGE_INFO].nField[5].nType=FT_PUT_BUTTON;	// ��� Ż��
		{
			SMenu[MN_GUILD_CHANGE_INFO].nField[5].nSpecialWillDo=SWD_CALL_OKCANCEL_BOX;
			SMenu[MN_GUILD_CHANGE_INFO].nField[5].nSHideNomalCount=25;
			SMenu[MN_GUILD_CHANGE_INFO].nField[5].nRectImage=27;
			SMenu[MN_GUILD_CHANGE_INFO].nField[5].nImageNumber=26;
			SMenu[MN_GUILD_CHANGE_INFO].nField[5].nImageType=GUILD_MENU_IMG;
		}

	}

	SMenu[MN_GUILD_CHANGE_INFO].bActive = true;
	y_MenuFocus = MN_GUILD_CHANGE_INFO;
	return 1;
}	//> CSD-030324

/**
 * @brief 將 UTF-8 字串轉換為 UTF-16 (wchar_t) 字串。
 * * @param utf8_str 要轉換的 UTF-8 字串。
 * @param wide_str 輸出緩衝區 (UTF-16)。
 * @param wide_str_size 輸出緩衝區的大小 (以 wchar_t 為單位)。
 * @return 成功返回 0，失敗返回 -1。

int utf8_to_wide(const char* utf8_str, wchar_t* wide_str, size_t wide_str_size)
{
	// 檢查是否有足夠的空間 (包括終止符 \0)
	int result = MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, wide_str, (int)wide_str_size);

	if (result == 0)
	{
		// 轉換失敗或緩衝區不足
		// 確保至少清空緩衝區
		if (wide_str_size > 0) {
			wide_str[0] = L'\0';
		}
		return -1;
	}
	return 0;
}
*/

/**
 * @brief 構建完整的存檔路徑，並處理非法字元編碼。
 * 優化：直接使用已緩存的 g_GameInfo.configFilePath，避免重複調用 SDL_GetPrefPath。
 */
int GetEncodedFilePath(char* file, size_t file_size, const char* charname, const char* extension)
{
    // 1. 取得根路徑 (直接從全域變數讀取，假設已在初始化時設定好)
    // 注意：g_GameInfo.configFilePath 結尾通常帶有 '/' (例如: /data/.../files/)
    const char* root_path = g_GameInfo.RootPath.c_str();

    // 2. 確保目標資料夾存在 (例如 character 資料夾)
    char dir_path[MAX_PATH];

    // 組合出資料夾路徑: /data/.../files/character
    snprintf(dir_path, MAX_PATH, "%scharacter", root_path);

    // 在 Linux/Android 上建立目錄 (如果已存在會失敗並返回 -1，但不影響後續)
#ifdef _WIN32
    _mkdir(dir_path); // Windows
#else
    mkdir(dir_path, 0777); // Android / Linux
#endif

    // 3. 處理檔名編碼 (保持原邏輯)
    char encoded_charname[NM_LENGTH * 4] = "";
    size_t encoded_len = 0;
    const char* illegal_chars = "\\/:*?\"<>|";

    if (charname && charname[0] != '\0') {
        for (int i = 0; charname[i] != '\0'; i++) {
            unsigned char current_byte = (unsigned char)charname[i];
            int is_illegal = 0;
            for (const char* p = illegal_chars; *p != '\0'; p++) {
                if (current_byte == (unsigned char)*p) {
                    is_illegal = 1;
                    break;
                }
            }

            if (is_illegal) {
                int bytes_written = snprintf(encoded_charname + encoded_len, sizeof(encoded_charname) - encoded_len, "%%%02X", (unsigned int)current_byte);
                // 檢查緩衝區溢位
                if (bytes_written < 0 || (size_t)bytes_written >= (sizeof(encoded_charname) - encoded_len)) {
                    return -1;
                }
                encoded_len += bytes_written;
            } else {
                if (encoded_len + 1 >= sizeof(encoded_charname)) {
                    return -1;
                }
                encoded_charname[encoded_len++] = charname[i];
            }
        }
        encoded_charname[encoded_len] = '\0';
    }

    // 4. 組合最終路徑
    // 我們可以直接利用上面算好的 dir_path 來接檔名
    // 路徑變成: /data/.../files/character/User%2Aname.qm2
    int total_len;
    if (charname && charname[0] != '\0') {
        // 注意這裡加了 '/'，因為 dir_path 通常不帶結尾斜線 (除非 root_path 本身就是 character/)
        total_len = snprintf(file, file_size, "%s/%s%s", dir_path, encoded_charname, extension);
    } else {
        // 用於沒有角色名的情況 (例如 menu.de)
        total_len = snprintf(file, file_size, "%s/menu%s", dir_path, extension);
    }

    // 檢查最終路徑是否過長
    if (total_len < 0 || (size_t)total_len >= file_size) {
        return -1;
    }

    return 0;
}
void SaveMemorize()
{
    FILE* fp = NULL;
    char file_utf8[MAX_PATH];

    // --- 1. 儲存角色記憶資料 (.qm2) ---

    // 1.1. 獲取完整的 UTF-8 路徑
    if (GetEncodedFilePath(file_utf8, MAX_PATH, SCharacterData.sCharacterName, ".qm2") != 0) {
        SDL_Log("Error: Failed to generate file path for SaveMemorize.");
        return;
    }

    // 1.2. Android 直接使用 fopen (UTF-8 路徑)
    // 注意：不再需要 utf8_to_wide 和 _wfopen
    fp = fopen(file_utf8, "wb");

    if (!fp) {
        // 印出錯誤碼以便除錯 (例如 Permission denied 或 No such file)
        SDL_Log("Error: Could not open file for writing: %s (errno: %d)", file_utf8, errno);
        return;
    }

    // 寫入資料 (保持不變)
    fwrite(&magic_plus_start, sizeof(int), 1, fp);
    fwrite(&magic_plus_count, sizeof(int), 1, fp);
    fwrite(magic_plus, sizeof(int), MAX_MAGIC, fp);
    fwrite(SaveMemory, sizeof(t_QuickMemory), MAX_MAGIC_MEMORY, fp);
    fwrite(SaveArmor, sizeof(SaveArmor), 1, fp);
    fwrite(EquipSaveFlag, sizeof(EquipSaveFlag), 1, fp);

    fclose(fp);
    fp = NULL;


    // --- 2. 儲存選單狀態 (menu.de) ---

    // 使用相同的邏輯獲取 menu.de 的路徑
    if (GetEncodedFilePath(file_utf8, MAX_PATH, NULL, ".de") == 0) {
        fp = fopen(file_utf8, "wb");
    }

    if (fp)
    {
        fwrite(&SMenu, sizeof(SMenu), 1, fp);
        fclose(fp);
    }
}

void LoadMemorize()
{
    FILE* fp = NULL;
    char file_utf8[MAX_PATH];

    // --- 載入角色記憶資料 (.qm2) ---

    // 1. 獲取路徑 (必須與 Save 使用相同的邏輯)
    if (GetEncodedFilePath(file_utf8, MAX_PATH, SCharacterData.sCharacterName, ".qm2") != 0) {
        return;
    }

    // 2. 使用 fopen 開啟 (UTF-8)
    fp = fopen(file_utf8, "rb");

    if (!fp) {
        // 檔案不存在是正常的
        return;
    }

    // 讀取邏輯 (保持不變)
    int start, plus_count;

    if (SCharacterData.nCharacterData[SPELL] == WIZARD_SPELL)
    {
        fread(&magic_plus_start, sizeof(int), 1, fp);
        fread(&magic_plus_count, sizeof(int), 1, fp);
        fread(magic_plus, sizeof(int), MAX_MAGIC, fp);
    }
    else
    {
        fread(&start, sizeof(int), 1, fp);
        fread(&plus_count, sizeof(int), 1, fp);

        // 簡易校驗，防止資料錯亂
        if (magic_plus_count != plus_count) {
            fseek(fp, sizeof(int) * MAX_MAGIC, SEEK_CUR);
        }
        else
        {
            magic_plus_start = start;
            magic_plus_count = plus_count;
            fread(magic_plus, sizeof(int), MAX_MAGIC, fp);
        }
    }

    fread(SaveMemory, sizeof(t_QuickMemory), MAX_MAGIC_MEMORY, fp);
    fread(SaveArmor, sizeof(SaveArmor), 1, fp);
    fread(EquipSaveFlag, sizeof(EquipSaveFlag), 1, fp);
    fclose(fp);
}

bool IsReporterMode()
{
	if( Hero && Hero->name_status.reporter && Hero->reporter_mode ) return true;
	else return false;
}

void CallGuildMemberListMenu()
{	//< CSD-030324
	short int guild_code = Hero->GetGuildCode();
	
	if (!guild_code) 
	{
		Kein_PutMessage( KM_FAIL, kein_GetMenuString( 94 ) );
		return;
	}

	SendGetGuildMasterAndSubMasterName( guild_code, 3 );
	SendDirectDB( CMD_GET_GUILD_NOTICE, &guild_code, sizeof( short int ) );
	CallSmallMenu( MN_GUILD_MEMBER_LIST );

	static int pos_to_degree[] = {1, 2, 3, 4, 0 };
	int pos = SMenu[MN_GUILD_MEMBER_LIST].key;
	if( !g_Menu_Variable.m_GuildMemberName.GetRecv( pos_to_degree[pos] ) )
	{
		// ���� ��û
		k_get_guild_member_list_req data;
		data.guild_code = Hero->GetGuildCode();
		data.degree = pos_to_degree[pos];
		SendDirectDB( CMD_GET_NAME_GUILD_MEMBER_LIST, &data, sizeof( k_get_guild_member_list_req ) );
		g_Menu_Variable.m_GuildMemberName.SetRecv( pos_to_degree[pos] );
	}
}	//> CSD-030324

void SendGetGuildMasterAndSubMasterName(int guild_code, int type)
{	// 1:�渶��, 2:�α渶��, 3:���
	k_req_guild_master_and_sub_master data;
	data.guild_code = guild_code;
	data.type = type;
	SendDirectDB(CMD_GUILD_MASTER_AND_SUBMASTER, &data, sizeof(k_req_guild_master_and_sub_master) - 40);
}

void GetFriendGroup()
{
	if( g_Menu_Variable.m_FriendGrup )
	{
		g_Menu_Variable.m_FriendGrup->clear();
	}
	else g_Menu_Variable.m_FriendGrup = new CFriendGrup;

	SendDirectDB( CMD_GET_FRIEND, Hero->name, strlen( Hero->name )+1 );
}

int CallFriendMenu()
{
	GetFriendGroup();

	CallSmallMenu(MN_FRIEND_MENU);
	return 1; 
}

void ClickCharacter()		//  Ŭ�� 
{
	if( SMenu[MN_FRIEND_REGIST].bActive )
	{
		//EWndMgr.SetTxt(HWND_3,g_GameInfo.lpcharacter->name);//021001 lsw
	}
}

void SendSelectPartyExp( char select )
{
	SendPacketDefault( CMD_CHANGE_PARTY_SELECT_EXP, &select, 1 );
	switch( select )
	{
		case PS_EXP :
			Kein_PutMessage( KM_INFO, kein_GetMenuString( 125 ), kein_GetMenuString( 122 ) );
			break;

		case PS_TACTIC :
			Kein_PutMessage( KM_INFO, kein_GetMenuString( 125 ), kein_GetMenuString( 123 ) );
			break;

		case PS_MAGIC_TAC :
			Kein_PutMessage( KM_INFO, kein_GetMenuString( 125 ), kein_GetMenuString( 124 ) );
			break;
	}
}

void CallGetGuildItem()
{
	short int guild_code = Hero->GetGuildCode(); // CSD-030324

	if( !guild_code ) 
	{
		Kein_PutMessage( KM_FAIL, kein_GetMenuString( 75 ) );
		return;
	}
	// if( !Hero->name_status.guild_master ) return false; 	// ��޿� ���� ����

	Kein_PutMessage( KM_OK, kein_GetMenuString( 144 ) );
	if( g_Menu_Variable.m_pGuildItem )
	{
		delete g_Menu_Variable.m_pGuildItem ;
		g_Menu_Variable.m_pGuildItem = 0;
	}

	CloseAllMenu();
	
	SendDirectDB( CMD_GET_GUILD_ITEM_FIRST, &guild_code, sizeof( short int ) );
	SendDirectDB( CMD_GET_GUILD_BOX_MONEY, &guild_code, sizeof( short int ) );

	CallMenu( MN_GUILD_ITEM_GET);
	CallMenu( MN_ITEM );

	return;
}

void CallGuildItemPut()
{	//< CSD-030324
	CloseAllMenu();
	CallMenu( MN_ITEM );
	CallMenu( MN_GUILD_ITEM_PUT);
	
	short int guild_code = Hero->GetGuildCode();
	SendDirectDB( CMD_GET_GUILD_ITEM_COUNT, &guild_code, sizeof( short int ) );		// ������ ���� ����
}	//> CSD-030324

int GetMoneyItemNumber()
{
#ifdef CHANGE_MONEY_
	return IN_NEW_COINS;
	
#else
	return IN_COINS;
#endif	
}

// ��� ������ ���� ��ư�� ��������, �޽��� �ڽ� ���� �� �ؾ� ����
void CallChangeMaster()
{	//< CSD-030324
	if( CheckGuildMaster( Hero ) )		// �渶�� ��ư�� ��������
	{
		SendGetGuildMasterAndSubMasterName( Hero->GetGuildCode() , 2 );		// ���� ������ �̸� ��������
		strcpy( SMenu[MN_GUILD_CHANGE_INFO].nField[4].temp, kein_GetMenuString( 137 ) );
	}
	else		// �� �渶�� ��ư�� ��������
	{
		strcpy( SMenu[MN_GUILD_CHANGE_INFO].nField[4].temp, kein_GetMenuString( 139 ) );
	}
}	//> CSD-030324

void CallHtmlMessageBox( int next_menu, char *file_name )
{
	if( is_mainmenu_active ) CallSmallMenu( MN_HTML_MESSAGEBOX );
	else 
	{
		SMenu[MN_HTML_MESSAGEBOX].bActive = true;
		y_MenuFocus = MN_HTML_MESSAGEBOX;
	}
	
	int line = g_Menu_Variable.LoadingGuildRegistExplain( file_name );

	WORD page =0;
	WORD page_max = line/5+1;
	SMenu[MN_HTML_MESSAGEBOX].work = MAKELONG( page, page_max );
	SMenu[MN_HTML_MESSAGEBOX].key = line;

	SMenu[MN_HTML_MESSAGEBOX].nTemp = next_menu;
}

int GetDegree2Param( int degree )		// degree ���� �Ķ���ͷ� �����Ѵ�.
{
	static int degree2param[] = { 4, 0, 1, 2, 3 };
	return degree2param[degree];
}

// 020620 YGI
bool CheckGuildPower( int type, LPCHARACTER ch )
{	//< CSD-030324
	if( !ch ) return 0;
	if( !ch->GetGuildCode() ) return 0;
	if( type < 0 || type > GDP_MAX ) return 0;
	static bool power_table[][5] = {
		{ 1, 1, 0, 0, 0 },		// ��� ����				// GDP_JOINING
		{ 1, 1, 0, 0, 0 },		// ��� ��ü �޽���			// GDP_BBS
		{ 1, 0, 0, 0, 0 },		// ���� ��å����			// GDP_CHANGE_DEGREE		// 020529 acer
		{ 1, 1, 0, 0, 0 },		// ��� â�� �̿�			// GDP_GUILD_BOX
		{ 1, 1, 0, 0, 0 },		// ���� ��� Ż�� ��Ű��	// GDP_EXIT_GUILD_HIM
		{ 1, 0, 0, 0, 0 },		// ��� ��ũ ����			// GDP_CHANGE_MARK
		{ 1, 0, 0, 0, 0 },		// ��� ��å �� ����		// GDP_CHANGE_DEGREE_INFO
		{ 1, 0, 0, 0, 0 },		// ��� ������ ����			// GDP_GUILD_CHANGE_MASTER
		{ 1, 0, 0, 0, 0 },		// ��� ����				// GDP_GUILD_BREAK
		{ 1, 0, 0, 0, 0 },		// �α渶 ���				// GDP_REGIST_SUB_MASTER
		{ 0, 1, 1, 1, 1 },		// ������ ��� Ż��			// GDP_EXIT_GUILD_SELF
		{ 1, 0, 0, 0, 0 },		// �������� ��� ����		// GDP_BUY_GUILD_HOUSE
	};

	int para = GetDegree2Param( ch->name_status.guild_master );
	return power_table[type][para];
}	//> CSD-030324

void LoadFileText( char *file_name )
{
	FILE *fp = Fopen( file_name, "rt" );
	if( !fp ) return;

	/*
	if( g_Menu_Variable.m_szFileTextPut ) 
	{
		delete [] g_Menu_Variable.m_szFileTextPut;
		g_Menu_Variable.m_szFileTextPut = 0;
	}
	*/

	char temp[1024];
	while( fgets( temp, 1024, fp ) )
	{
		if( CheckContinueLine( temp[0] ) ) continue;
		g_Menu_Variable.m_szFileTextPut += temp;
		g_Menu_Variable.m_szFileTextPut += "\n";
	}

	fclose( fp );
}

void CallNationItem()
{
	CallServer( CMD_GET_NATION_ITEM );
	CallSmallMenu( MN_NATION_ITEM );
}

void CallGuildHouseInfo()
{
	char nation = Hero->name_status.nation;
	CallServer( CMD_GUILD_HOUSE_INFO );
	//SendDirectDB( CMD_GUILD_HOUSE_INFO, &nation, 1 );
}

char *GetIsGotGuildHouse( int time )		// �ð��� ������ ���� ������� �Ǵ�.
{
	int mon, day, hour;
	GetRealTime( mon, day, hour );
	int str = 0;
	if( !time )
	{
		str = 166; // ����
	}
	else if( time > 30 && day >= 25 )
	{
		str = 164; // ����
	}
	else if( time < 5 || day >= 25 )
	{
		str = 165; // �̿���						
	}
	else
	{
		str = 163; // ����
	}
	return kein_GetMenuString( str ); 
}


//020808 YGI -----------------------------
bool DecoadFile(char *filename, void *pData, int size)
{
    CHSEL_STREAM m_hsel;

    // [Android] 1. 路徑修正：移除開頭的 ./
    std::string path = filename;
    if (path.find("./") == 0) path = path.substr(2);

    // [Android] 2. 使用 SDL_RWops 開啟檔案
    SDL_RWops *fp = SDL_RWFromFile(path.c_str(), "rb");

    if( !fp ) {
        SDL_Log("DecoadFile Error: Could not open %s", path.c_str());
        return false;
    }

    // 3. 讀取版本
    int nVersion = 0;
    SDL_RWread(fp, &nVersion, sizeof(int), 1);

    if (m_hsel.GetVersion() != nVersion) {
        SDL_Log("DecoadFile Error: Version mismatch in %s", path.c_str());
        SDL_RWclose(fp);
        return false;
    }

    // 4. 讀取初始化 Header
    HselInit deinit;
    SDL_RWread(fp, &deinit, sizeof(HselInit), 1);

    if (!m_hsel.Initial(deinit)) {
        SDL_Log("DecoadFile Error: HSEL Init failed in %s", path.c_str());
        SDL_RWclose(fp);
        return false;
    }

    // 5. 讀取加密內容
    // 注意：SDL_RWread 的參數順序是 (context, ptr, size, maxnum)
    // 這裡讀取 size 個 1 byte，或者 1 個 size bytes 都可以
    size_t bytesRead = SDL_RWread(fp, pData, 1, size);

    if (bytesRead != size) {
        SDL_Log("Warning: DecoadFile requested %d bytes but read %zu bytes in %s", size, bytesRead, path.c_str());
        // 視情況決定是否要 return false，原版沒檢查，這裡先只印警告
    }

    // 6. 解密
    // 因為您已經修改了 HSEL.cpp 內部的 Decrypt 函式強制使用 signed char*
    // 這裡直接傳入 (char*) 是安全的，解密邏輯會在內部自動處理好
    m_hsel.Decrypt((char *)pData, size);

    SDL_RWclose(fp);
    return true;
}

bool IncordFile( char *filename, void *pData, int size )
{
	CHSEL_STREAM m_hsel;
	FILE *fp = Fopen( filename, "wb" );
	if( !fp ) return false;

	HselInit eninit;
	eninit.iEncryptType	=	HSEL_ENCRYPTTYPE_RAND;
	eninit.iDesCount	=	HSEL_DES_TRIPLE;
	eninit.iCustomize	=	HSEL_KEY_TYPE_DEFAULT;
	eninit.iSwapFlag	=	HSEL_SWAP_FLAG_ON;

	if (!m_hsel.Initial(eninit)) 
	{
		return false;
	}

	HselInit deinit;
	deinit = m_hsel.GetHSELCustomizeOption();
	const int nVersion = m_hsel.GetVersion();
	fwrite((void *)(&nVersion), sizeof(nVersion), 1, fp ); 
	fwrite((void *)(&deinit), sizeof(HselInit), 1, fp ); 
	m_hsel.Encrypt((char*)pData, size);
	fwrite( pData, 1, size, fp );
	fclose( fp );
	return true;
}
//021030 YGI
int RecvEventObject( k_event_object *pData )
{
	k_event_object_sound *pData3 = (k_event_object_sound *)pData;
	k_event_sound *pData2 = (k_event_sound *)pData;

	int sound = ReturnBGMNumber(MapNumber);
	if( sound > 0  )
	{
		StopWave(sound);
	}

	RainStop(); //Eleval 12/08/09
	ThunderStop(); //Eleval 12/08/09
	SnowStop(); //Eleval 12/08/09

	if( g_Menu_Variable.m_pEventObjcet )
	{
		delete g_Menu_Variable.m_pEventObjcet;
		g_Menu_Variable.m_pEventObjcet = 0;
	}

	if( pData->active & ( g_Menu_Variable.ACTIVE_EVENT_OBJECT | g_Menu_Variable.ACTIVE_EVENT_SOUND ) )
	{
		g_Menu_Variable.m_pEventObjcet = new k_event_object_sound;
		if( pData->active & g_Menu_Variable.ACTIVE_EVENT_OBJECT )
		{
			*g_Menu_Variable.m_pEventObjcet = *pData3;
		}
		else
		{
			g_Menu_Variable.m_pEventObjcet->active = pData2->active;
			g_Menu_Variable.m_pEventObjcet->sound = pData2->sound;
		}
	}

	return 1;
}

// 021107 YGI
void SendEvnetMoveMap()
{
	if( !g_Menu_Variable.m_pEventMoveMap ) return;
	if( !g_Menu_Variable.m_pEventMoveMap->IsCanMove() ) return;
	CallServer( CMD_EVENT_MOVE_MAP, &g_Menu_Variable.m_pEventMoveMap->data.event_no, sizeof(int) );
}