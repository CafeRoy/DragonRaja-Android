#include "skill_lsw.h"
#define CMD_SKILL_RARE_UPGRADE					5600
#define CMD_SKILL_RARE_UPGRADE_RESULT			5601

#define GM_TOOL_VERSION							2122//020101 lsw 

typedef struct HWOARANG_SANGWOO_SKILLRAREUPGRADE
{
	short int iSkillNo;

	ItemAttr	SourceItem;

	int iBuffer1;
	int iBuffer2;
	
	POS		pos[MAX_UPGRADE_RARE];

}t_SkillRareUpgrade;

typedef struct HWOARANG_SANGWOO_SKILLRAREUPGRADE_RESULT
{
	short int	iSuccess;
	ItemAttr	SourceItem;

}t_SkillRareUpgradeResult;


#define CMD_CALL_SMALLMENUCUSTOM				2002
typedef struct server_call_smallmenucustom
{
	short int iMenuNo;
	short int iPicNo;
	short int iWidth;
	short int iHeight;
	short int iExt;
}t_server_call_smallmenucustom;

#define CMD_RARE_UPGRADE_BBS					2003
#define CMD_RARE_UPGRADE_BBS_MAP_TO_MAP			2004

typedef struct HWOARANG_SANGWOO_RAREUPGRADE_BBS
{	
	char	name[20];
	ItemAttr	Item;
	bool        Santa;
}t_RareUpgradeBBS;

typedef struct HWOARANG_SANGWOO_RAREUPGRADE_BBS_MAP_TO_MAP
{	
	char	name[20];
	ItemAttr	Item;
}t_RareUpgradeBBSMapToMap;


#define CMD_ON_CONNECT	6099
typedef struct ONCONNECTHSELINIT
{
	HSEL_INITIAL	eninit;
	HSEL_INITIAL	deinit;
}t_OnConnectHselInit;

#define CMD_MOVE_TO_HOMETOWN					2005
#define CMD_MOVE_TO_HOMETOWN_WITH_ALIVE			2007

#define CMD_DAYLIGHT							2006//���� ó�� ������
typedef struct DAYLIGHT
{
	short int nDayLightControl;
}t_DayLight,*LPDAYLIGHT;

#define CMD_GET_SKILL_POINT						9018

#define CMD_NOW_SKILL_POINT						9019
typedef struct NOWSKILLPOINT
{
	ItemAttr	item;
	int			iNowPoint;	
}t_NowSkillPoint,*LPNOWSKILLPOINT;

#define CMD_MERCHANT_BUY_ITEM_SEARCH			16000//020815 lsw

//<soto-030511 ����.
typedef struct SEARCHPACKETCLIENT
{
	char	szKeyWord[21];	//Ű���� �˻��� ��
	char	szMerchant[21]; //�����̸� �˻��� ��
	int		nPeriod;		//��� �Ⱓ ����.// 0:1��, 1:3��,  2:������.
	int		iIndex;
	int		iKey;
}t_SearchPacketClient,*lpSEARCHPACKETCLIENT;
//>soto-030511 ����.


//�������� ��ġ�� ���� ����ü��
typedef struct SEARCHRESULT
{
	int			iIndex;
	char		szSellerName[21];
	char		szBuyerName[21];
	int			iSellValue;//�Ǹűݾ��� 20���� �ѱ��� �ʽ��ϴ�.
	int			iIsEnd;
	int			iSellerTake;
	int			iBuyerTake;
	ItemAttr	m_ResultItem;
}t_SearchResult,*lpSEARCHRESULT;

#define CMD_MERCHANT_BUY_ITEM_SEARCH_RESULT			16001//020815 lsw
#define CMD_MERCHANT_SELL_ITEM_INFO_REQUEST_RESULT	16004

#define CMD_MERCHANT_RESULT_LIST_REQUEST			16030
#define CMD_MERCHANT_RESULT_LIST_REQUEST_RESULT		16031

const int MAX_SEARCH_RESULT_LIST = 24;
const int MAX_VIEW_ABLE_PER_PAGE = 6;//���������� �� �� �ִ� ����Ʈ ��

