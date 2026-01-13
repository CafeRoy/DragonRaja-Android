#ifndef __MenuNetwork_h__
#define __MenuNetwork_h__

typedef struct tagItem
{
	int			x;
	int			y;
	RECT		rect;
	K_ITEM		item;
}DefaultItem;

class CServerText
{
public :
	bool is_first;
	char *m_pszMessage;
	int	 m_cPrintCount;

public :
	CServerText() { is_first = true; m_pszMessage = NULL; m_cPrintCount = 0; }
	~CServerText() { ReSet(); }
	void ReSet(){ 
		is_first = true; 
		if( m_pszMessage ) delete m_pszMessage; 
		m_pszMessage = NULL; 
		m_cPrintCount = 0;
	}
	void SetText( char *szText ){
		if( m_pszMessage ) delete m_pszMessage;
		int len = strlen( szText );
		m_pszMessage = new char[len+1];
		strcpy( m_pszMessage, szText );
		is_first = false;
	}
	int operator++( int ){ return ++m_cPrintCount; }
};
extern CServerText		public_bbs;

//extern LINECOMMAND LC[ MAX_LINE_COMMAND_];

//////////////////////////////////////////////////////
//	���� ��ĵ�Ҷ� �� �Ѽ� 
const int MAX_ENEMY_SCAN_TARGET	= 50;



enum eLOCALWARCAST
{
	LWCAST_LV = 2,
	LWCAST_KILLMUCH = 3,
	LWCAST_DEADMUCH = 4,
	LWCAST_STOPKILL = 5
};

enum eRESERVEDATATYPE {
	RES_DATA_5CH = 0,//5轉任務使用
	RES_DATA_KILL = 1,//連續殺人使用
	RES_DATA_DEAD = 2,//總死亡次數
	RES_DATA_SAVELIFE = 3,//救人
	RES_DATA_TOTAL_KILL = 4,//總殺人數
	RES_DATA_TAKE_ITEM = 5,//檢拾物品
	RES_DATA_GET_DMG_TOTAL = 6,//總承受傷害
	RES_DATA_ATK_DMG_TOTAL = 7,//總打出傷害
	RES_DATA_ASSISTANT = 8,//助攻數
};
//////////////////////////////////////////////
// BBS�� ��� �ַ� �����ϴµ� ����ϰ� �ȴ�. ( �̸��� �޾ƿͼ� �ѷ��ش�. )
#define		BA_GM_REGIST	1


///////////////////////////////////////////
//	RecvServerResult ���� ����� define
#define CM_REPAIR			0
#define CM_LEARNSKILL		1
#define CM_LEARNMAGIC		2
#define CM_PREVENTING		3		// ����
#define CM_CURE_DISEASE		4		// ġ��
#define CM_INN_SLEEP		5		// ����
#define CM_INN_REST			6		// ���ڱ�
#define CM_OPEN_SUPPER		7		// ���� ���� ���� 

///////////////////////////////////////////

#define ET_ADD_FIGHT_MEMBER	9777

extern SkillExp t_skillexp;

extern char nation_bbs[2000];
extern char nation_bbs2[8][8][50];

extern k_send_client_req_stop_war recv_stop_war_data;;

////////////////////////////////////////////////////// ���� �Լ� 
extern	void	ReqCurrentTime();
extern	void	RecvPublicNoticeTitle( t_server_menu_message_title *p )	;
extern	void	ReqMessageTitle( int type, int count );
extern	void	ReqCharList( );
extern	void	RecvCharInfoBasic( t_char_info_basic *p );
extern	void	RecvCharInfomation( t_server_user_db_data *p );
extern	void	SendShowItemOfOtherCh( int ch_id );
extern	void	RecvOtherChInv( kein_server_other_ch_inv *inv );
extern	void	SendDivideItem( K_ITEM *item, int much );		// �������� �������� �Ҷ�...
extern	void	SendCharInfoBasic( int kind, DWORD data );		// Ŭ���̾�Ʈ���� ������ �ɷ�ġ�� ������ ��ü�Ѵ�.
extern	void	SendGenrateItemByTool( k_client_create_item p );		// ������ �������� �����Ѵ�.
extern	void	RecvDeleteItem( POS pos, int is_broken = 0 );
extern	void	RecvDecreaseItem( k_server_decrease_item *item );

