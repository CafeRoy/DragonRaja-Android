/*****************************************************************************\
* Copyright (c), Future Entertainment World / Seoul, Republic of Korea        *
* All Rights Reserved.                                                        *
*                                                                             *
* This document contains proprietary and confidential information.  No        *
* parts of this document or the computer program it embodies may be in        *
* any way copied, duplicated, reproduced, translated into a different         *
* programming language, or distributed to any person, company, or             *
* corporation without the prior written consent of Future Entertainment World *
\*****************************************************************************/

#include "StdAfx.h"
#include <sstream>
#include "dragon.h"
#include "Hong_Sub.h"
#include "gameproc.h"
#include "Menu.h"
//#include "MenuDef.h"
#include "MenuNetWork.h"
#include "char.h"
#include "map.h"
#include "counseler.h"		// 001129 KHS
#include "LineCommand.h"
#include "SmallMenu.h"
#include "menuset.h"
#include "SmallMenuSet.h"//020620 lsw
#include "chat.h"
#include "MailMgr.h"
#include "Skill.h"//021030 lsw
#include "Auction.h"//021030 lsw
#include "cScreenEffect.h" // CSD-030306
#include "ArenaManager.h"
#include "Hong_sprite.h" //Eleval
extern CHARACTERLIST g_CharacterList;
extern void SendSquadChatMessage(int type,const char* Message);			
extern void WarMenuOpen();											
extern void WarMenuNoticeOpen();

extern void ShowCommanderInfo(); 				// 010924 LTS
extern void ShowSquadInfo();
extern void WarGiveLife();						// 011015 LTS
extern void WarControl(char* Arg);				// 011020 LTS
extern void WarLoopTime(char* Arg);				// 011025 LTS
extern void WarGiveLife(char* Arg);				// 011213 LTS
extern void WarGiveLife2(const char* Arg);			
extern int	IsDead( LPCHARACTER ch );
extern void SendLocalWarChat(const char* Arg);		// LTS NEW LOCALWAR	
extern void SendNationChat(std::string text);
extern void  SendWorldBBSChat(char* Arg);
extern void SendAction(int i,int Direction);	// LTS ACTION
extern void SendCMD_SET_QUESTMODE(const char* Arg);	// LTS DRAGON MODIFY
extern void SendCheckDualFame();              // LTS DUAL FAME
extern bool g_bStopWeatherSystem; // CSD-CN-031215
extern bool g_bOffWeatherSystem; // CSD-CN-031217

