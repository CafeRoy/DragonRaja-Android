#ifndef	__SKILLTOOL_H__
#define	__SKILLTOOL_H__

#define		DEF_SKILLTILE_RANGE		4
#define		MAX_SKILLTILE_RANGE		10

#define		TOOL_FARMING			1
#define		TOOL_MINING				2
#define		TOOL_FISHING			3
#define		TOOL_CHOPPING			4
#define		TOOL_HUB				5	
#define		TOOL_NPC_GENER			6
#define		TOOL_DONTSKILL			7
#define		TOOL_BUILDHOUSE			8

//////////////// 0429 lkh �߰� ///////////////////
#define		MAX_KEY					4	//������ ������ �ִ� ���� �ִ��
#define		MAX_HOUSEITEM			45	//�Ѱ� ���� ����˼� �ִ� �������� �Ѱ���

//////////////////////////////////////////////////

typedef struct  tagMAPSKILLTABLE
{
	int type;							//�Ϲ����� ������� Ÿ���� ���->0 / NPC ���� ��ġ ����->1
	int x, y;							//�ش� ����Ӽ��� ���� Ÿ���� ��ǥ(Ÿ�ϴ���) //��ȹ�䱸->�������̽��� ������� ����
	int skillno;						//����Ӽ� ������ȣ(1~18����)
	int tile_Range;						//�ش�Ÿ�Ϸκ��� ȿ���� ��ȿ�� Ÿ�� �Ÿ�(������) // �ִ� �߻���. 
	int probability;					//�ش����� ������ Ȯ��	//  Event��ȣ..
	int type_Num;						//NPC ���� Ÿ�� ��ȣ(0~99)
	int subType;						//�ش� ����� ���� ���� �����Ͱ�

	struct  tagMAPSKILLTABLE *prev;		//���� ��ų �Ӽ� ���̺� ������	
	struct  tagMAPSKILLTABLE *next;		//���� ��ų �Ӽ� ������


}	MAPSKILLTABLE, *lpMAPSKILLTABLE;

typedef struct  tagDRAGMOUSE
{
	int sx, sy;
	int ex, ey;
}	DRAGMOUSE;

typedef struct	tagMYHOUSETOOL
{
//	int	map;			//������(1->ma-in/2->k_sung2/3->source/4->gray/5->hu_vm)
	int sx, sy;			//������
	int ex, ey;			//������
	int type;			//���� ���� ��ȣ
	int object_Num;		//���� ����ִ� ������Ʈ ����
//	TILE* startTile;	//������ Ÿ���� �ּ�
}	MYHOUSETOOL;

typedef struct	tagMyHouse
{
	BYTE		houseType;			//���� ����
	char		Owner[20];			//���� ���� - NM_LENGTH
	BYTE		mapType;			//���� ��ġ�ϴ� ���� ����
	POINT		position;			//���� ��ġ ����
	BYTE		doorKey[MAX_KEY];	//���� ���� ����(�ִ� ����4��)
	char		doorBoard[30];		//���п� ���� ����
	//ITEMGROUND	houseItem[MAX_HOUSEITEM];	//������ �����Ҽ� �ִ� ������
	int			durability;			//���� ������
	BYTE		houseStatue;		//���ǻ������� (0->�μ���/1->�������/2->���ϰ� ������ �������)
}	MYHOUSE;

extern int		radio_Statue;
extern void		PutMyhouse(int x, int y);
extern void		MapSkillTool_Attr( int mx, int my );
extern bool		LoadSkillMapTable(void);
extern bool		LoadSkillMapTable_Refactored(void);

extern MAPSKILLTABLE	g_MapSkillTable;	//�Ѱ� Ÿ���� ��ų �Ӽ��� ������ �ִ� ����ü	
extern lpMAPSKILLTABLE	Header[8][8];
extern DRAGMOUSE		g_DragMouse;		//���콺�� �巡�� ó���� ���� ����ü
extern MYHOUSETOOL		g_MyhouseTool;		//���� �ý��� �̹���Ÿ�� ���� ����ü
extern MYHOUSE			g_MyHouse;			//���� ���� ���� ����ü

// thai2 YGI
extern char	subFarmType[20][25];
extern char	subMineType[20][25];
extern char	subHubType[20][25];

////////////////////// 0613 lkh �߰� ///////////////////////
extern void DeleteNPCGenerate(int npc_Num=0);
extern void DeleteAllSkillData( void );
extern void ChangeNPCGenerate(int npc_from, int npc_to );


extern void		DrawSkillBox(int mox, int moy);
extern void		CheckHouseObjectEdit ( HWND hwnd, HINSTANCE hInstance);

extern void DeleteSkill( lpMAPSKILLTABLE *Header, lpMAPSKILLTABLE lpST);	//�ι�° ������ ��� FindSkill�Լ��� ���� �Ѱ��� ������
extern void AddSkill( lpMAPSKILLTABLE *Header, lpMAPSKILLTABLE	lpST );
extern lpMAPSKILLTABLE FindSkill( lpMAPSKILLTABLE *Header, int x, int y, int order = 0);		// 010314 KHS

extern void DeleteAllType(int delete_Type);



#endif	// __SKILLTOOL_H__