extern	void	SendLvUpPointEach( int type );		// 1����Ʈ �ö� ��������..
extern	void	SendLvDownPointEach( int type );
extern	void	ReqLvUpPoint( );	// �ӽ� ������ ����Ʈ ��...
extern	void	RecvLevelDownResult( int type );


extern	void	ReqItemInfo0();
extern	void	ReqItemInfo1( );
extern	void	ReqItemInfo2( int count );
extern	void	ReqMagicInfo();
extern	void	ReqSkillInfo();
extern	void	ReqConnectInfo();

extern	void	RecvLearnSkill( int s );
extern  int 	SendLearnItem( ItemJoinSelect item );	// ������ ����.. 
extern	void	RecvLearnItemOk( k_item_no item );		// ������ ���� OK,  ����Ʈ���� �ϳ� �������ش�.
extern	void	RecvItemRepar( k_server_item_repair_ok *r );
extern  void	RecvItemReparHammer( k_server_item_repair_ok *r );
extern	void	SendItemRepair( int item_pos, int store_id );
extern  void	SendItemRepairHammer( PosItem pos);

extern  void	SendMagicItem(int type, PosItem item_pos);
extern void SendCraftItem(PosItem pos);

extern	void	RecvServerResult( k_server_result *p );
extern	void	RecvTacSkillExp( t_char_info_tac_skillexp *s );
extern	void	ReqTacSkillExp( );
//011022 lsw >
extern	void	RecvSkillExp( int type, t_SkillExp exp );
//011022 lsw <
extern	void	RecvStatusOpen( k_server_status_open *p );
extern	void	RecvBankInventory( k_server_bank_item *i );
extern	void	RecvBank2Inventory(k_server_bank_item* i);
extern  void     RecvbbsKill(tag_team_battle_msg2* p);

extern	void	RecvParty( k_server_party *p );
extern	void	RecvRelation( k_server_relation *p );
extern	void	RecvEmployment( k_server_party *p );

extern	int		SendInputParty( int type, int ct, LpCharacterParty ch );		// ��Ƽ�� �α� ���� ����
extern	void	SendDeleteParty( int type, int ct );
extern	void	SendLearnMagic( int magic_num );
extern	void	RecvLearnMagicOk( int num );
extern	void	RecvPreventingOk( short int num );
extern	void	SendClientPreventing( int num );		// ���� ��ȣ
extern	void	SendClientCureDisease( int num );
extern	void	RecvCureDiseaseOk( short int num );


extern void ReqItemValue( int item_no );
extern void RecvTidyInventory(t_packet* t);
extern void RecvItemValue( int item_no, int money );
extern void RecvItemValue_Sell( int item_no, int money );
extern void RecvItemValue_Repair( int item_no, int money );
extern void ReqMallItemPrice(int item_no);
extern void RecvMallItemPrice(int item_no, int money);
extern void SetInputParty( int type, int num, char *name );		// Ŭ���̾�Ʈ���� ��Ƽ�� �ΰ� �װ� ������ ������ �ش�

extern void RecvPartyInfo( k_party_info *p );					// ��Ƽ������ �⺻���� �ɷ�ġ
extern void RecvDeleteParty_server( int type, int index );
extern void SendReqPartyInfo( int type, int num );

extern void SendClientInn( int type );
extern void ReqItemValue_Sell( int type, POS pos );

extern void RecvServerWakeUp();					// �������� ������ ����� ����...

