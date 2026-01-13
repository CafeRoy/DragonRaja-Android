#if !defined(AFX_MENUDEFINE_H__CCD50119_39F2_4932_8DEF_B46CD7E4330A__INCLUDED_)
#define AFX_MENUDEFINE_H__CCD50119_39F2_4932_8DEF_B46CD7E4330A__INCLUDED_

#define CENTER_X			(-1)
#define RIGHT_X				(-2)
#define CENTER_Y			(-3)
#define GABX_SCREEN			80		// 800*600 變為 640*480 時的間距調整值
#define GABY_SCREEN			60

#define MAX_MONEY			1000000	// 玩家能擁有的最大金錢數量
#define MAX_FIELD			50	// 一個菜單中能包含的最大欄位數
#define MAX_STRING			500	// 菜單中文字的最大長度
#define MAX_CHEAKARRAY		3 // 檢查陣列欄位時使用的陣列最大值
#define MAX_SHN				50	// FT_HIDE_NOMAL_ 類型的隱藏物件最大數量
#define MAX_SHN_FIELD		15	// FT_HIDE_NOMAL_ 類型物件中的最大欄位數量
#define MAX_CHARACTERDATA	12	// nCharacterData 中能包含的角色資料數
#define MAX_MENUDATA		40	// 一次可以繪製的最大菜單數
#define MAX_FILENAME		100
#define MAX_LEARN_ITEM		1000

#define MEMORIZE_MAX			200		// 魔法記憶槽的最大值...

#define MAX_SYSDAT				1100		// sysmenu.dat 中讀取的圖片數，或可能是圖片編號的最大值...
#define MAX_IMAGE2				40
#define MAX_MAGIC				251			// 魔法的最大數量
#define GOD_MENU_IMG_MAX		50
#define TIME_MENU_IMG_MAX		10
#define MEMBOOK_MENU_IMG_MAX	10
#define MAGICBOOK_MENU_IMG_MAX	20
#define MAIL_MENU_IMG_MAX		60
#define START_ETC_IMG_MAX		40			
#define START_ETC2_IMG_MAX		10
#define JOB_ETC_IMG_MAX			20
#define START_MAP_IMG_MAX		35			// 從 15 增加
#define CHAT_MENU_IMG_MAX		10
#define MAIN_ETC_IMG_MAX		70
#define MAIN_ETC3_IMG_MAX		10
#define SMALL_MAP_MAX			50			// 小地圖點陣圖最大值
#define SMALL_ETC_MAX			20
#define SKILL_MENU_IMG_MAX		30			// 技能菜單等圖片最大值
#define PUBLIC_BBS_MENU_IMG_MAX 10			// 公告板菜單
#define HUNTER_MENU_IMG_MAX		40			// 獵人公告板
#define QUEST_MENU_MAX			10
#define FAME_MENU_MAX			1
#define COLOR_CHANGE_MENU_MAX	10			// 角色顏色變更菜單最大值
#define ITEM_MAKE_IMAGE_MAX		15			// 物品製作圖片最大值
#define NATION_MENU_IMG_MAX		30			// 國家公告板菜單
#define START_FIGHT_IMG_MAX		25			// 開始戰鬥
#define STOP_FIGHT_IMG_MAX		15			// 停止戰鬥
#define FIGHTMAP_TIME_IMG_MAX	50			// 時間顯示
#define FIGHTMAP_WIN_IMG_MAX	120			// 勝利
#define FIGHTMAP_LOSE_IMG_MAX	90			// 失敗

#define VOTE_MENU_IMG_MAX		20			// 投票菜單
#define GUILD_MARK_MAX			512			// guild mark

#define GUILD_ADMIN_MAX			20			// guild 管理員圖片最大值
#define GUILD_SKILL_LIST_MAX	20			// 公會技能列表
#define GUILD_ADMIN_ETC_MAX		10			// guild 管理員額外圖片
#define HELP_PCX_MAX			20			// tip 按鈕圖片
#define BANK_MAX				10			// 銀行介面
#define SALVATION_MAX			20			// 贖罪菜單
#define SALVATION_ANI_MAX		10			// 贖罪角色動畫
#define TEAM_BATTLE_MAX			10			// 團隊戰鬥
#define SELECT_SERVER_IMG_MAX	10			// 選擇伺服器菜單
#define CAHNGE_MONEY_MAX		10

#define GUILD_WAIT_SPR_MAX		30
#define MERCHANT_SPR_MAX		30	
#define GUILD_MENU_SPR_MAX		80
#define DUAL_IMG_SPR_MAX		30
#define DUAL_MARK_SPR_MAX		10

#define TUTORIAL_DISPLAY_SPR_MAX 30

//011013 lsw >
#define ITEM_MAKE_RARE_SELECT_SPR_MAX 30
//011013 lsw <

//011014 lsw >
#define TUTORIAL_DISPLAY_SPR_TMP_MAX 30
//011014 lsw <

#define RUNE_SPR_MAX			70
#define RUNE_EFFECT_SPR_MAX		20	
#define SKIN_MENU_MAX			20


#define	GM_2ND_SKILL_SPR_MAX			30	//010708 lsw 2dn GM 技能介面
#define NATION_BATTLE_BBS_SPR_MAX		35	// 010730 lsw 國家戰爭公告板
#define	NATION_INFO_SPR_MAX				35	// 010730 lsw
#define	NATION_BUDAE_SPR_MAX			35	// 010730 lsw	
#define	NATION_LADDER_SPR_MAX			35	// 010730 lsw	
#define	NATION_VOTE_SPR_MAX				35	// 010730 lsw
#define	NATION_JOIN_SPR_MAX				35	// 010730 lsw
#define	NATION_NOW_SPR_MAX				45	// 010730 lsw
#define	NATION_GUEST_SPR_MAX			35	// 010730 lsw
#define	NATION_GOTO_WAR_MAP_SPR_MAX		35	// 010730 lsw
#define	NATION_MSGBOX_SPR_MAX			35	// 010730 lsw
#define	NATION_DONEBOX_SPR_MAX			35	// 010730 lsw
#define	NATION_TODAY_NOTICE_SPR_MAX		35	// 010730 lsw
#define	NATION_WAR_RESULT_SPR_MAX		35	// 010910 lsw
#define	NATION_WAR_START_SPR_MAX		35	// 010925 lsw

#define	NEW_CHAT_SPR_MAX		30 // 010928 lsw 3 行聊天
#define MAX_MESSAGE				200
#define MAX_SKILL_KIND			45

#define GAMBLE_SPR_MAX			15
#define EXP_GAUGE_MAX			15

#define GAMBLE_EFFECT_MAX				20
#define	ABILITY_REDISTRIBUTION_SPR_MAX	5	// 011201 LTS	// LTS MENU
#define IM_GUILD_MEMBER_LIST			20	// IMAGE_MAX
#define LOCALWAR_JOIN_SPR_MAX			40	// 011201 LTS
#define IM_FRIEND_MENU					20
#define	LOCALWAR_OK_SPR_MAX				50	// LTS NEW LOCALWAR
#define EVENT_LOCALWAR_OK_SPR_MAX		20	// 020115 LTS
#define	EVENT_LOCALWAR_MOVE_SPR_MAX		20

#define WAR_START_SPR_MAX				20
#define WAR_MOVE_SPR_MAX				20
#define WAR_STATUS_SPR_MAX				30

#define MONSTERWAR_INFO_SPR_MAX			10	// BBD 040311

#define IM_POTAL_MENU					30
#define IM_PARTY_BUTTON					10
#define IM_GUILD_ITEM					20
#define IM_GUILD_CHANGE_INFO			20
////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//菜單編號定義