const int AUCTION_PAGE_MIN = 0;//���ſ��� ���� ������ ��
const int AUCTION_PAGE_MAX = 3;//���ſ��� ���� ������ ��

typedef struct SEARCHRESULTLIST
{
	int iCn;
	char szName[21];
	int iKey;
	SEARCHRESULT ResultList[MAX_SEARCH_RESULT_LIST];
}t_SearchResultList,*lpSEARCHRESULTLIST;
//
//�����ڿ� ������ ������ �ϰڴٴ� ��Ŷ
#define CMD_MERCHANT_ITEM_BUY						16005
typedef struct MERCHANT_ITEM_BUY
{
	int			iIndex;//������ �������� �ε��� ��
	int			iCn;//Ŀ�ؼ� ��
	char		szSellerName[21];//�Ǹ��� �̸�
	char		szBuyerName[21];//������ �̸�
	DWORD		dwSellValue;//�Ǹűݾ��� 20���� �ѱ��� �ʽ��ϴ�.
	ItemAttr	SellItem;
	int			iKey;
}t_MerchantItemBuy,*lpMERCHANTITEMBUY;

#define CMD_MERCHANT_ITEM_BUY_RESULT	16006
//

//�Ǹ��ڿ� ����ü
#define CMD_MERCHANT_SELL_ITEM_INFO_REQUEST		16003
typedef struct SELLERITEMREQUEST
{
	int iCn;
	int iIndex;
	int iKey;
	char szName[21];//�ڽ��� �̸��� ������ �������� �˻� �մϴ�.
}t_SellerItemRequest,*lpSELLERITEMREQUEST;


#define CMD_MERCHANT_SELL_ITEM_REGISTER					16009//�Ǹž������� ��� �մϴ�.
#define CMD_MERCHANT_SELL_ITEM_REGISTER_RESULT			16010//�Ǹž������� ��� �մϴ�.
typedef struct SELLERITEMREGISTER
{
	int			iCn;
	char		szName[21];
	int			iFKRareType;
	int			iFKLevel;
	int			iFKTacticType;
	int			iFKWearType;
	POS			pos;
	DWORD		dwSellValue;//--> ���� 0 �̸� �����ŷ�
	ItemAttr	SellItem;
	int			iKey;
}t_SellerItemRegister,*lpSELLERITEMREGISTER;

#define CMD_MERCHANT_SELL_ITEM_DELETE				16020
#define CMD_MERCHANT_SELL_ITEM_DELETE_RESULT		16021
#define CMD_MERCHANT_RESULT_TAKE					16032
#define CMD_MERCHANT_RESULT_TAKE_RESULT				16033

#define CMD_MERCHANT_BACKUP_LIST_REQUEST			16036	// BBD 040303
#define CMD_MERCHANT_BACKUP_LIST_RESPONSE			16037	// BBD 040303
#define	CMD_MERCHANT_BACKUP_TAKE_REQUEST			16038	// BBD 040303
#define CMD_MERCHANT_BACKUP_TAKE_RESPONSE			16039	// BBD 040303

typedef struct SELLERITEMDELETE
{
	int			iIndex;//�ǸŹ�ǰ�� �ε���
	int			iCn;
	char		SellerszName[21];
	char		szName[21];
	DWORD		dwSellValue;
	ItemAttr	SellItem;
	int			iKey;
}t_SellerItemDelete,lpSELLERITEMDELETE;

//#define CMD_MERCHANT_RESULT_TAKE						16032
//#define CMD_MERCHANT_RESULT_TAKE_RESULT					16033
#define CMD_MERCHANT_RESULT_TAKE_COMFORM				16034
#define CMD_MERCHANT_RESULT_TAKE_COMFORM_RESULT			16035
typedef struct MERCHANTRESULTTAKE
{
	int			iIndex;//�ǸŹ�ǰ�� �ε���
	int			iCn;
	char		szMyName[21];
	char		SellerName[21];
	char		BuyerName[21];
	DWORD		dwSellValue;
	ItemAttr	SellItem;
	int			iSellerTake;
	int			iBuyerTake;
	int			iKey;
}t_MerchantResultTake,*lpMERCHANTRESULTTAKE;