extern bool g_allowDrop; //Eleval for /drop command 15/05/09
extern bool g_autoAttack; //Eleval 22/05/09 - for /auto attack command
extern bool g_autoDefence; //Eleval 23/05/09 - for /auto defence command
extern	ItemAttr EquipItemAttr[ITEM_EQUIP_MAX];
/*
LINECOMMAND LC[MAX_LINE_COMMAND_] = {	//< CSD-030311 : ����� �빮�ڷ� ���ּ���..
	{"", LC_EAT_ITEM},					// �Ա�										
	{"", LC_CHAT_WHISPER},				// @										
	{"", LC_CHAT_WHISPER},				// TO
	{"", LC_CHAT_PARTY},				// ��Ƽ����
	{"", LC_CHAT_PARTY},				// ����											
	{"", LC_CHAT_GUILD},				// ��忡��								
	{"", LC_TOTAL_MAP_CONNECTIONS},		// �������ο�								
	{"", LC_TOTAL_CONNECTIONS},			// �����ο�											
	{"", LC_TOTAL_CONNECTIONS},			// ����											
	{"", LC_TOTAL_CONNECTIONS},			// USER												
	{"", LC_MAKE_PARTY},				// PARTY											
	{"", LC_MAKE_PARTY},				// ��Ƽ													
	{"", LC_DEFAULT_WALK},				// �ȱ�													
	{"", LC_DEFAULT_WALK},				// �⺻�ȱ�													
	{"", LC_DEFAULT_RUN},				// �޸���												
	{"", LC_DEFAULT_RUN},				// �⺻�޸���											
	{"", LC_TAME_STOP},					// ����												
	{"", LC_TAME_STOP},					// �׸�																
	{"", LC_TAME_STOP},					// STOP														
	{"", LC_TAME_ATTACK},				// ������													
	{"", LC_TAME_ATTACK},				// ATTACK												
	{"", LC_TAME_FOLLOWME},				// �����											
	{"", LC_TAME_FOLLOWME},				// FOLLOW												
	{"", LC_TAME_NAME},					// ���̸���												
	{"", LC_TAME_NAME},					// NAME											
	{"", LC_OPEN_TIMEMENU},				// �ð�����								
	{"", LC_OPEN_TIMEMENU},				// TIME 								
	{"", LC_CLOSE_TIMEMENU},			// �ð����									
	{"", LC_CLOSE_TIMEMENU},			// �ð��ݱ�											
	{"", LC_OPEN_TIMEMENU},				// �ð�										
	{"", LC_CLOSE_ALL},					// OFF													
	{"", LC_EAR_MESSAGE_REFUSE_WHO},	// �źλ��												
	{"", LC_EAR_MESSAGE_REFUSE},		// �ӼӸ��ź�									
	{"", LC_EAR_MESSAGE_OK},			// �ӼӸ����									
	{"", LC_REPRINT_EAR_MESSAGE},		// ����				
	{"", LC_GREETING},					// �λ縻 								
	{"", LC_EXPLAIN_CLOSE},				// ����ݱ�																
	{"", LC_EXPLAIN_CLOSE},				// ������									
	{"", LC_EXPLAIN_OPEN},				// �������					
	{"", LC_REQ_MAN_TO_MAN},			// ����						
	{"", LC_RESET_MAGIC_ARRAY},			// ���ǱǴ� 						
	{"", LC_RESET_MAGIC_ARRAY},			// �������ġ										
	{"", LC_MSG_ALL_NATION},			// ���							
	{"", LC_GUILD_EXIT},				// ���Ż��					
	{"", LC_GUILD_JOIN},				// ��尡��						
	{"", LC_GUILD_ALL_MSG},				// �����					
	{"", LC_GUILD_ALL_MSG},				// ������					
	{"", LC_GUILD_CHANGE_EACH_LEVEL},	// �����							
	{"", LC_GUILD_CMD_INFO},			// �����
	{"", LC_CHANGE_JOB},				// �����ٲٱ�
	{"", LC_CHANGE_JOB},				// ������ü						
	{"", LC_STOP_WAR},					// ����
	{"", LC_SCAN_TARGET_VYSEUS},		// �߻罺�� ���̿�
	{"", LC_SCAN_TARGET_ZYPERN},		// �Ҹ��� ������
	{"", LC_KILL_CHARACTER},			// �޽�
	{"", LC_DIRECTIONS},				// ����
	{"", LC_OPEN_DISPLAY_SKILL_EXP},	// ��ų����
	{"", LC_CLOSE_DISPLAY_SKILL_EXP},	// ��ų�ݱ�
	{"", LC_OPEN_DISPLAY_TAC_EXP},		// ��ƽ����
	{"", LC_CLOSE_DISPLAY_TAC_EXP},		// ��ƽ�ݱ�
	{"", LC_LIGHTOFRESCUE},				// �����Ǻ�
	{"", LC_AUTOLEVELUP_CLOSE},			// �ڵ�����
	{"", LC_AUTOLEVELUP_OPEN},			// �ڵ��й�							
	{"", LC_ANNONCE_ON},			    // ���򸻵��
	{"", LC_ANNONCE_OFF},			    // ���򸻲��
	{"", LC_EAR_COMMAND_INFO},		    // �ӼӸ�
	{"", LC_FIGHTMAP_ALL_CHAT},		    // ��ü
	{"", LC_SMALL_TIPS},			    // ����
	{"", LC_SMALL_TIPS},			    // HELP
	{"", LC_SMALL_TIPS},			    // ?
	{"", LC_EMOTION},				    // ����
	{"", LC_EXIT_GUILD},
	{"", LC_VIEW_NK},				    // NK ��ġ ����
	{"", LC_SQUAD_LEADER},			    // ��ɰ� -> �δ���
	{"", LC_SQUAD},					    // ��ɰ� -> �δ�
	{"", LC_SQUAD_MEMBER},              // �δ��� -> �δ��
	{"", LC_SQUAD_MEMBER_TO_MEMBER},    // �δ�� -> �δ��	
	{"", LC_WAR_MENU_OPEN},		        // War Menu Open
	{"", LC_WAR_MENU_NOTICE_OPEN},	    // War Menu NOTICE
	{"", LC_GUILD_MAIL},			    // ������
	{"", LC_SQUAD_CHAT},			    // 010915 LTS	//�ӽ�
	{"", LC_GUILD_CHAT},			    // #
	{"", LC_COMMANDER_INFO},		    // 010924 LTS
	{"", LC_SQUAD_INFO},			    // 010924 LTS	
	{"", LC_GUILD_CHAT_ON},		        // ���ä���ѱ�
	{"", LC_GUILD_CHAT_OFF},		    // ���ä�ò��
	{"", LC_RESET_ABILITY},		        // �����й�
	{"", LC_WAR_GIVE_LIFE},		        // 011015 LTS	// ������ ���� �츮�� // ��Ȱ
	{"", LC_PARTY_ALL_FREE},		    // ��Ƽ����
	{"", LC_SEND_GUILD_MAIL},	        // �����Ϻ�����
	{"", LC_SEND_MAIL},		 	        // ���Ϻ�����
	{"", LC_SHOW_ME_DUAL},		  	    // ����� �Ȱ� �ð��� �޸���
	{"", LC_REPORTER_MODE},			    // ���ڸ��
	{"", LC_FRIEND_MENU},			    // ģ��
	{"", LC_WAR_GIVE_LIFE2},		    // ��Ȱ���				// 011213 LTS	N_YILSE
	{"", LC_SCAN_TARGET_YILSE},		    // 011217 LTS
	{"", LC_GETCOLOSSUS_INFO},		    // �ݷμ���, 
	{"", LC_GETCOLOSSUS_INFO},		    // colossus, 
	{"", LC_INVITE_COLOSSUS},		    // �ʴ� 
	{"", LC_JOIN_COLOSSUS},			    // ����
	{"", LC_PK_ON_OFF},				    // PK
	{"", LC_COMBAT_SKILL_ON_OFF},	    // PK//020420 lsw
	{"", LC_LOCALWAR_CHAT},			    // ������	%		// LTS NEW LOCALWAR
	{"", LC_QUESTMODE},				    // LTS DRAGON MODIFY
	{"", LC_WEATHER_ON}, // CSD-CN-031215 : ����ȿ���ѱ�
	{"", LC_WEATHER_OFF}, // CSD-CN-031215 : ����ȿ�����
	{"", LC_COMBAT_RESET},			    // CSD-020611
	{"", LC_MAGIC_DISPLAY_ON},          // CSD-020620
	{"", LC_MAGIC_DISPLAY_OFF},         // CSD-020620
	{"", LC_SET_NORMAL_CHAT_MODE},      // CSD-020620
	{"", LC_REPRINT_NORMAL_CHAT},	    // ��������
	{"", LC_REPRINT_GUILD_CHAT},	    // �������
	{"", LC_REPRINT_GUILD_BBS},	        // ����������
	{"", LC_TRADE},			            // 020808 YGI 
	{"", LC_CHECK_DUAL_FAME},		    // �����     // LTS DUAL FAME
	{"", LC_WHAT_QUEST_IN_MAP},	        // 021007	//�ʿ� � ����Ʈ�� �ִ°�?
	{"", LC_MERCHANT_BBS},		        // 021022 ���ΰ� ���� �ŷ�
	{"", LC_MERCHANT_EXCHANGE},		    // 021022 ���ΰ� �ŷ�
	{"", LC_EVENT_MOVE_MAP},		    // 021107 YGI
	//kyo <<
	{"", LC_EMOTI_LOVE},
	{"", LC_EMOTI_SMILE},
	{"", LC_EMOTI_HAPPY},
	{"", LC_EMOTI_SHY},
	{"", LC_EMOTI_CRY},
	{"", LC_EMOTI_GOOD},
	{"", LC_EMOTI_SORRY},
	{"", LC_EMOTI_ANGER},
	{"", LC_EMOTI_THANKS},
	{"", LC_EMOTI_TELL},
	{"", LC_GAME_SCISSOR},
	{"", LC_GAME_ROCK},
	{"", LC_GAME_PAPER},
	{"", LC_GAME_GO},
	{"", LC_GAME_HELP},
	{"", LC_GAME_PARTY},
	{"", LC_GAME_HEAL},
	{"", LC_GAME_TRANSACTION},
	{"", LC_GAME_BEG},
	{"", LC_GAME_CONGRAGULATION},

	{"", LC_BBS_LIMITED_USER_ADD},
	{"", LC_BBS_ON},
	{"", LC_BBS_OFF},
	{"", LC_RAID_START}, // CSD-040310
	{"", LC_RAID_END}, // CSD-040310
	{"", LC_GET_SMP},
	{"", LC_REPRINT_PARTY_CHAT}, // Finito for /party note command
	{"", LC_DENY_DUEL_REQ}, // Finito 25/08/07 command to deny dual requests
	{"", LC_ALLOW_DUEL_REQ}, // Finito 25/08/07 command to allow dual requests
	{"", LC_TOGGLE_DROP},	// Eleval 15/05/09 command to allow / block drop
	{"", LC_TAME_AUTO_ATTACK}, // Eleval 22/05/09 command to enable / disable auto-attack for tamed monsters
	{"", LC_TAME_AUTO_DEFENCE}, // Eleval 23/05/09 command to enable / disable auto-defence for tamed monsters
	{"", LC_TAME_ALL_FOLLOW}, // Eleval 24/05/09 command to tell all tamed creatures to follow their master (/tame follow)
	{"", LC_TAME_ALL_STOP}, // Eleval 24/05/09 command to tell all tamed creatures to stop what they are doing (/tame stop)
	{"", LC_REPORTER_TELEPORT}, //Eleval 15/08/09 - Command for RCs to teleport themselves anywhere (/teleport map x y)
	{"", LC_HIDE}, //Eleval 18/08/09 - So that people can hide some stuff, like hats
	{"", LC_HONOR}, //Eleval 06/09/09 - To show the Player's Honor

	{"", LC_EXP_RESET},
	{"", LC_NATION_CHAT},
	{ "", LC_WORLDBBS_CHAT},
	{ "", LC_USER_IMPRISON },		// LTH-040812-KO ������ ����!!
	{ "", LC_USER_RELEASE },		// LTH-040812-KO ������ ���!!
	{ "", LC_USER_BAN },
	//>>kyo
	{"", 0}				
};  //> CSD-030311
*/
std::map<std::string, LineCommandIndex> g_LineCommandMap;
char	Lc_ChatType=0;
char	Lc_TempChatName[ 31];
char	Lc_DefaultMoveType;