enum MenuIndex
{
	MN_MAKECHARACTER_BASIC = 0,	// 角色創建菜單中的基本選擇區域
	MN_MAINSTART_BASIC = 1,	// 遊戲開始菜單的背景區域
	MN_MAINSTART_START = 2,	// 點擊開始畫面上的 START 按鈕後進入的菜單
	MN_MAKECHARACTER_CHEAK1 = 3,	// 角色創建菜單中的確認窗口 1
	MN_MAKECHARACTER_CHEAK3 = 4,	// 角色創建菜單中的確認窗口 3
	MN_MAKECHARACTER_GENDER = 5,	// 角色創建菜單中的性別選擇菜單
	MN_MAKECHARACTER_NAME = 6,	// 角色創建菜單中的名稱輸入菜單
	MN_MAKECHARACTER_FACE = 7,	// 角色創建菜單中的臉部選擇菜單
	MN_MAKECHARACTER_ARIGEMENT = 8,	// 角色創建菜單中的陣營選擇菜單
	MN_MAKECHARACTER_CLASS = 9,	// 角色創建菜單中的職業選擇菜單
	MN_MAKECHARACTER_SPELL = 10,	// 角色創建菜單中的法術選擇菜單
	MN_MAKECHARACTER_COLOR = 11,	// 角色創建菜單中的顏色選擇菜單
	MN_MAKECHARACTER_TACTICS_MAN = 12,	// 角色創建菜單中的男性戰術選擇菜單
	MN_MAKECHARACTER_TACTICS_WOMAN = 13,	// 角色創建菜單中的女性戰術選擇菜單
	MN_MAKECHARACTER_JOB = 14,	// 角色創建菜單中的工作選擇菜單
	MN_MAKECHARACTER_ABILITY = 15,	// 角色創建菜單中的能力值調整菜單
	MN_MAININTERFACE = 16,	// 主介面菜單
	MN_STATUS = 17,	// 狀態介面菜單
	MN_ABILITY = 18,	// 能力值菜單
	MN_NATION_MAIN_VA = 19,		// 國家菜單 VA
	MN_NATION_MAIN_ZY = 20,
	MN_NATION_MAIN_ILL = 21,
	MN_VISA_MENU = 22,		// 簽證菜單
	MN_SCALE = 23,		// 比例菜單
	MN_GUILD_ITEM_OR_MEMBER_LIST = 24,
	MN_ITEM = 25,	// 物品欄菜單
	MN_ARIGEMENT = 26,	// 從狀態/能力菜單切換時顯示的陣營菜單
	MN_RUNE = 27,
	MN_SKIN = 28,
	MN_GAMBLE = 29,
	MN_GUILD_MAIN = 30,		// 公會主菜單
	MN_GUILD_EXPLAIN = 31,		// 公會物品說明菜單
	MN_GUILD_REGIST_IMSI = 32,
	MN_GUILD_REGIST_DEGREE = 33,		// 公會等級菜單
	MN_GUILD_REGIST = 34,		// 處理公會註冊請求的公會菜單
	MN_GUILD_INTRODUCTION = 35,
	MN_GUILD_CHANGE_INFO = 36,		// 公會資訊變更菜單
	MN_FRIEND_MENU = 37,		// 朋友系統
	MN_FRIEND_REGIST = 38,
	MN_POTAL_MENU = 39,
	MN_DEFAULT_MESSAGEBOX = 40,		// 預設訊息框
	MN_VIEW_TIP = 41,		// 顯示提示資訊的按鈕
	MN_SALVATION2 = 42,
	MN_CHANGE_MONEY = 43,		// 換錢菜單
	MN_SELECT_SERVER = 44,		// 選擇伺服器菜單
	MN_DRINK_RUMOR_VIEW = 45,
	MN_ARENA_LOBBY = 46,
	MN_MERCHANT_MAIN = 47,		// 商人菜單主頁
	MN_MERCHANT_BBS = 48,		// 商人菜單
	MN_SALVATION = 49,		// 贖罪菜單
	MN_FACE = 50,	// 主介面中彈出顯示臉部表情的菜單
	MN_BANK_DEPOSIT = 51,	// 銀行菜單中的存款
	MN_BANK_DEFRAYAL = 52,	// 銀行菜單中的提款
	MN_BANK_CUSTODY = 53,	// 銀行菜單中的保管
	MN_BANK_MAIN = 54,	// 銀行菜單中的尋找
	MN_BANK_LOAN = 55,	// 銀行菜單中的貸款
	MN_BANK_REPAYMENT = 56,	// 銀行菜單中的還款
	MN_BANK_AUCTION = 57,	// 銀行菜單中的拍賣
	MN_POTAL_SELECT = 59,		// 傳送門移動時的選擇菜單
	MN_PARTY = 60,	// 主介面中的隊伍菜單
	MN_GUILD_ITEM_GET = 61,
	MN_GUILD_ITEM_PUT = 62,
	MN_GUILD_MEMBER_LIST = 63,
	MN_OKCANCEL_FILE_TEXT = 64,
	MN_HTML_MESSAGEBOX = 65,		// 可使用 HTML 標籤顯示文字的訊息框 (使用 CallHtmlMessageBox())
	MN_NATION_ITEM = 66,
	MN_REVIVAL = 67,
	MN_GUILD_HOUSE = 68,		// 公會房屋管理
	MN_SMALL_TIPS = 69,		// 小提示菜單
	MN_LOGO_START = 70,		// 會員登入
	MN_LODING = 71,		// 載入畫面
	MN_LOGIN = 72,		// 登入或帳號創建
	MN_LOGON = 73,		// 登入時出現的菜單，無法在此處開始新遊戲
	MN_SELECT = 74,		// 選擇角色
	MN_LOCATION = 75,		// 選擇角色位置
	MN_BANK_AUCTION_SALE = 76,		// 拍賣菜單中的出售菜單
	MN_PARTY_FACE1 = 77,		// 隊伍成員頭像顯示窗口 1
	MN_PARTY_FACE2 = 78,
	MN_PARTY_FACE3 = 79,
	MN_PARTY_FACE4 = 80,
	MN_PARTY_FACE5 = 81,
	MN_PARTY_FACE6 = 82,
	MN_GOD_EDELBLHOY = 83,		// 神殿 Edelbroy
	MN_GOD_TEFFERY = 84,		// 神殿 Teffery
	MN_GOD_LETTY = 85,		// 神殿 Letty
	MN_EFFECT_TOOL = 86,		// 效果工具菜單
	MN_GOD_OREM = 87,		// 神殿 Orem
	MN_MESSAGEBOX_BIG = 88,		// 程式碼中可以呼叫的大型訊息框
	MN_MESSAGEBOX_SMALL = 89,
	MN_YESNO = 90,		// YES-NO 菜單
	MN_DISTANCE_TABLE = 91,		// 距離表
	MN_STORE_SIGN = 92,		// 商店招牌
	MN_MAGIC_GUILD = 93,		// 魔法公會
	MN_ACADEMY = 94,		// 學院
	MN_SKILLGUILD_KNOWLAGE = 95,		// 學習知識
	MN_AWAKE = 96,		// 覺醒按鈕
	MN_TOWNHALL_BBS1 = 97,		// 鎮政廳公告板 (有 OK, Cancel)
	MN_TOWNHALL_BBS2 = 98,		// 鎮政廳公告板 (有 Join, Cancel)
	MN_TOWNHALL_BBS3 = 99,		// 鎮政廳公告板 (有 Write, Exit)
	MN_BLACKSMITH = 100,		// 鐵匠鋪菜單
	MN_FARM = 101,
	MN_RESTAURANT = 102,
	MN_HERBSHOP = 103,
	MN_SHAMBLES = 104,
	MN_CANDLE = 105,
	MN_SLOPSHOP = 106,
	MN_ALCHEMY = 107,
	MN_WOODWORKING = 108,
	MN_BOWCRAFT = 109,
	MN_SHIPYARD = 110,
	MN_SKILLGUILD_BUY = 111,
	MN_SKILLGUILD_SELL = 112,
	MN_SKILLGUILD_SKILL = 113,
	MN_SKILLGUILD_ITEM = 114,
	MN_SKILLGUILD_REPAIR = 115,
	MN_ITEM_EXPLAN = 116,
	MN_TOWNHALL = 117,	// 鎮政廳介面
	MN_TOWNHALL_PUBLICNOTICE = 118,	// 鎮政廳菜單中的公告菜單
	MN_TOWNHALL_BUILDING = 119,	// 鎮政廳菜單中的建築菜單
	MN_TOWNHALL_COMMERCE1 = 120,	// 鎮政廳菜單中的商業菜單 1
	MN_TOWNHALL_COMMERCE2 = 121,	// 鎮政廳菜單中的商業菜單 2
	MN_TOWNHALL_COMMERCE3 = 122,	// 鎮政廳菜單中的商業菜單 3 (出售窗口)
	MN_TOWNHALL_HOUSEREPAIR = 123,	// 鎮政廳菜單中的房屋修復菜單
	MN_TOWNHALL_GUILD = 124,	// 鎮政廳菜單中的組織公會菜單
	MN_TOWNHALL_FORUM = 125,	// 鎮政廳菜單中的論壇菜單
	MN_TOWNHALL_EVENT = 126,	// 鎮政廳菜單中的活動菜單
	MN_TOWNHALL_RANK1 = 127,	// 鎮政廳菜單中的排名菜單 1
	MN_TOWNHALL_RANK2 = 128,	// 鎮政廳菜單中的排名菜單 2
	MN_TOWNHALL_NOTICE4 = 129,	// 鎮政廳菜單中的公告菜單 4
	MN_TOWNHALL_NOTICE3 = 130,	// 鎮政廳菜單中的公告菜單 3
	MN_TOWNHALL_NOTICE2 = 131,	// 鎮政廳菜單中的公告菜單 2
	MN_TOWNHALL_NOTICE1 = 132,	// 鎮政廳菜單中的公告菜單 1
	MN_TOWNHALL_COMMERCE4 = 133,	// 鎮政廳菜單中的商業菜單 4 (購買窗口)
	MN_TOWNHALL_CONSTRUCTION = 134,	// 鎮政廳菜單中的建設菜單
	MN_GUILD_HOUSE_INFO = 135,
	MN_SCENARIO_TRAP_INFO = 136,		// 情景陷阱資訊
	MN_SCENARIO_TRAP_KEY = 137,		// 情景陷阱按鍵

	MN_WAR_MOVE = 138,
	MN_WAR_STATUS = 139,

	MN_CHATTING = 140,		// 聊天窗口
	MN_GOD1 = 141,	// 神殿菜單 1
	MN_GOD2 = 142,	// 神殿菜單 2
	MN_GOD3 = 143,	// 神殿菜單 3
	MN_LADDER_RANK = 144,		// 聯盟天梯排名公告板
	MN_MAGICBOOK = 145,	// 魔法書
	MN_FUNCTION = 146,	// 功能按鈕
	MN_SYSTEM = 147,	// 系統菜單
	MN_SYSTEM_OPTION = 148,	// 系統菜單中的選項菜單
	MN_DEFAULT_MESSAGEBOX2 = 149,

	MN_DEL_CHAR_COMFORM = 150,
	MN_SELECT_TUTORIAL_LOGIN = 151,
	MN_WAR_START = 152,

	MN_SYSTEM_LOGOUT = 153,
	MN_DRINK_MAIN = 154,		// 酒館
	MN_DRINK_RUMOR = 155,		//
	MN_DRINK_BUY = 156,		//
	MN_SHOP_MAIN = 157,		// 商店主頁
	MN_SHOP_BUY = 158,		//
	MN_SHOP_SELL = 159,		//
	MN_HEAL_MAIN = 160,		// 治療中心
	MN_HEAL_HEALING = 161,		// 治療
	MN_HEAL_PREVENTING = 162,		// 預防
	MN_HEAL_LEARN = 163,		// 學習技能
	MN_SKILL_LEARN = 164,		// 學習技能
	MN_INN_MAIN = 165,
	MN_INN_REST = 166,
	MN_INN_SLEEP = 167,
	MN_SELECT_CHARACTER = 168,		// 角色選擇窗口
	MN_TACTICS = 169,		// 戰術窗口
	MN_LEVELUP = 170,		// 升級窗口
	MN_MAKECHARACTER_AGE = 171,		// 角色創建中的年齡選擇
	MN_MAKECHARACTER_SPELL2 = 172,		// Wizard 和 Priest 的法術選擇
	MN_INFO = 173,
	MN_CREDIT = 174,
	MN_OKCANCEL_MESSAGEBOX = 175,		// OK-CANCEL 訊息框
	MN_OK_MESSAGEBOX = 176,		// OK 按鈕訊息框
	MN_ITEM_MESSAGEBOX = 177,		// 物品相關訊息窗口
	MN_DIVIDE_ITEM = 179,		// 分割物品數量窗口
	MN_MAGIC_BOOK = 180,		// 魔法書
	MN_ITEM_MAKE = 181,		// 物品製作窗口
	MN_ITEM_MAKE_SELECT = 182,		// 可製作物品列表
	MN_ITEM_MAKE_OK = 183,		// 物品製作完成確認
	MN_CHARACTER_ITEM = 184,		// 其他角色持有/掉落的物品窗口
	MN_ITEM_SELECT = 185,		// 選擇要使用的技能物品窗口
	MN_LEARN_MAGIC_BOOK = 186,		// 學習魔法書
	MN_MEM_BOOK = 187,		// 記憶魔法書
	MN_SMALL_MAP = 188,		// 小地圖
	MN_EXCHANGE_ITEM = 189,		// 物品交換
	MN_DUAL_MAIN = 190,		// 決鬥介面
	MN_INIT_ABILITY = 191,
	MN_GOD_INTRUDUCTION = 192,		// 神的介紹菜單
	MN_GOD_MEETING = 193,		// 神的會面菜單
	MN_GOD_REGIST1 = 194,		// 信奉神 Wizard
	MN_GOD_PREACH = 195,		// 宣傳我神菜單
	MN_GOD_REGIST_ERROR = 196,		// 信奉神失敗情況
	MN_GOD_REGIST_MESSAGE = 197,		// 信奉神訊息處理
	MN_GOD_PREACH_ERROR1 = 198,
	MN_GOD_PREACH_RESULT = 199,
	MN_GOD_MEETING_MAIN = 200,		// 時間到時彈出的神菜單
	MN_GOD_SUPPER = 201,		// 超級背包中的神菜單
	MN_TIME_DISPLAY_MENU = 202,		// 時間顯示菜單
	MN_MAIL_MENU = 203,
	MN_MAIL_WRITE = 204,
	MN_MAIL_READ = 205,
	MN_RESIST_FRIEND = 207,		// 遠程朋友註冊菜單
	MN_MAIN_EXPLAIN = 208,		// 主菜單說明窗口
	MN_QUICK_MAGIC_SCROLL = 209,		// 快捷魔法陣列超過 5 個時的捲動處理
	MN_QUICK_MAGIC_PLUS = 210,		// 快捷魔法陣列菜單
	MN_TIME_DISPLAY_ON_OFF = 211,		// 菜單開關/激活按鈕所在的菜單
	MN_START_MAP = 212,		// 初始開始地圖菜單
	MN_PUBLIC_BBS = 213,		// 公告板菜單
	MN_QUICK_SKILL = 214,		// 快捷技能窗口
	MN_HUNTER_MAIN = 215,		// 獵人公告板主頁
	MN_HUNTER_REGIST = 216,		// 獵人註冊菜單
	MN_HUNTER_LIST = 217,		// 獵人列表菜單
	MN_QUEST = 218,		// 任務菜單
	MN_FAME = 219,		// 名聲菜單
	MN_QUEST_INDEX = 220,		// 任務索引
	MN_RELATION = 221,		// 關係菜單
	MN_MAN_TO_MAN = 222,		// 1:1 交易系統
	MN_RESET_JOB = 223,		// 職業重置
	MN_RESET_JOB_EXPLAIN = 224,		// 職業重置說明窗口
	MN_CHARACTER_COLOR_CHANGE = 225,		// 角色顏色變更
	MN_SELECT_NATION = 226,		// 國家選擇
	MN_NATION_EXPLAIN = 227,		// 國家說明
	MN_NATION_BBS = 228,		// 國家公告板
	MN_FIGHT_MAP_SIGN = 229,		// 確認戰鬥地圖標誌
	MN_SCENARIO = 230,		// 情景/劇情相關菜單
	MN_VOTE_REGIST = 231,		// 投票註冊
	MN_VOTE_SUPPORT = 232,		// 投票支持
	MN_VOTE_SIGN = 233,		// 投票
	MN_GUILD_ADMIN = 234,		// 公會管理菜單
	MN_GM_LIST = 235,		// 公會成員列表
	MN_GM_ITEM_EXPLAIN = 236,		// 公會物品說明
	MN_DISPLAY_SKILL_EXP = 238,		// 顯示技能經驗值菜單
	MN_DISPLAY_TAC_EXP = 239,
	MN_DISPLAY_MAGIC_EXP = 240,
	MN_GM_2ND_SKILL = 241,
	MN_NATION_BATTLE_BBS = 242,
	MN_NATION_INFO = 243,
	MN_NATION_VOTE = 244,
	MN_NATION_BUDAE = 245,
	MN_NATION_NOW = 246,
	MN_NATION_LADDER = 247,
	MN_NATION_JOIN = 248,
	MN_NATION_GUEST = 249,
	MN_NATION_GOTO_WAR_MAP = 250,
	MN_NATION_MSGBOX = 251,
	MN_NATION_DONEBOX = 252,
	MN_NATION_TODAY_NOTICE = 253,
	MN_NATION_WAR_RESULT = 254,
	MN_NATION_WAR_START = 255,
	//< soto-030425
	MN_LUCKY_MENU = 256,
	MN_LOTTO_MENU = 257,
	MN_WINNER_MENU = 258,// 確認獲勝者
	//> soto-030425


