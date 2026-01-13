extern void CallGuildMemberListMenu();
extern int CallFriendMenu();
extern void SendWisperByLineCommand(std::string name, std::string text);
extern bool SendPartyChatByLineCommand(const char *str_buf );//020701 lsw
extern char *GetNationName( int nation );
extern void CallGetGuildItem();
extern void CallGuildItemPut();
extern int GetMoneyItemNumber();
extern void CallChangeMaster();
extern void SendGetGuildMasterAndSubMasterName( int guild_code, int type );
extern void CallHtmlMessageBox( int next_menu, char *file_name );
extern void CallNationItem();


// guild_degree_power
#define GDP_JOINING				0	// ��� ����				
#define GDP_BBS					1	// ��� ��ü �޽���			
#define GDP_CHANGE_DEGREE		2	// ���� ��å����			
#define GDP_GUILD_BOX			3	// ��� â�� �̿�			
#define GDP_EXIT_GUILD_HIM		4	// ���� ��� Ż�� ��Ű��	
#define GDP_CHANGE_MARK			5	// ��� ��ũ ����			
#define GDP_CHANGE_DEGREE_INFO	6	// ��� ��å �� ����		
#define GDP_GUILD_CHANGE_MASTER	7	// ��� ������ ����			
#define GDP_GUILD_BREAK			8	// ��� ����				
#define GDP_REGIST_SUB_MASTER	9	// �α渶 ���				
#define GDP_EXIT_GUILD_SELF		10	// ������ ��� Ż��			
#define GDP_BUY_GUILD_HOUSE		11	// �������� ��� �ִ� ����
#define GDP_MAX					11	// �ƽ� ó��			


extern bool CheckGuildPower( int type, LPCHARACTER ch );
extern void LoadFileText( char *file_name );
extern void CallGuildHouseInfo();
extern char *GetIsGotGuildHouse( int time );		// �ð��� ������ ���� ������� �Ǵ�.



//021030 YGI
extern int RecvEventObject( k_event_object *pData );
extern void SendEvnetMoveMap();	// 021107 YGI