extern void RecvJoinGameWho( char *name );		// �̸����� �ұ�? �ƴ�, ��ȣ�� �ұ�?~~
extern void RecvExitGameWho( char *name );
extern void RecvMyPartyExist( char *name );		// ��Ƽ���� �̹� ��� �� ����� �ִ� ��...
extern void RecvPartyJoinOk( char *name );		// ���� ��Ƽ�� ���� ����̴�...
extern void RecvPartyJoinRelease( char *name );
extern void RecvServerEachItem( K_ITEM *item );
extern void RecvServerResist( k_server_resist *p );

extern int SubtractMoney( DWORD money );
extern int AddMoney( DWORD money );

extern void RecvItemAttrChange( int type, k_server_item_attr *p );
extern void RecvOtherGhost( );
extern void OpenStatus();
extern void RecvServerMagery(char nStep, DWORD magery);
extern void RecvCreateAbility( );		// �ֻ��� �������� �⺻��ġ �ʱ�ȭ
extern void RecvThrowDice( start_throw_dice *p );
extern void SendThrowDice( char type );
extern void RecvExchangeItemDelWhereRecv( K_ITEM &lpItem );
extern void ReqMessageBbs( int type, int count );
extern void RecvServerMessage( k_get_server_text *p );
extern void SendPreachName( char *name, BYTE god );
extern void SendGodRegistOk( int type );
extern void RecvPreachName(  k_server_client_preach_name *p  );
extern void SendPreachNameOk( char *name, BYTE god );

extern void RecvGodMeetinMainStart( );
extern void RecvGodMeetingMainEnd( );
extern void SendToolPassword( char *password );
extern int  SendCheckNewChar( char *name );
extern void RecvServerMessageDefault( k_server_text_default *p, CServerText *text );		// ������ ��û�� �� �ʱ�ȭ �ʿ�
extern CServerText g_ServerText;
extern void RecvCharInfoPlus( k_server_to_client_char_info_plus *p );
extern void RecvCanMemorizeOk();
extern void SendRecvEarMessageResult( short int send_game_server, short int send_server_id, char ret );			//  ���� �޽ÿ� ���� ���

extern void SendChangeMap_kein( short int port );	// �� ��ȯ - ÷��: �̵��� ���Ӽ����� ��Ʈ��ȣ
extern void AllReqSkillExp( int is_end=0 );		// ��ų ��û.. ���� �־� �ٰ�� �ٽ� �����Ҷ�
extern void RecvOpenSupper( char much );
extern void SendIGetItemOfSupper( short int item_no );

extern void RecvSetNoteMeetingMessage( short int god, short int type );
extern void SendReqCharInfo( int id );		// Ŀ���� ��� �ִ� ���� ĳ������ ������ �����´�.
extern void RecvReqCharInfo( k_server_req_char_info *p );

extern void SendReqHpOfParty( char *party_name );
extern void RecvReqPartyInfo( k_rep_party_info *p );
extern void RecvCallPublicBbs( k_server_text_default *p );		// �������� Ŭ���̾�Ʈ�� �������� ������ ������..
extern void RecvLearnedItem( k_server_learn_item *p );
extern void SendHunterRegist( char *name, DWORD money, char *text );
extern void SendGetHunterList( int id );
extern void RecvHunterList( k_hunter_list *hunterlist );		// ����� ����Ʈ ��������
extern void SendKillHunterList( int id );
extern void RecvKillWho( char *who );
extern void RecvKillHunterOk( k_kill_hunter_ok *result );
extern void RecvHunterRegistResult( char *hunter, char *target );
extern void SendDangerOfHunter( short int target_id, char type );

extern void RecvDangerByHunter( char type );
extern void SendReqQuestIndex( int page);
extern void RecvQuestIndex( k_rep_quest_index *p );
extern void RecvPartyNotByLevel( short int type );
extern void RecvDoPartyOk( char *name, short int type );