	MN_MERCHANT_FIND,// 商人查找器 -> 與 MN_MERCHANT_BUY 相同 (用於查找)
	MN_MERCHANT_BUY,// 商人購買
	MN_MERCHANT_SELL_INFO,// 商人出售 (賣家自己的狀態)
	MN_MERCHANT_REGISTER,// 商人註冊
	MN_MERCHANT_RESULT_TAKE,// 提取商人結果
	MN_MERCHANT_DIRECT_EXCHAGE_LIST,// 直接交易購買列表
	NM_SCRIPT_TIMMER,	// 腳本計時器計數顯示
	NM_SCRIPT_COUNTER,	// 腳本計數器計數顯示

	NM_MERCHANT_QUEST,	// 商人任務

	MN_ITEM_MAKE_RARE_SELECT,
	MN_COMBAT_SKILL_POINT,
	MN_COMBAT_SKILL_DOING,
	MN_ABILITY_REDISTRIBUTION,
	MN_LOCALWAR_JOIN,
	MN_LOCALWAR_OK,
	//MN_EVENT_LOCALWAR_OK			,
	//MN_EVENT_LOCALWAR_MOVE			,
	MN_WISPER_LIST,
	MN_GHOST_TO_ALIVE,
	MN_SKILL_MASTER_LIST,
	MN_SKILL_MASTER_MAKE,// 技能大師製作
	MN_TUTORIAL_DISPLAY = 280,
	MN_EXCHANGE_BOND_MONEY = 281,// 交換綁定金

	MN_OBSERVER_MODE,
	MN_BETTING,
	MN_EVENT_LOCALWAR_OK = 293, // 活動地方戰爭 OK
	MN_EVENT_LOCALWAR_MOVE = 294,
	MN_MERCHANT_BACKUP_MAIN = 295,	// BBD 040303
	MN_MAINSTART_BACK,
	MN_MANTLE_COLOR_CHANGE,
	MN_ITEM_MALL,
	MN_BANK_CUSTODY2,
	MAX_MENU// 最大菜單數量 (此 enum 必須位於最後)
};
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
//FT_HIDE_NOMAL_ 中使用的隱藏物件編號定義
#define HN_MAKECHARACTER_GENDER_RECT			0	// 角色創建菜單中，性別選擇欄位的矩形區域
#define HN_MAKECHARACTER_GENDER_TEXT			20	// 角色創建菜單中，性別選擇欄位的文字

#define HN_MAKECHARACTER_FACE					1	

#define HN_MAKECHARACTER_ARIGEMENT_TEXT			21	// 角色創建菜單中，陣營選擇欄位的文字
#define HN_MAKECHARACTER_ARIGEMENT_RECT			2	// 角色創建菜單中，陣營選擇欄位的矩形區域

#define HN_MAKECHARACTER_CLASS_TEXT				22	// 角色創建菜單中，職業選擇欄位的文字
#define HN_MAKECHARACTER_CLASS_RECT				3	// 角色創建菜單中，職業選擇欄位的矩形區域

#define HN_MAKECHARACTER_SPELL_RECT				4	// 角色創建菜單中，法術選擇欄位的矩形區域
#define HN_MAKECHARACTER_SPELL_TEXT				23	// 角色創建菜單中，法術選擇欄位的文字

#define HN_MAKECHARACTER_COLOR_SELECT			5	// 顏色選擇
//#define HN_MAKECHARACTER_COLOR_HAIR			5	// 角色創建菜單中，選中的頭髮顏色
//#define HN_MAKECHARACTER_COLOR_SKIN			6	// 角色創建菜單中，選中的皮膚顏色
//#define HN_MAKECHARACTER_COLOR_CLOTHES		7	// 角色創建菜單中，選中的衣服顏色
//#define HN_MAKECHARACTER_COLOR_ORNAMENT		8	// 角色創建菜單中，選中的飾品顏色

#define HN_MAKECHARACTER_TACTICS_MAN_RECT		9	// 角色創建菜單中，男性戰術選擇欄位的矩形區域
#define HN_MAKECHARACTER_TACTICS_MAN_TEXT		24	// 角色創建菜單中，男性戰術選擇欄位的文字

#define HN_MAKECHARACTER_TACTICS_WOMAN_RECT		10	// 角色創建菜單中，女性戰術選擇欄位的矩形區域
#define HN_MAKECHARACTER_TACTICS_WOMAN_TEXT		25	// 角色創建菜單中，女性戰術選擇欄位的文字

#define HN_MAKECHARACTER_JOB_RECT				11	// 角色創建菜單中，工作選擇欄位的矩形區域
#define HN_MAKECHARACTER_JOB_TEXT				26	// 角色創建菜單中，工作選擇欄位的文字

#define HN_MAKECHARACTER_ABILITY				12	// 角色創建菜單中，能力值調整欄位 (顯示能力值本身)

#define HN_TEMPLE_INTRODUTION_TEXT				13	// 神殿介紹
#define HN_TEMPLE_OPERATION_TEXT				15	// 神殿操作
#define HN_TEMPLE_TEMPLE1_TEXT					17	// 神殿 1
#define HN_TEMPLE_TEMPLE2_TEXT					19	// 神殿 2

#define HN_GUILD_INTRODUTION_TEXT				27	// 公會介紹
#define HN_GUILD_OPERATION_TEXT					28	// 公會操作
#define HN_GUILD_GUILD1_TEXT					29	// 公會 1
#define HN_GUILD_GUILD2_TEXT					30	// 公會 2
#define HN_INVENTORY							31	// 物品欄窗口 (用於處理捲動等)
#define HN_PARTY_SELECT							32	// 主介面/隊伍菜單中，隊伍按鈕列表

#define HN_SKILL_MAIN_TEXT						33	// 技能菜單中的主頁文字

#define HN_TOWNHALL_TEXT						34	// 鎮政廳菜單中的菜單文字
#define HN_TOWNHALL_CONSTRUCTION_TEXT			35	// 鎮政廳菜單中，建設菜單的文字

#define HN_SYSTEM_TEXT							36	// 介面系統菜單中的文字
#define HN_SYSTEM_HELP_TEXT						37	// 介面系統菜單中的幫助文字

#define HN_DRINK_MAIN_TEXT						38	// 酒館主菜單的文字
#define HN_SHOP_MAIN_TEXT						39	// 商店主頁
#define HN_HEAL_MAIN_TEXT						40	// 治療中心
#define HN_INN_MAIN_TEXT						41	// 旅館
#define HN_INN_REST_TEXT						42	// 旅館休息