int		Lc_AutoMovementCmd=0;


char	GreetingStr[ 100];

/////////////////////////////////////////////////////////////////////////////////////////
//
//		User Functions Declaration..
//
//
void SendTotalMapConnections( void );
void SendTotalConnections( void );
void RecvTotalMapConnections( int no );
void RecvTotalConnections( int no );

void LC_SendInputParty(std::string &name );

void SendTameCommand(int command, std::string &name, std::string &toname);
void RecvHostEnd(  tame_end *p );
void SendTeleportCommand(std::string target_name, std::string target_map, std::string x, std::string y); //Eleval 15/08/09 - To manage RCs Teleport
void HideSomething(std::string text);	//Eleval 18/08/09 - To hide something




///////////////////////////////////////////////////////////////////////////////////////////
//
//		User Functions..
//
//
std::string getCommandArguments(const std::string& input) {
    // 尋找第一個空格的位置
    size_t first_space_pos = input.find(' ');

    // 檢查是否找到了空格
    if (first_space_pos != std::string::npos) {
        // 從空格的下一個位置開始擷取
        return input.substr(first_space_pos + 1);
    }

    // 如果沒找到空格 (例如只有 "/quit" 指令)，就回傳一個空字串
    return "";
}

int IsLineCommand(const char *message, int nsize )
{	
	LineCommandIndex lc;
	std::string command;
    std::string param[5];
	
	if( *message != '/' ) return 0;
	
	message++;
    // Trim leading spaces
    std::stringstream ss(message);
    // Get the command
    ss >> command;

    // Get command from map container
    CharToUpper(command.data());
    auto it = g_LineCommandMap.find(command);
    if (it == g_LineCommandMap.end()) {
      return 0;
    }
    // Get the LineCommandIndex
    lc = it->second;

    //save the string for if the  command is chat strig
	std::string chat_string = getCommandArguments(message);

    // Get parameters
    for (int i = 0; i < 5; i++) {
      ss >> param[i];
#ifdef _DEBUG
      SDL_Log("LC_NAME[%s] LC_INDEX[%d] PARAM%d[%s]", 
              it->first.c_str(), 
              static_cast<int>(lc),
              i,param[i].c_str());
#endif
    }
    switch (lc) {
      case LC_NO_COMMAND:
      case LC_EAT_ITEM:
      case LC_NOT_GO: {
        break;
      }
      case LC_GO_NORTH:
      case LC_GO_WEST:
      case LC_GO_EAST:
      case LC_GO_SOUTH:
      case LC_GO_RANDOM: {
        Lc_AutoMovementCmd = lc;
        switch (Lc_AutoMovementCmd) {
          case LC_GO_EAST: {//人物移動
            //SendMessage(g_hwndMain, WM_LBUTTONDOWN, 0,
                        ///MAKELONG(Hero->x - Mapx + 100, Hero->y - Mapy));
            break;
          }
          case LC_GO_WEST: {
            //SendMessage(g_hwndMain, WM_LBUTTONDOWN, 0,
                        //MAKELONG(Hero->x - Mapx - 100, Hero->y - Mapy));
            break;
          }
          case LC_GO_NORTH: {
            //SendMessage(g_hwndMain, WM_LBUTTONDOWN, 0,
                       //MAKELONG(Hero->x - Mapx, Hero->y - Mapy - 100));
            break;
          }
          case LC_GO_SOUTH: {
            //SendMessage(g_hwndMain, WM_LBUTTONDOWN, 0,
                        //MAKELONG(Hero->x - Mapx, Hero->y - Mapy + 100));
            break;
          }
        }
        break;
      }
      case LC_CHAT_WHISPER: {
        SendWisperByLineCommand(param[0], chat_string);
        break;
      }
      case LC_TOTAL_CONNECTIONS: {
        // SendTotalConnections();
        break;
      }
      case LC_CHAT_GUILD: {
        SendChatSet(CHATTYPE_GUILD, NULL);
        break;
      }
      case LC_TOTAL_MAP_CONNECTIONS: {
        // SendTotalMapConnections();
        break;
      }
      case LC_CHAT_PARTY: {
        SendPartyChatByLineCommand(chat_string.c_str());
        break;
      }
      case LC_MAKE_PARTY: {
        LC_SendInputParty(param[0]);
        break;
      }
      case LC_DEFAULT_WALK: {
        Lc_DefaultMoveType = 0;
        Kein_PutMessage(KM_INFO, kein_GetMenuString(40));
        break;
      }
      case LC_DEFAULT_RUN: {
        Lc_DefaultMoveType = 1;
        Kein_PutMessage(KM_INFO, kein_GetMenuString(39));
        break;
      }
      case LC_GUILD_PARTY: { int type = 0; CallServer(CMD_CHANGE_CHARACTERMODE, &type, sizeof(int)); } break;
      case LC_NATION_PARTY: { int type = 1; CallServer(CMD_CHANGE_CHARACTERMODE, &type, sizeof(int)); } break;
      case LC_ENTERHOUSE: {
        // 這兩個 case 在原始程式碼中沒有對應的區塊，因此被保留。
        break;
      }
      case LC_TAME_STOP:
      case LC_TAME_ATTACK:
      case LC_TAME_FOLLOWME:
      case LC_TAME_NAME: {
        SendTameCommand(lc, param[0], param[1]);
        break;
      }
      case LC_GO_STOP: {
        Lc_AutoMovementCmd = 0;
        break;
      }
      case LC_OPEN_TIMEMENU: {
        g_bTimeMenuOn = true;
        break;
      }
      case LC_CLOSE_TIMEMENU: {
        g_bTimeMenuOn = false;
        break;
      }
      case LC_CLOSE_ALL: {
        g_bTimeMenuOn = false;
        g_ExplainMenuOn = false;
        CloseAllMenu();
        break;
      }
      case LC_EAR_MESSAGE_REFUSE: {
        ChatMgr.SetRefuseWhiper(param[0].c_str());
        break;
      }
      case LC_EAR_MESSAGE_OK: {
        ChatMgr.SetAcceptWhiper(param[0].c_str());
        break;
      }
      case LC_REPRINT_EAR_MESSAGE: {
        ChatMgr.DisplayPastData(CHATTYPE_WHISPER);
        break;
      }
      case LC_EAR_MESSAGE_REFUSE_WHO: {
        ChatMgr.DisplayRefuseList();
        break;
      }
      case LC_GREETING: {
        strcpy_s(GreetingStr, param[0].c_str());
        AddCurrentStatusMessage(FONT_COLOR_SOLID_GREEN,
                                lan->OutputMessage(3, 49), GreetingStr);
        AddCurrentStatusMessage(FONT_COLOR_SOLID_GREEN,
                                lan->OutputMessage(3, 48));
        break;
      }
      case LC_EXPLAIN_OPEN: {
        g_ExplainMenuOn = true;
        break;
      }
      case LC_EXPLAIN_CLOSE: {
        g_ExplainMenuOn = false;
        break;
      }
      case LC_REQ_MAN_TO_MAN: {
        SendReqManToMan(param[0].c_str());
        break;
      }
      case LC_RESET_MAGIC_ARRAY: {
        if (SCharacterData.nCharacterData[SPELL] == PRIEST_SPELL) {
          MagicSetting2();
        }
        break;
      }
      case LC_MSG_ALL_NATION: {
        SendAllMsgNation(chat_string);
        break;
      }
      case LC_GUILD_EXIT: {
        SendGuildExit(param[0].c_str());
        break;
      }
      case LC_GUILD_JOIN: {
        SendGuildJoin(param[0].c_str());
        break;
      }
      case LC_GUILD_ALL_MSG: {
        SendGuildAllMsg(chat_string);
        break;
      }
      case LC_GUILD_CHANGE_EACH_LEVEL: {
        SendGuildChangeEachLevel(param[0], param[1]);
        break;
      }
      case LC_GUILD_CMD_INFO: {
        ViewGuildCommandInfo();
        break;
      }
      case LC_CHANGE_JOB: {
        break;
      }
      case LC_STOP_WAR: {
        // SendReqStopWar(t1);
        break;
      }
      case LC_SCAN_TARGET_VYSEUS: {
        if (!ScanTarget(N_VYSEUS)) {
          return 0;
        }
        break;
      }
      case LC_SCAN_TARGET_ZYPERN: {
        if (!ScanTarget(N_ZYPERN)) {
          return 0;
        }
        break;
      }
      case LC_KILL_CHARACTER: {
        if (Hero->IsCounselor()) {
          LPCHARACTER ch = nullptr;
          t_packet p;
          if (g_GameInfo.lpcharacter) {
            if (g_GameInfo.lpcharacter != Hero) {
              ch = g_GameInfo.lpcharacter;
              if (!ch->IsNpc()) {
                p.h.header.type = CMD_MAKE_GHOST;
                p.u.make_ghost.id = ch->id;
                p.h.header.size = sizeof(t_make_ghost);
                QueuePacket(&p, 1);
              }
            }
          }
        }
        break;
      }
      case LC_CENTER_HERO: {
          g_center_hero = !g_center_hero;
        break;
      }
      case LC_SHIFT://SHIFT 指令
      {
          Hero->DontMoveAttackFlag = !Hero->DontMoveAttackFlag;
      }break;
      case LC_EXP:
      {
          int type = 2; CallServer(CMD_CHANGE_CHARACTERMODE, &type, sizeof(int));
      }break;
      case LC_OPEN_DISPLAY_TAC_EXP:
      case LC_CLOSE_DISPLAY_TAC_EXP: {
        break;
      }
      case LC_LIGHTOFRESCUE: {
        gr.GuideGhost_TypeTheHelp(true);
        break;
      }
      case LC_AUTOLEVELUP_CLOSE: {
        if (SCharacterData.nLevel <= 30) {
          AddCurrentStatusMessage(FONT_COLOR_SOLID_GREEN,
                                  lan->OutputMessage(3, 19));
        }
        AutoLevelUpOn = false;
        break;
      }
      case LC_AUTOLEVELUP_OPEN: {
        if (SCharacterData.nLevel <= 30) {
          AddCurrentStatusMessage(FONT_COLOR_SOLID_GREEN,
                                  lan->OutputMessage(3, 20));
        }
        AutoLevelUpOn = true;
        break;
      }
      case LC_ANNONCE_ON: {
        break;
      }
      case LC_ANNONCE_OFF: {
        break;
      }
      case LC_EAR_COMMAND_INFO: {
        ViewEarCommandInfo();
        break;
      }
      case LC_FIGHTMAP_ALL_CHAT: {
        SendChatDataGlobalFightMap(param[0].c_str());
        break;
      }
      case LC_SMALL_TIPS: {
        CallViewSmallTips(param[0]);
        break;
      }
      case LC_EMOTION: {
        break;
      }
      case LC_EXIT_GUILD: {
        SendExitGuild();
        break;
      }
      case LC_VIEW_NK: {
        ViewNk(param[0]);
        break;
      }
      case LC_SQUAD_LEADER:
      case LC_SQUAD:
      case LC_SQUAD_MEMBER:
      case LC_SQUAD_MEMBER_TO_MEMBER:
      case LC_SQUAD_CHAT: {
        SendSquadChatMessage(lc, param[0].c_str());
        break;
      }
      case LC_WAR_MENU_OPEN: {
        // WarControl(t1);
        break;
      }
      case LC_WAR_MENU_NOTICE_OPEN: {
        // WarLoopTime(t1);
        break;
      }
      case LC_GUILD_MAIL: {
        break;
      }
      case LC_GUILD_CHAT: {
        SendChatGuild(param[0].c_str());
        break;
      }
      case LC_COMMANDER_INFO: {
        ShowCommanderInfo();
        break;
      }
      case LC_SQUAD_INFO: {
        ShowSquadInfo();
        break;
      }
      case LC_GUILD_CHAT_ON: {
        g_Menu_Variable.m_bGuildChatOn = true;
        break;
      }
      case LC_GUILD_CHAT_OFF: {
        g_Menu_Variable.m_bGuildChatOn = false;
        break;
      }
      case LC_RESET_ABILITY: {
        /*int iAble = 1;
        for (int i = 0; i < 8; i++) {
          if (EquipItemAttr[i].item_no) {
            iAble = 0;
          }
        }
        if (iAble) {
          CallSmallMenu(MN_ABILITY_REDISTRIBUTION);
        } else {
          AddCurrentStatusMessage(FONT_COLOR_RED, lan->OutputMessage(0, 500));
        }*/
        break;
      }
      case LC_WAR_GIVE_LIFE: {
        WarGiveLife();
        break;
      }
      case LC_PARTY_ALL_FREE: {
        memset(SCharacterData.party, 0, sizeof(CharacterParty) * 6);
        CallServer(CMD_PARTY_ALL_FREE);
        break;
      }
      case LC_SEND_GUILD_MAIL:
      case LC_SEND_MAIL: {
        ::Call_MAIL_WRITE_MENU(param[0].c_str());
        break;
      }
      case LC_SHOW_ME_DUAL: {
        CheckShowMeDual();
        break;
      }
      case LC_REPORTER_MODE: {
        if (Hero->name_status.reporter) {
          if (!Hero->reporter_mode && IsDead(Hero)) {
            break;
          }
          CallServer(CMD_REPORTER_MODE);
          // Hero->reporter_mode = !Hero->reporter_mode;
        } else {
          Kein_PutMessage(KM_FAIL, kein_GetMenuString(97));
        }
        break;
      }
      case LC_FRIEND_MENU: {
        CallFriendMenu();
        break;
      }
      case LC_WAR_GIVE_LIFE2: {
        WarGiveLife2(param[0].c_str());
        break;
      }
      case LC_SCAN_TARGET_YILSE: {
        if (!ScanTarget(N_YILSE)) {
          return 0;
        }
        break;
      }
      case LC_GETCOLOSSUS_INFO: {
        CallServer(CMD_CHECK_COLOSSUS_MAP);
        Kein_PutMessage(KM_INFO, kein_GetMenuString(152));
        break;
      }
      case LC_INVITE_COLOSSUS: {
        if (!g_pArenaManager->IsLeader(Hero->id)) {
          Kein_PutMessage(KM_FAIL, kein_GetMenuString(153));
        } else {
          static DWORD time = g_curr_time - 20;
          DWORD gab = g_curr_time - time;
          if (gab < 10) {
            MP3(SN_WARNING);
            Kein_PutMessage(KM_FAIL, kein_GetMenuString(156), 10 - gab);
          } else {
            time = g_curr_time;
            CallServer(CMD_INVITE_COLOSSUS);
          }
        }
        break;
      }
      case LC_JOIN_COLOSSUS: {
        SendJoinColossusMap();
        break;
      }
      case LC_PK_ON_OFF: {
        CallServer(CMD_GET_PK_ON_OFF);
        break;
      }
      case LC_COMBAT_SKILL_ON_OFF: {
        g_mgrBattle.SendCombatRequest(Hero);
        break;
      }
      case LC_SET_NORMAL_CHAT_MODE: {
        // SetChatTarget(CHAT_TARGET_NORMAL);
        // SendChatNormal(param[0].c_str());
        break;
      }
      case LC_LOCALWAR_CHAT: {
        SendLocalWarChat(param[0].c_str());
        break;
      }
      case LC_QUESTMODE: {
        SendCMD_SET_QUESTMODE(param[0].c_str());
        break;
      }
      case LC_WEATHER_ON: {
        AddCurrentStatusMessage(FONT_COLOR_GREEN, lan->OutputMessage(0, 628));
        /*t_packet packet;
        packet.h.header.size = sizeof(t_rm_change_weather);
        packet.h.header.type = CMD_RM_CHANGE_WEATHER;
        packet.u.RmChangeWeather.bStopWeather = -1;
        packet.u.RmChangeWeather.bWeather = -1;
        packet.u.RmChangeWeather.dwAmount = -1;
        QueuePacket(&packet, 1);*/
        g_bOffWeatherSystem = false;
        break;
      }
      case LC_WEATHER_OFF: {
        AddCurrentStatusMessage(FONT_COLOR_GREEN, lan->OutputMessage(0, 629));
        RainStop();
        ThunderStop();
        SnowStop();
        g_bOffWeatherSystem = true;
        break;
      }
      case LC_COMBAT_RESET: {
#ifdef _DEBUG
        CallServer(CMD_COMBAT_RESET);
#endif
        break;
      }
      case LC_MAGIC_DISPLAY_ON: {
        AddCurrentStatusMessage(FONT_COLOR_GREEN, lan->OutputMessage(0, 626));
        g_mgrBattle.SetDisplay(true);
        g_ParticleManager.SetDisplay(true);
        break;
      }
      case LC_MAGIC_DISPLAY_OFF: {
        AddCurrentStatusMessage(FONT_COLOR_GREEN, lan->OutputMessage(0, 627));
        g_mgrBattle.SetDisplay(false);
        g_ParticleManager.SetDisplay(false);
        break;
      }
      case LC_CHATLOG: {
        ChatMgr.DisplayPastData(CHATTYPE_NORMAL);
        break;
      }
      case LC_REPRINT_GUILD_CHAT: {
        ChatMgr.DisplayPastData(CHATTYPE_GUILD);
        break;
      }
      case LC_REPRINT_GUILD_BBS: {
        ChatMgr.DisplayPastData(CHATTYPE_GUILD_BBS);
        break;
      }
      case LC_TRADE: {
        TradeONOFF(param[0].c_str());
        break;
      }
      case LC_CHECK_DUAL_FAME: {
        SendCheckDualFame();
        break;
      }
      case LC_WHAT_QUEST_IN_MAP: {
        SendWhatQuestInMap(param[0].c_str());
        break;
      }
      case LC_MERCHANT_BBS: {
        if (Hero->viewtype == VIEWTYPE_GHOST_) {
          break;
        }
        if (IsMerchant()) {
          ::SendChatData(param[0], CHATTYPE_MERCHANT_BBS);
        }
        break;
      }
      case LC_MERCHANT_EXCHANGE: {
        Auction.SendCMD_MERCHANT_EXCHANGE_LIST_REQUEST(param[0], 0, 1, true);
        break;
      }
      case LC_EVENT_MOVE_MAP: {
        SendEvnetMoveMap();
        break;
      }
      case LC_EMOTI_LOVE:
      case LC_EMOTI_SMILE:
      case LC_EMOTI_HAPPY:
      case LC_EMOTI_SHY:
      case LC_EMOTI_CRY:
      case LC_EMOTI_GOOD:
      case LC_EMOTI_SORRY:
      case LC_EMOTI_ANGER:
      case LC_EMOTI_THANKS:
      case LC_EMOTI_TELL:
      case LC_GAME_SCISSOR:
      case LC_GAME_ROCK:
      case LC_GAME_PAPER:
      case LC_GAME_GO:
      case LC_GAME_HELP:
      case LC_GAME_PARTY:
      case LC_GAME_HEAL:
      case LC_GAME_TRANSACTION:
      case LC_GAME_BEG:
      case LC_GAME_CONGRAGULATION: {
        ::SendSmileFace(lc - LC_EMOTI_SMILE + 2);
        break;
      }
      case LC_BLOCK: {
        if (!ChatMgr.AddLimintedBBSName(param[0])) {
          ::AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW,
                                    lan->OutputMessage(0, 512));
        } else if (param[0].empty()) {
          ::AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW,
                                    lan->OutputMessage(0, 510),
                                    param[0].c_str());
        }
        break;
      }
      case LC_BBS_ON: {
        ChatMgr.SetActiveBBS(true);
        ::AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW,
                                  lan->OutputMessage(0, 513));
        break;
      }
      case LC_BBS_OFF: {
        ChatMgr.SetActiveBBS(false);
        ::AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW,
                                  lan->OutputMessage(0, 514));
        break;
      }
      case LC_RAID_START: {
#ifdef _DEBUG
        CallServer(CMD_MONSTER_RAID_START);
#endif
        break;
      }
      case LC_RAID_END: {
#ifdef _DEBUG
        CallServer(CMD_MONSTER_RAID_END);
#endif
        break;
      }
      case LC_GET_SMP: {
        CallServer(CMD_GET_SKILL_POINT);
        break;
      }
      case LC_REPRINT_PARTY_CHAT: {
        ChatMgr.DisplayPastData(CHATTYPE_PARTY);
        break;
      }
      case LC_DENY_DUEL_REQ: {
        t_packet packet;
        packet.h.header.type = CMD_DUEL_ALLOWDENY;
        {
          packet.u.DuelAllowDeny.bIsDuelAllow = false;
        }
        packet.h.header.size = sizeof(t_DuelAllowDeny);
        QueuePacket(&packet, 1);
        AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW,
                                lan->OutputMessage(3, 293));
        break;
      }
      case LC_ALLOW_DUEL_REQ: {
        t_packet packet;
        packet.h.header.type = CMD_DUEL_ALLOWDENY;
        {
          packet.u.DuelAllowDeny.bIsDuelAllow = true;
        }
        packet.h.header.size = sizeof(t_DuelAllowDeny);
        QueuePacket(&packet, 1);
        AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW,
                                lan->OutputMessage(3, 294));
        break;
      }
      case LC_TOGGLE_DROP: {
        if (g_allowDrop) {
          g_allowDrop = false;
          AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW,
                                  lan->OutputMessage(3, 209));
        } else {
          g_allowDrop = true;
          AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW,
                                  lan->OutputMessage(3, 210));
        }
        break;
      }
      case LC_TAME_AUTO_ATTACK: {
        if (g_autoAttack) {
          g_autoAttack = false;
          AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW,
                                  lan->OutputMessage(3, 295));
        } else {
          g_autoAttack = true;
          AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW,
                                  lan->OutputMessage(3, 296));
        }
        break;
      }
      case LC_TAME_AUTO_DEFENCE: {
        if (g_autoDefence) {
          g_autoDefence = false;
          AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW,
                                  lan->OutputMessage(3, 297));
        } else {
          g_autoDefence = true;
          AddCurrentStatusMessage(FONT_COLOR_SOLID_YELLOW,
                                  lan->OutputMessage(3, 298));
        }
        break;
      }
      case LC_TAME_ALL_FOLLOW: {
        g_mgrBattle.SendTameCommand(Hero, Hero, LC_TAME_FOLLOWME);
        break;
      }
      case LC_TAME_ALL_STOP: {
        g_mgrBattle.SendTameCommand(Hero, Hero, LC_TAME_STOP);
        break;
      }
      case LC_REPORTER_TELEPORT: {
        SendTeleportCommand(param[0], param[1], param[2], param[3]);
        break;
      }
      case LC_HIDE: {
        HideSomething(param[0].c_str());
        break;
      }
      case LC_HONOR: {
        CallServer(CMD_REQ_HONOR_AMOUNT);
        break;
      }
      case LC_EXP_RESET: {
#ifdef _DEBUG
        //CallServer(CMD_EXP_RESET);
#endif
        break;
      }
      case LC_NATION_CHAT: {
        SendNationChat(chat_string.c_str());
      } break;
      case LC_WORLDBBS_CHAT: {
        break;
      }
      case LC_USER_IMPRISON: {
      } break;
      case LC_USER_RELEASE: {
      } break;
      case LC_USER_BAN: {
      } break;
    }
	return 1;
}