extern void SendReqManToManResult( short int target_id, int ret );
extern void RecvReqManToMan( short int target_id );
extern void RecvReqManToManResult( char ret );
extern void SendReqManToMan(const char *name );		// ���� ��û

extern void SendManToManRefusal( short int target_id );
extern void RecvStartManToMan( int target_id );
extern void SendManToManStart( short int target_id  );
extern void SetFightMode(int target_id, int	stone_X, int stone_Y);
extern void RecvReqManToManRefusal( );
extern void MenuSoundClose( int menu );
extern void SendManToManEachItem( short int target_id );
extern void RecvFightMapAddOk( t_packet *p );
extern void RecvFightMapStart( );
extern void RecvFightMapAddNot( t_packet *p );
extern void RecvFightMapEnd( );
extern void SendPacketDefault(const int type, void *data = NULL, const int size = 0 );
extern void SendResetJob( char job, char cls, char *name );
extern void SendSelectNation( int nation, char *name );
extern void SendChangeScriptValue( int script_num, int value );
extern void RecvChangeDuration( t_packet *p );

extern void SendIsThereCharNameForNation( char *name );		// ���� �񱳸� ���� ����
extern int CheckCharNameAndNation( char *name );
extern void RecvCallRegistCandidate( );
extern void RecvCallVoteSupport( t_packet *p );
extern void RecvCheckVoteOk( t_packet *p );		// �ĺ� ������ ������ �ͼ� �ѷ��ش�.
extern void SendCheckVote( );
extern void RecvCheckVoteNot( t_packet *p );
extern void RecvRegistCandidateNot();
extern void SendVoteSupport( short int num );		// �ĺ� ��ȣ
extern int SendReqVoteScore();
extern int RecvVoteScore( t_packet *p );
extern void SendReqSkillInfo( short int id );
extern void SendReqSkillInfo( LPCHARACTER ch );		// ������ ��ų ������ ��û�Ѵ�.
extern void RecvGetSkillInfo( t_packet *p );
extern void SendAllMsgNation( std::string& text);
extern void RecvNationMassage( t_packet* p );
extern void RecvYouAreKing( int type );

extern void SendGuildExit(const char * name );			// ������ Ż���Ų��.
extern void RecvGuildExistYou( );
extern void RecvGuildExistOk(  t_packet *p );
extern void RecvGuildExistWho( t_packet *p );

extern void SendGuildJoin(const char *name );
extern void RecvGuildJoinOk( t_packet *p );
extern void RecvGuildJoinFail( t_packet *p );
extern void RecvGuildJoinConfirm( t_packet *p );


extern void SendGuildJoinConfirmOk( short int target_id );		// ��� ������ ����ߴ�.
extern void SendGuildJoinConfirmNot( short int target_id );		// ��� ������ �ź��ߴ�.

extern void RecvScanTarget( t_packet *p );
extern void SendGuildAllMsg(std::string &msg );
extern void RecvGuildAllMsg( t_packet *p );
extern void SendGuildChangeEachLevel(std::string &lv_str, std::string &name);		// ������ ����� �ٲ۴�.
extern void RecvGuildEachLevel( t_packet *p );		// ������ ����� �ٲ���.

extern void RecvGuildJoinConfirmIng( );
extern void SendGMRegist( int guild_count, int type = CMD_GM_REGIST );
extern void CheckAndSendGmRegist( int index )	;
extern void RecvBbsAll( t_packet *p );
extern void RecvRegsitGmWho( t_packet *p );
extern void SendReqStopWar( char *text );
extern int ScanTarget( int nation );		// ���� ����

extern void RecvScriptMessageBox( t_packet *p );

extern void SendSalvationMoney( DWORD money );
extern void RecvRepVationMoney( t_packet *p );
extern void RecvSalvationTotalMoneyMsg( t_packet *p );
extern void SendChangeMoney( DWORD old_money );
extern void RecvChangeMoney( t_packet *p );
extern void RecvEatChocolateOk( t_packet *p );
extern void RecvRumor( t_packet *p );
extern void RecvRumorNot();
extern void RecvTEAMBATTLE_INFO();