#define CMD_MERCHANT_DIRECT_EXCHANGE_LIST_REQUSET					16090
#define CMD_MERCHANT_DIRECT_EXCHANGE_LIST_REQUSET_RESULT			16091
#define CMD_MERCHANT_EXCHANGE_RESULT								16101
typedef struct MERCHANTEXCHANGEREQUEST
{
	int			iIndex;//0���� �������� �ϰ�
	int			iCn;
	char		szSellerName[21];
	char		szMyName[21];
	int			iKey;//�ε��� ���ΰ� �Ʒ��ΰ�.
}t_MerchantExchangeRequest,*lpMERCHANTEXCHANGEREQUEST;

const int CMD_SKILL_MASTER_MULTI_RARE_MAKE				=		16200;

typedef struct MULTIRARESRC
{
	MakeItem MultiRareSrc[2];
}t_MultiRareSrc,*LPMULTIRARESRC;

typedef struct SKILLMASTERMULTIRAREMAKE
{
	int iCn;
	int iSkillNo;
	MULTIRARESRC MultiRareSrc;
}t_SkillMasterMultiRareMake,*LPSKILLMASTERMULTIRAREMAKE;

const int CMD_EXCHANGE_BOND_MONEY				=		16201;
typedef struct EXCHANGEBONDMONEY
{
	int iCn;
	POS	pos;//��ġ 
	ITEMATTR BondMoneyItem;//����Ӵ� ����
	int iKey;//�������� ����� Ű
}t_ExchangeBondMoney,*LPEXCHANGEBONDMONEY;

const int CMD_SELECTED_CHAR_RESULT_CLIENT_BOTH_DB_DEMON = 16103;
typedef struct SELECTEDCHARRESULT
{
	int			iKey;
	char		szName[MAX_NAME];
	int			iLv;
}t_SelectedCharResult,*LPSELECTEDCHARRESULT;

const int CMD_EXCHANGE_CANCEL_SIGN = 16202;

const int CMD_MAP_MOVE_LV_LIMIT		= 16203;

typedef struct MAPMOVELVLIMIT
{
	int iLvMin;
	int iLvMax;
}t_MapMoveLvLimit,*LPMAPMOVELVLIMIT;

const int CMD_TRADE_MODE_CHANGE				=		16204;//030127 lsw
typedef struct TRADEMODECHANGE
{
	int iCn;
	bool bIsTradeOn;
}t_TradeModeChange,*LPTRADEMODECHANGE;

#define CMD_CONNECT_AGENT_SERVER_CONFORM	6098
typedef struct CONNECTAGENTSERVERCONFORM
{
	char szUserID[ID_LENGTH];
	char szUserPASS[PW_LENGTH];
	int iKey;
}t_ConnectAgentServerConform,*LPCONNECTAGENTSERVERCONFORM;

// Mail System //////////////////////////////////////////////////////////////////////////////
//���� ���� ������ (Ŭ���̾�Ʈ)
const int MAX_MAIL_PAGE	= 12;
const int MAX_MAIL_RECEIVER	= 5;
const int MAX_MAIL_BODY_LEN	= 1000;

typedef struct MAILDATA
{
	int iIndex;

	int iType;
	bool bIsRead;
	
	char szSender[MAX_NAME];
    char szReceiver[MAX_NAME];
	
	char szTitle[80];
	TIMESTAMP_STRUCT	date;
	int	iBodyIndex;
	ItemAttr item;
	int take;
}t_MailDdata, *LPMAILDATA;

typedef struct INTERFACEMAILDATA//�������̽����� �� ����ü
{
	bool bIsSelect;		
	MAILDATA MailData;
}t_InterfaceMailData,*LPINTERFACEMAILDATA;

typedef struct MAILBODY
{
	int iIndex;
	char szBody[MAX_MAIL_BODY_LEN];
}t_MailBody, *LPMAILBODY;