void SendTotalMapConnections( void )
{
	//	t_packet p;
	//	p.h.header.type = CMD_TOTAL_MAP_CONNECTIONS;
	//	p.h.header.size = 0;
	//	QueuePacket( &p, 1 );
}

void SendTotalConnections( void )
{
	//	t_packet p;
	//	p.h.header.type = CMD_TOTAL_CONNECTIONS;
	//	p.h.header.size = 0;
	//	QueuePacket( &p, 1 );
}


void RecvTotalMapConnections( int no )
{
	AddCurrentStatusMessage( 255,255,255, lan->OutputMessage(3,53), no );//010215 lsw
}

void RecvTotalConnections( int no )
{
	AddCurrentStatusMessage( 255,170,170, lan->OutputMessage(3,54),  no );//010215 lsw
}

void LC_SendInputParty(std::string &name )		// 0929 YGI
{
	LPCHARACTER target = ExistHe( name.c_str() );
	if( !target ) return;
	
	int i;
	for(i=0; i<6; i++ )
	{
		if( !SCharacterData.party[i].m_Name[0] ) break;
	}
	if( i == 6 ) return;
	
	SetMouseCh( target );
	SendInputParty( PARTY, i, &mouse_ch);
	memset( &mouse_ch, 0, sizeof( CharacterParty ) );
}