#define HN_HEAL_PREVENTING						43	// 預防中心
/////////////////////////////////////////////////////////////////////////////////
//Ư���� �Ͼ�� �� ���� ���� ��� ���ÿ� ���� �޴��� �Ҽ�ȭ ��Ű�ų� � ������ üũ�� �޴��� ���ų�
/////////////////////////////////////////////////////////////////////////////////
// 當點擊按鈕或特定欄位時，呼叫特定菜單或執行某些動作
enum SpecailWillDoIndex//FT_xxx_SPECIAL_WILLDO 類型欄位被點擊時的動作
{
	SWD_NONE = 0,
	SWD_ENTER_MAKECHARACTER,		// 從開始畫面進入角色創建畫面
	SWD_MAKECHARACTER_ENTERNAME,		// 在角色創建菜單中進入輸入名稱的欄位
	SWD_MAKECHARACTER_SPELLORCOLOR,		// 在職業菜單中選擇職業後，切換到法術或顏色選擇
	SWD_MAKECHARACTER_TACTICS,		// 決定好職業後，進入戰術選擇
	SWD_MAKECHARACTER_ABILITY,		// 進入依據職業的能力值菜單
	SWD_FOCUS_WILLDO,
	SWD_CALLMENU,		// 呼叫菜單
	SWD_MN_MERCHANT_BBS_OK,		// 商人公告板
	SWD_CALLSMALLMENU,		// 呼叫小菜單
	SWD_MAININTERFACE_STATUS,		// 在主介面中彈出狀態菜單
	SWD_MAININTERFACE_ABILITYTOSTATUS,		// 從能力菜單切換到狀態菜單
	SWD_MAININTERFACE_MAGIC,		// 在主介面中彈出魔法菜單 (如果使用裝備欄位，魔法菜單是第一個)
	SWD_MAININTERFACE_ITEM,		// 在主介面中彈出物品菜單
	SWD_MAININTERFACE_ABILITY,		// 在主介面中彈出狀態能力菜單
	SWD_MAININTERFACE_ARIGEMENT,		// 在狀態菜單中切換到能力菜單 (陣營/善惡值)
	SWD_MAININTERFACE_PARTY,		// 在主介面中彈出隊伍菜單
	SWD_ARIGEMENT_STATUS,		// 無條件返回到狀態菜單
	SWD_MAININTERFACE_FACE,		// 在主介面中彈出角色表情選擇窗口
	SWD_FACE_CHOOSE,		// 選擇表情
	SWD_FACE_AUTOCLOSE,		// 自動關閉表情菜單
	SWD_UNMOUSEFOCUS,		// 使被滑鼠焦點選中的菜單無效
	SWD_LMENUCLOSE,		// 在主介面中關閉左側菜單
	SWD_RMENUCLOSE,		// 在主介面中關閉右側菜單
	SWD_YESNO_YES,		// 在 YES-NO 菜單中選擇 "是" (YES)
	SWD_YESNO_NO,		// 在 YES-NO 菜單中選擇 "否" (NO)
	SWD_CALL_OTHER,		// 呼叫菜單之外的其他特殊菜單
	SWD_IDPASS,		// 啟動 ID 和密碼的輸入框
	SWD_TURN_BUTTON,		// 主介面中切換的按鈕
	SWD_IDEDITBOX,		// ID 輸入框
	SWD_PASSEDITBOX,		// 密碼 輸入框
	SWD_MAKECHARACTER_GENDERBACK,		// 在性別選擇菜單中按返回到開始菜單
	SWD_SMALLMENU_END,		// 結束小菜單 (用作取消鍵/ESC)
	SWD_BANK_OK,		// 銀行菜單中的 OK 按鈕
	SWD_BANK_CANCEL,		// 銀行菜單中的 CANCEL 按鈕
	SWD_BANK_ITEM,		// 銀行物品相關
	SWD_BANK2_ITEM,
	SWD_BANK_ITEM_END,
	SWD_TWO_MENU_OPEN,		// 同時打開兩個菜單。使用 nWillDo 和 nSHideNomalNumber。用於兩個菜單互相切換的情況
	SWD_SIMPLE_CLOSE,		// 簡單關閉主介面的菜單 (不適用於兩個以上同時打開的菜單)
	SWD_SMALL_MENU,
	SWD_MENU_OPEN,		// 在主介面中僅打開菜單
	SWD_ITEM_SELLBUY_OK,		// 在物品買賣菜單中點擊 OK 按鈕
	SWD_JOBSKILL_SCROLL_UP,		// 物品買賣菜單中的向上捲動按鈕
	SWD_JOBSKILL_SCROLL_DOWN,		// 物品買賣菜單中的向下捲動按鈕
	SWD_ITEM_SELLBUY_CANCEL,		// 物品買賣菜單中的取消
	SWD_LOGON,		// 在開始菜單中登入時，檢查 ID 和密碼
	SWD_LOGON_OK,
	SWD_TOWN_CONSTRUCTION,		// 城鎮建設
	SWD_EDIT_BOX,		// 啟動新的輸入框
	SWD_MY_HOME,		// 在 MN_MY_HOME 中按返回
	SWD_INFO_FLAG_SET,		// 點擊後設定介面顯示旗標
	SWD_NORMAL_CHECK_PUT,		// 點擊後改變其他欄位的目標值。
	SWD_QUIT,		// 退出遊戲
	SWD_SELL_BUY,		// 買賣
	SWD_LEARN,		// 學習技能
	SWD_CHARACTER_SELECT,		// 選擇角色
	SWD_CHARACTER_DEL_ADD,
	SWD_STATUS_TACTICS,		// 從狀態菜單切換到戰術菜單
	SWD_TACTICS_STATUS,		// 從戰術菜單切換到狀態菜單
	SWD_PARTY_BUTTON,
	SWD_BACK_SKILL,
	SWD_SKILLGUILD_LEARN,		// 學習技能，學習魔法
	SWD_LEARNSKILL_OTHER,
	SWD_TOWNHALL_TITLE,		// 城鎮公告板標題繪製
	SWD_TOWNGALL_TITLE_READ,		// 進入公告板內容閱讀窗口
	SWD_OKCANCEL_OK,
	SWD_OKCANCEL_CANCEL,
	SWD_BBS_READ,		// 點擊公告板內容時查看詳細
	SWD_SUBMENU_BACK,		// 從子菜單返回上一個菜單 (使用 nTemp)
	SWD_EVENT_JOIN,		// 在活動菜單中加入
	SWD_PARTY_RELEASE,		// 解散隊伍
	SWD_SELECT_HOUSEKIND,		// 選擇房屋種類
	SWD_BUY_HOUSE,		// 購買房屋
	SWD_WILLDO,		// 執行菜單中的 WillDo 動作
	SWD_MOVE_OPEN_CLOSE,		// 打開或關閉帶有標籤的菜單。使用 nWillDo
	SWD_DIVIDE_OK,		// 物品分割窗口中的 OK 按鈕
	SWD_ITEM_MAKE,		// 按下物品製作按鈕
	SWD_ITEM_MAKE_CANCEL,		// 取消物品製作按鈕
	SWD_ITEM_MAKE_SELECT_OK,		// 選擇物品後，開始製作
	SWD_ITEM_MAKE_AFTER,		// 物品製作完成後關閉
	SWD_LEARNSKILL_OK,		// 學習技能...
	SWD_LEARNITEM_OK,		// 學習物品...
	SWD_ITEM_USE_SELECT_OK,		// 選擇要使用的物品，點擊 OK
	SWD_SKILL_CANCEL,		// 技能菜單中的 CANCEL 按鈕
	SWD_HEALING_OK,		// 在治療中心治療
	SWD_PREVENTING_OK,		// 在治療中心預防
	SWD_INN_OK,		// 旅館休息...
	SWD_MEMORIZE_BUTTON,		// 記憶魔法相關的按鈕
	SWD_SELECT_LOCATION,		// 選擇地圖位置
	SWD_ITEM_EXCHANGE_CANCEL,		// 物品交換菜單中的取消按鈕
	SWD_BETA_TEST_GO,		// 進入 Beta 測試菜單
	SWD_SORRY,
	SWD_GO_BBS_WRITE,		// 啟用寫作菜單
	SWD_BBS_WRITE_SEND,		// 發送寫作內容
	SWD_BBS_WRITE_EXIT,		// 寫作退出
	SWD_GOD_REGIST_OK,		// 我相信神。
	SWD_PREACH_OK,		// 宣傳我神
	SWD_PREACH_CANCEL,		// 取消宣傳神
	SWD_GOD_REGIST_ERROR_OK,
	SWD_GOD_PREACH_RESULT_OK,
	SWD_TIMEMENU_CLOSE,		// 關閉時間顯示菜單
	SWD_ONLY_OK,		// "OK/Cancel 訊息框" 中的 OK 按鈕
	SWD_CONNECT_RECENT,		// 連接到最近登出的位置
	SWD_READ_TO_WRITE,		// 從閱讀模式切換到寫作模式
	SWD_SUPPER_BACK,		// 從超級背包返回
	SWD_WILLDO_WORK,		// 呼叫 willdo 中的 work
	SWD_CONNECT,		// 連接到伺服器
	SWD_MAININTERFACE_SKILL,		// 在主菜單中顯示技能
	SWD_HUNTER_REGIST_OK,		// 獵人註冊菜單中的 OK
	SWD_HUNTER_LIST_OK,		// 刷新獵人列表
	SWD_MAN_TO_MAN_CANCEL,		// 關閉 1:1 交易菜單
	SWD_RESET_JOB,		// 重置職業菜單
	SWD_RESET_JOB_EXPLAIN,		// 重新解釋職業重置
	SWD_CHARACTER_COLOR_CHANGE,		// 0905 kkh 增加 (角色顏色變更)
	SWD_GO_VOTE,		// 打開投票菜單 (必須是投票期間)
	SWD_VOTER_REGIST_OK,		// 註冊投票
	SWD_VOTER_SUPPORT_OK,
	SWD_VOTER_SUPPORT_SCROLL,
	SWD_GM_LIST_SCROLL,		// 公會成員列表捲動
	SWD_GUILD_EXPLAIN_SCROLL,		// 公會物品說明捲動
	SWD_GM_REGIST,		// 加入公會
	SWD_GM_REGIST_2,		// 第二次加入公會 //021030 lsw
	SWD_NEW_ID,		// 在註冊菜單中點擊 now id
	SWD_DEFAULT_MESSAGEBOX_OK,		// 預設訊息框的 OK 按鈕
	SWD_RE_LOGON,		// 重新進入四個角色的選擇畫面
	SWD_HELP,		// 顯示提示 (tip) 資訊
	SWD_SALVATION_OK,		// 贖罪
	SWD_CHANGE_MONEY_OK,		// 換錢
	SWD_RUMOR_OK,		// 散播謠言？...
	SWD_BATTLE_REGIST,		// 戰鬥註冊
	SWD_REQ_LOAN,		// 要求貸款
	SWD_FIGHTMAP_WAIT_MEMBER,		// 再次刷新戰鬥等待成員
	SWD_FIGHT_WAIT_EXIT,		// 退出戰鬥等待窗口
	SWD_SEARCH_LADDER_RANK,		// 在天梯排名公告板中搜尋
	SWD_SMALL_MENU_WITH_SWD_TO_WORK,		// 將 nSHideNomalNumber 中的值傳遞給關閉菜單的 WORK
	SWD_BUY_VISA,		// 購買簽證
	SWD_GUILD_EXPLAIN_YES,
	SWD_REGIST_GUILD_IMSI_SEND,		// 發送臨時公會註冊請求
	SWD_GM_2ND_CANCEL,
	SWD_GM_2ND_OK,
	SWD_SKILL_MASTER_MULTIRARE_OK,		// 021111 lsw 精煉道具完成
	SWD_NATION_INFO,		// 國家資訊
	SWD_NATION_BUDAE,		// 國家部隊
	SWD_NATION_JOIN,		// 國家加入
	SWD_NATION_NOW,		// 國家現狀
	SWD_NATION_LADDER,		// 國家天梯
	SWD_NATION_VOTE,		// 國家投票
	SWD_NATION_GUEST,		// 國家訪客
	SWD_NATION_GOTO_WAR_MAP,		// 進入戰爭地圖
	SWD_NATION_DECSRIPT,		// 國家描述
	SWD_NATION_WEAPON_BUY,		// 國家武器購買
	SWD_NATION_ATTACK_WAY_POINT,		// 國家攻擊路徑點
	SWD_NATION_JOIN_,		// 010730 lsw 國家加入 (舊)
	SWD_NATION_JOIN_JOIN_WAR,		// 加入國戰
	SWD_NATION_NOW_,		// 010730 lsw 國家現狀 (舊)
	SWD_NATION_NOW_REFRESH,		// 刷新國家現狀
	SWD_NATION_NOW_BUDAE,		// 國家現狀部隊
	SWD_NATION_LADDER_,		// 010730 lsw 國家天梯 (舊)
	SWD_NATION_VOTE_,		// 010730 lsw 國家投票 (舊)
	SWD_NATION_VOTE_SELECT_YOU,		// 投票給你
	SWD_NATION_VOTE_I_AM_HERO,		// 我是英雄投票
	SWD_NATION_BUDAE_PREV_REFRESH,		// 國家部隊前一頁刷新
	SWD_NATION_BUDAE_NEXT_REFRESH,		// 國家部隊下一頁刷新
	SWD_NATION_BUDAE_JOIN,		// 加入國家部隊
	SWD_NATION_VOTE_PREV_REFRESH,		// 國家投票前一頁刷新
	SWD_NATION_VOTE_NEXT_REFRESH,		// 國家投票下一頁刷新
	SWD_NATION_INFO_SELECT,		// 國家資訊選擇
	SWD_NATION_GUEST_BUDAE,		// 國家訪客部隊
	SWD_NATION_GUEST_JOIN,		// 國家訪客加入
	SWD_NATION_GOTO_WAR_MAP_TARGET,		// 進入戰爭地圖目標
	SWD_NATION_TODAY_NOTICE_IMAGE,		// 國家今日公告圖片
	SWD_NATION_WAR_RESULT,		// 011001 lsw 國戰結果
	SWD_REGIST_GUILD_SEND,		// 發送公會註冊
	SWD_NATION_MSGBOX_OK,		// 國家訊息框 OK
	SWD_DUAL_OK,		// 決鬥 OK
	SWD_CALL_GUILD_EXPLAIN,		// 呼叫公會物品說明菜單
	SWD_CALL_GUILD_CHANGE_DEGREE,		// 呼叫公會等級變更菜單
	SWD_GUILD_DEGREE_OK,		// 變更公會等級
	SWD_CALL_OKCANCEL_BOX,		// 呼叫訊息框
	SWD_CALL_GUILD_CHANGE,
	SWD_INIT_ABILITY_OK,		// 初始化能力值 OK
	SWD_CALL_SKIN_MENU,		// 呼叫皮膚菜單