const int CMD_MAIL_REQ_BODY		= 16102;//�ش� ������ �ٵ�
typedef struct REQESTMAILBODY
{
//	int iWantMailType;
	int iMailIndex;
	char szName[MAX_NAME];
}t_ReqestMailBody,*LPREQESTMAILBODY;

const int CMD_MAIL_REQ_BODY_RESULT		= 16113;//�ش� ������ �ٵ�
typedef struct REQESTMAILBODYRESULT
{
	//	int iWantMailType;
	int iMailIndex;
	int iType;
	ItemAttr item;
	int take;
	TIMESTAMP_STRUCT	date;
	char szSender[MAX_NAME];
	char szReceiver[MAX_NAME];
	char szTitle[MAX_NAME];
	char szBody[MAX_MAIL_BODY_LEN];
}t_ReqestMailBodyResult, * LPREQESTMAILBODYRESULT;

const int CMD_MAIL_REQUEST_LIST = 16114;//�������� �з��� ���� ����Ʈ.
typedef struct REQESTMAILLIST
{
	int iWantMailType;
	int iPageIndex;
	char szName[MAX_NAME];
}t_ReqestMailList,*LPREQESTMAILLIST;

//���� ���� �ޱ� (Ŭ���̾�Ʈ)
const int CMD_MAIL_ALERT = 16400;//���� ��

const int CMD_MAIL_SEND	=10142;	// ���� ������
typedef struct MAILSEND
{
	int iType;
	char szTarget[MAX_NAME*MAX_MAIL_RECEIVER];//5��
	char szSender[MAX_NAME];
	char szTitle[60];
	char szBody[MAX_MAIL_BODY_LEN];
}t_MailSend,*LPMAILSEND;

const int CMD_MAIL_LIST_FROM_DBDEMON = 16100;
typedef struct MAILLISTRESULT
{
	char szName[MAX_NAME];
	int iWantViewType;
	int iWantViewPage;
	MAILDATA MailData[MAX_MAIL_PAGE];
}t_MailListResult,*LPMAILLISTRESULT;

const int CMD_MAIL_DELETE	=10143;	// ���� ����
const int CMD_MAIL_REPAIR	=10145;	// ���� ����
typedef struct MAILDELETE
{
	int cn;
	char szName[MAX_NAME];
	int aiMailIndex[MAX_MAIL_PAGE];
}t_MailDelete,*LPMAILDELETE;


#define	CMD_REQ_RECV_MAIL_TITLE_GUILD		10357
#define CMD_REQ_RECV_MAIL_BODY_GUILD		10359
#define CMD_UPDATE_GUILD_MAIL_ID			10360

#define CMD_RECV_MAIL_DELETE_GUILD			10361
#define CMD_REQ_RECV_MAIL_BODY				10144	// ���� ���� ��û


#define CMD_REP_RECV_MAIL_BODY				10146
#define	CMD_RECV_MAIL_DELETE				10147

//< CSD-030808
#define CMD_SEND_ACCEL_TYPE	6061
typedef struct tag_ACCEL_TYPE
{
	int		nType;
	char	strMsg[64];
}t_ACCEL_TYPE;
//> CSD-030808

typedef struct HWOARANG_PACKET
{
	union			
	{
		char	data[MAX_PACKET_SIZE];
		t_SkillMasterMultiRareMake	SMMultiRareMake;
		t_ExchangeBondMoney			ExBondMoney;
		t_SelectedCharResult		SelectedCharResult;
		t_MapMoveLvLimit			MapMoveLvLimit;
		t_TradeModeChange			TradeModeChange;
		t_ConnectAgentServerConform	ConnectAgentServerConform; //030325 �±�����
	
		t_ReqestMailList			ReqestMailList;
		t_MailListResult			MailListResult;
		t_MailSend					MailSend;
		t_MailDelete				MailDelete;
		t_ReqestMailBody			ReqestMailBody;
		t_ReqestMailBodyResult		ReqestMailBodyResult;		
	}rang;
}t_Hwoarang;