extern void RecvOpenSchoolResult( t_packet *p );
extern void RecvLoanInfo( t_packet *p );
extern void SendReqLoan( DWORD loan );

extern void SendReqRepayment( DWORD repayment );
extern void RecvFightMapCharInfo( t_packet *p );
extern void RecvMerchantMessage( t_packet *p );
extern void RecvMerchantItemBbs( t_packet *p );
extern void SendMerchantItemMessage( char *item_name, DWORD money, int map_type );

extern void RecvAliveFailResult( t_packet *p );
extern void SendReqLadderRank( int page );
extern void RecvRankLadderData( t_packet *p );
extern void RecvRankLadderFinish( );
extern void SendDirectDB( int type, void *msg, int size );

extern bool g_bSendLadderRank;

extern void RecvSearchRankLadderFail();
extern void RecvtEffectByScript( t_packet *p );
extern int SendReqVisa( int nation );
extern void RecvReqVisaItem( t_packet *p );
extern k_visa_item NationVisa[7];
extern void SendBuyVisa( int nation, int select );
extern int g_VisaBuyFlag;



extern void SendRegistGuildImsi( char *guild_name, char *guild_explain );
extern void RecvWhoRegistGuildOk( t_packet *p );
extern void RecvGuildInfoName( t_packet *p );
extern void SendReqGuildInfo1( int guild_code );		// 010719 YGI
extern void RecvGuildExplain( t_packet *p );
extern void SendGuildMark( int guild_code, Spr *guild_mark );

extern void RecvGuildMarkImage( t_packet *p );
extern void SendRegistGuild( int guild_code, char *szSubMasterName  );
extern void SendChangeGuildDegree( int guild_code );
extern void RecvGuildDegreeInfo( t_packet *p );
extern void RecvReqGuildIntroduction( int is_first, t_packet *p );
extern void SendReqGuildIntroduction( int nation );

extern void SendUpdateGuildMailId( DWORD id );
extern void RecvGuildMailDeleteResult( t_packet *p );
extern void RecvGetGuildMasterName( t_packet *p );
extern void RecvPutMenuString( t_packet *p );
extern void RecvRegistGuildImsiFail( t_packet *p );
extern void RecvRegistGuildCheckOk();
extern void RecvRegistGuildCheckFail( t_packet *p );
extern void RecvCheckGuildCode( t_packet *p );

//////////////////////////////
#define CS_INPUT				1		// ��� ��� �޴��� ��� ���� ��Ų��.
#define CS_CHANGE_MASTER		2		// ��� ������ ������ ��� �����Ų��.
#define CS_CHANGE_SUB_MASTER	3		// ���� ������ ������ ��� �����Ų��.

extern void SendCheckSubGuildMaster( char *sub_master_name );
extern void RecvCheckSubMasterOk( t_packet *p );
extern void RecvCheckSubMasterFail( t_packet *p );

/////////////////////////////////////////////
extern void RecvRegistGuildOk( t_packet *p );

extern void RecvRefreshGuild( int guild_code );

extern void RecvGuildChatData( t_packet *p );
extern void RecvResetAbility( t_packet *p );
extern void CheckGuildActive( int guild_code );
extern void RecvDeleteGuild( t_packet *p );

extern void SendExitGuild( );
extern void RecvGhostQuestInfo( t_packet *p );
extern void SendChangeColor( int type1, int type2, int r, int g, int b );
extern void RecvChangeColor( t_packet *p );