void LC_SendInputnATIONParty()		// 0929 YGI
{

	t_packet packet;
	NationParty* N;
	//N->On = TRUE;
	
}


void SendTameCommand(int command, std::string &name, std::string &toname)
{	
	if (name.empty()) 
	{
		switch (command)
		{
		case LC_TAME_STOP:
			{
				AddCurrentStatusMessage(100, 100, 255, lan->OutputMessage(3, 55));
				break;
			}
		case LC_TAME_ATTACK:
			{
				AddCurrentStatusMessage(100, 100, 255, lan->OutputMessage(3, 56));
				break;
			}
		case LC_TAME_FOLLOWME:
			{
				AddCurrentStatusMessage(100, 100, 255, lan->OutputMessage(3, 57));
				break;
			}
		case LC_TAME_NAME:
			{
				AddCurrentStatusMessage(100, 100, 255, lan->OutputMessage(3, 58));
				break;
			}
		}
		
		return;
	}

	if (toname.empty())
	{
		switch (command)
		{
		/*
		case LC_TAME_ATTACK:
			{
				AddCurrentStatusMessage(100, 100, 255, lan->OutputMessage(3, 59));	
				return;
			}
		*/
		case LC_TAME_NAME:
			{
				AddCurrentStatusMessage(100, 100, 255, lan->OutputMessage(3, 60));
				return;
			}
		}
	}
	
	t_packet p;
	p.h.header.type = CMD_TAME_COMMAND;
	p.u.tame_command.cmd = command;
	strcpy(p.u.tame_command.mybaby, name.c_str());
	strcpy(p.u.tame_command.toname, toname.c_str());
	p.h.header.size= sizeof(t_tame_command);
	QueuePacket(&p, 1);
}	//> CSD-031106