	SWD_SEND_GET_MERCHANT_SELL_LIST,		// 獲取出售物品列表 (呼叫 Sendxxxx_xxxx_xx() 函數)
	SWD_SEND_GET_MERCHANT_BUY_LIST,		// 獲取購買物品列表

	SWD_SEND_MERCHANT_BUY_LIST_SCROLL_UP,
	SWD_SEND_MERCHANT_BUY_LIST_SCROLL_DOWN,

	SWD_SEND_MERCHANT_DIRECT_LIST_SCROLL_UP,
	SWD_SEND_MERCHANT_DIRECT_LIST_SCROLL_DOWN,

	SWD_SEND_MERCHANT_SELL_LIST_SCROLL_UP,
	SWD_SEND_MERCHANT_SELL_LIST_SCROLL_DOWN,

	SWD_SEND_MERCHANT_RESULT_LIST_SCROLL_UP,
	SWD_SEND_MERCHANT_RESULT_LIST_SCROLL_DOWN,

	SWD_SEND_GET_MERCHANT_RESULT_LIST,		// 獲取購買結果列表

	SWD_SEND_MERCHANT_BUY_THIS_ITEM,		// 買家購買此物品
	SWD_SEND_MERCHANT_SELL_ITEM_DELETE,		// 賣家刪除出售中的物品
	SWD_SEND_MERCHANT_SELL_ITEM_REGISTER,		// 賣家註冊出售物品
	SWD_MERCHANT_SELL_TYPE_SET,		// 設定出售類型

	SWD_SEND_MERCHANT_RESULT_ITEM_TAKE,			// 提取結果物品

	SWD_CALL_MERCHANT_REGISTER,		// 呼叫商人註冊

	SWD_ITEM_MAKE_SELECT_RARE,		// 選擇稀有物品製作
	SWD_ITEM_MAKE_SELECT_RARE_OK,		// 稀有物品製作 OK
	SWD_ITEM_MAKE_SELECT_RARE_CANCEL,		// 稀有物品製作 Cancel
	SWD_GABLE_ITEM,		// 可賭博物品 (Gamble Item)
	SWD_ABILITY_REDISTRIBUTION,		// 011201 LTS 能力值重新分配
	SWD_LOCALWAR_JOIN,		// 011201 LTS 地方戰爭加入
	SWD_REGIST_FRIEND,		// 註冊朋友
	SWD_DELETE_FRIEND,		// 刪除朋友
	SWD_SMALLMENU_CLOSE,		// 關閉小菜單 (與 SWD_SMALLMENU_END 不同，不響應 ESC 鍵)
	SWD_POTAL_MENU,		// 傳送門菜單
	SWD_GOTO_POTAL,		// 前往傳送門
	SWD_EVENT_LOCALWAR_JOIN,		// 020115 LTS 活動地方戰爭加入
	SWD_EVENT_LOCALWAR_MOVE,		// 020115 LTS 活動地方戰爭移動
	SWD_PUT_GUILD_ITEM,		// 放置公會物品
	SWD_SMALL_MENU_CLOSE_CALL_FUCTION,		// 關閉小菜單並呼叫函數
	SWD_CALL_OKCANCEL_BOX_ETC,		// 呼叫帶有附加資訊的 OK/CANCEL 訊息框
	SWD_SET_SUB_MASTER,		// 設定副會長
	SWD_NATION_ITEM_OK,		// 國家物品 OK
	SWD_GUILD_HOUSE_OK,		// 公會房屋 OK
	SWD_GUILD_HOUSE_BUY,		// 購買公會房屋
	SWD_CSP_GROUP_ICON,		// CSP == CombatSkillPoint (戰鬥技能點) 群組圖示
	SWD_CSP_GROUP_SELECT,		// 戰鬥技能點群組選擇
	SWD_CSP_GROUP_SELECT_DONE,		// 戰鬥技能點群組選擇完成
	SWD_SC_ICON_POINT,		// POINT 模式時點擊的圖示
	SWD_SC_ICON_DOING,		// DOING 模式時點擊的圖示
	SWD_CALL_COMBAT_SKILL_POINT,		// 呼叫戰鬥技能點菜單
	SWD_LOCALWAR_MAPMOVE,		// LTS NEW LOCALWAR 地方戰爭地圖移動
	SWD_LOCALWAR_END,		// LTS NEW LOCALWAR 地方戰爭結束
	SWD_PARTY_ALL_DEL,		// 020515 lsw 刪除所有隊伍
	SWD_TACTICS_ABILITY,		// 從戰術切換到能力
	SWD_CALL_FRIEND_MENU,		// 呼叫朋友菜單
	SWD_MOVE_TO_HOMETOWN,		// 移動到家鄉
	SWD_MOVE_TO_HOMETOWN_WITH_ALIVE,		// 活著移動到家鄉
	SWD_LW_DONE_PREV,		// LTS LOCALWAR MENU 地方戰爭完成前一頁
	SWD_LW_DONE_NEXT,		// LTS LOCALWAR MENU 地方戰爭完成下一頁
	SWD_DELETE_CHARATER,		// 020725 lsw 刪除角色
	SWD_DELETE_CHARATER_CLOSE,		// 020725 lsw 關閉刪除角色
	SWD_SELECT_TUTORIAL_OK,		// 020815-2 lsw 選擇教程 OK
	SWD_SET_LOGIN_TARGET,		// 020815-2 lsw 設定登入目標
	SWD_AUCTION_FIND_KEY_RARE_SET,				// 拍賣搜尋鍵：稀有度設定
	SWD_AUCTION_FIND_KEY_LV_SET,				// 拍賣搜尋鍵：等級設定
	SWD_AUCTION_FIND_KEY_TACTIC_SET,			// 拍賣搜尋鍵：戰術設定
	SWD_AUCTION_FIND_KEY_WEAR_SET,				// 拍賣搜尋鍵：穿戴位置設定
	SWD_WAR_MOVE,		// 戰爭移動
	SWD_WAR_STATUS_REFRESH,						// 戰爭狀態刷新
	SWD_CALL_MERCHANT_QUEST,					// 呼叫商人任務
	SWD_CALL_MERCHANT_OR_SKILL_MASTER,			// 呼叫商人或技能大師
	SWD_CALL_SKILL_MASTER_QUEST,				// 呼叫技能大師任務
	SWD_EXCHANGE_BOND_MONEY_OK,					// 交換綁定金 OK
	SWD_CALL_EXCHANGE_BOND_MONEY,				// 呼叫交換綁定金菜單

	SWD_LOTTO_NUMBER,		// soto-030428 樂透號碼
	SWD_LOTTO_AUTO_GEN,		// soto-030428 樂透自動生成
	SWD_LOTTO_OK,		// soto-030428 樂透 OK

	SWD_LOTTO_OPEN,		// soto-030429 打開樂透窗口
	SWD_LUCKY_EVENT,		// soto-030429 打開幸運活動窗口
	SWD_COMFIRMATION_WINNER,		// soto-030429 確認獲勝者

	SWD_WINNER_OK,		// soto-030501 獲勝者確認窗口 OK。按下後傳送郵件給玩家
	SWD_LOTTO_NUM_LEFTSEEK,		// soto-Lotto 增加：樂透框號碼向左尋找
	SWD_LOTTO_NUM_RIGHTSEEK,		// soto-Lotto 增加：樂透框號碼向右尋找

	SWD_MAIL_SAVE,								// 郵件儲存
	SWD_MAIL_WRITE,								// 寫郵件

	SWD_MAIL_WRITE_EXIT,						// 寫郵件菜單：退出
	SWD_MAIL_WRITE_SEND,						// 寫郵件菜單：發送
	SWD_MAIL_WRITE_DELETE,						// 寫郵件菜單：刪除
	SWD_MAIL_CALL_RECV_LIST,					// 呼叫接收郵件列表
	SWD_MAIL_CALL_DELETE_LIST,					// 呼叫刪除郵件列表
	SWD_MAIL_SELECT_ALL,						// 全選郵件
	SWD_CALL_MAILMENU,							// 呼叫郵件菜單
	SWD_CALL_MAIL_WRITE,						// 呼叫寫郵件菜單
	SWD_MAIL_REPAIR,							// 郵件修復
	SWD_MAIL_DELETE,							// 郵件刪除
	SWD_MAIL_SCROLL,							// 郵件捲動
	SWD_TEMP_MAIL_TYPE_SCROLL,					// 臨時郵件類型切換
	SWD_MAIL_SENDER_REGIST_FRIEND,				// 將發送者註冊為朋友
	SWD_OBSERVER_TEAM_SELECT,					// 競技場觀察者選擇隊伍
	SWD_SEND_MY_ARENA_BETTING_MONEY,			// 發送我的競技場投注金額

	SWD_AUCTION_UPDOWN_BTN,						// 拍賣升降按鈕
	SWD_CALL_ARENA_OBSERVER_MENU,				// 呼叫競技場觀察者菜單
	SWD_CALL_ARENA_BETTING_MENU,				// 呼叫競技場投注菜單

	SWD_SEND_MERCHANT_BACKUP_ITEM_TAKE,			// BBD 040303 提取商人備份物品
	SWD_SEND_MERCHANT_BACKUP_LIST_SCROLL_UP,	// BBD 040303 商人備份列表向上捲動
	SWD_SEND_MERCHANT_BACKUP_LIST_SCROLL_DOWN,	// BBD 040303 商人備份列表向下捲動
	SWD_MANTLE_COLOR_CHANGE,					// 改變披風顏色
	SWD_MALL_ITEM_BUY,							// 商城物品購買
	SWD_MANTLEDYE_CANCEL,						// 商城物品取消按鈕
};