extern void RecvSalvationAddFame( t_packet *p );
extern void RecvRepNationMoney( t_packet *p );
extern void RecvCheckSalvationName( t_packet *p );
extern void SendGameItem( MakeItem *matter );
extern void RecvGambleItemResult( t_packet *p );
extern void RecvGambleStart( t_packet *p );
extern void RecvReporterMode( t_packet *p );
extern void RecvGuildMemberName( t_packet *p );
extern void RecvGetGuildNotice( t_packet *p );
extern void RecvRegistFriendResult( t_packet *p );
extern void SendRegistFriend( const char *name );//030504 lsw
extern void RecvRegistFriendName( t_packet *p );
extern void RecvAddFriend( t_packet *p );
extern void RecvChangeDate( t_packet *p );
extern void RecvGetTodayGambleItem( t_packet *p );
extern void RecvTodyaItemGetWho( t_packet *p );
extern void SendOXEventAboutMap( char type );
extern void SendOXQuestion( char *str );
extern void RecvOXQuestion( t_packet *p );
extern void RecvSetOXTime( t_packet *p );
extern void SendSelectPartyExp( char select );

extern void RecvGuildItemFirst( t_packet *p );
extern void RecvGuildItem( t_packet *p );
extern void SendDBForGuildItem( DWORD index );

extern void SendInsertItem( POS *pPos);
extern void RecvGuildBoxMoney( t_packet *p );
extern void RecvGuildItemCount( t_packet *p );

extern void RecvChangeGuildDegree( t_packet *p );
extern void SendChangeGuildMasterBySubMaster( int type, int guild_code, char *name );

extern void SendCheckGuildChange( int type );
extern void RecvCheckGuildChange( t_packet *p );
extern void RecvGetNationItem( t_packet *p );
extern void RecvBuyNationItemOk( t_packet *p );
extern void RecvCheckColossusMapReply( t_packet *p );
extern void RecvInviteColossusReply( t_packet *p );
extern void RecvAttackColossusStoneResult( t_packet *p );
extern void RecvCallMainMenu( t_packet *p );

// 020320 YGI
extern void RemoveExchangeItemStatus();
extern void RecvGuildHouseInfo( t_packet *p );
extern void RecvSetPkOnOff( t_packet *p );
extern void RecvGetPkOnOff( t_packet *p );

extern void RecvSeachOtherChar( t_packet *p );

// 020620 YGI
extern void RecvSettingMapTile( t_packet *p );
extern void DeleteDinamicTile( int index );
extern void RecvDeleteMapTileEvent( t_packet *p );
extern void RecvGetScenarioInfo( t_packet *p );
extern void RecvScenarioMessage( t_packet *p );
extern void RecvScenarioTrapInfo( t_packet *p );
extern void RecvScenarioTrapKey( t_packet *p );// 020701 YGI
extern void RecvGetScenarioTime( t_packet *p );		// 020830 YGI
extern void RecvMoveMapForLightVersion( t_packet *p );	// 021008 YGI
// 021107 YGI
extern void RecvCheckEventMoveMap( t_packet *p );
extern void SendCheckEventMoveMap();	
extern void SendGetTreasureMapDetail( POS *pPos );	// 040105 YGI

extern void SendMantleDye(int r, int g, int b); // finito 25/06/08
extern void RecvMantleDye( t_packet *p ); // finito 25/06/08

extern void SendApplicationOfExile(LPCHARACTER pMaster); //Eleval 12/06/09
extern void RecvApplicationOfExileResult( t_packet *p ); //Eleval 12/06/09

extern void SendSexChange(LPCHARACTER pUser); //Eleval 12/06/09 - Sex Change
extern void RecvSexChangeResult( t_packet *p ); //Eleval 12/06/09 - Sex Change

extern void RecvToolPlaySound( t_packet *p ); //Eleval 19/06/09 - Tool Play Sound

extern void RecvCharOptionsChange( t_packet *p ); //Eleval 18/08/09 - To change a char's options
extern void RecvTeleportToCharResult( t_packet *p ); //Eleval 19/08/09 - Tool to teleprot to a char

extern void RecvReqHonorAmount( t_packet *p ); //Eleval 07/09/09 - To request the player's current honor
#endif		// #ifndef __MenuNetwork_h__
