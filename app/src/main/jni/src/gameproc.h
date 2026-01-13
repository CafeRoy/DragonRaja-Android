	/*

	*/

	#ifndef	__GAMEPROC_H__
	#define	__GAMEPROC_H__

	#include "dragon.h"
	#include "CurrentMsgMgr.h"//021025 lsw

	#define	MAX_ORDERTABLE	4000

	typedef struct	tagORDERTABLE
	{
		short int	show;
		short int		wType;
		int			x,y;
		int			height;
		int			offset;
		SIZE		size;
		LPVOID		lpvData;
		LPVOID		*lppvData;
		int			type;
		int			alpha;
		DWORD		rgb;
		int			rank;
		int			sort;
		void		*p;
		int			sprno;

	} ORDERTABLE, *LPORDERTABLE;

	typedef struct	tagORDERINFO
	{
		int			count;
		ORDERTABLE	order[ MAX_ORDERTABLE ];
	} ORDERIFNO, *LPORDERINFO;

	#ifdef __cplusplus
	extern "C"
	{
	#endif

		
	extern BOOL		GameProc( void );
	extern BOOL		GameTestProc( void );
	extern BOOL		StartMenuProc( void );

	extern int		ProtocolProc( t_connection *c, bool flag = true );
	extern int		ProtocolWritingProc( t_connection *c );
	extern void		KeyboardProc( void );
	extern void		MouseProc( void );
	extern void		MapDisplay(int nType);
	extern void		DisplaySpriteUnderTile( void );
	extern void		DisplaySprite( void );
	extern void		DisplaySpriteTrans( void );
	extern void		DisplaySpriteCeiling( void );
	extern void		DisplaySpriteBottom( void );
	//extern void	BuffTimer();
	extern void		AttackProc( LPCHARACTER lpCharacter, int minr, int maxr );
	extern int CheckAgentServer( t_connection *c, std::string id, std::string password );
////
	extern void UnDisplayDestPoing( LPCHARACTER ch );

	//////////////////// SoundUp lkh ���� //////////////////////
	extern void BackgroundMusic( void );
	extern int	ReturnBGMNumber(int map_Number);
	////////////////////////////////////////////////////////////

	extern bool CheckCharName( char *name );

	extern void RecvDebugReqCharInfo( t_debug_server_req_char_info *p );

	extern ORDERIFNO		g_OrderInfo;
	extern int				g_OldOrderInfoCount;
	
	extern int				NewOrderC;
	extern ORDERTABLE		neworder[ MAX_ORDERTABLE];

	//////////////////// 0430 lkh �߰� //////////////////////////
	extern BOOL				g_AttackSkill_Trace1;
	extern BOOL				g_AttackSkill_Trace2;

	extern int real_move;
	extern int				YouCanHeroActionProc;
	extern int				YouCanViewTipsCheck;
	extern bool				ViewTipToggle;		// 0818 khs

	extern int				Moveoldx, Moveoldy;
	extern DWORD			Movetime;

    extern bool    CheckPC;
	extern int ChangeMapFlag;					// 0825 KHS
	extern t_server_change_map ChangeMapPacket;

	// 020428 YGI acer
	extern bool IsHidenGhost( LPCHARACTER ch );


	#ifdef __cplusplus
	}
	#endif

	#endif	// __GAMEPROC_H__