void RecvTameCommandResult( int result )
{
	switch( result )
	{
	case TAME_FAIL_CHANGE_NAME		: AddCurrentStatusMessage( 214,100,122, lan->OutputMessage(3,74));		break;//010215 lsw
	case TAME_NO_CHAR				: AddCurrentStatusMessage( 214,100,122,lan->OutputMessage(3,75) ); break;
	case TAME_NO_NAME_CHAR			: AddCurrentStatusMessage( 214,100,122, lan->OutputMessage(3,76));	break;
	case TAME_FOLLOW				: AddCurrentStatusMessage( 114,100,122, lan->OutputMessage(3,77));	break;
	case TAME_STOP					: AddCurrentStatusMessage( 114,100,122, lan->OutputMessage(3,78));	break;
	case TAME_ATTACK				: AddCurrentStatusMessage( 114,100,122, lan->OutputMessage(3,79));	break;
	case TAME_SUCCESS_CHANGE_NAME	: AddCurrentStatusMessage( 114,100,122, lan->OutputMessage(3,80));	break;//010215 lsw
	}
}

void RecvTameNameChange( t_tame_name_change *p )
{
	LPCHARACTER ch = FindCharacter( &g_CharacterList, p->id);
	if( ch == NULL ) return;
	strcpy( ch->name, p->name );
}