// 菜單介面欄位類型編號定義
enum FieldTypeIndex
{
	FT_NONE = 0,	// 不是任何東西 //020420 lsw
	FT_NOMAL_PUT,		// 只是顯示圖片的欄位
	FT_NOMAL_PUT_CENTER,		// 圖片居中顯示，在菜單 x 軸中間對齊
	FT_TEXT_PUT,		// 顯示文字
	FT_TEXT_PUT_CENTER,		// 文字在菜單 x 軸中間對齊
	FT_HIDE_PUT,		// 按下滑鼠 L 鍵時圖片會改變並顯示的欄位
	FT_HIDE_PUT_CENTER,		// FT_HIDE_PUT 的居中版本，在菜單 x 軸中間對齊
	FT_HIDE_PUT_AUTO,		// 即使沒有按下按鈕，只要滑鼠懸停圖片就會改變 (自動檢查)
	FT_HIDE_PUT_CENTER_AUTO,		// FT_HIDE_PUT_AUTO 的居中版本
	FT_HIDE_NOMAL_ONE_PUT,		// 在菜單處理中，如果滑鼠指標移入，圖片會改變為編號 1 並顯示的欄位
	FT_HIDE_NOMAL_GROUP_PUT,		// 在菜單處理中，如果滑鼠指標移入，圖片會改變為一個群組並顯示的欄位
	FT_HIDE_WILLDO_PUT,		// 按下滑鼠 L 鍵時，呼叫其他菜單的欄位
	FT_HIDE_SPECIAL_WILLDO_PUT,		// 不僅呼叫菜單，還執行特定的特殊操作
	FT_SPECIAL_WILLDO,		// 不顯示圖片，直接執行動作
	FT_HIDE_ISCROLL_PUT,		// 向上捲動按鈕的欄位，當滑鼠指標移入時，圖片改變並顯示
	FT_HIDE_DSCROLL_PUT,		// 向下捲動按鈕的欄位，當滑鼠指標移入時，圖片改變並顯示
	FT_THROWDICE,		// 擲骰子的欄位
	FT_DATA_PUT,		// 顯示資料。如果 nImageName 中有 DP_，則使用 DP_
	FT_FIRE,		// 顯示火焰效果的欄位
	FT_INVENTORY,		// 物品欄。只檢查選中欄位的滑鼠事件並移動
	FT_NO_CHECK,		// 即使在群組欄位中，也不進行任何檢查，僅用於背景
	FT_ACCEL_INVENTORY,		// 快速物品欄 (檢查滑鼠事件的加速物品欄)
	FT_ARMOR_INVENTORY,		// 裝備欄位 (在裝備顯示畫面中檢查滑鼠事件)
	FT_WILLDO_PUT,		// 僅在滑鼠點擊時呼叫其他菜單
	FT_DO,		// 僅執行動作 (Do)
	FT_GAUGE,		// 顯示量條並填充狀態 (僅用於載入畫面，如有需要，可修改為通用量條)
	FT_DO_DEFAULT,		// 類似 FT_DO，但如果畫面捲動，則重新繪製
	FT_IDEDITBOX,		// 呼叫 ID 輸入框
	FT_PASSEDITBOX,		// 呼叫密碼輸入框
	FT_NOMAL_PUT_TALK_BATTLE_PEACE,		// 根據 TALK, PEACE, BATTEL 狀態切換按鈕顯示
	FT_LODING_LIGHT,		// 載入時的燈光效果
	FT_MAGIC_ITEM_PUT,		// 顯示魔法物品
	FT_NOMAL_PUT_CENTER_1,		// _1 是用於額外處理的標記
	FT_HOTKEY_ICON,		// 顯示快捷鍵圖示，使用 t_QuickMemory
	FT_COLOR_GAUGE,		// 顏色量條
	FT_MENU_MOVE,		// 拖動菜單
	FT_HIDE_WILLDO_AUTO_PUT,		// 滑鼠懸停時改變圖片，點擊時圖片改變並呼叫其他菜單
	FT_HIDE_SPECIAL_WILLDO_AUTO_PUT,		// 滑鼠懸停時改變圖片，點擊時執行特殊操作
	FT_HIDE_ISCROLL_AUTO_PUT,
	FT_HIDE_DSCROLL_AUTO_PUT,
	FT_BANK_INVENTORY,		// 銀行物品欄
	FT_BANK2_INVENTORY,		// 銀行物品欄
	FT_TEXT_PUT_CENTER_X,		// X 軸座標僅給出一次
	FT_HIDE_AUTO_PUT,		// 滑鼠懸停時圖片改變
	FT_JOB_ITEM,
	FT_TOWN_CONSTRUCTION,		// 城鎮建設檢查
	FT_HOME_EDITBOX,		// 在房屋系統中，訊息窗口中的按鈕欄位
	FT_SWITCH_PUT,		// 顯示開關圖片
	FT_SKILL_PUT,		// 技能顯示
	FT_RETURN_FUCTION,		// 返回函數
	FT_OPTION_GAUGE,		// 在選項菜單中顯示量條值
	FT_SYSTEM_OPTION_SCROLL,		// 在系統選項菜單中顯示捲動值
	FT_KNOWLEDGE_ICON_PUT,
	FT_INN,		// 旅館休息欄位
	FT_LEVELUP_BUTTON,		// 狀態畫面中的按鈕
	FT_LEVELUP,		// 狀態窗口中的 + 按鈕
	FT_NOMAL_PUT_FX,		// 訊息窗口背景中的畫面效果
	FT_BBS_GO,		// 點擊公告板後，切換到下一個畫面
	FT_PARTY,		// 隊伍菜單中的欄位類型
	FT_PARTY_FACE,		// 隊伍成員頭像
	FT_DIVIDE_ITEM_PUT,		// 在分割物品窗口中顯示物品數量
	FT_MAKE_ITEM,		// 在物品製作窗口中顯示物品製作槽
	FT_ITEM_MAKE_SELECT,		// 可製作的物品列表
	FT_MAIN_MENU,		// 主菜單中的翻轉菜單按鈕... 類似 FT_HIDE_SPECIAL_WILLDO_AUTO_PUT
	FT_MAKE_ITEM_OK,		// 完成物品製作
	FT_LEARN_ITEM,		// 在學習菜單中學習物品
	FT_CHARACTER_ITEM,		// 其他角色的物品
	FT_TEXT_BUTTON,		// 點擊文字時，儲存該文字
	FT_ITEM_USE_SELECT,		// 選擇要使用的技能物品
	FT_SELECT_FACE,		// 在角色創建中選擇臉部
	FT_MAGIC_CLASS,		// 魔法陣列
	FT_DSCROLL_BOOK,		// 魔法書向下捲動
	FT_ISCROLL_BOOK,		// 魔法書向上捲動
	FT_MAGIC_BOOK_ETC,
	FT_LEARN_MAGIC,		// 在魔法書介面中學習魔法
	FT_HEAL_TEXT_BUTTON,		// 治療時的按鈕欄位
	FT_DSCROLL_MEM_BOOK,		// 記憶魔法書向下捲動
	FT_ISCROLL_MEM_BOOK,		// 記憶魔法書向上捲動
	FT_MEM_MAGIC_ICON,		// 記憶魔法書中的魔法圖示按鈕
	FT_MEM_BOOK_ETC,		// 記憶魔法書中背景的圖片
	FT_PARTY_FACE_PUT,		// 在隊伍欄位菜單中顯示頭像
	FT_VIEW_SMALL_MAP,		// 顯示小地圖
	FT_MENU_MOVE_DIRECT,		// 直接拖動菜單
	FT_EXCHANGE_ITEM,		// 交換物品
	FT_EXCHANGE_ITEM_CHECK,		// 交換物品檢查
	FT_EXCHANGE_ITEM_ETC,
	FT_MAN_TO_MAN_INFO_DISPLAY,		// 1:1 交易資訊顯示欄位
	FT_TEST_CHECK,		// Beta 測試檢查
	FT_BETA_TEST_VIEW,		// Beta 測試相關顯示
	FT_CREDIT_PUT,		// 顯示信用度
	FT_INFO_PUT,		// 顯示資訊
	FT_PUT_DISTANCE,		// 顯示座標
	FT_STORE_SIGN,		// 商店招牌
	FT_AWAKE_BUTTON,		// 覺醒按鈕
	FT_GOD_INTRUDUCTION,		// 神的介紹
	FT_GOD_MEETING,		// 神的會面
	FT_GOD_REGIST_OKCANCEL,		// 願意侍奉神嗎？
	FT_PREACH_NAME,		// 宣傳者名稱顯示
	FT_GOD_REGIST_ERROR,		// 神的錯誤
	FT_QUICK_KEY,		// 快捷鍵
	FT_VIEW_GOD_MEETING,		// 顯示神或宣傳
	FT_NATION_DISPLAY,		// 國家顯示
	FT_HIDE_WILLDO_AUTO_PUT_MAGIC,		// 在魔法菜單中用於快捷鍵
	FT_MENU_MOVE_DEFAULT,		// 菜單移動
	FT_WILLDO_WORK,		// 執行 willdo 中的 work
	FT_BELIEVE_GOD,		// 信奉神的名字
	FT_SUPPER_INV,		// 超級背包物品欄
	FT_SMALL_MAP_HOUSE,		// 在城鎮界面中，地圖上顯示房屋名稱
	FT_EXPLAIN_MENU,		// 菜單說明顯示
	FT_QUICK_MAGIC_PLUS,		// 魔法陣列
	FT_QUICK_MAGIC_ARRAY_SCROLL,		// 魔法陣列捲動
	FT_XY_DISPLAY,		// 座標顯示
	FT_TIME_DISPLAY_ON_OFF,		// 時間菜單開關按鈕
	FT_CHAT_SCROLL,		// 聊天框捲動按鈕
	FT_SELECT_CITY,		// 在城市畫面中選擇城市
	FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2,		// 兩個菜單同時使用
	FT_FIRST_QUICK_KEY,		// 首次呼叫快捷鍵時 (例如，用於魔法快捷鍵)
	FT_PUBLIC_BBS,		// 公告板
	FT_QUICK_SKILL_PUT,		// 技能菜單
	FT_QUICK_SKILL_SCROLL,		// 技能捲動 (上下)
	FT_QUICK_SKILL_SCROLL_LR,		// 技能捲動 (左右)
	FT_PUBLIC_BBS_NEXT,		// 公告板下一頁
	FT_HUNTER_REGIST,		// 獵人註冊菜單
	FT_HUNTER_LIST,		// 獵人列表顯示
	FT_HUNTER_LIST_SCROLL,		// 獵人列表捲動
	FT_DEFAULT_BUTTON,		// 預設按鈕
	FT_QUEST_OK,		// 任務 OK
	FT_QUEST_SCROLL,		// 任務捲動
	FT_QUEST_INDEX,		// 任務索引
	FT_QUEST_INDEX_SCROLL,		// 任務索引捲動
	FT_MAN_TO_MAN_CHECK,		// 1:1 交易檢查欄位
	FT_RESET_JOB_EXPLAIN,		// 職業重置說明欄位
	FT_RESET_JOB_SCROLL,		// 職業重置時的捲動欄位
	FT_COLOR_CHANGE_GAUGE,		// 0905 kkh 增加 顏色變更量條
	FT_SELECT_NATION,		// 國家選擇
	FT_SELECT_NATION_EXPLAIN,		// 國家選擇說明
	FT_SELECT_NATION_SCROLL,		// 國家選擇捲動
	FT_NATION_BBS,		// 國家公告板
	FT_NATION_BUTTON,		// 國家公告板按鈕
	FT_FIGHT_MAP_SIGN,		// 確認戰鬥地圖標誌
	FT_DISPLAY_SCENARIO,		// 顯示情景/劇情
	FT_VOTE_REGIST,		// 投票菜單註冊欄位
	FT_VOTE_SUPPORT,		// 投票支持
	FT_VOTE_SCORE_PUT,		// 顯示投票分數狀態
	FT_GM_LIST,		// 公會成員列表顯示
	FT_GM_ITEM_EXPLAIN,		// 顯示公會物品說明
	FT_GM_ITEM_EXPLAIN_BUTTON,		// 公會物品說明按鈕
	FT_TEXT_BUTTON_EXIT,		// 退出按鈕
	FT_DISPLAY_EXP,		// 顯示技能經驗值 (僅用於裝備)
	FT_VIEW_TIP,		// 顯示提示按鈕
	FT_PUT_TACTICS,		// 在戰術說明窗口中顯示戰術圖片
	FT_SCROLL_TEXT_PUT,		// 可捲動文字顯示 (需要檢查 checktype 是否為 1)
	FT_SCROLL_BUTTON,		// 捲動按鈕
	FT_SALVATION_MONEY,		// 贖罪金額
	FT_SALVATION_MONEY_SCROLL,		// 贖罪金額捲動
	FT_USED_MAGIC_EXPLAIN,		// 顯示已使用魔法說明
	FT_ANIMATION,		// 動畫
	FT_TEMBATLE_INFO,		// 顯示團隊戰鬥相關資訊
	FT_SELECT_SERVER_BUTTON,		// 選擇伺服器菜單中的文字按鈕
	FT_SELECT_SERVER_SCROLL,		// 選擇伺服器捲動
	FT_TEMBATLE_ADD,		// 加入團隊戰鬥
	FT_CHANGE_MONEY,		// 換錢
	FT_BATTLE_WAIT,		// 戰鬥等待
	FT_VIEW_BATTLE_MEMBER,		// 顯示戰鬥成員
	FT_DELAY_BUTTON,		// 延遲按鈕
	FT_BANK_LOAN,		// 銀行貸款
	FT_VIEW_BATTLE_WAIT_MEMBER,		// 顯示戰鬥等待成員
	FT_FIGHTMAP_START,		// 開始戰鬥地圖
	FT_CHECK_COMBO,		// 組合按鈕 (使用 nValue 和 nWillDo 進行處理)
	FT_VIEW_BBS_MONEY,		// 公告板費用顯示
	FT_WRITE_BBS,		// 繪製一個輸入框並初始化
	FT_WRITE_BBS_ITEM_NAME,		// 寫公告板物品名稱
	FT_WRITE_BBS_ITEM_MONEY,		// 寫公告板物品金錢
	FT_SMALL_TIPS_BUTTON,
	FT_VIEW_LADDER_RANK,		// 顯示天梯排名
	FT_VIEW_EFFECT_TOOL,		// 效果工具
	FT_VISA,		// 顯示簽證相關資訊
	FT_SELECT_VISA,		// 選擇簽證
	FT_INPUT_GUILD_INFO,		// 輸入公會資訊
	FT_GUILD_DEGREE,		// 公會等級輸入菜單
	FT_NORMAL_PUT_CENTER2,		// 臨時使用 m_lpTemp 的圖片居中顯示
	FT_GUILD_INTRODUCTION,		// 公會介紹
	FT_VIEW_TEXT_BY_HTML,		// 以 HTML 格式顯示文字
	FT_INPUT_GUILD_MASTER,		// 公會會長輸入
	FT_GM_2ND_SKILL_MAKE_ITEM,		// 2001 -> 201 轉換 (二次技能製作物品)
	FT_SKILL_MASTER_MAKE_ITEM,		// 021113 lsw 技能大師製作物品
	FT_EXCHANGE_BOND_MONEY_ITEM,		// 交換綁定金物品 021126 lsw
	FT_TODAY_NOTICE,		// 今日公告
	FT_NOMAL_PUT_BY_NWILLDO,		// 010910 lsw 根據 nWilldo 值顯示圖片
	FT_SHOW_SPECIAL_WILLDO_AUTO_PUT,		// 矩形圖片按鈕 (點擊 nImageNumber 改變)
	FT_PUT_BUTTON,		// 顯示按鈕欄位
	FT_CHECK_COMBO2,		// 組合框，只顯示文字
	FT_NOMAL_PUT_BY_NWILLDO_CUSTOM,		// 011008 lsw 自訂根據 nWilldo 顯示圖片
	FT_VIEW_RUNE_LEFT,		// 顯示左邊符文
	FT_VIEW_RUNE_RIGHT,		// 顯示右邊符文
	FT_SCROLL_BUTTON2,		// 捲動按鈕 2 (特殊欄位，用於處理按鍵)
	FT_SKIN_MONEY,		// 皮膚金錢
	FT_RARE_GROUP,		// 011013 lsw 稀有物品群組
	FT_RARE_GROUP_SCROLL,		// 011013 lsw 稀有物品群組捲動
	FT_MAKE_RARE_MORE,		// 011013 lsw 更多稀有物品製作
	FT_CHECK_SALVATION,		// 贖罪檢查
	FT_ROULETTE,		// 輪盤
	FT_LOCALWAR_JOIN,		// LTS MENU 地方戰爭加入
	FT_GUILD_MEMBER_LIST_DEGREE_TAB,		// 公會成員列表等級標籤
	FT_GUILD_PUBLIC_NOTICE,		// 公會公告
	FT_FRIEND_NAME,		// 朋友名稱
	FT_FRIEND_REGIST_NAME,		// 朋友註冊名稱
	FT_GAMBLE_TODAY,		// 今日賭博
	FT_POTAL_MAP,		// 傳送門地圖
	FT_POTAL_SELECT,		// 傳送門選擇
	FT_POTAL_SELECT_PUT,		// 傳送門選擇顯示
	FT_GUILD_ITEM_GET,		// 公會物品獲取
	FT_GUILD_ITEM_GET_ETC,		// 公會物品獲取（其他）
	FT_GUILD_ITEM_PUT_BLANK,		// 在公會窗口中顯示空白物品欄位
	FT_GUILD_ITEM_PUT,		// 在公會窗口中放置物品
	FT_RARE_GROUP_MEMBER_DISPLAY,		// 稀有物品群組成員顯示
	FT_FILE_TEXT_PUT,		// 檔案文字顯示
	FT_NATION_ITEM,		// 國家物品
	FT_REVIVAL_BUTTON,		// 復活按鈕
	FT_GUILD_HOUSE,		// 公會房屋
	FT_GUILD_HOUSE_INFO,		// 公會房屋資訊
	FT_POTION_BOX,		// 藥水盒
	FT_CS_ICON,		// CS == CombatSkill (戰鬥技能)
	FT_CSD_ICON,		// 戰鬥技能顯示圖示 (nWillDo 為技能索引)
	FT_CSD_MAIN_ICON,
	FT_CSD_POINT_LINE_BAR,		// 戰鬥技能點線條量條
	FT_CSD_POINT_COUNT_BAR,		// 戰鬥技能點計數量條
	FT_CSP_POINT_UP,		// 戰鬥技能點升級
	FT_CS_GROUP_EXPLAIN,		// 戰鬥技能群組說明 (nWillDo 為群組類型)
	FT_CS_EXPLAIN,		// 戰鬥技能說明 (nWillDo 為技能索引)
	FT_LOCALWAR_MAPMOVE,		// LTS NEW LOCALWAR 地圖移動
	FT_ANIMATION2,		// 動畫 2
	FT_PEACE_BATTLE_PUT,		// 和平/戰鬥狀態顯示
	FT_CANDLE_INVENTORY,		// 蠟燭物品欄
	FT_DISPLAY_TAC_EXP,		// 顯示戰術經驗值
	FT_DISPLAY_MAGIC_EXP,		// 顯示魔法經驗值
	FT_DISPLAY_SKILL_EXP,		// 顯示技能經驗值
	FT_ALPHA_PUT,		// 透明度顯示
	FT_SELECT_WISPER_TARGET,		// 選擇密語目標
	FT_SELECT_WISPER,		// 選擇密語
	FT_SELECT_CHAT_TYPE,		// 選擇聊天類型
	FT_CHAT_LOCK,		// 聊天鎖定
	FT_CHAT_VIEW_SET,		// 聊天顯示設定
	FT_DRAG_ZONE,		// 拖動區域
	FT_SCENARIO_TRAP_INFO,
	FT_SCENARIO_TRAP_KEY,
	FT_INPUT_DEL_CHAR_COMFORM_KEY_VIEW,		// 刪除角色確認碼顯示 //020802 lsw
	FT_HUNTER_VERSUS_DISPLAY,		// 011001 lsw 獵人對戰顯示
	FT_MERCHANT_FINDER_LIST,					// 商人查找器列表欄位
	FT_MERCHANT_SELL_ITEM_EXPLAIN,				// 商人出售物品說明
	FT_MERCHANT_SELL_ITEM_DROP_ZONE,			// 商人出售物品拖放區域
	FT_MERCHANT_SELL_ITEM_CHECK,				// 商人出售物品檢查按鈕
	FT_MERCHANT_BUY_ITEM_CHECK,					// 商人購買物品檢查按鈕
	FT_MERCHANT_DIRECT_BUY_ITEM_CHECK,			// 商人直接購買物品檢查按鈕
	FT_MERCHANT_RESULT_ITEM_CHECK,				// 商人結果物品檢查按鈕
	FT_MERCHANT_SELL_ITEM_VALUE_INPUT,			// 商人出售物品價格輸入

