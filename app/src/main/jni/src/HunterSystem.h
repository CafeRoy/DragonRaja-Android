// HunterSystem.h: interface for the CHunterSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HUNTERSYSTEM_H__818A3CCE_7C23_476F_AEFD_212049A85436__INCLUDED_)
#define AFX_HUNTERSYSTEM_H__818A3CCE_7C23_476F_AEFD_212049A85436__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHunterSystem  
{
public:
	CHunterSystem();
	virtual ~CHunterSystem();

};

class CHunterList
{
public:
	int my_sight;		// �� �þ߷� ���Դ�. count�� �־ ó�� ���ö� ����̰� �Ѵ�.
	bool my_regist;		// �� ��Ͽ� ����� ���̴�.
	bool is_end;
	k_hunter_list list;
	CHunterList *next;
	CHunterList *prev;


public:
	CHunterList() { next = NULL; prev = NULL; is_end = 0; list.index = 0; my_regist=0; my_sight=0; }
	~CHunterList() { SAFE_DELETE(next); };
	void DeleteAll();
	CHunterList *AddNode( k_hunter_list *temp );
	int DeleteNode( char *name );		// ���° ��带 ������ ���ΰ�..
	int print( int x, int y );
	int CheckList( char *name, int type );			// �þ� ������ �� ��Ͽ� �ִ� ���� ��� �Դ���.. ��������...
	int print2( int x, int y );						// ���� ����� ����� ��� ���...
	int KillCharacter( char *name );
	bool ExistMyHunterList( char *name );
	int CompName( char *name );
	int CompTargetName( char *name );
	int CompTargetId( short int id );

	friend void CHunterList_Next( CHunterList *& );
	friend void CHunterList_Prev( CHunterList *& );

};
extern CHunterList *hunter_list;
extern CHunterList *curr_hunter_list;
extern CHunterList *my_hunter_list;
extern CHunterList *DeleteMyHunterList( CHunterList *header, char *name );

#endif // !defined(AFX_HUNTERSYSTEM_H__818A3CCE_7C23_476F_AEFD_212049A85436__INCLUDED_)
