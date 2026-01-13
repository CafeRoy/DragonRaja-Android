#if !defined(AFX_CHARDATATABLE_H__3028D503_725C_4F31_8429_95303BA39959__INCLUDED_)
#define AFX_CHARDATATABLE_H__3028D503_725C_4F31_8429_95303BA39959__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define Num_Of_NPC_Lev 256
#define TAC_INDEX 3

struct NPCLev_to_Exp
{	//< CSD-030314
	int nMinExp;         // ���� ������ �ּ� ����ġ
	int nMaxExp;         // ���� ������ �ִ� ����ġ
	int nNeedExp;        // ���� ������ �ö󰡱� ���� ȹ���ؾߵ� ����ġ
	int nTrainingExp;    // ��ƽ �����忡�� ��� ����ġ
	int nExpReduction;	 // ��ɽ� �׾��� ��� ����ġ �谨��
	int nNKReduction;	 // NK�� �׾��� ��� ����ġ �谨��
	int nTacRate;		 // ��ƽ ����ġ
	int nMaxTactic;	     // ������ ���� ��ƽ �����忡�� �ø� �� �ִ� �ִ� ��ƽ ����
	int	nDyeingPrice;    // ���� ����
	int nLessRate;       // ���� ���� ����ġ  
	int nGreatRate;      // ���� ���� ����ġ  
	int nCspMax;         // �ִ� ������ų ������ ȹ���
	int nBaseCP;         // ������ų �⺻��
	int bank_loan;	     // 010314 YGI
	int nStep;           // ����ġ �ܰ�
	int nWarriorLife;    // ������ �⺻ Life
	int nThiefLife;      // ������ �⺻ Life
	int nArcherLife;     // �ü��� �⺻ Life
	int nWizardLife;     // �������� �⺻ Life
	int nPriestLife;     // �������� �⺻ Life
	int nWarriorExpRate; // ������ ����ġ ������(����%)
	int nThiefExpRate;   // ������ ����ġ ������(����%)
	int nArcherExpRate;  // �ü��� ����ġ ������(����%)
	int nWizardExpRate;  // �������� ����ġ ������(����%)
	int nPriestExpRate;  // �������� ����ġ ������(����%)
};	//> CSD-030314

extern LPCHARACTER ReturnCharacterPoint(int id);
extern NPCLev_to_Exp		NPC_Lev_Ref[Num_Of_NPC_Lev] ;

extern int			ReturnPCAutoWave(LPCHARACTER lpChar, BOOL type);
extern int			ReturnNPCAutoWave(LPCHARACTER lpChar, BOOL type);

extern int			ReturnWaveAnimation( LPCHARACTER lpCharacter );
extern int			ReturnEffectAnimation( LPCHARACTER ch );
extern int			ReturnAttackWaveAnimation( LPCHARACTER lpCharacter );
extern int			ReturnAttackedWaveAnimation( LPCHARACTER lpCharacter );
extern LPCHARACTER  ReturnCharacterID( int id );

extern LPCHARACTER	ReturnCharacterPoint( int server_id );
extern void			ReturnCharMoveDistance( LPCHARACTER ch, XorCrypt *dist );


extern int			IsDead( LPCHARACTER ch );
extern bool IsAttackAble( LPCHARACTER pTarget );


extern void			LoadLevelExpTable( void );

extern void SendSmileFace( int smileno );
extern void RecvSmileFace( int id, int smileno );

extern int	ReturnWaveNatural( LPCHARACTER	ch );

#endif //#if !defined(AFX_CHARDATATABLE_H__3028D503_725C_4F31_8429_95303BA39959__INCLUDED_)