	FT_SCRIPT_TIMMER_COUNT,						// 021021 kyo 計時器計數顯示
	FT_SCRIPT_COUNTER_COUNT,					// 021021 kyo 計數器計數顯示
	FT_DISPLAY_GM_REGIST,						// 顯示公會成員註冊

	FT_LOTTO_RUN,								// soto-030425 樂透運行按鈕
	FT_LOTTO_NUMBER,							// soto-030425 樂透號碼按鈕

	FT_MAIL_LIST,								// 郵件列表
	FT_MAIL_WRITE_RECV,		// 寫郵件收件人按鈕
	FT_MAIL_WRITE_TITLE,		// 郵件標題
	FT_MAIL_WRITE_BODY,		// 寫郵件內容
	FT_MAIL_WRITE,
	FT_MAIL_READ,		// 郵件閱讀內容
	FT_SELECT_OBSERVER_TARGET_TEAM,		// 觀察者選擇目標隊伍按鈕
	FT_ARENA_BETTING,		// 競技場投注金額
	FT_SELECT_BETTING_TARGET_TEAM,
	FT_WRITE_MERCHANT,		// 商人查找器窗口中的輸入框欄位 // soto-030511
	FT_CHECK_MERCHANT_FIND,		// 商人查找器檢查
	FT_MAININTERFACE_MAIL_CALL,		// 主介面郵件呼叫
	FT_MERCHANT_BACKUP_GET_LIST,		// BBD 040303 商人備份獲取列表
	FT_MANTLE_COLOR_CHANGE_GAUGE,		// Finito 23/06/08 - 披風染色量條
	FT_MALL_ITEM,		// 商城物品物品欄
};