void RecvHostName( tame_host_name *p )
{
	LPCHARACTER ch = FindCharacter( &g_CharacterList, p->id);
	if( ch == NULL ) return;
	strcpy( ch->HostName, p->hostname );
}

//   Tamming ��..
void RecvHostEnd(  tame_end *p )
{
	LPCHARACTER ch = FindCharacter( &g_CharacterList, p->id);
	if( ch == NULL ) return;
	
	ch->HostName[0] = 0;
}

void SendBBS( char *bbs)
{
	t_packet p;
	int len;
	
	if( bbs == NULL ) return;
	len = strlen( bbs );
	if( len == 0 ) return;
	if( len >= MAX_PATH-1 ) return;
	
	p.h.header.type = CMD_BBS;
	strcpy( p.u.server_bbs.bbs, bbs );
	p.h.header.size = sizeof( t_server_bbs ) - MAX_PATH + len;
	
	QueuePacket( &p, 1 );
}

void SendBBSAll(const char *bbs)
{
	t_packet p;
	int len;
	
	if( bbs == NULL ) return;
	len = strlen( bbs );
	if( len == 0 ) return;
	if( len >= MAX_MSG-1 ) return;
	
	p.h.header.type = CMD_SV_SEND_MESSAGE_ALL;
	strcpy( p.u.kein.default_msg, bbs );
	p.h.header.size = len;
	
	QueuePacket( &p, 1 );
}

////////////// 0810 lhs ����Ʈ��  ���� //////////////
void RecvBbs_quest( char *bbs )
{
	AddCurrentStatusMessage( 200,200,145, lan->OutputMessage(3,132) );//010215 lsw
	AddCurrentStatusMessage( 255,255,145, bbs );
}


////////////////////////////////////////////////////////////////////////////////
// CSmallTipCount member functions
CSmallTipCount	g_SmallTipCount;
int LoadSmallTipsPage()
{
	FILE *fp;
	fp = Fopen( "./data/small_tips.txt", "rt" );
	if( !fp ) return 0;
	
	char str[512];
	while( fgets( str, 512, fp ) )
	{
		if( str[0] == ':' ) break;
		char *token;
		token = strtok( str, "\t\n" );
		if( !token ) continue;
		
		int number = atoi( token );
		if( !number ) continue;
		
		token = strtok( NULL, "\t\n" );
		if( !token ) continue;
		
		while( token )
		{
			g_SmallTipCount.AddData( number, token );
			token = strtok( NULL, "\t\n" );
		}
	}
	
	fclose( fp );
	return 1;
}

int CSmallTipCount::AddData( int number, char *str )
{
	CSmallTipCount* target = this;
	
	while( target->next ) target = target->next;
	if( m_nPage == 0 )
	{
		strncpy( m_szKey, str, 49 );		// 020925 YGI
		m_szKey[49] = 0;					// 020925 YGI
		m_nPage = number;
		return 1;
	}
	else
	{
		target->next = new CSmallTipCount;
		strcpy( target->next->m_szKey, str );
		target->next->m_nPage = number;
		return 1;
	}
}