/////////////////////////////////////////////////////////////////////////////////
// 顯示資料類型 (如果 FT_DATA_PUT 等欄位類型使用 DP_，則會繪製此處的資料)
enum DisplayIndex
{
	DP_NONE = 0,
	DP_GENDER,		// 性別
	DP_NAME,		// 名稱
	DP_ARIGEMENT,		// 陣營/善惡值
	DP_CLASS,		// 職業
	DP_SPELL,		// 法術
	DP_TACTICS,		// 戰術值
	DP_JOB,		// 職業
	DP_STR,		// STR (力量)
	DP_CON,		// CON (體質)
	DP_DEX,		// DEX (敏捷)
	DP_WIS,		// WIS (智慧)
	DP_INT,		// INT (智力)
	DP_CHA,		// CHA (魅力)
	DP_MOVP,		// MOVP (移動點數)
	DP_ENDU,		// ENDU (耐力)
	DP_MOR,		// MOR (士氣)
	DP_WSPS,		// WSPS (法術抵抗)
	DP_LUCK,		// LUCK (幸運)
	DP_NAMEEDITBOX,		// 輸入名稱的編輯框
	DP_FACE,		// 臉部表情顯示
	DP_INVENTORY,		// 物品欄顯示
	DP_LEVEL,		// 顯示等級
	DP_EXP,		// 顯示經驗值
	DP_ARIGEMENT_BAR,		// 陣營/善惡值狀態條
	DP_HP_BAR,		// 在主介面中顯示 HP 狀態條
	DP_MP_BAR,		// 在主介面中顯示 MP 狀態條
	DP_HP,		// 顯示 HP 數值
	DP_MP,		// 顯示 MP 數值
	DP_MAIN_LEVEL,		// 在主介面中顯示等級
	DP_MAIN_STR,		// 在主介面中顯示 STR 數值
	DP_ACCEL_INVENTORY,		// 在加速物品欄中顯示物品圖片
	DP_ARMOR_INVENTORY,		// 在裝備欄中顯示物品圖片
	DP_CONDITION,		// 在主介面中顯示角色狀態
	DP_FACEIMAGE,		// 顯示臉部圖片
	DP_GUILD,		// 公會名稱
	DP_TACTICE,		// 戰術
	DP_DAMAGE,		// 傷害
	DP_HEALTH,		// 健康
	DP_MOV,		// 移動
	DP_HEAT,		// 熱度
	DP_AC,		// 防禦等級 (AC)
	DP_FAME,		// 名聲
	DP_MONEY,		// 金錢
	DP_IDEDITBOX,		// ID 顯示
	DP_PASSEDITBOX,		// 密碼 顯示
	DP_HEALTH_GAUGE,		// 健康量條
	DP_LEVEL_LOGON,		// 登入畫面中的等級顯示
	DP_TEMP_MONEY,		// 在輸入金額窗口中顯示的金額
	DP_BANK_SAVE,		// 銀行儲蓄
	DP_BANK_TOTAL,		// 銀行總金額
	DP_BANK_INVENTORY,		// 銀行物品欄
	DP_WEIGHT,		// 負重顯示
	DP_BANK_CURRENT_PAY,		// 當前還款金額
	DP_BANK_PAY,		// 貸款金額
	DP_CREDIT_TEXT,		// 信用度文字顯示
	DP_LOAN_TOTAL,		// 總貸款金額
	DP_LOAN_ABLE,		// 可貸款金額
	DP_DAY_TEXT,		// 還款日期顯示
	DP_REPAYMENT_MONEY,		// 應還款金額
	DP_REPAYMENT_DATE,		// 應還款日期
	DP_GUILD_INVENTORY,		// 公會物品欄
	DP_SELECT_ITEM,		// 選擇物品
	DP_CHARACTER_MONEY,		// 顯示角色擁有的金錢 (使用 nWillDo 決定 x,y 座標)
	DP_STRING_PUT,		// 顯示字串
	DP_RECT_STRING_PUT,		// 在菜單 1 中顯示 Message
	DP_SKILL_ICON,		// 在技能菜單中顯示技能圖示
	DP_HEALING_EXPLAIN,		// 治療說明
	DP_NAME_SELECT,		// 在角色選擇畫面中選中的名稱
	DP_LAST_LOAN,		// 最後一筆貸款
	DP_FACE_PUT,		// 010925 lsw 臉部圖片顯示 (使用 Gender、圖片編號、大小)
	DP_RECT_STRING_PUT_CUSTOM,		// 011218 lsw 自訂矩形字串顯示
	DP_TACTICS_DATA,		// 戰術資料
	DP_ABILITY,		// 在能力值窗口中顯示
	DP_DATAPUT,		// 顯示資料 (nWilldo 決定值，nRectImage 決定圖片)
	DP_RESIST,		// 在能力值窗口中顯示抵抗值
	DP_EXP_NEXT,		// 下一級所需經驗值
	DP_AGE_MAKE,		// 角色創建年齡
	DP_AGE,		// 年齡
	DP_TOWNGALL_TITLE,		// 公告板標題顯示
	DP_TOWNGALL_TITLE_DATE,		// 公告板日期顯示
	DP_BBS_PUT,		// 公告板內容顯示
	DP_BBS_WRITE_PUT,		// 公告板寫作顯示
	DP_HOUSE_PUT,		// 房屋顯示
	DP_HOUSE_REPAIR,		// 房屋修復菜單
	DP_ITEM_MESSAGE,		// 物品訊息窗口
	DP_SELECT_SELECT,		// 在選擇菜單中顯示被選中的項目
	DP_SELECT_SKILL,		// 在技能選擇中顯示被選中的技能
	DP_USE_SELECT,		// 顯示被選中的物品使用選擇
	DP_MAGIC_PUT,		// 在魔法書介面中顯示魔法
	DP_ITEM_PUT,		// 顯示物品圖示
	DP_DISEASE_PUT,		// 顯示疾病或中毒
	DP_HEAL_MONEY,		// 治療費用
	DP_PREVENT_MONEY,		// 預防費用
	DP_INN,		// 旅館信息顯示
	DP_MAGIC_CLASS,		// 在記憶魔法菜單中顯示陣列
	DP_MEMORIZE_POINT,		// 在記憶魔法菜單中顯示記憶點數
	DP_NORMAL_PUT,		// 0908 kkh 增加 正常顯示
	DP_DATAPUT_DEFAULT,		// 居中顯示資料值（顯示值和顏色）
	DP_MONEY2,		// 帶有 ',' 分隔符的金額顯示
	DP_BACK_IMAGE_VALUE_TEXT,		// 背景圖片上的數值文字顯示
	DP_TITLE_PUT,		// 菜單標題顯示
	DP_STATUS_IMAGE,		// 010830 lsw 狀態相關圖片顯示
	DP_NATION_JOIN_STATUS_BAR,		// 國家加入狀態條
	DP_DATAPUT_DEFAULT_STRING,		// 顯示字串 (使用 m_lpTemp)
	DP_GM_2ND_SKILL_TARGET_ITEM_PUT,
	DP_NATION_WAR_RESULT_BAR,
	DP_REMAIN_TIME_PUT,		// 剩餘時間顯示
	DP_NATION_WAR_RESULT_ANIMATION,		// 國戰結果動畫
	DP_RARE_UPGRADE_RESULT,
	DP_RARE_SUCCESS_RATE_DISPLAY,		// 011013 lsw 稀有物品升級成功率顯示
	DP_RARE_GROUP_MEMBER_DISPLAY,		// 011013 lsw 稀有物品群組成員顯示
	DP_RARE_SUCCESS_RATE_DISPLAY_NO_TOTAL,	// 不顯示總計的成功率
	DP_LEVEL_IN_TIME_MENU,		// 在時間菜單中顯示等級
	DP_NAME_CENTER,		// 020515 lsw 名稱居中顯示

	DP_AUCTION_FK_RARE,						// 拍賣查找鍵：稀有度
	DP_AUCTION_FK_LV,						// 拍賣查找鍵：等級
	DP_AUCTION_FK_TACTIC,					// 拍賣查找鍵：戰術
	DP_AUCTION_FK_WEAR,						// 拍賣查找鍵：穿戴位置

	DP_SKILL_MASTER_RESULT_ITEM,			// 技能大師製作結果物品顯示
	DP_BETTING_MONEY,						// 投注金額
	DP_MY_BETTING_MONEY,					// 我的投注金額
	DP_ARENA_WAIT_TIME,						// 競技場等待時間計數
	DP_TEMP_MAIL_ICON,						// 臨時郵件圖示
	DP_NOW_VIEW_MAIL_PAGE,					// 當前郵件頁數
	DP_ARENA_TEAM_WIN_RATE,					// 競技場隊伍勝率

	DP_AUCTION_DATE,						// soto-030512 拍賣菜單中日期查找欄位文字
	DP_CHARACTER_RAJA_EMBLEM,				// 角色 Raja 徽章
};

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
// FT_DO 使用的動作 (10, 11, 35, 36, 100, 101, 102, 103, 114, 115, 200, 210, 300, 310, 320, 330, 340)
enum DoIndex
{
	DO_CHARACTER = 10,		// 繪製角色圖片
	DO_TEXT_PUT = 11,		// 在訊息框中顯示字串
	DO_BANK_SCROLL_UP = 35,		// 銀行向上捲動
	DO_BANK_SCROLL_DOWN = 36,		// 銀行向下捲動
	DO_BANK_TIME_SCROLL_UP = 100,		// 銀行日期向上捲動
	DO_BANK_TIME_SCROLL_DOWN = 101,		// 銀行日期向下捲動
	DO_BANK_SCROLL_LEFT = 102,		// 銀行向左捲動 (查找)
	DO_BANK_SCROLL_RIGHT = 103,		// 銀行向右捲動 (查找)
	DO_SKILL_ICON_SELECT = 114,		// 技能菜單捲動時改變圖示圖片
	DO_SKILL_ICON = 115,		// 技能欄位標題和其他欄位顯示
	DO_FACE_MENU = 200,		// 表情選擇窗口
	DO_SELECT_LOAD_CHAR = 210,		// 載入選中的角色
	DO_CHATTING_VIEW = 300,		// 在聊天窗口中顯示對話
	DO_CHATTING_EDITBOX = 310,		// 聊天窗口中寫作的編輯框
	DO_LEARNSKILL_ICON = 320,		// 顯示可學習的技能
	DO_DIVIDE_SCROLL_UP = 330,		// 分割物品窗口向上捲動按鈕
	DO_DIVIDE_SCROLL_DOWN = 340		// 分割物品窗口向下捲動按鈕
};

/////////////////////////////////////////////////////////////////////////////////////////////////
// 處理菜單開關的巨集定義
#define MM_STATUS_OPEN				1			// 主菜單：開啟狀態菜單
#define MM_STATUS_CLOSE				2			// 主菜單：關閉狀態菜單
#define MM_ABILITY_CLOSE			3			// 主菜單：關閉能力菜單
#define MM_ARIGEMENT_CLOSE			4			// 主菜單：關閉陣營/善惡值菜單
#define MM_ITEM_OPEN				13			// 主菜單：開啟物品菜單
#define MM_ITEM_CLOSE				14			// 主菜單：關閉物品菜單
#define MM_PARTY_OPEN				15			// 主菜單：開啟隊伍菜單
#define MM_PARTY_CLOSE				16			// 主菜單：關閉隊伍菜單
#define MM_TACTICS_CLOSE			17			// 主菜單：關閉戰術菜單

#define MOVE_OPEN					1			// 移動狀態：開啟
#define MOVE_CLOSE					0			// 移動狀態：關閉

extern char* GetDayOfWeekByNo(const int iDOWIndex);
#endif//AFX_MENUDEFINE_H__CCD50119_39F2_4932_8DEF_B46CD7E4330A__INCLUDED_