int CSmallTipCount::GetPage(const char *key )
{
	if( !m_nPage ) return 0;
	
	CSmallTipCount *target = this;
	while( target )
	{
		if( strcmp( target->m_szKey, key ) == 0 ) return target->m_nPage;
		target = target->next;
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
void CallViewSmallTips(std::string& txt) {
    if (IsDead(Hero)) {
        return;
    }

    std::stringstream ss(txt);
    std::string token;
    
    // 試著讀取第一個詞
    ss >> token;

    // 如果第一個詞不存在（例如空字串），則 token 會是空字串
    if (token.empty()) {
        token = txt;
    } else {
        // 試著讀取第二個詞
        std::string temp_token;
        if (ss >> temp_token) {
            token = temp_token;
        } else {
            token = txt;
        }
    }

    static bool first = true;
    if (first) {
        LoadSmallTipsPage();
        first = false;
    }

    int t = 0;
    // 使用 std::string 版本的 EatRearWhiteChar
    std::string token_clean = token;
    EatRearWhiteChar(token_clean); 
    
    if (!token_clean.empty()) {
      t = g_SmallTipCount.GetPage(token_clean.c_str());
    }

    CloseAllMenu();
    SMenu[MN_SMALL_TIPS].key = t;
    bSmallTips = true;
}

void SendExitGuild()
{
	if (!CheckGuildCode(Hero->GetGuildCode())) 
	{	//< CSD-030324
		return;
	}	//> CSD-030324

	if( CheckGuildMaster( Hero ) ) return;		// �渶�� ���� ���ϰ�..
	CallServer( CMD_EXIT_GUILD_SELF );
}

void ViewNk(std::string& name )
{
	EatRearWhiteChar( name );
	//name = EatRearWhiteChar( name );
	LPCHARACTER target = ExistHe( name.c_str() );
	if( !target ) target = Hero;
	
	Kein_PutMessage( KM_INFO, "[%s] Vysues NK:%3d", target->name, IsNK( target, N_VYSEUS ));
	Kein_PutMessage( KM_INFO, "[%s] Zypern NK:%3d", target->name, IsNK( target, N_ZYPERN ));
	Kein_PutMessage( KM_INFO, "[%s] Yilse  NK:%3d", target->name, IsNK( target, N_YILSE ));
}

extern int IsDead( LPCHARACTER ch );

void RecvGuildChatData( t_packet *p )
{
	//if( IsDead( Hero ) ) return; To see guildchat while dead - Thralas
	
	static int first = 1;
	if( first && !g_Menu_Variable.m_bGuildChatOn )
	{
		// ������ �޽����� ���½��ϴ�. ��� ä�� ��ɾ �̿��ϼ��� /#, /���ä���ѱ�, /���ä�ò��
		Kein_PutMessage( KM_INFO, kein_GetMenuString( 70 ) );
		first = 0;
	}
	char *name = p->u.kein.guild_chat.name;
	char *msg = p->u.kein.guild_chat.msg;
	ChatMgr.AddString( name, msg, CHATTYPE_GUILD );//021001 lsw
}

void CheckShowMeDual()
{
	if( EquipItemAttr[WT_NECK].item_no == 10175 )
	{
		CallServer( CMD_SHOW_ME_DUAL_CHAR );
	}
	else
	{
		CItem *item = ItemUnit( 10175 );
          if (system_info.lang == 0) {
            if (item)
              Kein_PutMessage(KM_FAIL, kein_GetMenuString(78),
                              item->GetItemName());
          } else {
            if (item)
              Kein_PutMessage(KM_FAIL, kein_GetMenuString(78),
                              item->GetItemHanName());
          }

	}	
}
//void sendBanCommand(char* s)
//
//{
//	t_packet packet;
//	char* tok = strtok(s, " ");
//
//	if (tok == NULL)
//	{
//		AddCurrentStatusMessage(255, 0, 0, "Invalid name !");
//		return;
//	}
//	char charname[256];
//	strcpy(charname, tok);
//	tok = strtok(NULL, " ");
//
//	
//	strcpy(Reason, tok);
//
//	tok = strtok(NULL, " ");
//	char Duration[256];
//	strcpy(Duration, tok);
//	tok = strtok(NULL, " ");
//
//
//	AddCurrentStatusMessage(255, 0, 0, "Asking ban for %s %s, %s", charname, Reason, Duration);
//
//	
//	packet.u.BanAccount.characterName=charname;
//	packet.u.BanAccount.reason;
//	packet.u.BanAccount.duration = Duration;
//	strcpy(packet.u.kein.direct_map_move.name, "");
//	packet.h.header.type = CMD_DIRECT_MAP_MOVE;
//	packet.h.header.size = sizeof(k_direct_map_move) - 20;// + strlen(temp);
//	QueuePacket(&packet, 1);
//}
void SendTeleportCommand(std::string target_name, std::string target_map, std::string _x, std::string _y) //Eleval 15/08/09 - To manage RCs Teleport
{
    if (Hero->accessory[0] != 114) return;

    if (target_name.length() <= 0) {
        AddCurrentStatusMessage(255, 0, 0, "Invalid Command!");
        AddCurrentStatusMessage(255, 255, 0, "Comand param [0]who? [1]target_map [2]x [3]y");
        AddCurrentStatusMessage(255, 255, 0, "use space to split the param");
        AddCurrentStatusMessage(255, 255, 0, "if you teleport your self, you can type 'me' for name param");
        return;
    }
	
    int i = 0;
    CharToUpper(target_map.data());
    for (i = 0; i < MAX_MAP_; i++) {
        std::string map_name(MapInfo[i].mapfile);
        CharToUpper(map_name.data());
        EatRearWhiteChar(map_name.data());
        if (target_map == map_name) {
            break;
        }
    }

    if (target_map.length() <=0 || i == MAX_MAP_ )
	{
		AddCurrentStatusMessage(255, 0,0, "Invalid Map !");
		return;
	}

    int x = 0, y = 0;
    try {
        x = std::stoi(_x);
        y = std::stoi(_y);
    }
    catch (...) {

        AddCurrentStatusMessage(255, 0, 0, "Invalid CoordS !");
        return;
    }

/*	char MapToChange[128];
	strcpy(MapToChange, tok);
	
	tok = strtok(NULL, " ");

	char CoordX[32];

	if (tok == NULL) 
	{
		char lpszPW[21] = {0,};

		strcpy(lpszPW, MapToChange);
		CallServer( CMD_TELEPORT_OTHER_CHAR, lpszPW, strlen(lpszPW)  );
		return;
	}
	else
	{
		strcpy(CoordX, tok);
	}

	tok = strtok(NULL, " ");

	char CoordY[32];

	if (tok == NULL) 
	{
		AddCurrentStatusMessage(255, 0,0, "Invalid Coords !");
		return;
	}
	else
	{
		strcpy(CoordY, tok);
	}

	int iCoordX = 0;
	int iCoordY = 0;

	for(int i = 0 ; i < 3 ; i++)
	{
		if(CoordX[i] >= 48 && CoordX[i] <= 57)iCoordX = iCoordX * 10 + (CoordX[i] - 48);
		if(CoordY[i] >= 48 && CoordY[i] <= 57)iCoordY = iCoordY * 10 + (CoordY[i] - 48);
	}

	if(iCoordX < 0 || iCoordX > 999 || iCoordY < 0 || iCoordY > 999)
	{
		AddCurrentStatusMessage(255, 0,0, "Invalid Coords !");
		return;
	}

	AddCurrentStatusMessage(255, 0,0, "Asking tp at %s %d, %d", MapToChange, iCoordX, iCoordY);
*/
	t_packet packet;
	packet.u.kein.direct_map_move.map_number = ConvMapnameToMapnumber(target_map.data());
	packet.u.kein.direct_map_move.x = x;
	packet.u.kein.direct_map_move.y = y;
	strcpy(packet.u.kein.direct_map_move.name, target_name.c_str());
	packet.h.header.type = CMD_DIRECT_MAP_MOVE;
    packet.h.header.size = sizeof(k_direct_map_move);
	QueuePacket(&packet, 1);
	
}

void HideSomething(std::string text)
{
  if (text == "hat")
	{
		if(Hero->hat_is_hidden)
		{
			Hero->hat_is_hidden = false;

			t_packet packet;
			packet.h.header.type = CMD_CHAR_OPTIONS_CHANGE;
			packet.h.header.size = sizeof(t_change_char_options);
			packet.u.change_char_options.flag = false;
			packet.u.change_char_options.type = 1;

			QueuePacket(&packet, 1);
		}
		else
		{
			Hero->hat_is_hidden = true;

			t_packet packet;
			packet.h.header.type = CMD_CHAR_OPTIONS_CHANGE;
			packet.h.header.size = sizeof(t_change_char_options);
			packet.u.change_char_options.flag = true;
			packet.u.change_char_options.type = 1;

			QueuePacket(&packet, 1);
		}
		return;
	}
}