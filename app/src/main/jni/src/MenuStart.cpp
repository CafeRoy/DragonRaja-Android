// menuStart.cpp

#include "stdafx.h"
#include "Hangul.h"
#include "Menu.h"
#include "Hong_Sprite.h"
#include "dragon.h"
#include "MenuStartSet.h"
#include <cstdio>
#include "convert565to555.h"
#include "directsound.h"
#include "Char.h"
#include "GameProc.h"
#include "Hong_Sub.h"
#include "tool.h"
#include "smallmenu.h"
#include "MenuNetwork.h"
#include "menuset.h"
#include "smallmenuset.h"		// 020701 YGI
#include "menudef2.h"			//020802 lsw
#include "chat.h"
#include "TextOutMgr.h"			//020828 lsw
#include "FrameMgr.h" //030703 lsw
#include "SDLRenderer.h"
#include "AudioManager.h"
extern void SWD_LOGON_Clicked(const int nMenuIndex);
extern bool PutTestAnimation(const int iGender, const int iClass, const DWORD dwBodyRGB, DWORD dwLineRGB, float fElapse);
extern bool PutTestAnimation2(const int iGender, const int iClass, const DWORD dwBodyRGB, DWORD dwLineRGB, float fElapse);
//010909 lsw 
extern void ResetCharacter(LPCHARACTER lpChar);
extern void ResetEffect(LPCHARACTER lpChar);
t_client_login_fail_reason g_client_login_fail_reason;		// �α��� �� �޽���
extern void Line(int sx, int sy, int ex, int ey, int color, bool is_screen_coords);
extern void PutCmprsImgCliping_CPP(int x, int y, Spr* sp, void* dest,int destPitch, int lx, int rx,int ty, int by);
extern void StartMenuSubProcessType2(SMENU* SubMenu, int i, int j, int k);//020515 lsw
extern CBitBuf guild_mark_main;
int g_SelectServerIndex;

CMenuString		g_MenuStr[MAX_MENU_STRING];

char select_server_name[20];
BYTE* buf_alphabet;
Spr		spr_alphabet[36];

bool g_GoDiceRecv = 0;
bool g_GoDice = 0;
bool g_GoDice2 = 0;

int BetaTest = 0;

int StartPosition;
int StartMap;

Spr open_spr[75];
BYTE* open_spr_buf;

HANDLE ghThread; // ������ �ڵ�
DWORD gdwThread; // ������ ID

HANDLE ghThread1; // ������ �ڵ�
DWORD gdwThread1; // ������ ID


int		dw_TotalImageSize;
int		g_StartMenuOn;

//	Spr		ch_armor_image_befor[7][80];//020530 lsw
Spr		ch_armor_image_after[7][80];
//	Spr		ch_body_image_befor[7][80];
Spr		ch_body_image_after[7][80];
//	Spr		ch_line_image_befor[7][80];
Spr		ch_line_image_after[7][80];

//Spr		ch_armor_image_befor_b[7][80];//020530 lsw
Spr		ch_armor_image_after_b[7][80];
//Spr		ch_body_image_befor_b[7][80];
Spr		ch_body_image_after_b[7][80];
//Spr		ch_line_image_befor_b[7][80];
Spr		ch_line_image_after_b[7][80];

char* ani_buf[12];


int		character_active = 0;		// ���õ� ĳ����//020815-2 lsw
int		move_text_flag;

// BOOL RButtonDown;
BOOL LButtonDown;                      //���콺 ���ʹ�ư�� ���Ⱦ����� �ȴ��Ⱦ����� ǥ��
SHN SHideNomal[MAX_SHN][MAX_SHN_FIELD];   //FT_HIDE_NOMAL_�� ����� ����ü ����

SCharacter SCharacterData = {
								7, 3333, 10000,
								{"NoConnect"},
								{""},
								{0,1000,4,3,0,0,0,0,0,1,1,3},
								{10,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19},
								{10,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19},
								20000000,

								0,70,30,100,100,50,100,
								0,0,
								0,0,0,0,0L,
								0,
};

SCharacter SCharSource = {
							1,0,0,										// lv, exp
							{""},
							"None",
							{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},		//data
							{0, },										//STR,CON,DEX,WIS,INT,CHA,MOVP,ENDU,MOR,CONV 
							//LUCK, WSPS,HEALTH,MOV,AC,FAME,DAMAGE,HEAT,TACTICE, money
{0, },
600,

0,70,30,100,100,50,100,
0,0,
0,0,0,0,0L,
0,
};
SCharacter SCharSource1 = SCharSource;

SCharacter LoadCh[4];								// �������� ĳ�� ������ ���۹޴´�

extern CHARACTERLIST g_CharacterList;

// int y_PutBarSize=0;              //HP�� MP�� ����� ���� �׷����� ���̸� ����� ���� �����Ѵ�.
char y_temp[20];                 //ĳ������ �ɷ�ġ ���� ����Ҵ� strlen�� �̿��� �� ���̸� �̸� �˱� ���ؼ� ����ϴ� �ӽ� ���� ����


int cDiceCount;							//���̽��� ���� Ÿ�̹��� ����, �Ҳ��� �����ӵ� ǥ��
int nDiceNumber[5] = { 1,1,1,1,1 };			//���̽��� 5���� �� 
//int nDiceNumber2[5]={1,1,1,1,1};
int PutDiceNumber;						//���� ���̽��� ����

int y_delay = 0;
int y_MenuFocus = 0; //�� �ϳ��� �޴����� ���콺 üũ�� �ϱ����� ������ 0�϶��� ���޴��� ���콺�� ����ǰ� 
//���ϴ� MN_���� �־� �� �޴��� �۵��ϰ� �Ѵ�
std::string sId;
std::string sPassword;

SMENU SMenu[MAX_MENU];    //�޴� ����ü�� �ִ�ġ ��ŭ ����
POINT StateMousePoint;	// �巡�� ������ ���

//�迭��� üũ�� �ʿ��� �迭
BYTE CheakArray[MAX_CHEAKARRAY][16][33] =
{
	{
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
		{1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,},
		{1,1,1,1,1,2,2,2,2,2,2,1,1,1,1,1,},
		{1,1,1,1,0,2,2,2,2,2,2,0,1,1,1,1,},
		{0,0,0,0,0,2,2,2,2,2,2,0,0,0,0,0,},
		{0,0,0,0,0,2,2,2,2,2,2,0,0,0,0,0,},
		{3,3,3,3,0,2,2,2,2,2,2,0,4,4,4,4,},
		{3,3,3,3,3,0,2,2,2,2,0,4,4,4,4,4,},
		{3,3,3,3,3,3,0,2,2,0,4,4,4,4,4,4,},
		{3,3,3,3,3,3,3,0,0,4,4,4,4,4,4,4,},
		{3,3,3,3,3,3,3,0,0,4,4,4,4,4,4,4,},
		{3,3,3,3,3,3,3,0,0,4,4,4,4,4,4,4,},
		{3,3,3,3,3,3,3,0,0,4,4,4,4,4,4,4,}
	},
	{
		{0,0,0,0,0,1,1,0,0,2,2,0,0,0,0,0,},
		{0,0,0,1,1,1,1,0,0,2,2,2,2,0,0,0,},
		{0,0,1,1,1,1,1,0,0,2,2,2,2,2,0,0,},
		{0,1,1,1,1,1,1,0,0,2,2,2,2,2,2,0,},
		{0,1,1,1,1,1,0,4,4,0,2,2,2,2,2,0,},
		{1,1,1,1,1,4,4,4,4,4,4,2,2,2,2,2,},
		{1,1,1,1,0,4,4,4,4,4,4,0,2,2,2,2,},
		{0,0,0,0,0,4,4,4,4,4,4,0,0,0,0,0,},
		{0,0,0,0,0,4,4,4,4,4,4,0,0,0,0,0,},
		{5,5,5,5,0,4,4,4,4,4,4,0,3,3,3,3,},
		{5,5,5,5,5,4,4,4,4,4,4,3,3,3,3,3,},
		{0,5,5,5,5,5,0,4,4,0,3,3,3,3,3,0,},
		{0,5,5,5,5,5,5,0,0,3,3,3,3,3,3,0,},
		{0,0,5,5,5,5,5,0,0,3,3,3,3,3,0,0,},
		{0,0,0,5,5,5,5,0,0,3,3,3,3,0,0,0,},
		{0,0,0,0,0,5,5,0,0,3,3,0,0,0,0,0,}
	},
	{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,1,1,1,1,1,1,0,4,4,4,4,4,4,4,4,4,4,4,0,2,2,2,2,2,2,0,0,0,0},
		{0,0,0,1,1,1,1,1,1,1,0,4,4,4,4,4,4,4,4,4,4,4,0,2,2,2,2,2,2,2,0,0,0},
		{0,0,1,1,1,1,1,1,1,1,0,0,4,4,4,4,4,4,4,4,4,0,0,2,2,2,2,2,2,2,2,0,0},
		{0,1,1,1,1,1,1,1,1,1,1,0,0,4,4,4,4,4,4,4,0,0,2,2,2,2,2,2,2,2,2,2,0},
		{0,1,1,1,1,1,1,1,1,1,1,1,0,0,4,4,4,4,4,0,0,2,2,2,2,2,2,2,2,2,2,2,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	},

};

//�޴��� ����ؼ� �ʿ�� �ε��� �̹����� ��ȣ�� �����ϴ� �޴� ���κ��� 9999�� ���� �ִ´�
int aMenuImageNumber[8][MAX_MENUDATA * 5] = {
	/* 0 */	{ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10, 11, 12, 13, 14,17, 18, 19, 20, 22, 24, 25, 26, 27, 28, 29, 30, 31,73,74,75,76,77,80, // ��ŸƮ
			 81,82,83,32, 33, 34, 64, 65, 66, 67, 68, 142,143,144,206,207,208,209,242,243,244,400,401,486,549,550,551,552,553,554,555,556,
			 557,558,503,510,511,512,513,514,515,516,517,153,823,824,825,826,827,828,829,830,831,832,833,834,835,836,837,838,543,544,
			 839,840,841,MAX_SYSDAT + 3,MAX_SYSDAT + 2, 844,845,846,847,76,77,468,469,470,471,472,473,305,306,307,308,309,877,878,879, 240,
			 906,907,908,909,910,911,912,913,914,915,916,917,918,919,920,921,922,923,924,925,926,927,928,929,930, 9999,},

			 /* 1 */	{ 21, 35, 36, 37, 69, 120,121,122,123,124,125,126,127,128,129,130,131,145,146,147,148,149,150,151,152,172,173,174,175,176,310, // ����
					  177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,245,246,247,248,249,250,
					  251,252,253,254,255,256,257,258,259,260,261,262,263,264,265,266,267,268,269,270,271,272,273,274,275,276,277,278,279,280,
					  281,282,283,311,312,313,314,315,316,317,318,319,320,321,322,323,324,325,326,327,328,329,330,331,332,333,334,335,336,337,
					  338,339,340,341,342,343,344,345,346,347,348,349,201,202,203,204,205,504,505,301,439,437,440,438,441,442,487,
					  9999,},

					  /* 2 */	{350,351,352,353,354,355,356,357,358,359,360,361,362,363,364,365,366,367,368,369,370,371,372,373,374,375,376,377,378,379,	//����
							   380,381,382,383,384,385,386,387,388,389,390,391,392,393,394,395,396,397,398,399,456,457,458,459,460,461,462,463,464,
							   465,466,467,474,475,476,477,478,483,498,508,509,521,MAX_SYSDAT + 4,MAX_SYSDAT + 5,523,524,528,70,72,86,87,499,500,501,241,525,526,527,302 ,
							   502,506,507,488,489,492,493,494,495,496,497,479,480,481,878,879,880,881,882,883,884,885,886,887,888,889,890,891,
							   892,893,894,895,78,79,84,85,944,945,946,947,948,949,950,951,952,953,954,955,956,9999,},

							   /* 3 */	{132,133,134,135,136,137,138,139,140,141,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,		//����
										231,232,233,234,235,236,237,238,239,284,286,287,288,289,290,291,292,293,294,295,300,303,304,490, 491,
										433,434,435,436,443,444,445,446,447,448,449,450,451,452,453,454,455,586,587,588,589,590,591,592,593,594,595,596,
										597,598,599,600,601,602,603,604,605,607,608,609,610,611,612,613,614,615,616,617,618,619,620,621,622,623,624,625,626,627,628,
										629,630,631,632,633,634,635,636,637,9999,},

										/* 4 */	{38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,90,484,485,0,296,297,298,299, 71,			// ����	
												 154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,16,15,285,23,645,646,647,648,649,706,210,
												 211,801,802,803,804,805,806,807,808,809,810,811,812,813,814,815,816,817,758,941,942,943,
												 935,936,957,958,959,960,961,962,963,964,896,897,898,899,900,901,902,903,904,905,
												 9999,},

												 /* 5 */	{559,560,561,562,563,564,565,566,567,568,569,570,571,572,573,574,575,576,577,578,579,580,581,582,583,584,585,638,639,640,	// ���� �߰�	
														  641,642,643,644,760,761,762,763,764,765,767,768,769,770,771,772,773,774,775,776,777,778,779,780,781,782,783,784,
														  785,786,766,848,849,850,851,852,853,854,855,856,857,858,859,860,861,862,863,864,865,866,867,868,869,870,871,872,873,
														  874,875,876,638,639,640,641,642,643,644,732,733,734,735,736,737,738,931,932,933,934,1022,1023,1024,1025,1026,1027,1028,
														  1029,1030,1031,1032,1033,1034,1035,1036,1037,1038,1039, 1040, MAX_SYSDAT + 1, 9999,},

														  /* 6 */	{402,403,404,405,406,407,408,409,410,411,412,413,414,415,416,417,418,419,420,421,422,423,424,425,426,427,428,429,430,431,	//���� �߰�	
																   432,650,651,652,653,654,655,656,657,658,659,660,661,662,663,664,665,666,667,668,669,670,671,672,673,674,759,
																   675,676,677,678,679,680,681,682,683,684,685,686,687,688,689,690,691,692,693,694,695,696,697,698,699,700,701,702,703,704,
																   705,707,708,709,710,711,712,713,714,715,716,717,718,719,720,721,722,723,724,725,726,727,728,729,730,731,
																   965,966,967,968,969,970,971,972,973,974,975,976,977,978,979,980,981,982,983,984,985,986,987,988,989,990,991,992,993,
																   994,995,996,997,998,999,1000,9999,},

																   /* 7 */ {739,740,741,742,743,744,745,746,747,748,749,750,751,752,753,754,755,756,757,787,788,789,790,791,792,793,794,795,796,797,	//���� �߰�	
																			798,799,800,529,530,531,532,533,534,535,536,537,538,539,540,541,542,545,546,547,548,818,819,820,821,822,1001,1002,1003,1004,
																			1005,1006,1007,1008,1009,1010,1011,1012,1013,1014,1015,1016,1017,1018,1019,1020,1021,518,519,520,9999,},
};


int ClosemenuWhenLogin[42] = {//	MN_MAINSTART_BACK,
	//	MN_LOGO_START_SAMSUNG,

		MN_MAKECHARACTER_BASIC,
		MN_MAINSTART_BASIC,
		MN_MAINSTART_START,
		MN_MAKECHARACTER_CHEAK1,
		MN_MAKECHARACTER_CHEAK3,

		MN_MAKECHARACTER_GENDER,
		MN_MAKECHARACTER_NAME,
		MN_MAKECHARACTER_FACE,
		MN_MAKECHARACTER_ARIGEMENT,
		MN_MAKECHARACTER_CLASS,

		MN_MAKECHARACTER_SPELL,
		MN_MAKECHARACTER_COLOR,
		MN_MAKECHARACTER_TACTICS_MAN,
		MN_MAKECHARACTER_TACTICS_WOMAN,
		MN_MAKECHARACTER_AGE,

		MN_MAKECHARACTER_JOB,
		MN_MAKECHARACTER_ABILITY,
		MN_RESET_JOB,
		MN_RESET_JOB_EXPLAIN,
		MN_SELECT_NATION,

		MN_NATION_EXPLAIN,
		MN_LOGO_START,
		MN_LODING,
		MN_LOGIN,
		MN_LOGON,

		MN_SELECT,
		MN_LOCATION,
		MN_MESSAGEBOX_BIG,
		MN_MESSAGEBOX_SMALL,
		MN_YESNO,

		MN_SELECT_CHARACTER,
		MN_MAKECHARACTER_SPELL2,
		MN_START_MAP,
		MN_SELECT_SERVER,
		MN_INFO,

		MN_CREDIT,
		MN_OKCANCEL_MESSAGEBOX,
		MN_OK_MESSAGEBOX,
		MN_DEL_CHAR_COMFORM,//020802 lsw
		MN_SELECT_TUTORIAL_LOGIN,//020815-2 lsw
		MN_REVIVAL,
		MN_SYSTEM_LOGOUT,
};

const int MAX_START = (int)sizeof(ClosemenuWhenLogin) / sizeof(int);//020815-2 lsw

//010216 lsw
//char NationName[7][20] = { "�߸�", "","", "���̼���", "������", "��Ը�Ͼ�", "�Ͻ�" };
char NationName[8][25];

char* GetNationName(int nation)
{
	return NationName[nation];
}

const DWORD NationColor[7] = { RGB(200,200,255), RGB(200,200,255), RGB(200,200,255), RGB(0,0,255), RGB(255,0,0),  RGB(200,200,255), RGB(215,215,145) };
const WORD NationColor16[7] = { RGB16(200,200,255), RGB16(200,200,255), RGB16(200,200,255), RGB16(0,0,255), RGB16(255,0,0),  RGB16(200,200,255), RGB16(215,215,145) };

void	StartMenuSubProcessType(SMENU* SubMenu);                //�޴� ������ҿ� Ÿ���� �Ǻ��ؼ� ����
void	SetCharacterData();                   //nCharacterData �������� ����ü�� �ȿ� ����
int		k_PutImage(int x, int y, int nImageNumber, BOOL bEnd);		//x, y��ǥ�� �̹��� ����
//���� �޴��� x,���� �޴��� y,���� ������� x,���� ������� y,���� �̹���
void	FieldTypeNomalPutFx(int mx, int my, int x, int y, int nImageNumber, int t, int j);
void	FieldTypeTextPut(int mx, int my, int x, int y, char* temp);  //���� ���ڿ��� ���
void	PutImageCenter(int mx, int my, int x, int y, int nImageNumber); // ��� ������ �׸� ���
void	start_KeyProc();
void	LoadChImageNumber(char* filename, char** buf, Spr Ani[][80], int max_ani[7]);		// ĳ���� ���ϸ��̼� ��������Ʈ �ε�
void	LoadChImage();
void	MakeCharacterAbility();
int		ImgToAbility(int num_to_img[10], int img);				// �ֻ��� ������, �̹��� ��ȣ�� �ɷ�ġ �� ã��
void	DoLButtonDoubbleClickOfStartMenu(int i, int j);
void	SetDamageOfHero(SCharacter* ch);						//	�⺻ ���ݷ� ���� 
void	InitStartMenu();				// �ܸ� �� �ʱ� ������ ���� -1�� ���� 


int CalCreateMoney(const SCharacter* s);
extern void LoadSpriteOnlyOne(Spr* s, char filename[MAX_FILENAME]);		// ��������Ʈ ȭ�� �ϳ��� �о� �ö�
extern void LoadSpriteOnlyOneForLoadMenuData(Spr* s, char filename[MAX_FILENAME]);
extern void SetTacticsBySpell(SCharacter* ch);
extern bool bSoundOn;
extern void OpenSelectNationMenu();

char job_explain[7][6][1000];
char nation_explain[7][11][1000];
void LoadJobExplain();
void LoadNationExplain();
void SetJobExplainDetect(int job, int width, char explain[5][11][200], WORD color[5][11]);
void SetNationExplainDetect(int nation, int width, char explain[7][11][200], WORD color[7][11]);
int CheckNationCity(int nation);

/*************************************************************************************/
//ing 1 = loop  else = play once
void MP3(int n, int ing)
{
	/*SoundOn = false;
	if( n == SN_TOUCH || n == SN_TOUCH_START )
	{
		if( LButtonDown ) return;
	}

	DWORD	dwStatus;
	if( !g_SoundInfo.lpDirectSoundBuffer[ n ] )
	{
		PlayListAutoSounds( n, 0, 0, ing );
		return;
	}
	if( IDirectSoundBuffer_GetStatus( g_SoundInfo.lpDirectSoundBuffer[ n ], &dwStatus ) == DS_OK )
	{
		if ( dwStatus & DSBSTATUS_PLAYING ) return;

		PlayListAutoSounds( n, 0, 0, ing );
	}*/
	SoundOn = false; // 這行是原始程式碼就有的，但看起來是直接賦值，可能不影響後續判斷

	if (ing == 1)// loop
	{
		ing = -1;
	}
	else ing = PLAY_ONCE;

	// 處理滑鼠觸控邏輯 (這部分與音效無關，直接保留)
	if (n == SN_TOUCH || n == SN_TOUCH_START) {
		if (LButtonDown) {
			return; // 如果滑鼠左鍵按下，則直接返回，不播放音效
		}
	}

	if (g_audioManager.IsPlayingSDL(n) != -1) return;
    //SDL_Log("MP3 sound ID %d with volume %d", n, system_info.sound);
	if(n==SN_START)
		g_audioManager.PlaySoundA(n, system_info.music, ing);
	else
		g_audioManager.PlaySoundA(n, system_info.sound, ing);
}


/*********************************�Լ� �ҽ�*******************************************/
void PutImageCenter(int mx, int my, int x, int y, int nImageNumber)  // �߰������� �׸� ���, �ʵ常 ��� ����. �޴��� �ȵſ�..
{
	if (!spr[nImageNumber].img)
	{
		LoadSmallMenuImage(nImageNumber);
	}
	PutCompressedImage(mx + x, my + y, &spr[nImageNumber]);
}

void FieldTypeNomalPut(int mx, int my, int x, int y, int nImageNumber, int type)
{
	Spr* s;
	s = GetSprOfMenu(type, nImageNumber);
	if (s) PutCompressedImage(mx + x + s->ox, my + y + s->oy, s);
}

//010910 lsw
void FieldTypeNomalPutFx3(int mx, int my, int x, int y, int nImageNumber, int type, int t, int j)
{

	Spr* s = GetSprOfMenu(type, nImageNumber);
	if (s)
	{
		PutCompressedImageFX(mx + x + s->ox, my + y + s->oy, s, t, j);
	}
}


void FieldTypeNomalPutFx(int mx, int my, int x, int y, int nImageNumber, int t,
                         int j) {
  if (nImageNumber >= 10000) {
    FieldTypeNomalPutFx2(mx, my, x, y, nImageNumber - 10000, t, j);
    return;
  }

  if (nImageNumber > 0 && nImageNumber < SYSTEM_SPRITE_MAX_){
    if (!spr[nImageNumber].img) {
      LoadSmallMenuImage(nImageNumber);
    }
    PutCompressedImageFX(mx + x + spr[nImageNumber].ox, my + y + spr[nImageNumber].oy, &spr[nImageNumber], t, j);
  }
}

//acer7
void FieldTypeTextPut(int mx, int my, int x, int y, char* temp)
{
	int i = 0;
	int lenght = 0;
	int code;

	while (temp[i] != '\0')
	{
		code = temp[i];
		if (temp[i] == ' ' || (temp[i] >= 'A' && temp[i] <= 'Z'))
		{
			if (code == ' ') lenght += 4;
			else
			{
				code = code - 'A';		// A�� 0���̴�.
				Spr* s = &spr_alphabet[code];
				PutCompressedImage(mx + x + lenght + s->ox, my + y + s->oy, s);
				lenght += s->xl;
			}
		}
		i++;
	}
}


void LoadMenuData(int nNum)
{
    // [Android] 1. 路徑修正：移除 ./，使用相對路徑 data/SysMenu.dat
    const char* mainFilename = "data/sysmenu.dat";

    // [Android] 2. 使用 SDL 開檔
    SDL_RWops* fp = SDL_RWFromFile(mainFilename, "rb");

    // 如果主檔案開不起來，我們還是得繼續跑迴圈，
    // 因為有可能某些圖片是讀取外部 .csp 的 (大於 MAX_SYSDAT 的部分)
    // 但原本邏輯是直接 return，這裡維持原邏輯，視需求調整
    if (fp == NULL) {
        SDL_Log("Error: Failed to open %s", mainFilename);
        return;
    }

    int i = 0;
    char temp[256]; // 加大緩衝區比較安全

    while (1)
    {
        int imgIdx = aMenuImageNumber[nNum][i];

        // 結束條件
        if (imgIdx == 9999)
            break;

        // 情況 A: 讀取外部 .csp 檔案
        if (imgIdx > MAX_SYSDAT)
        {
            // [Android] 路徑修正：data/plus...
            snprintf(temp, sizeof(temp), "data/plus%03d.csp", imgIdx - MAX_SYSDAT);

            // 注意：LoadSpriteOnlyOneForLoadMenuData 內部也需要確保支援 Android 路徑/SDL
            LoadSpriteOnlyOneForLoadMenuData(&spr[imgIdx], temp);
        }
            // 情況 B: 從 SysMenu.dat 讀取
        else
        {
            // 如果圖片已經載入過 (img 指標不為空)，則跳過
            if (spr[imgIdx].img)
            {
                // 什麼都不做，保留原意
            }
            else
            {
                DWORD Off = 0;
                DWORD size = 0;

                // [Android] 3. Seek 到 Offset 表的位置
                // 邏輯: 每個索引佔 4 bytes (DWORD)，加上開頭可能的 2 bytes Header (Count)
                SDL_RWseek(fp, 4 * imgIdx + 2, RW_SEEK_SET);

                // [Android] 4. 讀取該圖片在檔案中的 Offset
                SDL_RWread(fp, &Off, sizeof(DWORD), 1);

                // [Android] 5. 跳轉到圖片資料開頭
                SDL_RWseek(fp, Off, RW_SEEK_SET);

                // [Android] 6. 讀取圖片檔頭資訊
                SDL_RWread(fp, &size, sizeof(DWORD), 1);
                SDL_RWread(fp, &spr[imgIdx].xl, sizeof(short), 1);
                SDL_RWread(fp, &spr[imgIdx].yl, sizeof(short), 1);
                SDL_RWread(fp, &spr[imgIdx].ox, sizeof(short), 1);
                SDL_RWread(fp, &spr[imgIdx].oy, sizeof(short), 1);
                SDL_RWread(fp, &spr[imgIdx].size, sizeof(unsigned int), 1);

                // 分配記憶體
                MemAlloc(spr[imgIdx].img, spr[imgIdx].size);

                if (spr[imgIdx].img != NULL)
                {
                    // [Android] 7. 讀取像素資料
                    SDL_RWread(fp, spr[imgIdx].img, spr[imgIdx].size, 1);

                    // 格式轉換
                    convert565to555(&spr[imgIdx]);

                    // 如果你需要在這裡建立 Texture (視你的引擎架構而定)
                    // g_SDLInfo.CreateTextureFromSingleSpr_CPP(mainFilename, &spr[imgIdx]);
                }
                else
                {
                    SDL_Log("Error: Memory allocation failed for Menu Spr %d", imgIdx);
                }
            }
        }

        // 更新總圖片大小統計
        dw_TotalImageSize += spr[imgIdx].size;
        i++;
    }

    // [Android] 8. 關檔
    SDL_RWclose(fp);
}
//�ʿ� ���� �޴� �̹��� �޸𸮸� ��ȯ
void FreeMenuData(int nNum)
{
	int i = 0;
	while (1)
	{
		if (aMenuImageNumber[nNum][i] == 9999)
			break;
		if (spr[aMenuImageNumber[nNum][i]].img)
		{
			MemFree(spr[aMenuImageNumber[nNum][i]].img);
			dw_TotalImageSize -= spr[aMenuImageNumber[nNum][i]].size;
		}
		spr[aMenuImageNumber[nNum][i]].img = NULL;


		i++;
	}
}



void kein_TextPut(int x, int y, const std::string& sSource) // sSource 應為 const 引用以避免不必要複製
{
	// 使用 std::string 就不需要固定大小的 temp 緩衝區了
	// 也不需要手動計算 count 陣列來儲存位置，可以直接用 std::string::find

	std::string::size_type current_pos = 0;
	int line_num = 0;
	const int LINE_HEIGHT = 15; // 定義行高為常數，提高可讀性

	while (current_pos < sSource.length())
	{
		// 尋找下一個反引號 '`'
		std::string::size_type next_backtick_pos = sSource.find('`', current_pos);

		std::string current_line_text;
		if (next_backtick_pos == std::string::npos) // 如果沒有找到下一個反引號，表示是最後一行
		{
			current_line_text = sSource.substr(current_pos); // 從當前位置截取到字串末尾
			current_pos = sSource.length(); // 移動到字串末尾，結束迴圈
		}
		else // 找到了反引號
		{
			// 截取從 current_pos 到 next_backtick_pos 之間的子字串
			current_line_text = sSource.substr(current_pos, next_backtick_pos - current_pos);
			current_pos = next_backtick_pos + 1; // 跳過反引號，從下一個字元開始
		}

		// 將 current_line_text 轉換為 C 風格字串傳給 Hprint
		// c_str() 會回傳一個指向內部 C 風格字串的指標
		Hprint(x, y + line_num * LINE_HEIGHT, g_DestBackBuf, current_line_text.c_str());

		line_num++;

		// 如果原始函式中有一行邏輯是：
		// if(j==1) Hprint(x, y, g_DestBackBuf, sSource);
		// 這部分行為在上面的 while 迴圈中已經自然處理了，當 sSource 不含 '`' 時，
		// next_backtick_pos 會是 npos，會一次性處理整個字串。
	}
}

//############################# �۾��� ����������~ ##########################
void MoveTextPut(int x, int y, char* sSource, int type)		 // �� �������� ������ ó���� �ʿ�
{

	int i = 0, count[20], j = 1;
	int start = 0;
	int row;
	char temp[1000];


	int y_source = y;
	static int delay = 0;
	static int skip_count = 0;
	static int skip = 0;
	static int y_plus = y;

	if (type)
	{
		delay = 0;
		skip_count = 0;
		skip = 0;
		y_plus = y_source;
		move_text_flag = 0;
	}

	count[0] = 0;
	while ((unsigned int)i < strlen(sSource))
	{
		if ((*(sSource + i)) == '`') { count[j] = i; j++; }
		i++;
	}
	count[j] = i;			// ������ ó��
	row = j;
	if (j > 20) j = 20;		//  20�� �̻��� ó������ �ʴ´�.

	y = y_plus;
	delay++;
	static int limits = 4;
	if (delay > limits)
	{
		y--;
		delay = 0;
		skip_count++;
		if (y + skip * 20 < 140 /*y_source - (FONTSIZE_+skip)*(skip+1)*/)
		{
			skip++;
			if (j < row) j++;
		}
	}
	y_plus = y;


	if (j == 1) Hprint(x, y, g_DestBackBuf, sSource);
	else
	{
		for (i = 1; i <= j; i++)
		{
			start = count[i - 1];
			memset(temp, ' ', 1000);
			memcpy(temp, (start) ? sSource + start + 1 : sSource, count[i] - count[i - 1] - 1);
			if (i > skip)
			{
				if (y + (i - 1) * 20 < 420) Hprint(x, y + (i - 1) * 20, g_DestBackBuf, temp);
			}
		}
	}

}

//##############################  MENU CALL ####################################//
void CallMenu(int Menu, int x, int y, BOOL bMenuFocus)
{
  if (Menu == MN_YESNO) {
    x += 240;
  }
	SMenu[Menu].bActive = TRUE;
	SMenu[Menu].x = x + GABX_SCREEN;
	SMenu[Menu].y = y;
	if (bMenuFocus) y_MenuFocus = Menu;
	MP3(SN_MENU_OPEN);
	SoundOn = false;
	SetChatMode(CM_COMMAND, true);//021001 lsw //�������̽� �߸� ������ ä��â �ݴ´�
	EWndMgr.ClearAllTxt();//021001 lsw
}

void CallMenu(int Menu, int temp, bool is_focus)
{
	if (Menu == MN_MERCHANT_FIND)
	{
		//		::AddCurrentStatusMessage(255,0,0,"���� ���ξ����� ������ ���� �ʽ��ϴ�.");
		//		return; //soto-030511 �߰�..�ӽ� ���ΰ˻� ������ ������ ��.
	}

	if (Menu != MN_FIGHT_MAP_SIGN && Menu != MN_EXCHANGE_ITEM && Menu != MN_MAN_TO_MAN)
		/* To prevent arenamenu events closing chat, e.g. joining a team */
	{
		SetChatMode(CM_COMMAND, true);//021001 lsw //�������̽� �߸� ������ ä��â �ݴ´�
		EWndMgr.ClearAllTxt();//021001 lsw
	}

	if (SMenu[Menu].bActive) return;
	SoundOn = false;
	SMenu[Menu].bActive = TRUE;
	SMenu[Menu].nTemp = temp;
	switch (Menu)
	{
	case MN_YESNO:
	case MN_LEVELUP:
	case MN_STATUS:
	case MN_ABILITY:
	case MN_ITEM:
	case MN_ARIGEMENT: MP3(SN_MENU_OPEN); break;


	case MN_PARTY_FACE1:
	case MN_PARTY_FACE2:
	case MN_PARTY_FACE3:
	case MN_PARTY_FACE4:
	case MN_PARTY_FACE5:
	case MN_PARTY_FACE6:


	case MN_AWAKE:	break;

	case MN_GOD_EDELBLHOY:
	case MN_GOD_TEFFERY:
	case MN_GOD_OREM:
	case MN_GOD_LETTY:	MP3(SN_MENU_OPEN); break;

	case MN_DISTANCE_TABLE:
	case MN_STORE_SIGN:	MP3(SN_MENU_OPEN); break;

	case MN_DIVIDE_ITEM:	MP3(SN_BOX_OPEN); break;

	case MN_ITEM_EXPLAN:
	case MN_MAGICBOOK:

	case MN_OKCANCEL_MESSAGEBOX:
	case MN_OK_MESSAGEBOX:
	case MN_MAGIC_BOOK:
	case MN_LEARN_MAGIC_BOOK:
	case MN_MEM_BOOK:
	case MN_SMALL_MAP:
	case MN_TIME_DISPLAY_MENU:
	case MN_QUICK_MAGIC_SCROLL:
	case MN_QUICK_MAGIC_PLUS:
	case MN_TIME_DISPLAY_ON_OFF:
	case MN_START_MAP:
	case MN_QUICK_SKILL:
	case MN_HUNTER_MAIN:
	case MN_HUNTER_REGIST:
	case MN_HUNTER_LIST:
	case MN_QUEST:
	case MN_FAME:
	case MN_QUEST_INDEX:
	case MN_RELATION:
	case MN_MAN_TO_MAN:

	default: MP3(SN_MENU_OPEN); break;
	}

	if (is_focus) y_MenuFocus = Menu;

}

//################################################################################//

void CallItemMessageBox(int menu, int item_num, char* source)
{
	y_MenuFocus = MN_ITEM_MESSAGEBOX;
	SMenu[MN_ITEM_MESSAGEBOX].nTemp = menu;
	SMenu[MN_ITEM_MESSAGEBOX].bActive = true;
	SMenu[MN_ITEM_MESSAGEBOX].nField[1].nWillDo = item_num;
	strcpy(SMenu[MN_ITEM_MESSAGEBOX].nField[1].temp, source);
	MP3(SN_MENU_OPEN);
}

void CallOkCancelMessageBox(int menu, int x, int y, const char* source, bool ok_cancel, int param)		// ok�� �ִ� ���� ok_cancel�� �ִ� �޽��� �ڽ������� ����,  menu�� ȣ���� �޴��� �ִ´�
{
	x += GABX_SCREEN+240;

	if (ok_cancel)
	{
		if (g_StartMenuOn)
			SMenu[MN_OKCANCEL_MESSAGEBOX].nTemp = menu;
		else SMenu[MN_OKCANCEL_MESSAGEBOX].nTemp = y_MenuFocus;
		y_MenuFocus = MN_OKCANCEL_MESSAGEBOX;
		SMenu[MN_OKCANCEL_MESSAGEBOX].bActive = true;
		SMenu[MN_OKCANCEL_MESSAGEBOX].x = x;
		SMenu[MN_OKCANCEL_MESSAGEBOX].y = y;
		strcpy(SMenu[MN_OKCANCEL_MESSAGEBOX].nField[4].temp, source);

		SMenu[MN_OKCANCEL_MESSAGEBOX].key = param;
	}
	else
	{
		if (g_StartMenuOn)
			SMenu[MN_OK_MESSAGEBOX].nTemp = menu;
		else SMenu[MN_OK_MESSAGEBOX].nTemp = y_MenuFocus;
		y_MenuFocus = MN_OK_MESSAGEBOX;
		SMenu[MN_OK_MESSAGEBOX].bActive = true;
		SMenu[MN_OK_MESSAGEBOX].x = x;
		SMenu[MN_OK_MESSAGEBOX].y = y;
		strcpy(SMenu[MN_OK_MESSAGEBOX].nField[3].temp, source);
		SMenu[MN_OK_MESSAGEBOX].key = param;
	}
	MP3(SN_WARNING);
}

//##############################  MessageBox CALL ################################//
void CallMessageBox(BOOL bBig, int x, int y, char* source, BOOL bType) // ū �޽��� �ڽ�?, x,y ��ǥ, �޽���, Ÿ���� ȿ�� ����?
{

	x += GABX_SCREEN;
	y += GABY_SCREEN;

	x += 240; y += 60;
	if (bBig)		// BigMessageBox Call
	{
		SMenu[MN_MESSAGEBOX_BIG].bActive = TRUE;
		SMenu[MN_MESSAGEBOX_BIG].x = x;
		SMenu[MN_MESSAGEBOX_BIG].y = y;
		SMenu[MN_MESSAGEBOX_BIG].nField[0].x = 30;
		SMenu[MN_MESSAGEBOX_BIG].nField[0].y = 35;
		strcpy(SMenu[MN_MESSAGEBOX_BIG].nField[0].temp, source);
	}
	else
	{
		SMenu[MN_MESSAGEBOX_SMALL].bActive = TRUE;
		SMenu[MN_MESSAGEBOX_SMALL].x = x;
		SMenu[MN_MESSAGEBOX_SMALL].y = y;
		SMenu[MN_MESSAGEBOX_SMALL].nField[0].x = 27;
		SMenu[MN_MESSAGEBOX_SMALL].nField[0].y = 27;
		strcpy(SMenu[MN_MESSAGEBOX_SMALL].nField[0].temp, source);
	}
}
//################################################################################//


void SetCharacterData()
{
	int i, j;

	for (i = 0; i < MAX_SHN; i++)
	{
		for (j = 0; j < MAX_SHN_FIELD; j++)
		{
			SHideNomal[i][j].bShow = FALSE;
		}
	}
	for (i = 0; i < MAX_CHARACTERDATA; i++)
	{
		if (SCharSource.nCharacterData[i] != -1) SHideNomal[i][SCharSource.nCharacterData[i]].bShow = TRUE;
	}
}

void StartMenuSetting()
{
	int i;
	SCharSource = SCharSource1;
	InitStartMenu();
	//g_SDLInfo.StartMenuOn = g_StartMenuOn = true;
	g_StartMenuOn = true;
	character_active = 0;

	for (i = 0; i < MAX_START; i++)  //�ϴ� ��� �޴� ����ü���� ��Ȱ������ ����
	{
		SMenu[ClosemenuWhenLogin[i]].bActive = FALSE;
		SMenu[ClosemenuWhenLogin[i]].Id = ClosemenuWhenLogin[i];
	}
	
	StartMenuSet();
	CommonMenuSet();  //ũ�Ⱑ ���� �޴����� �ʱ�ȭ
#ifdef _DEBUG // finito 060507
	if (SysInfo.notconectserver) LoadCh[0] = SCharacterData;
#endif
	SCharSource.nCharacterData[FACE] = -1;		// ó�� ������ -1;
	QuitFlag = 0;

}


BOOL MouseInRectCheak(int x, int y, RECT Box, BOOL flag)
{
	POINT pointMouse;
	if (flag) pointMouse = g_pointMouse;
	else pointMouse = StateMousePoint;

	if (pointMouse.x > Box.left + x
		&& pointMouse.x < Box.right + x
		&& pointMouse.y >Box.top + y
		&& pointMouse.y < Box.bottom + y)
	{
		LineDrawInfo order;
		order.rect.left = Box.left + x;
		order.rect.top = Box.top + y;
		order.rect.right = Box.right + x;
		order.rect.bottom = Box.bottom + y;
		order.is_screen_coords = false; // 根據您的原始呼叫，這裡設為 true
#ifdef _TEXTURE
		if (g_StartMenuOn)
		{
			g_SDLInfo.g_linesToDrawThisFrame.push_back(order);
		}
#else
         //g_SDLInfo.g_linesToDrawThisFrame.push_back(order);
#endif
		return TRUE;
	}
	return FALSE;
}

//######################################################################################################//
// �޽������� ����ϴ� �Լ�.
// thai2 YGI
void MessagePut()
{	//< CSD-030324
	Hcolor(FONT_COLOR_NUMBER);
	static int x = 99, y = 357 + GABY_SCREEN;
	std::string msg = "";
	if (SMenu[MN_MESSAGEBOX_BIG].bActive)
	{
#ifdef THAI_LOCALIZING_
		RectTextPut(SMenu[MN_MESSAGEBOX_BIG].x + SMenu[MN_MESSAGEBOX_BIG].nField[0].x + 20, SMenu[MN_MESSAGEBOX_BIG].y + SMenu[MN_MESSAGEBOX_BIG].nField[0].y + 20, 200, SMenu[MN_MESSAGEBOX_BIG].nField[0].temp);
#else
		msg = SMenu[MN_MESSAGEBOX_BIG].nField[0].temp;
		kein_TextPut(SMenu[MN_MESSAGEBOX_BIG].x + SMenu[MN_MESSAGEBOX_BIG].nField[0].x, SMenu[MN_MESSAGEBOX_BIG].y + SMenu[MN_MESSAGEBOX_BIG].nField[0].y, msg);
#endif

		if (LButtonDown || y_delay > 30)
			if (!SMenu[MN_YESNO].bActive)
				CallMenu(MN_YESNO, x, y, TRUE);		// �޴� ��ȣ�� MN_YESNO�̰� ���콺�� �����̴� */
		y_delay++;
	}
	else if (SMenu[MN_MESSAGEBOX_SMALL].bActive)
	{
		msg = SMenu[MN_MESSAGEBOX_SMALL].nField[0].temp;
		kein_TextPut(SMenu[MN_MESSAGEBOX_SMALL].x + SMenu[MN_MESSAGEBOX_SMALL].nField[0].x, SMenu[MN_MESSAGEBOX_SMALL].y + SMenu[MN_MESSAGEBOX_SMALL].nField[0].y, msg);
		if (LButtonDown || y_delay > 30)  CallMenu(MN_YESNO, x, y, TRUE);
		if (!SMenu[MN_YESNO].bActive)
			CallMenu(MN_YESNO, x, y, TRUE);		// �޴� ��ȣ�� MN_YESNO�̰� ���콺�� �����̴�
		y_delay++;
	}
}	//> CSD-030324


//################################ display image little-by-little ####################################//
// ������ ��Ÿ���� �̹���.	������ �� ������ �Ϸ��� bEnd�� TRUE�� �ִ´�.
int k_PutImage(int x, int y, int nImageNumber, BOOL bEnd)
{
	static int rx = 1;

	if (!spr[nImageNumber].img)
	{
		LoadSmallMenuImage(nImageNumber);
	}

	if (SMenu[MN_MAINSTART_BACK].nTemp)	rx = 1;
	SMenu[MN_MAINSTART_BACK].nTemp = FALSE;
	if (bEnd) rx = 39;
	if (2 * rx > x && 2 * rx + 1 < spr[nImageNumber].xl)
	{
        //x, y, sp, g_DestBackBuf, dDxSize, lx, rx + 1, ty, by
        //PutCmprsImgCliping_CPP(x, y, &spr[nImageNumber], g_DestBackBuf, dDxSize, 16 * rx + 1, 0, (int)(12 * (float)rx) + 1,0);
	}
	rx = (rx + 1) % 321;

	return rx;
}

//#########################################################################################################//
//�޴��� Ȱ��ȭ ���ִ��ĸ� �Ǵ��ؼ� ����� �ִ� �Լ�
extern void PutStartLodingImg();

void StartMenuDisplay()//020828 lsw
{
	int i, x;
	for (x = 0; x < MAX_START; x++)
	{
		i = ClosemenuWhenLogin[x];
		if (SMenu[i].bActive)   //i��° �޴��� Ȱ��ȭ ���ֳ�?
		{
			UpperMenuNomalPut(i, SMenu[i].x, SMenu[i].y, SMenu[i].nImageNumber, SMenu[i].nImageType);
			//g_DBGLog.Log(LOG_LV1, "UpperMenuNomalPut");
			StartMenuSubProcessType(&SMenu[i]); //g_DBGLog.Log(LOG_LV1, "StartMenuSubProcessType");
		}
	}
	MessagePut();
#ifdef _DEBUG
	if (GetSysInfo(SI_GAME_MAKE_MODE))
	{
		Hcolor(FONT_COLOR_WHITE);
		SetHangulAlign(TA_LEFT);
		const int iX = 5, iY = 5;
		const int iWidth = TXT_BG_SHORT_WIDTH;//021001 lsw
		int iCount = 0;
		int iReultLineCount = 0;

		int iBlankGab = 0;
		int iReultBlankSize = 0;

		const int iLineGab = TxtOut.GetLineGab();
		const int iBlankGabSize = TxtOut.GetBlankGabSize();

		TxtOut.Init(COLOR_BLACK, TXT_BG_SHORT_WIDTH, 12);
		TxtOut.PutTextHeaderBGImg(iX, iY + iBlankGab + iLineGab * iCount, iWidth);

		iReultLineCount = TxtOut.RcTXTOut(iX, iY + iBlankGab + iLineGab * iCount, iWidth, 1, "FPS:[%d] UPS:[%d]", g_FrameMgr.GetRenderFPS(), g_FrameMgr.GetLogicUPS());
		iCount += iReultLineCount;

		iReultLineCount = TxtOut.RcTXTOut(iX, iY + iBlankGab + iLineGab * iCount, iWidth, 1, "MyCode:[%d]", g_GameInfo.g_MyCode);
		iCount += iReultLineCount;

		iReultLineCount = TxtOut.RcTXTOut(iX, iY + iBlankGab + iLineGab * iCount, iWidth, 1, BetaTest ? "Beta Tester" : "Normal User");
		iCount += iReultLineCount;

		iReultLineCount = TxtOut.RcTXTOut(iX, iY + iBlankGab + iLineGab * iCount, iWidth, 1, "Start MapNo:[%d]", StartPosition);
		iCount += iReultLineCount;

		iReultLineCount = TxtOut.RcTXTOut(iX, iY + iBlankGab + iLineGab * iCount, iWidth, 1, "Menu Focus:[%d]", y_MenuFocus);
		iCount += iReultLineCount;

		iReultBlankSize = TxtOut.RcBlankOut(iX, iY + iBlankGab + iLineGab * iCount, iWidth, 1);
		iBlankGab += iReultBlankSize;

		iReultLineCount = TxtOut.RcTXTOut(iX, iY + iBlankGab + iLineGab * iCount, iWidth, 1, "Proxy IP:");
		iCount += iReultLineCount;
		iReultLineCount = TxtOut.RcTXTOut(iX, iY + iBlankGab + iLineGab * iCount, iWidth, 1, "[%s]", g_GameInfo.proxy_ip.c_str());
		iCount += iReultLineCount;

		iReultBlankSize = TxtOut.RcBlankOut(iX, iY + iBlankGab + iLineGab * iCount, iWidth, 1);
		iBlankGab += iReultBlankSize;

		iReultLineCount = TxtOut.RcTXTOut(iX, iY + iBlankGab + iLineGab * iCount, iWidth, 1, "ServerSet Name:");
		iCount += iReultLineCount;
		iReultLineCount = TxtOut.RcTXTOut(iX, iY + iBlankGab + iLineGab * iCount, iWidth, 1, "[%s]", select_server_name);
		iCount += iReultLineCount;

		iReultBlankSize = TxtOut.RcBlankOut(iX, iY + iBlankGab + iLineGab * iCount, iWidth, 1);
		iBlankGab += iReultBlankSize;

		iReultLineCount = TxtOut.RcTXTOut(iX, iY + iBlankGab + iLineGab * iCount, iWidth, 1, "Character LV:[%d]", SCharacterData.nLevel);
		iCount += iReultLineCount;

		TxtOut.PutTextTailBGImg(iX, iY + iBlankGab + iLineGab * iCount, iWidth);

	}

	if (SysInfo.notconectserver)
	{
		if (g_SetGuildMarkViewOn)
		{
			//EraseScreen(curr_direct_draw_info, RGB(0x00, 0x00, 0x00));
			if ((g_SetGuildMarkViewOn - 1) * 40 > g_GuildMax) g_SetGuildMarkViewOn = 1;

			for (int i = (g_SetGuildMarkViewOn - 1) * 40; i <= g_GuildMax && i < (g_SetGuildMarkViewOn) * 40; i++)
			{
				int x = 200 + (i % 40) / 7 * 80;
				int y = 80 + (i % 40) % 7 * 50;

				PutGuildImage(x, y, i);
			}

			//if(g_aCurrentKeys[DIK_PGDN] & 0x80)
			{
				g_SetGuildMarkViewOn++;
			}
			//if(g_aCurrentKeys[DIK_PGUP] & 0x80)
			{
				g_SetGuildMarkViewOn--;
				if (g_SetGuildMarkViewOn <= 0) g_SetGuildMarkViewOn = 1;
			}
		}
	}

	if (GetSysInfo(SI_GAME_MAKE_MODE))
	{
		if (guild_mark_main.m_Spr.img)
			PutCompressedImage(100, 100, &guild_mark_main.m_Spr);
	}
#endif _DEBUG
	return;
}


//�⺻ ���̽� �޴��� ���
void UpperMenuNomalPut(int menui, int& x, int y, int nImageNumber, int type)
{
	if (!nImageNumber && !type) return;

	Spr* s;
	s = GetSprOfMenu(type, nImageNumber);
	if (s)
	{
		if (x == CENTER_X)
			x = (SCREEN_WIDTH - s->xl) / 2;
		if (x == RIGHT_X)
			x = SCREEN_WIDTH - s->xl;

		SMenu[menui].xl = s->xl;
		SMenu[menui].yl = s->yl;

		//<< 031022 kyo
		if (type == NEW_EMOTE_IMG)
		{
			PutCompressedImageFX(x + s->ox, y + s->oy, s, 20, 1);
		}
		else
		{
			PutCompressedImage(x + s->ox, y + s->oy, s);
		}
		//>> 031022 kyo
	}
}

//###########################################################################################//
//###########################################################################################//
//###########################################################################################//
//###########################################################################################//
//���콺��Ȳ�� ���� �޴��� ó��


void StartMenuChecking()
{
	int i, j, k, x;
	static bool b_DoubleClickOn = false;

	for (x = 0; x < MAX_START; x++)
	{
		i = ClosemenuWhenLogin[x];
		if (SMenu[i].bActive)   //i��° �޴��� Ȱ��ȭ ���ֳ�?
		{
			if (y_MenuFocus && y_MenuFocus != i) continue;

			for (j = 0; j < SMenu[i].nFieldCount; j++)	// rect ��
			{
				switch (SMenu[i].CheakType)
				{
				default: {
					if (MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox))	SMenu[i].nField[j].fRectMouse = TRUE;
					else	SMenu[i].nField[j].fRectMouse = false;

					if (SMenu[i].nField[j].nType == FT_HIDE_NOMAL_ONE_PUT)
					{
						for (k = SMenu[i].nField[j].nSHideNomalStart; k < (SMenu[i].nField[j].nSHideNomalStart) + (SMenu[i].nField[j].nShideNomalPlus) && k < SMenu[i].nField[j].nSHideNomalCount; k++)
						{
							if (MouseInRectCheak(SMenu[i].x, SMenu[i].y, SHideNomal[SMenu[i].nField[j].nSHideNomalNumber][k].rCheakBox))
								SHideNomal[SMenu[i].nField[j].nSHideNomalNumber][k].bRect = true;
							else SHideNomal[SMenu[i].nField[j].nSHideNomalNumber][k].bRect = false;
						}
					}
				}
					   break;
				case 1:											// �迭��
				case 2: {
					int ArrayX = (g_pointMouseX - (SMenu[i].x + 31)) / 10;
					int ArrayY = (g_pointMouseY - (SMenu[i].y + 31)) / 10;
					if ((ArrayX >= 0) && (ArrayX <= 15) && (ArrayY >= 0) && (ArrayY <= 15)) // ���콺�� �̿��� ���� ��ǥ�� �迭�� ����?
					{
						if (((CheakArray[SMenu[i].CheakType - 1][ArrayY][ArrayX] - 1) == j))
							SMenu[i].nField[j].fRectMouse = true;
						else
						{
							SMenu[i].nField[j].fRectMouse = false;
							SMenu[i].nField[j].fLButtonDown = false;
							SMenu[i].nField[j].fCheakFlag = false;
						}
					}
					else
					{
						SMenu[i].nField[j].fRectMouse = false;
						SMenu[i].nField[j].fLButtonDown = false;
						//		SMenu[i].nField[j].fCheakFlag=false;
					}
					break;
				}
				}
			}
		}
		else
		{
			for (j = 0; j < SMenu[i].nFieldCount; j++)
			{
				SMenu[i].nField[j].fRectMouse = false;
			}
		}
	}

	if (g_nLDButtonState == STATE_BUTTON_DOUBLECLICK)
	{
		for (x = 0; x < MAX_START; x++)
		{
			i = ClosemenuWhenLogin[x];
			if (SMenu[i].bActive)
			{
				if (y_MenuFocus && y_MenuFocus != i) continue;
				for (j = 0; j < SMenu[i].nFieldCount; j++)
				{
					if (MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox))
					{
						DoLButtonDoubbleClickOfStartMenu(i, j);
						b_DoubleClickOn = true;
						return;
					}
					if (SMenu[i].nField[j].nType == FT_HIDE_NOMAL_ONE_PUT)
					{
						for (k = SMenu[i].nField[j].nSHideNomalStart; k < (SMenu[i].nField[j].nSHideNomalStart) + (SMenu[i].nField[j].nShideNomalPlus) && k < SMenu[i].nField[j].nSHideNomalCount; k++)
						{
							if (MouseInRectCheak(SMenu[i].x, SMenu[i].y, SHideNomal[SMenu[i].nField[j].nSHideNomalNumber][k].rCheakBox))
							{
								DoLButtonDoubbleClickOfStartMenu(i, j);
								//									MouseDrag = true;		// �̰� ������ Ŭ���� ������ ���� �ʱ� ���ؼ��̴�.. �ʹ��ϴ�~
								b_DoubleClickOn = true;
								return;
							}
						}
					}
				}
			}
		}
	}

	if (g_nLButtonState == STATE_BUTTON_PRESSED)
	{
		LButtonDown = TRUE;
		StateMousePoint = g_pointMouse;
	}
	if (g_nLButtonState == STATE_BUTTON_STILL && LButtonDown == TRUE)   // �ϴ� ���콺�� ��𼱰� ��� ���ȴ�. �װ� ���� �ȿ��� ���ȴ���, �ƴ��� �Ǵ� �κ�
	{
		MouseDrag = true;
		for (x = 0; x < MAX_START; x++)
		{
			i = ClosemenuWhenLogin[x];
			if (SMenu[i].bActive)
			{
				if (y_MenuFocus && y_MenuFocus != i) continue;
				switch (SMenu[i].CheakType)
				{
				case 0:    //RECT�� üũ Ÿ��
					for (j = 0; j < SMenu[i].nFieldCount; j++)  //�޴��� ������ҵ��� �˻�
					{
						if (MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox) &&
							MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox, FALSE))  //���콺�� �޴����� ����� rect�ȿ��� L��ư�� ���ȳ�
						{
							SMenu[i].nField[j].fLButtonDown = TRUE;  //�׷��� ���ȴٰ� ǥ��
						}
						else
						{
							SMenu[i].nField[j].fLButtonDown = FALSE;
						}
					}

					break;

				case 1:    //�迭��ȣ 1������ üũ
				case 2:    //�迭��ȣ 2������ üũ

					int ArrayX = (g_pointMouseX - (SMenu[i].x + 31)) / 10; //10�޴��� 10�ȼ��� ������ �迭ȭ ���״� (16*16)
					int ArrayY = (g_pointMouseY - (SMenu[i].y + 31)) / 10;
					int StateArrayX = (StateMousePoint.x - (SMenu[i].x + 31)) / 10;
					int StateArrayY = (StateMousePoint.y - (SMenu[i].y + 31)) / 10;

					for (j = 0; j < SMenu[i].nFieldCount; j++)
					{
						if (SMenu[i].nField[j].fLButtonDown)  //�� �޴���Ұ� ���Ⱦ���?
						{
							if ((ArrayX >= 0) && (ArrayX <= 15) && (ArrayY >= 0) && (ArrayY <= 15)) // ���콺�� �̿��� ���� ��ǥ�� �迭�� ����?
							{
								if (((CheakArray[SMenu[i].CheakType - 1][ArrayY][ArrayX] - 1) != j) ||
									((CheakArray[SMenu[i].CheakType - 1][StateArrayY][StateArrayX] - 1) != j))   //���콺�� ���ʹ�ư���� �����ߴ� �޴�������ҿ��� ����������?
								{
									SMenu[i].nField[j].fLButtonDown = FALSE;
								}
							}
							else
							{
								SMenu[i].nField[j].fLButtonDown = FALSE;
							}
						}
						else
						{
							if ((ArrayX >= 0) && (ArrayX <= 15) && (ArrayY >= 0) && (ArrayY <= 15) &&   //ó�� ���ʹ�ư���� ���� �� �޴����� ��ҿ� ���콺�� ����� �����߳�?
								(StateArrayX >= 0) && (StateArrayX <= 15) && (StateArrayY >= 0) && (StateArrayY <= 15))
							{
								if (((CheakArray[SMenu[i].CheakType - 1][ArrayY][ArrayX] - 1) == j) &&
									((CheakArray[SMenu[i].CheakType - 1][StateArrayY][StateArrayX] - 1) == j))   //���콺�� ���ʹ�ư���� �����ߴ� �޴�������ҿ��� ����������?
									SMenu[i].nField[(CheakArray[SMenu[i].CheakType - 1][ArrayY][ArrayX] - 1)].fLButtonDown = TRUE;
							}
						}
					}
				}
			}
		}
	}
	else if (g_nLButtonState == STATE_BUTTON_RELEASED) //���콺�� ��� ������ ���³�?
	{
		bSoundOn = false;
		if (b_DoubleClickOn)
		{
			b_DoubleClickOn = false;
			return;
		}
		LButtonDown = FALSE;
		for (x = 0; x < MAX_START; x++)
		{
			bool f_ClickOn = false;		// Ŭ���ϸ� ���̻� �޴� ������ ����.
			i = ClosemenuWhenLogin[x];
			if (SMenu[i].bActive)   //i��° �޴��� Ȱ��ȭ ���ֳ�?
			{
				if (y_MenuFocus && y_MenuFocus != i) continue;
				switch (SMenu[i].CheakType) //Ȱ��ȭ�� �޴��� üũ Ÿ���� ����?
				{
				case 0:    //RECT�� üũ Ÿ��
					for (j = 0; j < SMenu[i].nFieldCount; j++)  //�޴��� ������ҵ��� �˻�
					{
						if (SMenu[i].nField[j].nType == FT_HIDE_NOMAL_ONE_PUT)
						{
							for (k = SMenu[i].nField[j].nSHideNomalStart; k < (SMenu[i].nField[j].nSHideNomalStart) + (SMenu[i].nField[j].nShideNomalPlus) && k < SMenu[i].nField[j].nSHideNomalCount; k++)
							{
								if (MouseInRectCheak(SMenu[i].x, SMenu[i].y, SHideNomal[SMenu[i].nField[j].nSHideNomalNumber][k].rCheakBox))
								{
									SHideNomal[SMenu[i].nField[j].nSHideNomalNumber][k].bShow = TRUE;
									if (SMenu[MN_MAKECHARACTER_BASIC].bActive)
									{
										SCharSource.nCharacterData[SMenu[i].nField[j].nSHideNomalNumber] = k;
										MP3(SN_SELECT_START);
									}
								}
								else
								{
									SHideNomal[SMenu[i].nField[j].nSHideNomalNumber][k].bShow = FALSE;
								}
							}
						}
						else if (!MouseDrag || SMenu[i].nField[j].fLButtonDown)
						{
							if (MouseInRectCheak(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].rCheakBox))
							{
								SMenu[i].nField[j].fLButtonDown = FALSE;
								SMenu[i].nField[j].fCheakFlag = TRUE;
							}
							else
							{
								SMenu[i].nField[j].fLButtonDown = FALSE;
							}
						}
					}
					break;

				case 1:
				case 2:	int ArrayX = (g_pointMouseX - (SMenu[i].x + 31)) / 10;
					int ArrayY = (g_pointMouseY - (SMenu[i].y + 31)) / 10;
					for (j = 0; j < SMenu[i].nFieldCount; j++)
					{
						if (!MouseDrag || SMenu[i].nField[j].fLButtonDown)
						{
							SMenu[i].nField[j].fLButtonDown = FALSE;
							if (((ArrayX >= 0) && (ArrayX <= 15)) && ((ArrayY >= 0) && (ArrayY <= 15)))
							{
								if ((CheakArray[SMenu[i].CheakType - 1][ArrayY][ArrayX] - 1) == j)
								{
									SMenu[i].nField[j].fCheakFlag = TRUE;
									//											f_ClickOn = true;	// üũ �ϳ��� Ŭ���ϳ�
								}
							}
						}
					}
					break;
				}
			}
			if (f_ClickOn) break;
		}
		MouseDrag = false;
	}
}

void StartMenuSubProcessType(SMENU* SubMenu)
{
	int explain_start_map = 0;
	int i = 0, j = 0, k = 0;
	int code;
	int lenght;
	int LenghtTotal;
	static int nCursor_Flicker = 0; //      Ŀ
	BOOL flag = TRUE;               // int & ability=nWeapon_Type; //    ΰ   ɷ ġ      ӽ
	// static id_password = TRUE; //    ̵   н    ĭ  Դ      ϱ ...
	int cursor1 = 0, cursor2 = 0; //      Ŀ   ӽ
	char sPassword_temp[15] = "";

	//         ڽ       κ ....           δ ...
	if ((!SMenu[MN_LOGIN].bActive) && (!SMenu[MN_MAKECHARACTER_NAME].bActive))
	{
		if (!SMenu[MN_DEL_CHAR_COMFORM].bActive) //ĳ        ȣ κ
		{
			EWndMgr.ClearAllTxt(); //021001 lsw
		}
	}
	SetCharacterData();
	start_KeyProc();

	for (j = 0; j < SubMenu->nFieldCount; j++) // ޴        ҵ     ض
	{
		Hcolor(FONT_COLOR_NUMBER);
		SetHangulAlign(TA_LEFT);
		switch (SubMenu->nField[j].nType) //      Ұ    Ÿ  ?
		{
		case FT_NOMAL_PUT: // ̹
			// if( SubMenu->Id == MN_MAINSTART_BACK ) FieldTypeNomalPutFx(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, 15,1);
			// else
			FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);

			if (!SubMenu->key)
			{
				if (*SubMenu->nField[j].temp)
				{
					Hcolor(FONT_COLOR_NAME);
					static int old_field = j;
					static int count = 0;
					if (old_field != j)
					{
						old_field = j;
						count = 0;
					}

					if (!count)
						MP3(SN_MENU_OPEN);

					int curr_line = count / 30;
					int max_line = GetLineRectTextPut(170, SubMenu->nField[j].temp);
					if (max_line)
					{
						if (max_line == 2)
						{
							RectTextPut(SubMenu->x + SubMenu->nField[j].x + 13, SubMenu->y + SubMenu->nField[j].y + 14, 170, SubMenu->nField[j].temp); //
						}
						else
						{
							curr_line %= max_line;
							SkipRectTextPut(SubMenu->x + SubMenu->nField[j].x + 13, SubMenu->y + SubMenu->nField[j].y + 14, 170, curr_line, SubMenu->nField[j].temp, 2, 0); //
						}
						count++;
					}
				}
			}
			SubMenu->nField[j].fLButtonDown = false;
			SubMenu->nField[j].fCheakFlag = false;
			break;

		case FT_NOMAL_PUT_FX:
			FieldTypeNomalPutFx(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, 8, 3);
			break;

		case FT_NOMAL_PUT_CENTER_1:
			if (!spr[SubMenu->nField[j].nImageNumber].img)
			{
				LoadSmallMenuImage(SubMenu->nField[j].nImageNumber);
			}
			lenght = (spr[SubMenu->nImageNumber].xl - spr[SubMenu->nField[j].nImageNumber].xl) / 2;
			FieldTypeNomalPut(SubMenu->x, SubMenu->y, lenght + 1, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
			break;
		case FT_SWITCH_PUT:
		{
			static int stch = 0;
			static int de = 3;
			FieldTypeNomalPutFx(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, de / 3, 2);

			if (stch > 19)
				de--;
			else
				de++;
			stch++;
			stch %= 40;
		}

		case FT_SELECT_SERVER_SCROLL: //020828 lsw
		{
			const int willdo = SubMenu->nField[j].nWillDo;
			const int max_server = SubMenu->work;
			int& curr_page = SubMenu->key;
			const int max_page = (max_server - 1) / 5 + 1;
			switch (willdo)
			{
			case SCROLL_UP:
			{
				if (curr_page <= 0)
					break; //0
				if (SubMenu->nField[j].fCheakFlag)
				{
					curr_page--;
					SubMenu->nField[j].fCheakFlag = 0;
					break;
				}
			}
			break;
			case SCROLL_DOWN:
			{
				if (max_page <= curr_page + 1)
					break;
				if (SubMenu->nField[j].fCheakFlag)
				{
					curr_page++;
					SubMenu->nField[j].fCheakFlag = 0;
					break;
				}
			}
			break;
			default:
			{
			}
			break;
			}

			int target_number;
			if (SubMenu->nField[j].fLButtonDown)
			{
				target_number = SubMenu->nField[j].nImageNumber;
			}
			else if (SubMenu->nField[j].fRectMouse)
			{
				target_number = SubMenu->nField[j].nRectImage;
			}
			else
			{
				target_number = SubMenu->nField[j].nSpecialWillDo;
			}
			FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, target_number, SubMenu->nField[j].nImageType);
		}
		break;

		case FT_HIDE_AUTO_PUT:
			if (SubMenu->nField[j].fRectMouse)
			{
				// if( !bSoundOn ) { MP3( SN_TOUCH_START ); bSoundOn = true; }
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage);
			}
		case FT_HIDE_PUT_AUTO:
		case FT_HIDE_PUT: // 콺 ̺ Ʈ üũ  Ⱥ  ̴ ̹   ̴
			if (SubMenu->nField[j].fLButtonDown)
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
			break;

		case FT_SELECT_SERVER_BUTTON:
		{
			int willdo = SubMenu->nField[j].nWillDo;
			int key = SubMenu->key; // ũ    Ǿ  ִ
			SetHangulAlign(TA_CENTER);

			if (SubMenu->nField[j].fCheakFlag)
			{
				// ip
				g_SelectServerIndex = key * 5 + willdo;

				// strcpy( select_server_name,  server_set[g_SelectServerIndex].server_name );
				// strcpy( g_GameInfo.proxy_ip[0], server_set[g_SelectServerIndex].server_ip1 );
				// strcpy( g_GameInfo.proxy_ip[1], server_set[g_SelectServerIndex].server_ip2 );
				// memset(g_GameInfo.proxy_ip[0],0,sizeof (g_GameInfo.proxy_ip[0]));
				// strcpy(g_GameInfo.proxy_ip[0],"210.51.13.39");
				// memset(g_GameInfo.proxy_ip[1],0,sizeof (g_GameInfo.proxy_ip[1]));
				// strcpy(g_GameInfo.proxy_ip[1],"210.51.13.39");
				// SMenu[MN_MAINSTART_BACK].bActive=true;//020515 lsw
				// SMenu[MN_LOGIN].bActive=TRUE;
				MP3(SN_MENU_OPEN);
				::CallLoginMenu(true);
				SubMenu->nField[j].fCheakFlag = false;
				SubMenu->bActive = false;
				break;
			}

			if (SubMenu->nField[j].fLButtonDown)
			{
				Hcolor(FONT_COLOR_PLUS);
			}
			else if (SubMenu->nField[j].fRectMouse)
			{
				Hcolor(FONT_COLOR_SPECIAL);
			}
			else
			{
				Hcolor(FONT_COLOR_NAME);
			}

			Hprint2(SubMenu->x + SubMenu->nField[j].x, SubMenu->y + SubMenu->nField[j].y, g_DestBackBuf, "%s", server_set[key * 5 + willdo].server_name);
			break;
		}

		case FT_HIDE_WILLDO_AUTO_PUT:
		{
			int flag = FALSE;
			int sound = 0;
			if (SubMenu->nField[j].fCheakFlag)
			{
				switch (SubMenu->nField[j].nWillDo)
				{
				case MN_MAKECHARACTER_CLASS:
					SCharSource.nCharacterData[ARIGEMENT] = 4;
					SCharSource.age = 18;
					break;

				case MN_MAKECHARACTER_NAME:
					if (SCharSource.nCharacterData[GENDER] == -1)
					{
						flag = TRUE;
						sound = 1;
					}
					else EWndMgr.showInputBox(HWND_CREATE_CHAR_NAME);
					break;
				case MN_MAKECHARACTER_AGE:
					if (SCharSource.nCharacterData[ARIGEMENT] == -1)
					{
						flag = TRUE;
						sound = 1;
					}
					break;
				case MN_MAKECHARACTER_FACE:
					EWndMgr.GetTxt(HWND_CREATE_CHAR_NAME, SCharSource.sCharacterName);
					if (*SCharSource.sCharacterName)
					{
						if (SearchStrStr(SCharSource.sCharacterName, " "))
						{
							CallOkCancelMessageBox(MN_MAKECHARACTER_NAME, 0, 0, lan->OutputMessage(5, 61), 0); //lsw
							                                                     //021001 lsw
							flag = true;
							sound = 1;
							break;
						}

						if (!CheckCharName(SCharSource.sCharacterName))
						{
							CallOkCancelMessageBox(MN_MAKECHARACTER_NAME, 0, 0, lan->OutputMessage(5, 62), 0); //lsw
							                                                            //021001 lsw
							flag = TRUE;
							sound = 1;
						}
						else EWndMgr.hideInputBox(HWND_CREATE_CHAR_NAME);
					}
					else
						flag = true;
					break;
				case MN_MAKECHARACTER_COLOR:
					if (SCharSource.nCharacterData[CLASS] == -1 || SCharSource.nCharacterData[SPELL] == -1)
					{
						sound = 1;
						flag = TRUE;
					}
					SetTacticsBySpell(&SCharSource);
					break;
				case MN_MAKECHARACTER_JOB:
				{
					if (SCharSource.nCharacterData[CLASS] == -1 || SCharSource.nCharacterData[SPELL] == -1) //020725 lsw
					{
						sound = 1;
						flag = TRUE;
					}
					if (SCharSource.nCharacterData[GENDER])
					{
						SCharSource.body_r = 204;
						SCharSource.body_g = 146;
						SCharSource.body_b = 118;
						SCharSource.nCharacterData[TACTICS_MAN] = 1;
					}
					else
					{
						SCharSource.body_r = 237;
						SCharSource.body_g = 183;
						SCharSource.body_b = 156;
						SCharSource.nCharacterData[TACTICS_WOMAN] = 1;
					}
					if (SCharSource.nCharacterData[GENDER] && SCharSource.nCharacterData[TACTICS_MAN] == -1)
					{
						flag = TRUE;
						sound = 1;
					}
					else if (SCharSource.nCharacterData[GENDER] == 0 && SCharSource.nCharacterData[TACTICS_WOMAN] == -1)
						flag = TRUE;
					break;
				}
				case MN_MAKECHARACTER_ARIGEMENT: // SCharSource.nCharacterData[FACE]=SCharSource.nCharacterData[GENDER]*1000 + 0;
					break;                       //   ̹   ־  ֱ
				}
			}
			if (flag)
			{
				SubMenu->nField[j].fCheakFlag = FALSE;
				SubMenu->nField[j].fLButtonDown = FALSE;
				SubMenu->nField[j].fRectMouse = FALSE;
			}
			else if (sound)
				MP3(SN_WARNING);

			if (SubMenu->nField[j].fRectMouse)
			{
				// if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
			}
		}

		case FT_HIDE_WILLDO_PUT:
		{
			if (SubMenu->nField[j].fRectMouse ||
				SubMenu->nField[j].fLButtonDown)
			{
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
			}
		}

		case FT_WILLDO_PUT:
		{
			if (SubMenu->nField[j].fCheakFlag)
			{
				SubMenu->bActive = FALSE; // ڽ
				SMenu[SubMenu->nField[j].nWillDo].bActive = TRUE; // ٸ ޴ Ȱ ȭ
				MP3(SN_MENU_OPEN);

				switch (SubMenu->nField[j].nWillDo)
				{
				case MN_LODING:
				{
					SMenu[MN_SELECT].bActive = FALSE;
					SMenu[MN_LOCATION].bActive = FALSE;
					if (LoadCh[character_active].sCharacterName[0])
					{
						SendChoiceCharacter(character_active);
					}
					break;
				}
				case MN_CREDIT:
				case MN_INFO:
					// SMenu[MN_MAINSTART_BACK].bActive = false;
					SMenu[MN_LOGIN].bActive = false; //020515 lsw
					SMenu[SubMenu->nField[j].nWillDo].bActive = true; //020515 lsw
					move_text_flag = 1;
					break;
				}
				SubMenu->nField[j].fCheakFlag = FALSE;
				SubMenu->nField[j].fLButtonDown = FALSE;
			}
		}
		break;

		case FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2:
		{
			if (CheckNationCity(SubMenu->nField[j].nWillDo) == NOT)
				break;

            // 定義引用，方便閱讀 (原本的寫法)
            int& tt = SubMenu->nField[j].nSHideNomalStart; // 0 或 1，控制方向
            int& de = SubMenu->nField[j].nShideNomalPlus;  // Alpha 值 (2 ~ 15)

            // --- [修改開始] 使用 DeltaTime 控制呼吸速度 ---

            // 1. 宣告一個靜態計時器 (static 確保變數在函數結束後數值保留)
            // 注意：因為是 static，所有執行到這段代碼的按鈕都會共用這個計時器，這會讓它們的閃爍頻率同步，這通常是好事。
            static float s_fAnimTimer = 0.0f;

            // 2. 設定更新間隔 (秒)
            // 例如 0.05f 代表每 0.05 秒數值變動一次 (約每秒 20 次更新)
            // 數值越大，呼吸越慢；數值越小，呼吸越快。
            const float fStepTime = 0.05f;

            // 3. 累加時間
            s_fAnimTimer += g_FrameMgr.GetDeltaTime();

            // 4. 當累積時間超過設定的間隔時，執行一次數值變更
            // 使用 while 是為了防止當 FPS 極低(大卡頓)時，動畫變慢，while 可以讓它一次補回進度
            while (s_fAnimTimer >= fStepTime)
            {
                s_fAnimTimer -= fStepTime; // 扣除已消耗的時間

                // 原本的邏輯移到這裡面
                if (tt)
                {
                    de++;
                    if (de > 15)
                    {
                        tt = !tt;
                    }
                }
                else
                {
                    de--;
                    if (de < 2)
                    {
                        tt = !tt;
                    }
                }
            }

			Spr* s = GetSprOfMenu(SubMenu->nField[j].nImageType, SubMenu->nField[j].nRectImage);
			if (s)
				PutCompressedImageFX(SubMenu->x + SubMenu->nField[j].x + s->ox, SubMenu->y + SubMenu->nField[j].y + s->oy, s, de, 2);
		}
		case FT_HIDE_SPECIAL_WILLDO_AUTO_PUT:
			if (SubMenu->nField[j].fRectMouse)
			{
				// if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
				if (SubMenu->nField[j].nRectImage == 30 && SubMenu->nField[j].nImageType == START_ETC)
				{
					static int tt;
					static int de = 3;
                    static float s_fAnimTimer = 0.0f;

                    // 2. 設定更新間隔 (秒)
                    // 例如 0.05f 代表每 0.05 秒數值變動一次 (約每秒 20 次更新)
                    // 數值越大，呼吸越慢；數值越小，呼吸越快。
                    const float fStepTime = 0.05f;

                    // 3. 累加時間
                    s_fAnimTimer += g_FrameMgr.GetDeltaTime();

                    // 4. 當累積時間超過設定的間隔時，執行一次數值變更
                    // 使用 while 是為了防止當 FPS 極低(大卡頓)時，動畫變慢，while 可以讓它一次補回進度
                    while (s_fAnimTimer >= fStepTime)
                    {
                        s_fAnimTimer -= fStepTime; // 扣除已消耗的時間

                        // 原本的邏輯移到這裡面
                        if (tt)
                        {
                            de++;
                            if (de > 9)
                            {
                                tt = !tt;
                            }
                        }
                        else
                        {
                            de--;
                            if (de < 2)
                            {
                                tt = !tt;
                            }
                        }
                    }

					Spr* s = &start_etc_spr[SubMenu->nField[j].nRectImage];
					PutCompressedImageFX(SubMenu->x + SubMenu->nField[j].x + s->ox, SubMenu->y + SubMenu->nField[j].y + s->oy, s, de, 2);
				}
				else
				{
					if (SubMenu->Id == MN_START_MAP)
					{ //020828 lsw
						Hcolor(FONT_COLOR_WHITE);
						SetHangulAlign(TA_LEFT);
						const int iX = 620;
						const int iY = 363;

						const int iWidth = TXT_BG_SHORT_WIDTH;
						int iCount = 0;
						int iReultLineCount = 0;

						int iBlankGab = 0;
						int iReultBlankSize = 0;

						const int iLineGab = TxtOut.GetLineGab();
						const int iBlankGabSize = TxtOut.GetBlankGabSize();

						TxtOut.Init(COLOR_BLACK, TXT_BG_SHORT_WIDTH);
						explain_start_map = 1;
						switch (SubMenu->nField[j].nSpecialWillDo)
						{
						case SWD_CONNECT:
						{
							TxtOut.PutTextHeaderBGImg(iX, iY + iBlankGab + iLineGab * iCount, iWidth);
							iReultLineCount = TxtOut.RcTXTOut(iX, iY + iBlankGab + iLineGab * iCount, iWidth, 1, lan->OutputMessage(5, (!SubMenu->work) ? 63 : 65));
							iCount += iReultLineCount;
							TxtOut.PutTextTailBGImg(iX, iY + iBlankGab + iLineGab * iCount, iWidth);
						}
						break;
						case SWD_CONNECT_RECENT:
						{
							TxtOut.PutTextHeaderBGImg(iX, iY + iBlankGab + iLineGab * iCount, iWidth);
#ifdef LIGHT_VERSION_
							iReultLineCount = TxtOut.RcTXTOut(iX, iY + iBlankGab + iLineGab * iCount, iWidth, 1, kein_GetMenuString(208));
#else
							iReultLineCount = TxtOut.RcTXTOut(iX, iY + iBlankGab + iLineGab * iCount, iWidth, 1, lan->OutputMessage(5, 64));
#endif
							iCount += iReultLineCount;
							TxtOut.PutTextTailBGImg(iX, iY + iBlankGab + iLineGab * iCount, iWidth);
						}
						break;
						default:
						{
							if (SubMenu->nField[j].nWillDo == MN_SELECT_CHARACTER)
							{
								TxtOut.PutTextHeaderBGImg(iX, iY + iBlankGab + iLineGab * iCount, iWidth);
								iReultLineCount = TxtOut.RcTXTOut(iX, iY + iBlankGab + iLineGab * iCount, iWidth, 1, lan->OutputMessage(5, 66)); //lsw
								iCount += iReultLineCount;
								TxtOut.PutTextTailBGImg(iX, iY + iBlankGab + iLineGab * iCount, iWidth);
							}
							else
							{
								explain_start_map = 0;
							}
						}
						break;
						}
						FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
					}
					else
					{
						FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
					}
				}
			}
			else if (SubMenu->Id == MN_START_MAP && !explain_start_map)
			{
				if (SubMenu->nField[j].nWillDo == MN_SELECT_CHARACTER)
				{ //020828 lsw
					Hcolor(FONT_COLOR_WHITE);
					SetHangulAlign(TA_LEFT);
					const int iX = 620;
					const int iY = 363;

					const int iWidth = TXT_BG_SHORT_WIDTH;
					int iCount = 0;
					int iReultLineCount = 0;

					int iBlankGab = 0;
					int iReultBlankSize = 0;

					const int iLineGab = TxtOut.GetLineGab();
					const int iBlankGabSize = TxtOut.GetBlankGabSize();

					TxtOut.Init(COLOR_BLACK, TXT_BG_SHORT_WIDTH);

					TxtOut.PutTextHeaderBGImg(iX, iY + iBlankGab + iLineGab * iCount, iWidth);
					iReultLineCount = TxtOut.RcTXTOut(iX, iY + iBlankGab + iLineGab * iCount, iWidth, 1, lan->OutputMessage(5, 67));
					iCount += iReultLineCount;
					TxtOut.PutTextTailBGImg(iX, iY + iBlankGab + iLineGab * iCount, iWidth);
				}
			}
		case FT_HIDE_SPECIAL_WILLDO_PUT:
		{
			int flag = FALSE;
			switch (SubMenu->nField[j].nWillDo)
			{
			case MN_MAKECHARACTER_CLASS:
				if (SCharSource.nCharacterData[CLASS] == -1)
					flag = TRUE;
				break;
			case MN_MAKECHARACTER_JOB:
				if (SCharSource.nCharacterData[JOB] == -1)
					flag = TRUE;
				break;
			}
			if (flag)
				goto Label_2;
			if (SubMenu->nField[j].fLButtonDown)
			{
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
			}
			if (SubMenu->nField[j].nType == FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2 && SubMenu->nField[j].nSHideNomalCount)
			{
				if (SubMenu->nField[j].fRectMouse || SubMenu->nField[j].fLButtonDown)
				{
					// if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
					FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x + 20, SubMenu->nField[j].y + 20, SubMenu->nField[j].nSHideNomalCount, SubMenu->nField[j].nImageType);
				}
			}
		}
		case FT_SPECIAL_WILLDO:
		{
			if ((SubMenu->nField[j].fRectMouse ||
				SubMenu->nField[j].fLButtonDown) &&
				SubMenu->nField[j].nType != FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2)
			{
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
			}
			if (SubMenu->nField[j].fCheakFlag)
			{
				switch (SubMenu->nField[j].nSpecialWillDo)
				{
				case SWD_DELETE_CHARATER:
				{
					char szKeyCode[51] = { 0 };
					EWndMgr.GetTxt(HWND_DELETE_CHAR_KEY, szKeyCode); 

					if (TRUE == SendDeleteCharacter(LoadCh[character_active].sCharacterName, szKeyCode))
					{
						memset(&LoadCh[character_active], 0, sizeof(SCharacter));
					}
					SCharSource = LoadCh[character_active];
				}
				case SWD_DELETE_CHARATER_CLOSE:
				{
					SubMenu->bActive = false;
					y_MenuFocus = 0; EWndMgr.hideInputBox(HWND_DELETE_CHAR_KEY);
				}
				break;
				case SWD_LOGON_OK:
					if (*SCharSource.sCharacterName) //  ̸    ٸ   ->ĳ
					{
						if (SubMenu->Id == MN_RESET_JOB)
						{
							if (SCharSource.nCharacterData[JOB] == -1)
								goto Label_2;
							char temp[255];
							sprintf(temp, lan->OutputMessage(5, 68), SHideNomal[HN_MAKECHARACTER_JOB_TEXT][SCharSource.nCharacterData[JOB]].temp); //lsw
							CallOkCancelMessageBox(SubMenu->Id, 0, 0, temp);
							goto Label_2;
						}
						else if (SubMenu->Id == MN_SELECT_NATION)
						{
							if (SubMenu->nField[j].nWillDo == 0)
							{
								SubMenu->work = 0; //
								goto Label_2;
							}
							char temp[255];
							switch (SubMenu->work)
							{
							case 0:
								goto Label_2;
							case 100:
								goto Label_2;
								// sprintf( temp, " ߸        ϱ ?" );
								// break;
							case N_VYSEUS:
#ifdef KOREA_LOCALIZING_
							{
								sprintf(temp, lan->OutputMessage(3, 300), NationName[SubMenu->work]); //lsw
								CallOkCancelMessageBox(SubMenu->Id, 0, 0, temp);
							}
							break;
#endif
							case N_ZYPERN:
							case N_HEGEMONIA:
							case N_YILSE:
								sprintf(temp, lan->OutputMessage(5, 69), NationName[SubMenu->work]); //lsw
								break;
							default:
								goto Label_2;
							}
							for (int a = 0; a < 4; a++)
								LoadCh[a].nation = SubMenu->work;
							SCharSource.nation = SubMenu->work;
							CallOkCancelMessageBox(SubMenu->Id, 0, 0, temp);
							goto Label_2;
						}
					}
				case SWD_CHARACTER_DEL_ADD: //020802 lsw
				{
					if (*SCharSource.sCharacterName) // delete
					{
						MP3(SN_WARNING);
						char temp[255];
						sprintf(temp, lan->OutputMessage(5, 70), SCharSource.sCharacterName); //lsw
						CallOkCancelMessageBox(SubMenu->Id, 0, 0, temp);
						goto Label_2;
					}
				}
				case SWD_RESET_JOB:
					if (*SCharSource.sCharacterName)
					{
						if (SCharSource.nCharacterData[JOB] >= 20)
						{
							////////// ĳ     ʱ ȭ //////////////
							if (SCharSource.nation)
							{
								{
									int ret = SendCheckNewChar(SCharSource.sCharacterName);
									switch (ret)
									{
									default:
										CallOkCancelMessageBox(SubMenu->Id, 0, 0, lan->OutputMessage(5, 71), 0);
										goto Label_2; //lsw
									case -4:
										CallOkCancelMessageBox(SubMenu->Id, 0, 0, lan->OutputMessage(5, 72), 0);
										goto Label_2; //lsw
									case 1:
										break;
									}
								}

								CallMN_START_MAP_Menu();
								goto Label_2;
							}
							else //          ޴
							{
								OpenSelectNationMenu();
								break;
							}
						}
						else
						{
							SCharSource.nCharacterData[JOB] = -1;
							SMenu[MN_MAKECHARACTER_BASIC].bActive = TRUE;
							SMenu[MN_RESET_JOB].bActive = true;
							SMenu[MN_RESET_JOB_EXPLAIN].bActive = true;
							MP3(SN_MENU_OPEN);
							break;
						}
						break;
					}
					//      ŸƮ ޴   ĳ          ޴  
				case SWD_ENTER_MAKECHARACTER:
				{
					SMenu[MN_MAKECHARACTER_ABILITY].nField[1].nSHideNomalStart = 0; //      ʱ ȭ ۾
					SCharSource = SCharSource1;                                    //    "
					nCursor_Flicker = 0;
					for (k = 0; k < MAX_START; k++)
						SMenu[ClosemenuWhenLogin[k]].bActive = FALSE; //        ȭ    ݴ´ .
					SMenu[MN_MAKECHARACTER_BASIC].bActive = TRUE;
					SMenu[MN_MAKECHARACTER_CHEAK1].bActive = TRUE;
					// SMenu[MN_MAKECHARACTER_CHEAK1].x=SMenu[MN_MAKECHARACTER_CHEAK1].nField[MAX_FIELD-1].x;
					// SMenu[MN_MAKECHARACTER_CHEAK1].y=SMenu[MN_MAKECHARACTER_CHEAK1].nField[MAX_FIELD-1].y;
					SMenu[MN_MAKECHARACTER_GENDER].bActive = TRUE;
					MP3(SN_MENU_OPEN);
					break;
				}
				// ⺻   ĳ          Ⱑ   ֻ           ɷ ġ      Ϸ  
				case SWD_MAKECHARACTER_ABILITY:
					if (SCharSource.nCharacterData[SPELL]) // case priest
					{
						SMenu[MN_MAKECHARACTER_CHEAK3].nField[11].nType = FT_NO_CHECK;
					}
					else // case wizard
					{
						SMenu[MN_MAKECHARACTER_CHEAK3].nField[11].nType = FT_NOMAL_PUT;
					}
					SMenu[MN_MAKECHARACTER_CHEAK1].bActive = FALSE;
					SMenu[MN_MAKECHARACTER_CHEAK3].bActive = TRUE;
					SMenu[MN_MAKECHARACTER_ABILITY].bActive = TRUE;
					SCharSource.nCharacterData[JOB] += 20;
					MakeCharacterAbility();
					MP3(SN_MENU_OPEN);
					break;
					//ĳ    ̸        ޴
				case SWD_MAKECHARACTER_ENTERNAME: //SMenu[MN_MAKECHARACTER_NAME].bActive=TRUE;
					if (SCharSource.body_r == -1)
					{
						if (SCharSource.nCharacterData[GENDER])
						{
							SCharSource.body_r = 204;
							SCharSource.body_g = 146;
							SCharSource.body_b = 118;
						}
						else
						{
							SCharSource.body_r = 237;
							SCharSource.body_g = 183;
							SCharSource.body_b = 156;
						}
					}

					goto Label_2;

					//Ŭ          ϴ ޴      ̳          ϴ ޴
				case SWD_MAKECHARACTER_SPELLORCOLOR:
					//      Ŭ             ؾ  ϳ    ص  ǳ ?
					if (SCharSource.nCharacterData[CLASS] == WIZARD) //Ŭ       ڵ ?
					{
						SCharSource.nCharacterData[SPELL] = WIZARD_SPELL;
						SMenu[MN_MAKECHARACTER_SPELL2].bActive = TRUE;
						SMenu[MN_MAKECHARACTER_SPELL2].nField[2].nType = FT_NOMAL_PUT_CENTER_1;
						SMenu[MN_MAKECHARACTER_SPELL2].nField[3].nType = FT_NO_CHECK;
					}
					else if (SCharSource.nCharacterData[CLASS] == PRIEST) //Ŭ         Ʈ ?
					{
						SCharSource.nCharacterData[SPELL] = PRIEST_SPELL;
						SMenu[MN_MAKECHARACTER_SPELL2].bActive = TRUE;
						SMenu[MN_MAKECHARACTER_SPELL2].nField[2].nType = FT_NO_CHECK;
						SMenu[MN_MAKECHARACTER_SPELL2].nField[3].nType = FT_NOMAL_PUT_CENTER_1;
					}
					else
						SMenu[MN_MAKECHARACTER_SPELL].bActive = TRUE; // ƴϸ        ޴ ȣ
					MP3(SN_MENU_OPEN);
					break;

					//          ϴ ޴   ƽ          ϴ ޴
				case SWD_MAKECHARACTER_TACTICS:
				{
					if (SHideNomal[HN_MAKECHARACTER_COLOR_SELECT][0].bShow || SHideNomal[HN_MAKECHARACTER_COLOR_SELECT][1].bShow)
					{
						if (SCharSource.nCharacterData[GENDER]) // ڳ ?
							SMenu[MN_MAKECHARACTER_TACTICS_MAN].bActive = TRUE;
						else
							SMenu[MN_MAKECHARACTER_TACTICS_WOMAN].bActive = TRUE; // ڸ
						MP3(SN_MENU_OPEN);

						break;
					}
					goto Label_2;
				}

				case SWD_YESNO_NO:
					y_MenuFocus = 0;
					for (k = 0; k < MAX_START; k++)
						SMenu[ClosemenuWhenLogin[k]].bActive = FALSE;
					y_delay = 0; //
					MakeCharacterAbility(); //   ̽         ɷ  ٽ ʱ ȭ...

					// SMenu[MN_MAKECHARACTER_ABILITY].nField[1].nSHideNomalStart=0;
					SMenu[MN_MAKECHARACTER_BASIC].bActive = TRUE; // ɷ ġ        ư
					SMenu[MN_MAKECHARACTER_CHEAK3].bActive = TRUE;
					SMenu[MN_MAKECHARACTER_ABILITY].bActive = TRUE;
					break;

				case SWD_YESNO_YES:
				{
					for (k = 0; k < MAX_START; k++)
						SMenu[ClosemenuWhenLogin[k]].bActive = FALSE;
					y_delay = 0; //

					for (k = STR; k <= LUCK; k++)
					{
						SCharSource.nCharacterAbility[k] += SCharSource.nAbilityMax[k];
						SCharSource.nAbilityMax[k] = 0;
					}
					SCharSource.nMoney = CalCreateMoney(&SCharSource);

					SCharSource.nation = 0;
					for (k = 0; k < 4; k++)
					{
						if (LoadCh[k].sCharacterName[0] && LoadCh[k].nation)
						{
							SCharSource.nation = LoadCh[k].nation; //        󰣴 ..
							break;
						}
					}
					SCharacterData = SCharSource;
					int ret = SendCreateCharacter(&SCharacterData);
					if (ret == -1) // CMD_INVALID_DB
					{
						CallOkCancelMessageBox(0, 0, 0, kein_GetMenuString(0));
					}
					else
					{
						LoadCh[character_active] = SCharacterData;
					}

					SMenu[MN_SELECT_CHARACTER].bActive = TRUE;
					MP3(SN_MENU_OPEN);
					y_MenuFocus = 0;
					break;
				}

				case SWD_CALL_OTHER:
					if (SubMenu->nField[j].nWillDo == MN_SELECT && SMenu[SubMenu->nField[j].nWillDo].bActive)
					{
						SubMenu->nField[0].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
						SubMenu->nField[3].nType = FT_HIDE_WILLDO_AUTO_PUT;
						SMenu[SubMenu->nField[j].nWillDo].bActive = FALSE;
						SMenu[MN_LOCATION].bActive = TRUE;
						SubMenu->nField[j].nType = FT_NO_CHECK;
						MP3(SN_MENU_OPEN);
					}
					if (SubMenu->nField[j].nWillDo == MN_LOCATION && SMenu[SubMenu->nField[j].nWillDo].bActive)
					{
						SMenu[SubMenu->nField[j].nWillDo].bActive = FALSE;
						SubMenu->nField[j].nType = FT_NO_CHECK;
					}
					goto Label_2;

				case SWD_IDEDITBOX:
				{
					id_password = true;
					cursor2 = nCursor_Flicker;
					nCursor_Flicker = cursor1;
					goto Label_2;
				}
				break;
				case SWD_PASSEDITBOX:
				{
					id_password = false;
					cursor1 = nCursor_Flicker;
					nCursor_Flicker = cursor2;
					goto Label_2;
				}
				break;
				case SWD_WILLDO:
				{
					SMenu[SubMenu->nField[j].nWillDo].bActive = true;
					MP3(SN_MENU_OPEN);
				}
				break;

				case SWD_IDPASS:
				{ //021001 lsw
					sId[0] = 0;
					sPassword[0] = 0;
					SubMenu->bActive = false; //       δ //020515 lsw
					MP3(SN_MENU_OPEN);
					::CallLoginMenu();
				}
				break;

				case SWD_MAKECHARACTER_GENDERBACK:
					for (k = 0; k < MAX_START; k++)
					{
						SMenu[ClosemenuWhenLogin[k]].bActive = FALSE;
						SMenu[ClosemenuWhenLogin[k]].key = 0; //      ٽ
					}
					//SMenu[MN_MAINSTART_BACK].bActive=TRUE;
					SMenu[MN_SELECT_CHARACTER].bActive = TRUE;
					SCharSource = SCharSource1; //  ٽ ʱ ȭ
					EWndMgr.hideInputBox(HWND_CREATE_CHAR_NAME);
					break;

				case SWD_RE_LOGON:
					SCharSource = LoadCh[character_active];
					SMenu[SubMenu->nField[j].nWillDo].bActive = TRUE;
					// SMenu[MN_MAINSTART_BACK].bActive=false;
					SMenu[MN_LOGIN].bActive = false; //020515 lsw
					SMenu[MN_START_MAP].bActive = false;
					y_MenuFocus = 0; //020815-2 lsw
					break;

				case SWD_LOGON:
				{ //021021 lsw
					//g_bLogOnFlag = false;//010909 lsw           α׿   ÷ ׸  ׿   ش
					::SWD_LOGON_Clicked(SubMenu->Id);
					goto Label_2;
				}
				break;
				case SWD_CHARACTER_SELECT:
				{
					for (int a = 4; a <= 7; a++)
					{
						SubMenu->nField[a].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
					}
					SubMenu->nField[j].nType = FT_NOMAL_PUT;
					if (SubMenu->nField[j + 4].nSHideNomalStart /* == turn */) //      ȭ             ϰ  ִ
					{
						SubMenu->nField[j + 4].nSHideNomalCount = 1;
					}
					character_active = SubMenu->nField[j].nWillDo;
					SCharSource = LoadCh[character_active];
					MP3(SN_SELECT_START);
					goto Label_2;
				}
				case SWD_SELECT_LOCATION:
				{
					;
					StartPosition = SubMenu->nField[j].nWillDo;
					SMenu[MN_MAINSTART_START].nField[0].fCheakFlag = true;
					goto Label_2;
					break;
				}

				case SWD_SMALL_MENU:
					SMenu[SubMenu->nField[j].nWillDo].bActive = true;
					y_MenuFocus = SubMenu->nField[j].nWillDo;
					goto Label_2;

				case SWD_QUIT:
				{
					QuitFlag = SWD_QUIT;
					//MP3( SN_GAMEEND );
					GameEndFree();
					goto Label_2;
				}

				case SWD_OKCANCEL_CANCEL:
					if (SubMenu->nTemp == MN_START_MAP)
					{
						;
						SMenu[MN_START_MAP].nField[9].fCheakFlag = true;
					}
				case SWD_SMALLMENU_END:
					MP3(SN_MENU_CLOSE);
					y_MenuFocus = 0;
					break;
				case SWD_ONLY_OK:
				{
					if (SubMenu->nTemp == MN_SELECT_CHARACTER) // ĳ         ޽   ħ      ϱ
					{
						MP3(SN_MENU_CLOSE);
						y_MenuFocus = 0;
						break;
					}
					/*
					if( SubMenu->nTemp == MN_START_MAP )
					{
						MP3( SN_MENU_OPEN );
						StartPosition = SC_SCHOLIUM;
						SMenu[MN_LODING].bActive = true;
						SendChoiceCharacter( character_active );
						SMenu[SubMenu->nTemp].bActive = false;
						break;
					}*/
				}
				case SWD_OKCANCEL_OK:
				{
					MP3(SN_MENU_CLOSE);
					SubMenu->bActive = false; //020515 lsw
					switch (SubMenu->nTemp)
					{
					case MN_SELECT_NATION:
					{
						if (!SCharSource.nation)
							break;
						if (SCharSource.nation == 100)
						{
							SCharSource.nation = 0;
						}
						else
						{
							SendSelectNation(SCharSource.nation, SCharSource.sCharacterName);
						}

						SMenu[MN_MAKECHARACTER_BASIC].bActive = false;
						SMenu[MN_RESET_JOB_EXPLAIN].bActive = false;
						CallMN_START_MAP_Menu();
						goto Label_2;
					}
					case MN_RESET_JOB:
					{
						////////// ĳ     ʱ ȭ //////////////
						/*int ret;
						if( !SysInfo.notconectserver )
						{
							bool fBreak = false;
							ret = SendCheckNewChar( SCharSource.sCharacterName );
							switch( ret )
							{
								default : CallOkCancelMessageBox( MN_LOGIN, 0,0, lan->OutputMessage(5,71), 0 ); //lsw
											fBreak = true;
								case -4 : CallOkCancelMessageBox( MN_LOGIN, 0,0, lan->OutputMessage(5,72), 0 );//lsw
											fBreak = true;
								case 1 : break;
							}

							if( fBreak )
							{
								SMenu[MN_MAKECHARACTER_BASIC].bActive=false;
								SMenu[MN_RESET_JOB_EXPLAIN].bActive=false;
								SMenu[MN_RESET_JOB].bActive = false;
								::CallLoginMenu();
								break;
							}
						} finito 060507 - removed this check is not needed anymore as there wont be any alpha characters...*/
						SCharSource.nCharacterData[JOB] += 20;
						SendResetJob(SCharSource.nCharacterData[JOB], SCharSource.nCharacterData[CLASS], SCharSource.sCharacterName);

						if (SCharSource.nation)
						{
							SMenu[MN_MAKECHARACTER_BASIC].bActive = false;
							SMenu[MN_RESET_JOB_EXPLAIN].bActive = false;
							CallMN_START_MAP_Menu();
							goto Label_2;
						}
						else
						{
							OpenSelectNationMenu();
							break;
						}
						break;
					}
					case MN_MAKECHARACTER_NAME:
						break;
					case MN_SELECT_CHARACTER:
					{
						CallOkCancelMessageBox(SubMenu->Id, 0, 0, /*193, 75,*/ lan->OutputMessage(5, 87)); //lsw
						SubMenu->nTemp = MN_OKCANCEL_MESSAGEBOX;
						goto Label_2;
					}
					case MN_OKCANCEL_MESSAGEBOX:
					{
						// 020925 YGI
						/*#ifdef CHINA_LOCALIZING_		//   ߱            delete ޴         ʴ´ .
														if(TRUE == SendDeleteCharacter( LoadCh[ character_active ].sCharacterName, "temp" ) )
														{
															memset( &LoadCh[ character_active ], 0, sizeof( SCharacter ) );
														}
														SCharSource = LoadCh[ character_active ];
														break;
										#else
										*///021011 lsw
						CallDelCharComform(); //020802 lsw
						goto Label_2;
						// #endif
					}
					case MN_LOGIN:
						if (SubMenu->key == 100) // Ȩ         ̵ //020822 lsw
						{
							SubMenu->key = 0;
							//const int ret = LaunchApplication_kein("htmlfile", lan->OutputMessage(5, 89)); //lsw
							if (1)
							{
								QuitFlag = SWD_QUIT;
								GameEndFree();
								MP3(SN_WARNING);
								goto Label_2;
							}
						}
						else
						{
							EWndMgr.ClearAllTxt(); //021001 lsw
							id_password = true;
						}
						break;

					case MN_LODING: //SMenu[MN_MAINSTART_BACK].bActive=true;
						// SMenu[MN_LOGIN].bActive=true;//020515 lsw
						::CallLoginMenu();
						SMenu[MN_MAINSTART_BASIC].bActive = true;
						SMenu[MN_SELECT_CHARACTER].bActive = false;
						SMenu[MN_LODING].bActive = false;
						y_MenuFocus = 0;
						break;
					case MN_START_MAP:
					{
						MP3(SN_MENU_OPEN);
						StartPosition = SC_SCHOLIUM;
						SMenu[MN_LODING].bActive = true;
						SendChoiceCharacter(character_active);
						SMenu[SubMenu->nTemp].bActive = false;
						break;
					}
					}
					y_MenuFocus = 0;
					break;
				}
				case SWD_CONNECT: //雙click 選人
				{
					if (!SubMenu->work)
						goto Label_2;

					if (LoadCh[character_active].sCharacterName[0])
					{
						SMenu[SubMenu->nField[j].nWillDo].bActive = true;
						SubMenu->bActive = false;
						SendChoiceCharacter(character_active);
						MP3(SN_MENU_OPEN);
					}
					else
					{
						goto Label_2;
					}
				}
				break;
				case SWD_CONNECT_RECENT://最近離線
				{ //021022 lsw
					if (LoadCh[character_active].sCharacterName[0])
					{
						StartPosition = 99; //  ֱ   α׾ƿ
						SMenu[SubMenu->nField[j].nWillDo].bActive = true;
						SubMenu->bActive = false;
						SendChoiceCharacter(character_active);
						MP3(SN_MENU_OPEN);
					}
				}
				break;

				case SWD_WILLDO_WORK:
				{
					MP3(SN_SELECT_START);
					SubMenu->work = SubMenu->nField[j].nWillDo;
					SubMenu->key = SubMenu->nField[j].x * 1000 + SubMenu->nField[j].y; //  ǥ Ѱ  ٶ
					goto Label_2;
				}
				case SWD_RESET_JOB_EXPLAIN:
				{
					SMenu[MN_RESET_JOB_EXPLAIN].nField[0].fCheakFlag = true;
					goto Label_2;
				}
				case SWD_NEW_ID:
				{
					CallOkCancelMessageBox(SubMenu->Id, 0, 0, lan->OutputMessage(5, 89), false); //020822 lsw
					// SMenu[MN_OKCANCEL_MESSAGEBOX].key = 100;
					SMenu[MN_OK_MESSAGEBOX].key = 100;
					goto Label_2;
				}
				break;
				case SWD_SELECT_TUTORIAL_OK: //020815-2 lsw
				{                            //       ϰڴٴ   Ŷ
					int& iMyTarget = SubMenu->nField[j].nWillDo;
					switch (iMyTarget)
					{
					case -1: //Cancel ޴   willdo   -1      ִ
					{
						if (SCharacterData.nLevel > 1) //   8  ̻    ̵鸸   ޴         ִ //020821 lsw
						{
							SubMenu->bActive = false;
						}
						else //8    ϴ Cancel
						{
							goto Label_2; // ƹ  ϵ    ش // Ŀ    Ǯ   ε  ޴     Ѵ
						}
					}
					break;
					case SC_SCHOLIUM:  // ʺ  ü
					case SC_SCHOLIUM2: // ʺ  ü
					{
						if (LoadCh[character_active].sCharacterName[0]) //ĳ  ̸  ִٸ
						{
							StartPosition = iMyTarget;
							SMenu[MN_LODING].bActive = true;
							SendChoiceCharacter(character_active);
							MP3(SN_MENU_OPEN);
							SMenu[MN_START_MAP].bActive = false; //  ̵     ̽
						}
						else //ĳ      ( ̷    ֳ ??
						{
							goto Label_2; // ̷    µ ..
						}
					}
					break;
					default:
					{
						goto Label_2; // ⺻. ƹ  ͵        Ŭ   ص  ̺ Ʈ   Ͼ
					}
					break;
					}
					y_MenuFocus = 0;           // Ŀ  Ǯ ,
					SubMenu->bActive = false;  // ޴   ݰ
					SubMenu->nField[j].nWillDo = 0; //  ΰ

#ifdef TAIWAN_LOCALIZNG_
					for (int ct = 2; 5 > ct; ct++) // ʵ
					{                              // ޴
						SubMenu->nField[ct].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT; // ư
					}
#else
					for (int ct = 2; 6 > ct; ct++) // ʵ
					{                              // ޴
						SubMenu->nField[ct].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT; // ư
					}
#endif
					goto Label_2;
				}
				break;
				case SWD_SET_LOGIN_TARGET: //020815-2
				{
					//<soto-030915
					const int iMyLoginTarget = SubMenu->nField[j].nWillDo; // α
					SubMenu->nField[1].nWillDo = iMyLoginTarget;         //1

#ifdef TAIWAN_LOCALIZNG_
					for (int ct = 2; 5 > ct; ct++) // ʵ
					{
						if (iMyLoginTarget == SubMenu->nField[ct].nWillDo) //020821 lsw
						{
							SubMenu->nField[ct].nType = FT_NOMAL_PUT; // Ϲ  ׸
						}
						else
						{
							SubMenu->nField[ct].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT; // ư
						}
					}
#else
					for (int ct = 2; 6 > ct; ct++) // ʵ
					{
						if (iMyLoginTarget == SubMenu->nField[ct].nWillDo) //020821 lsw
						{
							SubMenu->nField[ct].nType = FT_NOMAL_PUT; // Ϲ  ׸
						}
						else
						{
							SubMenu->nField[ct].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT; // ư
						}
					}
#endif
					goto Label_2;
					//>soto-030915
				}
				break;
				} // switch{}
				// SWD_        ޴    .       goto
				SubMenu->bActive = FALSE;
			Label_2: //         ʴ ޴    쿡
				SubMenu->nField[j].fCheakFlag = FALSE;
				SubMenu->nField[j].fLButtonDown = FALSE;
				;
			} //if
		}
		break;

		case FT_SELECT_NATION:
		{
			int& selected = SubMenu->work;
			int willdo = SubMenu->nField[j].nWillDo;

			if (selected == willdo)
			{
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
			}
			else
			{
				if (SubMenu->nField[j].fLButtonDown)
				{
					FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
				}
				else if (SubMenu->nField[j].fRectMouse)
				{
					static int tt;
					static int de = 10;

					if (tt)
					{
						de++;
						if (de > 12)
						{
							tt = !tt;
						}
					}
					else
					{
						de--;
						if (de < 3)
						{
							tt = !tt;
						}
					}

					int img_num = SubMenu->nField[j].nImageNumber;
					Spr* s = &spr[img_num];
					PutCompressedImageFX(SubMenu->x + SubMenu->nField[j].x + s->ox, SubMenu->y + SubMenu->nField[j].y + s->oy, s, de, 2);
				}
				if (SubMenu->nField[j].fCheakFlag)
				{
					selected = willdo;
					SubMenu->nField[j].fCheakFlag = false;
					SubMenu->nField[j].fLButtonDown = false;
				}
			}
			break;
		}

		case FT_SELECT_NATION_EXPLAIN:
		{
			;
			int para = 0;
			int ret = true;
			switch (SMenu[MN_SELECT_NATION].work)
			{
			case 100:
			case 0:
				break;

			case N_VYSEUS:
			case N_ZYPERN:
			case N_HEGEMONIA:
			case N_YILSE:
				para = SMenu[MN_SELECT_NATION].work;
				break;
			default:
				ret = false;
				break;
			}
			if (ret)
			{
				int& scroll = SubMenu->work;
				static char explain[7][11][200];
				static WORD color[7][11];

				if (!nation_explain[0][0][0])
				{
					LoadNationExplain();
					SetNationExplainDetect(0, SubMenu->nField[j].nRectImage, explain, color);
				}
				static int old_para = para;
				if (old_para != para)
				{
					scroll = 0;
					old_para = para;
					SetNationExplainDetect(para, SubMenu->nField[j].nRectImage, explain, color);
				}

				for (int i = 0; i < 11; i++)
				{
					if (!i || color[scroll][i] != color[scroll][i - 1])
						Hcolor(color[scroll][i]);
					Hprint2(SubMenu->x + SubMenu->nField[j].x, SubMenu->y + SubMenu->nField[j].y + i * 18, g_DestBackBuf, "%s", EatFrontWhiteChar(explain[scroll][i]));
				}
				if (scroll < 6)
				{
					if (explain[scroll + 1][0][0] == NULL)
						SubMenu->key = 100;
					else
						SubMenu->key = 0;
				}
			}

			break;
		}
		case FT_SELECT_NATION_SCROLL:
		{
			;
			FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, 6, SubMenu->nField[j].nImageType);

			////   ũ   ó

			int block = 0; //   ư      ֱ      ÷
			int& scroll = SubMenu->work;
			int willdo = SubMenu->nField[j].nWillDo;
			if ((scroll == 0 && willdo == SCROLL_UP) ||
				(willdo == SCROLL_DOWN && scroll >= 6) ||
				(willdo == SCROLL_DOWN && SubMenu->key == 100))
				block = 1; // Ʒ  ȭ  ǥ  ư       ʱ

			if (!block) // ȭ  ǥ          ʴ´ .
			{
				/*
				int &delay=SubMenu->nField[j].nSHideNomalStart;
				if( (delay%8) < 4 )
				{
					int x = 217;
					int y;
					if( willdo == SCROLL_UP ) y = 25;
					else y = 223;

					FieldTypeNomalPut(SubMenu->x, SubMenu->y, x, y, SubMenu->nField[j].nSHideNomalCount);
				}
				delay++;*/

				if (SubMenu->nField[j].fLButtonDown)
				{
					FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x + 4, SubMenu->nField[j].y + 5, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
				}
				else if (SubMenu->nField[j].fRectMouse)
				{
					FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x + 4, SubMenu->nField[j].y + 5, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
				}
				else
					FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x + 4, SubMenu->nField[j].y + 5, SubMenu->nField[j].nSpecialWillDo, SubMenu->nField[j].nImageType);

				if (SubMenu->nField[j].fCheakFlag)
				{
					if (willdo == SCROLL_UP)
						scroll--;
					else if (willdo == SCROLL_DOWN)
						scroll++;
				}
			}
			SubMenu->nField[j].fCheakFlag = 0;
			break;
		}

		case FT_RESET_JOB_EXPLAIN:
		{
			;
			int& scroll = SubMenu->work;
			static char explain[5][11][200];
			static WORD color[5][11];
			if (!job_explain[0][0][0])
			{
				LoadJobExplain();
				SetJobExplainDetect(0, SubMenu->nField[j].nRectImage, explain, color);
			}
			int job = SCharSource.nCharacterData[JOB] + 1;
			static int old_job = job;
			if (old_job != job)
			{
				scroll = 0;
				old_job = job;
				SetJobExplainDetect(job, SubMenu->nField[j].nRectImage, explain, color);
			}

			for (int i = 0; i < 11; i++)
			{
				if (!i || color[scroll][i] != color[scroll][i - 1])
					Hcolor(color[scroll][i]);
				Hprint2(SubMenu->x + SubMenu->nField[j].x, SubMenu->y + SubMenu->nField[j].y + i * 18, g_DestBackBuf, "%s", EatFrontWhiteChar(explain[scroll][i]));
			}

			break;
		}
		case FT_RESET_JOB_SCROLL:
		{
			;
			if (SCharSource.nCharacterData[JOB] >= 0)
				break;
			FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->key, SubMenu->nField[j].nImageType);

			////   ũ   ó

			int block = 0; //   ư      ֱ      ÷
			int& scroll = SubMenu->work;
			int willdo = SubMenu->nField[j].nWillDo;
			if ((scroll == 0 && willdo == SCROLL_UP) || //   ȭ  ǥ  ư       ʱ
				(scroll == 2 && willdo == SCROLL_DOWN))
				block = 1; // Ʒ  ȭ  ǥ  ư       ʱ

			if (!block) // ȭ  ǥ          ʴ´ .
			{
				if (SubMenu->nField[j].fLButtonDown)
				{
					FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x + 4, SubMenu->nField[j].y + 5, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType);
				}
				else if (SubMenu->nField[j].fRectMouse)
				{
					FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x + 4, SubMenu->nField[j].y + 5, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType);
				}
				else
					FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x + 4, SubMenu->nField[j].y + 5, SubMenu->nField[j].nSpecialWillDo, SubMenu->nField[j].nImageType);

				if (SubMenu->nField[j].fCheakFlag)
				{
					if (willdo == SCROLL_UP)
						scroll--;
					else if (willdo == SCROLL_DOWN)
						scroll++;
				}
			}
			SubMenu->nField[j].fCheakFlag = 0;
			break;
		}
		case FT_HIDE_ISCROLL_AUTO_PUT:
			if (SubMenu->nField[j].fRectMouse)
			{
				// if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
				if (((SubMenu->nField[SubMenu->nField[j].nWillDo].nSHideNomalStart) + (SubMenu->nField[SubMenu->nField[j].nWillDo].nShideNomalPlus)) >= (SubMenu->nField[SubMenu->nField[j].nWillDo].nSHideNomalCount))
					break;
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage);
			}

		case FT_HIDE_ISCROLL_PUT:
		{
			; //  ũ    ɰ 츦      ̸     üũ    ũ    ϸ  ȵǸ  break Ѵ
			int willdo = SubMenu->nField[j].nWillDo;
			int& ct = SubMenu->nTemp;
			int& turn = SubMenu->work;

			if (SubMenu->nField[j].nSpecialWillDo == 100)
			{
				if (turn)
					ct--;
				else
					ct++;
				if (ct > 14)
					turn = true;
				if (ct < 1)
					turn = false;
			}

			if (((SubMenu->nField[willdo].nSHideNomalStart) + (SubMenu->nField[willdo].nShideNomalPlus)) >= (SubMenu->nField[willdo].nSHideNomalCount))
			{
				SubMenu->nField[j].fCheakFlag = FALSE;
				break;
			}
			if (SubMenu->nField[j].nSpecialWillDo == 100)
			{
				FieldTypeNomalPutFx(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, ct, 2);
			}

			if (SubMenu->nField[j].fLButtonDown)
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
			if (SubMenu->nField[j].fCheakFlag)
			{
				SubMenu->nField[willdo].nSHideNomalStart += SubMenu->nField[willdo].nShideNomalPlus;
				if (SubMenu->nField[willdo].nSHideNomalStart >= (SubMenu->nField[willdo].nSHideNomalCount))
				{
					(SubMenu->nField[willdo].nSHideNomalStart) -= (SubMenu->nField[willdo].nShideNomalPlus);
				}
				else
					MP3(SN_CLICK_START);
				SubMenu->nField[j].fLButtonDown = FALSE;
				SubMenu->nField[j].fCheakFlag = FALSE;
			}
			break;
		}

		case FT_HIDE_DSCROLL_AUTO_PUT:
			if (SubMenu->nField[j].fRectMouse)
			{
				//// if( !bSoundOn ) { MP3( SN_TOUCH ); bSoundOn = true; }
				if (((SubMenu->nField[SubMenu->nField[j].nWillDo].nSHideNomalStart) - (SubMenu->nField[SubMenu->nField[j].nWillDo].nShideNomalPlus)) < 0)
					break;
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage);
			}
		case FT_HIDE_DSCROLL_PUT:
		{
			;
			int willdo = SubMenu->nField[j].nWillDo;
			if (((SubMenu->nField[willdo].nSHideNomalStart) - (SubMenu->nField[willdo].nShideNomalPlus)) < 0)
			{
				SubMenu->nField[j].fCheakFlag = FALSE;
				break;
			}

			if (SubMenu->nField[j].nSpecialWillDo == 100)
			{
				int& ct = SubMenu->nTemp;
				int& turn = SubMenu->work;
				// if( turn ) ct--;
				// else ct++;
				// if( ct > 14 ) turn = true;
				// if( ct < 1 ) turn = false;
				FieldTypeNomalPutFx(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, ct, 2);
			}
			if (SubMenu->nField[j].fLButtonDown)
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
			if (SubMenu->nField[j].fCheakFlag)
			{
				SubMenu->nField[willdo].nSHideNomalStart -= SubMenu->nField[willdo].nShideNomalPlus;
				if (SubMenu->nField[willdo].nSHideNomalStart < 0)
				{
					SubMenu->nField[willdo].nSHideNomalStart = 0;
				}
				else
					MP3(SN_CLICK_START);
				SubMenu->nField[j].fLButtonDown = FALSE;
				SubMenu->nField[j].fCheakFlag = FALSE;
			}
			break;
		}

		case FT_THROWDICE:
		{
			if (!g_GoDice)
				break;
			int num_to_rate[2] = { 1003, 1002 };

			int class_ability[5][2] = { // RecvThrowDice
										{STR, CON},
										{DEX, MOVP},
										{DEX, CON},
										{INT_, WSPS},
										{WIS, WSPS},
			};

			if (SubMenu->nField[j].fCheakFlag)
			{
				SubMenu->nField[j].fLButtonDown = FALSE; //   ̽           ö    Ű  Է

				if (!g_GoDice2) //
				{
					SendThrowDice(SCharSource.nCharacterData[CLASS]);
					g_GoDice2 = true;
					g_GoDiceRecv = false;
				}

				if (!g_GoDiceRecv) //               Ŭ   ̾ Ʈ         ϰ  ֻ
				{
					for (i = 0; i < 2; i++)
					{
						int para = class_ability[SCharSource.nCharacterData[CLASS]][i];
						// if( para == -1 ) break;
						SCharSource.nAbilityMax[para] = GetDiceAbility(num_to_rate[i]);
						MP3(SN_DICE);
					}
					break;
				}

				cDiceCount++; //   ̽     ʴ         ߰  ǹǷ  װ    ϴ ī  Ʈ
				if (!(cDiceCount % 4))
					PutDiceNumber++;

				if (PutDiceNumber >= 7) // ֻ   5          ?-> delay   7  ..
				{
					//    // 010216 lsw  ּ  ó
					// char str[]= "    ġ   ĳ  Ͱ  ó      ` Ǵ  ġ   Դϴ . ` `    ġ         ϸ  'YES'  Ŭ  ` ؼ  ĳ  ͸  ⸦  Ϸ  ϼ  . `        ʴ´ٸ  'NO'  `Ŭ  ؼ  ٽ  ֻ
					SMenu[MN_MAKECHARACTER_ABILITY].bActive = FALSE;
					CallMessageBox(TRUE, 34, 110, lan->OutputMessage(6, 9), TRUE); //lsw

					SubMenu->nField[j].fCheakFlag = FALSE;
					cDiceCount = 0;
					PutDiceNumber = 0;
				}
			}
			if (SubMenu->nField[j].fLButtonDown)
			{
				PutDiceNumber = 0;
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);
				//   ̽
				for (i = 0; i < 2; i++)
				{
					int para = class_ability[SCharSource.nCharacterData[CLASS]][i];
					// if( para == -1 ) break;
					SCharSource.nAbilityMax[para] = GetDiceAbility(num_to_rate[i]);
					MP3(SN_DICE);
				}
			}
			break;
		}

		case FT_HIDE_PUT_CENTER:
			if (SubMenu->nField[j].fLButtonDown)
			{
				static int tt;
				static int de = 10;

				if (tt)
				{
					de++;
					if (de > 12)
					{
						tt = !tt;
					}
				}
				else
				{
					de--;
					if (de < 3)
					{
						tt = !tt;
					}
				}

				Spr* s = &spr[SubMenu->nField[j].nImageNumber];
				lenght = (spr[SubMenu->nImageNumber].xl - s->xl) / 2;

				PutCompressedImageFX(SubMenu->x + lenght + s->ox, SubMenu->y + SubMenu->nField[j].y + s->oy, s, de, 2);
			}
			break;

		case FT_SELECT_CITY:
		{
			/*for (int a = j; a < SubMenu->nFieldCount; a++)
			{
				if (SubMenu->nField[a].nType == FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2)
				{
					Spr* s = &start_map_spr[29]; // ȭ  ǥ
					PutCompressedImage(SubMenu->x + SubMenu->nField[a].rCheakBox.left, SubMenu->y + SubMenu->nField[a].rCheakBox.top, s);
				}
			}*/

			//      ̸

			static int return_flag = false;
			static int esc_flag = false;

			int field_num; //      Ʈ    ư      Ʈ  ȣ
			if (SubMenu->work)
				field_num = 0;
			else
				field_num = 1;

			if (!y_MenuFocus || y_MenuFocus == SubMenu->Id) //    Ű  Է  ó
			{
				if (isKeyDown(SDL_SCANCODE_RETURN))
				{
					SubMenu->nField[field_num].fLButtonDown = true;
					return_flag = true;
					// DoButtonCheckOfMenu( SubMenu->Id, 1 );        // recent connect
				}
				else if (return_flag)
				{
					SubMenu->nField[field_num].fCheakFlag = true;
					return_flag = false;
				}
				else if (isKeyDown(SDL_SCANCODE_ESCAPE))
				{
					SubMenu->nField[9].fLButtonDown = true;
					esc_flag = true;

					// DoButtonCheckOfMenu( SubMenu->Id, 9 );        // 3   ĳ          ư
				}
				else if (esc_flag)
				{
					SubMenu->nField[9].fCheakFlag = true;
					esc_flag = false;
				}
			}

			if (!SubMenu->work)
				break;

			static const int image[][4] = {
				// {ū    ̸ ,    ׸ ,      ̸ ,     ̹            ɼ    ִ }
				{1, 7, 17, 1}, // SC_HELTERNT_CASTLE
				{1, 7, 17, 1}, // SC_HELTERNT_VILLAGE
				{1, 0, 0, 0},  // SC_HUDAIN_RIVER
				{1, 0, 0, 0},  // SC_GRAY_MOUNTAIN
				{1, 0, 0, 0},  // SC_CALAHIL
				{1, 0, 0, 1},  // SC_BAISUS_IMPEL
				{2, 9, 19, 1}, // SC_RENUS
				{2, 0, 0, 1},  // SC_DEL_HAPPA
				{2, 11, 21, 1}, // SC_IRAMUS
				{2, 12, 22, 1}, // SC_DIPH_UBEN
				{0, 0, 0, 1},  // SC_SCHOLIUM
				{2, 10, 28, 1}, // SC_TYREN
				{3, 5, 6, 1},  // SC_BARANTAN
			};

			int image_a = image[SubMenu->work - 1][0];
			int image_b = image[SubMenu->work - 1][1];
			int image_c = image[SubMenu->work - 1][2];
			int offset = image[SubMenu->work - 1][3];

			// if( image_a ) //      ÷ //020515 lsw
			// FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, image_a, SubMenu->nField[j].nImageType);

			int gabx = SubMenu->key / 1000;
			int gaby = SubMenu->key % 1000;

			if (image_b)
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, gabx, gaby, image_b, SubMenu->nField[j].nImageType);
			if (image_c)
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, gabx + 20 * offset, gaby + 20 * offset, image_c, SubMenu->nField[j].nImageType);

			StartPosition = SubMenu->work;
			break;
		}

		case FT_TEXT_PUT: //      ڿ
			FieldTypeTextPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].temp);
			break;

		case FT_TEXT_PUT_CENTER: //   ڿ   ߰  κп
		{
			int not_english = 0;
			char* str = SubMenu->nField[j].temp;
			for (int a = 0; a < strlen(str); a++)
			{
				if (str[a] != ' ' && (str[a] > 'Z' || str[a] < 'A'))
				{
					not_english = 1;
					break;
				}
			}
			if (not_english)
			{
				SetHangulAlign(TA_CENTER);
				Hprint2(SubMenu->x + spr[SubMenu->nImageNumber].xl / 2, SubMenu->y + SubMenu->nField[j].y, NULL, "%s", str);
				break;
			}

			i = 0;
			LenghtTotal = 0;

			//   ڿ   ü   ̸
			while ((SubMenu->nField[j].temp[i]) != '\0')
			{
				code = SubMenu->nField[j].temp[i];
				if (code == 32)
					LenghtTotal += 4;
				else
				{
					code -= 27;
					LenghtTotal += spr[code].xl;
				}
				i++;
			}

			//  ߰        ϱ   ǥ
			lenght = (spr[SubMenu->nImageNumber].xl - LenghtTotal) / 2;

			FieldTypeTextPut(SubMenu->x, SubMenu->y, lenght, SubMenu->nField[j].y, SubMenu->nField[j].temp);
			break;
		}
		// ޴        ͸          ؾ  Ұ
		case FT_DATA_PUT:
			flag = TRUE; //  ÷        -ĳ  ̸        ƹ  ͵        ʴ´ .
			switch (SubMenu->nField[j].nImageNumber)
			{
			case DP_HEALING_EXPLAIN:
			{
				break; // This break makes the following code unreachable
				if (!GetSysInfo(SI_GAME_MAKE_MODE))
					break;
				static int index = 1;
				static int a = 1;

				// if (g_aCurrentKeys[DIK_1] & 0x80) index = 1;
				// if(g_aCurrentKeys[DIK_2] & 0x80) index = 4;
				// if(g_aCurrentKeys[DIK_3] & 0x80) index = 5;

				// if(g_aCurrentKeys[DIK_PGUP] & 0x80) a--;
				// if(g_aCurrentKeys[DIK_PGDN] & 0x80) a++;

				if (!ch_armor_image_after_b[index][a].img)
					a = 1;
				PutCompressedImageCharRGB(350 + 65, 100 + 96, &ch_body_image_after_b[index][a], 200);
				PutCompressedImage(350 + 65, 100 + 96, &ch_armor_image_after_b[index][a]);
				PutCompressedImageCharRGB(350 + 65, 100 + 96, &ch_line_image_after_b[index][a], 0);

				PutCompressedImageCharRGB(350 + 65, 100 + 96, &ch_body_image_after[index][a], 200);
				PutCompressedImage(350 + 65, 100 + 96, &ch_armor_image_after[index][a]);
				PutCompressedImageCharRGB(350 + 65, 100 + 96, &ch_line_image_after[index][a], 0);
				break;
			}

			case DP_FACEIMAGE: //   ̹
				if (SCharSource.nCharacterData[FACE] >= 0) //FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SCharSource.nCharacterData[FACE]);
				{
					if (SubMenu->Id == MN_MAKECHARACTER_FACE)
						PutCharImage(SubMenu->x + SubMenu->nField[j].x, SubMenu->y + SubMenu->nField[j].y, SCharSource.nCharacterData[FACE], 0, FS_BIG);
					else
						PutCharImage(SubMenu->x + SubMenu->nField[j].x, SubMenu->y + SubMenu->nField[j].y, SCharSource.nCharacterData[FACE], 0, FS_MIDDLE);
				}
				break;

			case DP_ARIGEMENT:
				Hprint2(SubMenu->x + SubMenu->nField[j].x, SubMenu->y + SubMenu->nField[j].y, g_DestBackBuf, "%s", SHideNomal[HN_MAKECHARACTER_ARIGEMENT_TEXT][SCharSource.nCharacterData[ARIGEMENT]].temp);
				break;
				// ̸
			case DP_NAME:
			{
				Hprint2(SubMenu->x + SubMenu->nField[j].x, SubMenu->y + SubMenu->nField[j].y, g_DestBackBuf, "%s", SCharSource.sCharacterName);
			}
			break;
			// ̸   ĭ
			case DP_NAMEEDITBOX:
			{
				/*SetFocus2(HWND_3); //021001 lsw
				EWndMgr.GetTxt(HWND_3, SCharSource.sCharacterName, 17); //021001 lsw
				if (strlen(SCharSource.sCharacterName) * 8 < 119) //     Ŀ     119     Ʈ  ڽ
				{
					nCursor_Flicker++;
					if ((nCursor_Flicker %= 17) < 8)
					{
						Hprint2(SubMenu->x + SubMenu->nField[j].x, SubMenu->y + SubMenu->nField[j].y, g_DestBackBuf, "%s_", SCharSource.sCharacterName);
						break;
					}
				}
				Hprint2(SubMenu->x + SubMenu->nField[j].x, SubMenu->y + SubMenu->nField[j].y, g_DestBackBuf, "%s", SCharSource.sCharacterName);*/
			}
			break;

			case DP_IDEDITBOX:
			{
				if (!EWndMgr.checkInputBoxVisibility(HWND_1)) EWndMgr.showInputBox(HWND_1);
				/*EWndMgr.GetTxt(HWND_1, sId, 15);
				if( id_password && EWndMgr.IsFocus(HWND_1))
				{
					//SetFocus2(HWND_1);//021001 lsw
				}

				if(id_password)
				{
					if(strlen(sId)*8 < 120) //     Ŀ     90     Ʈ  ڽ
					{
						nCursor_Flicker++;
						if((nCursor_Flicker%=32) < 16)
						{
							//Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%s_", sId );
							break;
						}
					}
				}*/
				// Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%s", sId );
			}
			break;

			case DP_PASSEDITBOX:
			{
				if (!EWndMgr.checkInputBoxVisibility(HWND_2)) EWndMgr.showInputBox(HWND_2);
				/*EWndMgr.GetTxt(HWND_2, sPassword, 15);

				for(k=0; k < (int)strlen(sPassword); k++)
				{
					sPassword_temp[k]='*'; sPassword_temp[k+1] = NULL;
				}
				Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%s", sPassword_temp);

				if(id_password)
				{
					SetFocus2(HWND_1);//021001 lsw
				}
				else
				{
					SetFocus2(HWND_2);//021001 lsw
					if(strlen(sPassword)*8 < 120) // 90     Ʈ  ڽ
					{
						nCursor_Flicker++;
						if((nCursor_Flicker%=32) < 16)
						{
							Hprint2( SubMenu->x+SubMenu->nField[j].x, SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf,"%s_", sPassword_temp);
							break;
						}
					}
				}	*/
			}
			break;

			case DP_AGE_MAKE:
				if (SubMenu->nField[j].nSHideNomalStart < 15)
					SubMenu->nField[j].nSHideNomalStart = 15;
				SCharSource.age = SubMenu->nField[j].nSHideNomalStart;
			case DP_AGE:
				if (SCharSource.age > 14)
				{
					SetHangulAlign(TA_RIGHT);
					Hprint2(SubMenu->x + SubMenu->nField[j].x + SubMenu->nField[j].nWillDo, SubMenu->y + SubMenu->nField[j].y, g_DestBackBuf, "%d", SCharSource.age);
				}
				break;

				//Ŭ
			case DP_CLASS:
				SetHangulAlign(TA_RIGHT);
				Hprint2(SubMenu->x + SubMenu->nField[j].x + SubMenu->nField[j].nRectImage, SubMenu->y + SubMenu->nField[j].y, g_DestBackBuf, "%s", SHideNomal[HN_MAKECHARACTER_CLASS_TEXT][SCharSource.nCharacterData[CLASS]].temp);
				break;
				//
			case DP_SPELL:
				SetHangulAlign(TA_RIGHT);
				if (SCharSource.nCharacterData[SPELL] == 0)
				{
					Hprint2(SubMenu->x + SubMenu->nField[j].x + SubMenu->nField[j].nRectImage, SubMenu->y + SubMenu->nField[j].y, g_DestBackBuf, "WIZARD");
				}
				else if (SCharSource.nCharacterData[SPELL] == 1)
				{
					Hprint2(SubMenu->x + SubMenu->nField[j].x + SubMenu->nField[j].nRectImage, SubMenu->y + SubMenu->nField[j].y, g_DestBackBuf, "PRIEST");
				}
				break;
				//ƽ
			case DP_TACTICS:
				SetHangulAlign(TA_RIGHT);
				if (SCharSource.nCharacterData[GENDER])
				{
					Hprint2(SubMenu->x + SubMenu->nField[j].x + SubMenu->nField[j].nRectImage, SubMenu->y + SubMenu->nField[j].y, g_DestBackBuf, "%s", SHideNomal[HN_MAKECHARACTER_TACTICS_MAN_TEXT][SCharSource.nCharacterData[TACTICS_MAN]].temp);
				}
				else
				{
					Hprint2(SubMenu->x + SubMenu->nField[j].x + SubMenu->nField[j].nRectImage, SubMenu->y + SubMenu->nField[j].y, g_DestBackBuf, "%s", SHideNomal[HN_MAKECHARACTER_TACTICS_WOMAN_TEXT][SCharSource.nCharacterData[TACTICS_WOMAN]].temp);
				}
				break;
				//
			case DP_JOB:
			{
				SetHangulAlign(TA_RIGHT);
				int job = SCharSource.nCharacterData[JOB];
				if (job >= 20)
					job -= 20;
				Hprint2(SubMenu->x + SubMenu->nField[j].x + SubMenu->nField[j].nRectImage, SubMenu->y + SubMenu->nField[j].y, g_DestBackBuf, "%s", SHideNomal[HN_MAKECHARACTER_JOB_TEXT][job].temp);
				break;
			}
			case DP_STR:
			case DP_CON:
			case DP_DEX:
			case DP_WIS:
			case DP_INT:
			case DP_MOVP:
			case DP_CHA:
			case DP_ENDU:
			case DP_MOR:
			case DP_WSPS:
			case DP_LUCK:
			{
				int result = SCharSource.nCharacterAbility[SubMenu->nField[j].nWillDo] + SCharSource.nAbilityMax[SubMenu->nField[j].nWillDo];
				sprintf(y_temp, "%d", result);
				if (SCharSource.nAbilityMax[SubMenu->nField[j].nWillDo])
				{
					sprintf(y_temp, "%d + %d", SCharSource.nCharacterAbility[SubMenu->nField[j].nWillDo], SCharSource.nAbilityMax[SubMenu->nField[j].nWillDo]);
					Hprint2(SubMenu->x + SubMenu->nField[j].x + (58 - strlen(y_temp) * FONTSIZE_), SubMenu->y + SubMenu->nField[j].y, g_DestBackBuf, "%d +", SCharSource.nCharacterAbility[SubMenu->nField[j].nWillDo]);

					Hcolor(FONT_COLOR_PLUS);
					sprintf(y_temp, "%d", SCharSource.nAbilityMax[SubMenu->nField[j].nWillDo]);
					Hprint2(SubMenu->x + SubMenu->nField[j].x + (58 - strlen(y_temp) * FONTSIZE_), SubMenu->y + SubMenu->nField[j].y, g_DestBackBuf, "%s", y_temp);
				}
				else
				{
					Hcolor(FONT_COLOR_NUMBER);
					Hprint2(SubMenu->x + SubMenu->nField[j].x + (58 - strlen(y_temp) * FONTSIZE_), SubMenu->y + SubMenu->nField[j].y, g_DestBackBuf, "%s", y_temp);
				}
				break;
			}

			case DP_LEVEL_LOGON:
				Hprint2(SubMenu->x + SubMenu->nField[j].x, SubMenu->y + SubMenu->nField[j].y, g_DestBackBuf, "%d", SCharSource.nLevel); //
				break;

			case DP_RECT_STRING_PUT:
				RectTextPut(SubMenu->x + SubMenu->nField[j].x, SubMenu->y + SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].temp);
				break;

			case DP_NAME_SELECT:
			{ // hometown  nation   ٲٱ
				// FieldTypeNomalPut( SubMenu->x, SubMenu->y, SubMenu->nField[j].x-9,SubMenu->nField[j].y+118, 0, START_ETC2 ); //020515 lsw
				//020515 lsw
				if (!SCharSource.sCharacterName[0]) // ׸  ٲٱ
				{
					SMenu[MN_SELECT_CHARACTER].nField[2].nType = FT_NOMAL_PUT;
					SMenu[MN_SELECT_CHARACTER].nField[3].nRectImage = 12;
					SMenu[MN_SELECT_CHARACTER].nField[3].nImageNumber = 13;
				}
				else
				{
					SMenu[MN_SELECT_CHARACTER].nField[2].nType = FT_NO_CHECK;
					SMenu[MN_SELECT_CHARACTER].nField[3].nRectImage = 5;
					SMenu[MN_SELECT_CHARACTER].nField[3].nImageNumber = 2;
				}

				if (!LoadCh[SubMenu->nField[j].nWillDo].sCharacterName[0])
					break;
				const int para = SubMenu->nField[j].nWillDo; //   ĳ
				if (para == character_active)                 //     ϸ  ̼
				{
					const int first_para = LoadCh[para].nCharacterData[GENDER];   //   Ʈ  ε        	//
					const int second_para = LoadCh[para].nCharacterData[CLASS]; //   Ŭ
					const DWORD body = ReturnBlendRGB(LoadCh[para].body_r, LoadCh[para].body_g, LoadCh[para].body_b);
					const DWORD line = ReturnBlendRGB(LoadCh[para].cloth_r, LoadCh[para].cloth_g, LoadCh[para].cloth_b);
					PutTestAnimation(first_para, second_para, body, line, g_FrameMgr.GetDeltaTime());
				}
				int gaby = 0;
				int job = LoadCh[SubMenu->nField[j].nWillDo].nCharacterData[JOB];
				if (job >= 20)
					job -= 20;
				Hprint2(SubMenu->x + SubMenu->nField[j].x + 245, SubMenu->y + SubMenu->nField[j].y + gaby+65, g_DestBackBuf, "%s", LoadCh[SubMenu->nField[j].nWillDo].sCharacterName);
				Hprint2(SubMenu->x + SubMenu->nField[j].x + 245, SubMenu->y + SubMenu->nField[j].y + gaby + 35+65, g_DestBackBuf, "%s", SHideNomal[HN_MAKECHARACTER_JOB_TEXT][job].temp);
				Hprint2(SubMenu->x + SubMenu->nField[j].x + 52 + 245, SubMenu->y + SubMenu->nField[j].y + gaby + 57+65, g_DestBackBuf, "%s", SHideNomal[HN_MAKECHARACTER_CLASS_TEXT][LoadCh[SubMenu->nField[j].nWillDo].nCharacterData[CLASS]].temp);
				Hprint2(SubMenu->x + SubMenu->nField[j].x + 52 + 245, SubMenu->y + SubMenu->nField[j].y + gaby + 78+65, g_DestBackBuf, "%d", LoadCh[SubMenu->nField[j].nWillDo].age);
				Hprint2(SubMenu->x + SubMenu->nField[j].x + 52 + 245, SubMenu->y + SubMenu->nField[j].y + gaby + 101+65, g_DestBackBuf, "%d", LoadCh[SubMenu->nField[j].nWillDo].nLevel);
				Hprint2(SubMenu->x + SubMenu->nField[j].x + 245, SubMenu->y + SubMenu->nField[j].y + gaby + 140+65, g_DestBackBuf, "%s", NationName[LoadCh[SubMenu->nField[j].nWillDo].nation]); //    ̸
				Hprint2(SubMenu->x + SubMenu->nField[j].x + 245, SubMenu->y + SubMenu->nField[j].y + gaby + 163+65, g_DestBackBuf, "%d", LoadCh[SubMenu->nField[j].nWillDo].nMoney);
				break;
			}
			case DP_BACK_IMAGE_VALUE_TEXT:
			{
				;
				PutCompressedImageFX(SubMenu->x + SubMenu->nField[j].x, SubMenu->y + SubMenu->nField[j].y + spr[71].oy - 3, &spr[71], 15, 1);
				SetHangulAlign(TA_CENTER);
				Hcolor(FONT_COLOR_NAME);
				Hprint2(SubMenu->x + SubMenu->nField[j].x, SubMenu->y + SubMenu->nField[j].y + 3, g_DestBackBuf, "%s", select_server_name);
				break;
			}
			}
			break;

		case FT_COLOR_GAUGE:
		{
			int select = 2;
			for (k = 0; k < 2; k++)
			{
				if (SHideNomal[HN_MAKECHARACTER_COLOR_SELECT][k].bShow)
					select = k;
			}
			if (select == 2)
				break;
			int start, end;
			int gab = 0;

			switch (j - 7) // field   ȣ   7
			{
			case 0:
				switch (select)
				{
				case 0:
					gab = SCharSource.body_r;
					break;
				case 1:
					gab = SCharSource.cloth_r;
					break;
				}
				break;
			case 1:
				switch (select)
				{
				case 0:
					gab = SCharSource.body_g;
					break;
				case 1:
					gab = SCharSource.cloth_g;
					break;
				}
				break;
			case 2:
				switch (select)
				{
				case 0:
					gab = SCharSource.body_b;
					break;
				case 1:
					gab = SCharSource.cloth_b;
					break;
				}
				break;
			}

			gab = (int)(gab / 255.0f * 83.0f + 0.5f);
			end = gab;
			start = SubMenu->nField[j].x;

			if (SubMenu->nField[j].fLButtonDown)
				end = g_pointMouseX - (SubMenu->x + start);
			if (end < 0)
			{
				gab--;
				if (gab < 0)
					gab = 0;
			}
			else if (end > 83)
			{
				gab++;
				if (gab > 83)
					gab = 83;
			}
			else
				gab = end;

			for (int x = start; x < gab + start; x++)
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber);

			gab = (int)(gab / 83.0f * 255.0f + 0.5);
			switch (j - 7) // field   ȣ   7
			{
			case 0:
				switch (select)
				{
				case 0:
					SCharSource.body_r = gab;
					break;
				case 1:
					SCharSource.cloth_r = gab;
					break;
				}
				break;
			case 1:
				switch (select)
				{
				case 0:
					SCharSource.body_g = gab;
					break;
				case 1:
					SCharSource.cloth_g = gab;
					break;
				}
				break;
			case 2:
				switch (select)
				{
				case 0:
					SCharSource.body_b = gab;
					break;
				case 1:
					SCharSource.cloth_b = gab;
					break;
				}
				break;
			}
			break;
		}
		// 0906 kkh     κ .. \
        //  ̺κ    ׸            ī  Ʈ    ÷    ׸    Ͱ     κп    Ѱ  ش .
		case FT_SELECT_FACE:
		{
			int tempi = 1;
			int a = 0;
			int& num = SubMenu->nField[j].nSHideNomalStart;
			int& count = SubMenu->nField[j].nSHideNomalCount;

			if (SCharSource.nCharacterData[GENDER])
			{
				for (a = 0; a < FACE_MAX; a++) //      ȭ    Ѿ
					if (!spr_face_man_b[a].img)
					{
						// 0906 KKH ߰
						for (tempi = 1; a < ADD_CHA_PIC; tempi++)
						{
							if (!spr_face_man_b_dash[tempi].img)
							{
								count = a + tempi;
								break;
							}
						}
					}
				if (!spr_face_man_b[num].img)
				{
					if (!spr_face_man_b_dash[num - 8].img)
						num--;
				}
			}
			else
			{
				for (a = 0; a < FACE_MAX; a++)
					if (!spr_face_woman_b[a].img)
					{
						// 0906 KKH ߰
						for (tempi = 1; a < ADD_CHA_PIC; tempi++)
						{
							if (!spr_face_woman_b_dash[tempi].img)
							{
								count = a + tempi;
								break;
							}
						}
					}
				if (!spr_face_woman_b[num].img)
				{
					if (!spr_face_woman_b_dash[num - 7].img)
						num--;
				}
			}

			// 0907 kkh     ĳ  ͸        ĳ  ͸        ׸
			//acer4
			if (num >= count - 1)
			{
				num = count - 2;
			}

			SCharSource.nCharacterData[FACE] = num;
			break;
		}

		case FT_DO: //           ؾ  Ҷ ,  Ѹ      ߳      ̴   , case   ߺ     ؾ  .
			switch (SubMenu->nField[j].nWillDo)
			{
			case MN_MAINSTART_BASIC:
			{
				SMenu[MN_MAINSTART_BASIC].bActive = TRUE;
				SubMenu->nField[j].nType = FT_NO_CHECK; //  Ÿ  Ʋ

				break;

				static int turn = 0;
				static unsigned short int gab = 8;

				static int go_flag = 0;
				static bool end_flag = true;
				static bool flag2 = false;
				static int flag = 0;

				Spr* s1 = &spr[SubMenu->nField[j].nImageNumber];
				Spr* s2 = &spr[SubMenu->nField[j].nRectImage];
				int lion[] = { 305, 306, 307, 308, 309, 308, 307, 306 };
				Spr* s3 = &spr[lion[flag % 8]];

				UpperMenuNomalPut(i, SMenu[MN_MAINSTART_BASIC].x, SMenu[MN_MAINSTART_BASIC].y, SMenu[MN_MAINSTART_BASIC].nImageNumber, 0);
				// if( turn > 2 )        // ׸
				{
					PutCompressedImageFX(SubMenu->x + 0 - turn * gab + s1->xl /*+s2->ox*/, SubMenu->y + 0 + s2->oy, s2, 8, 3);
					PutCompressedImageFX(SubMenu->x + SubMenu->nField[j].x + s2->ox + turn * gab - s2->ox, SubMenu->y + SubMenu->nField[j].y + s2->oy, s2, 8, 3);
				}
				PutCompressedImage(SubMenu->x + 0 - turn * gab + s1->ox, SubMenu->y + 0 + s1->oy, s1);
				PutCompressedImage(SubMenu->x + 0 - turn * gab + 94 + s3->ox, SubMenu->y + 0 + 187 + s3->oy, s3);

				PutCompressedImage(SubMenu->x + SubMenu->nField[j].x + s1->ox + turn * gab, SubMenu->y + SubMenu->nField[j].y + s1->oy, s1);
				PutCompressedImage(SubMenu->x + SubMenu->nField[j].x + turn * gab + 94 + s3->ox, SubMenu->y + SubMenu->nField[j].y + 187 + s3->oy, s3);

				if (turn * gab >= 320)
					go_flag = 2;
				if (go_flag == 2 && end_flag)
				{
					SMenu[MN_MAINSTART_BASIC].bActive = TRUE;
					SubMenu->nField[j].nType = FT_NO_CHECK; //  Ÿ  Ʋ
					go_flag = 0;
					turn = 0;
					gab = 8;
					end_flag = true;
					flag2 = false;
					flag = 0;
				}
				if (LButtonDown)
					go_flag = 1;
				if (go_flag == 1 && !LButtonDown)
					go_flag = 2;

				if (flag > 2)
					turn++;
				flag++;
				break;
			}
			case DP_ARIGEMENT:
				if (SCharSource.nCharacterData[ARIGEMENT] == -1)
					break;
				else
				{
					FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber + SCharSource.nCharacterData[ARIGEMENT]);
					static int stch = 0;
					static int de = 0;
					FieldTypeNomalPutFx(SubMenu->x + 11, SubMenu->y + 6, SubMenu->nField[j].x, SubMenu->nField[j].y, 801 + SCharSource.nCharacterData[ARIGEMENT], de, 2);
					// switch(stch%2)
					// {
					// case 0: FieldTypeNomalPutFx(SubMenu->x+11, SubMenu->y+4, SubMenu->nField[j].x, SubMenu->nField[j].y, 801+SCharSource.nCharacterData[ARIGEMENT],de, 2);break;
					// case 0: FieldTypeNomalPutFx(SubMenu->x+9, SubMenu->y+5, SubMenu->nField[j].x, SubMenu->nField[j].y, 801+SCharSource.nCharacterData[ARIGEMENT],de, 2);break;
					// case 1: FieldTypeNomalPutFx(SubMenu->x+11, SubMenu->y+6, SubMenu->nField[j].x, SubMenu->nField[j].y, 801+SCharSource.nCharacterData[ARIGEMENT],de, 2);break;
					// }
					if (stch > 15)
						de -= 2;
					else
						de += 2;
					stch++;
					stch %= 32;
				}
				break;

			case DO_CHARACTER:
			{
				int index = 0;
				if (SCharSource.nCharacterData[CLASS] == -1)
					break;
				switch (SCharSource.nCharacterData[GENDER])
				{
				case FEMALE:
					switch (SCharSource.nCharacterData[CLASS])
					{
					case WARRIOR:
						index = 4;
						break;
					case THIEF:
						index = 4;
						break;
					case ARCHER:
						index = 4;
						break;
					case WIZARD:
						index = 5;
						break;
					case PRIEST:
						index = 6;
						break;
					}
					break;
				case MALE:
					switch (SCharSource.nCharacterData[CLASS])
					{
					case WARRIOR:
						index = 1;
						break;
					case THIEF:
						index = 1;
						break;
					case ARCHER:
						index = 1;
						break;
					case WIZARD:
						index = 2;
						break;
					case PRIEST:
						index = 3;
						break;
					}
				}

				static int turn = true;
				static int a = 1;

				DWORD body = ReturnBlendRGB(SCharSource.body_r, SCharSource.body_g, SCharSource.body_b);
				DWORD line = ReturnBlendRGB(SCharSource.cloth_r, SCharSource.cloth_g, SCharSource.cloth_b);

				PutTestAnimation2(SCharSource.nCharacterData[GENDER], SCharSource.nCharacterData[CLASS], body, line, g_FrameMgr.GetDeltaTime());

				/*
				if( SMenu[ MN_MAKECHARACTER_CLASS ].bActive || SMenu[ MN_MAKECHARACTER_COLOR ].bActive )        //   ϸ  ̼
				{
					PutCompressedImageCharRGB( SubMenu->x+ SubMenu->nField[j].x+45, SubMenu->y+SubMenu->nField[j].y+96, &ch_body_image_after[index][a], body);
					PutCompressedImage( SubMenu->x+ SubMenu->nField[j].x+45, SubMenu->y+SubMenu->nField[j].y+96, &ch_armor_image_after[index][a]);
					PutCompressedImageCharRGB( SubMenu->x+ SubMenu->nField[j].x+45, SubMenu->y+SubMenu->nField[j].y+96, &ch_line_image_after[index][a], line);
					a++;
					if( !ch_body_image_after[index][a].img )
					{
						a=1;
						turn = true;
					}
					else turn = false;
				}
				else if( !turn )
				{
					PutCompressedImageCharRGB( SubMenu->x+ SubMenu->nField[j].x+45, SubMenu->y+SubMenu->nField[j].y+96, &ch_body_image_after[index][a], body);
					PutCompressedImage( SubMenu->x+ SubMenu->nField[j].x+45, SubMenu->y+SubMenu->nField[j].y+96, &ch_armor_image_after[index][a]);
					PutCompressedImageCharRGB( SubMenu->x+ SubMenu->nField[j].x+45, SubMenu->y+SubMenu->nField[j].y+96, &ch_line_image_after[index][a], line);
					a++;
					if( !ch_body_image_after[index][a].img )
					{
						a=1;
						turn = true;
					}
					else turn = false;
				}
				else
				{
					static int c=1;
					PutCompressedImageCharRGB( SubMenu->x+ SubMenu->nField[j].x+45, SubMenu->y+SubMenu->nField[j].y+96, &ch_body_image_befor[index][c], body);
					PutCompressedImage( SubMenu->x+ SubMenu->nField[j].x+45, SubMenu->y+SubMenu->nField[j].y+96, &ch_armor_image_befor[index][c]);
					PutCompressedImageCharRGB( SubMenu->x+ SubMenu->nField[j].x+45, SubMenu->y+SubMenu->nField[j].y+96, &ch_line_image_befor[index][c], line);
					c++;
					if( !ch_body_image_befor[index][c].img ) c=1;
					turn = true;
				}
				*/
				break;
			}

			// case DO_SELECT_LOAD_CHAR :    if(SubMenu->nField[j].nSpecialWillDo != SubMenu->nField[j].nSHideNomalStart)
			//                              {
			//                                  SubMenu->nField[j].nSpecialWillDo = SubMenu->nField[j].nSHideNomalStart;
			//                                  SCharSource = LoadCh[ SubMenu->nField[j].nSpecialWillDo ];
			//                              }
			//                              break;

			case FT_DO_DEFAULT: //ó   ΰ  ȭ 鿡
			{                 //020515 lsw
				static int tt = TRUE, pp;
				static int de = 0;
				if (tt)
				{
					if (pp)
						de++;
					pp = !pp;
					if (de > 25)
					{
						tt = !tt;
					}
				}
				else
				{
					if (pp)
						de--;
					pp = !pp;
					if (de < 5)
					{
						tt = !tt;
					}
				}
				FieldTypeNomalPutFx3(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y,
					SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType, de, 1);

				static bool f_LButtonDown = 0;
				if (LButtonDown)
					f_LButtonDown = true;
				if (!LButtonDown && f_LButtonDown)
				{
					tt = true;
					de = 0;
					pp = false;
					f_LButtonDown = false;
					SubMenu->nField[1].fCheakFlag = TRUE;
				}
				break;
			}
			}
			SubMenu->nField[j].fCheakFlag = FALSE;
			SubMenu->nField[j].fLButtonDown = FALSE;
			break;
		case FT_TEST_CHECK:
		{
			;
			int willdo = SubMenu->nField[j].nWillDo;
			if (BetaTest == willdo)
				FieldTypeNomalPut(SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, 0);

			if (SubMenu->nField[j].fCheakFlag)
			{
				BetaTest = willdo;
				MP3(SN_CLICK_START);
				SubMenu->nField[j].fCheakFlag = FALSE;
				SubMenu->nField[j].fLButtonDown = FALSE;
			}
			break;
		}

		case FT_INFO_PUT:
		{
			char* dr_saying = lan->OutputMessage(7, 171);
			RectTextPut(SubMenu->x + SubMenu->nField[j].x, SubMenu->y + SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, dr_saying);
			bool go = false;
			// if(g_aCurrentKeys[DIK_ESCAPE] & 0x80 ) go = true;
			static bool count = false;
			if (LButtonDown)
			{
				count = true;
			}
			else if (count)
				go = true;

			if (go)
			{
				count = false;
				// g_aCurrentKeys[DIK_ESCAPE] = 0;
				SubMenu->bActive = false;
				::CallLoginMenu();
			}
			break;
		}
		case FT_NO_CHECK:
			SubMenu->nField[j].fCheakFlag = FALSE; //   ϴ  üũ  ÷  ׸          ϰ  ƹ  ͵
			SubMenu->nField[j].fLButtonDown = FALSE;
			break;

		case FT_PUT_TACTICS:
		{
			;
			int img_num_man[] = { 46, 50, 51, 40, 48, 47, 42, 43, 41, 44, 45 };
			int img_num_woman[] = { 46, 50, 51, 49, 48, 47, 42, 43, 40, 41 };
			int* img_num;

			if (SCharSource.nCharacterData[SPELL] == WIZARD_SPELL)
			{
				img_num_woman[9] = img_num_man[8] = 54;
			}

			int para = SubMenu->nField[j].nSHideNomalNumber;
			if (para == HN_MAKECHARACTER_TACTICS_MAN_TEXT)
			{
				img_num = img_num_man;
			}
			else
				img_num = img_num_woman;

			int start = SubMenu->nField[j].nSHideNomalStart;
			int plus = SubMenu->nField[j].nShideNomalPlus;
			int max = SubMenu->nField[j].nSHideNomalCount;

			for (int a = start; a < start + plus && a < max; a++)
			{
				Spr* s = GetSprOfMenu(MAIN_ETC, img_num[a]);
				if (!s)
					continue;

				int lenght = (spr[SubMenu->nImageNumber].xl) / 2;
				int y = SHideNomal[para][a].y + 3;
				PutCompressedImage(SubMenu->x + lenght, SubMenu->y + y, s);
			}

			break;
		}

		case FT_HIDE_NOMAL_ONE_PUT:
		{
			;
			int count; //        õ  ׸
			int willdo = SubMenu->nField[j].nSHideNomalNumber;
			int plus = SubMenu->nField[j].nShideNomalPlus;
			int start = SubMenu->nField[j].nSHideNomalStart;
			int max = SubMenu->nField[j].nSHideNomalCount;
			//for( count = start ; count<start+plus; count++ )
			for (count = 0; count < max; count++)
			{
				if (SHideNomal[willdo][count].bShow)
					break;
			}

			// if( count < start+plus ) //010216 lsw
			if (count < max)
			{
				char str[10][15][256] = { 0 };

				for (int i = 0; i < 10; i++)
					for (int j = 0; j < 15; j++)
					{
						sprintf(str[i][j], "%s", lan->OutputMessage(9, i * 15 + j + 31));
					}

				sprintf(str[5][8], lan->OutputMessage(7, 162), SCharSource.nCharacterData[SPELL] == WIZARD_SPELL ? lan->OutputMessage(7, 163) : lan->OutputMessage(7, 164)); //010216 lsw
				sprintf(str[6][9], lan->OutputMessage(7, 162), SCharSource.nCharacterData[SPELL] == WIZARD_SPELL ? lan->OutputMessage(7, 163) : lan->OutputMessage(7, 164)); //010216 lsw

				SubMenu->key++;
				if (SubMenu->key >= 3) //          ġ   ʱ    ؼ
				{
					SubMenu->key = 3;
					char* text = NULL;
					switch (SubMenu->Id)
					{
					case MN_MAKECHARACTER_GENDER:
						text = str[0][count];
						break;
					case MN_MAKECHARACTER_ARIGEMENT:
						text = str[1][count];
						break;
					case MN_MAKECHARACTER_CLASS:
						text = str[2][count];
						break;
					case MN_MAKECHARACTER_SPELL:
						text = str[3][count];
						break;
					case MN_MAKECHARACTER_COLOR:
						text = str[4][count];
						break;
					case MN_MAKECHARACTER_TACTICS_MAN:
						text = str[5][count];
						break;
					case MN_MAKECHARACTER_TACTICS_WOMAN:
						text = str[6][count];
						break;
					case MN_RESET_JOB:
					case MN_MAKECHARACTER_JOB:
						text = str[7][count];
						break;
					default:
						SubMenu->key = 0;
						break;
					}

					static int count_old = count;
					static int menu_old = SubMenu->Id;
					static int scroll = 0;
					if (count_old != count || menu_old != SubMenu->Id)
					{
						count_old = count;
						menu_old = SubMenu->Id;
						scroll = 0;
					}

					if (text)
					{
						int curr_line = scroll / 30;
						int max_line = GetLineRectTextPut(170, text);
						if (!max_line)
							break;
						if (max_line <= 2)
						{
							RectTextPut(SubMenu->x + 5 + 13, SubMenu->y + 315 + 14, 170, text);
						}
						else
						{
							curr_line %= max_line;
							SkipRectTextPut(SubMenu->x + 5 + 13, SubMenu->y + 315 + 14, 170, curr_line, text, 2, 0); //
						}
					}
					else
					{
					}
					scroll++;
				} //  ߰    break;         ȵȴ .
			}
			else
				SubMenu->key = 0;
		}
		case FT_HIDE_NOMAL_GROUP_PUT:
			for (i = SubMenu->nField[j].nSHideNomalStart; i < (SubMenu->nField[j].nSHideNomalStart) + (SubMenu->nField[j].nShideNomalPlus) && i < SubMenu->nField[j].nSHideNomalCount; i++)
			{
				static int bSound = 0;
				int type = i * 100 + j;
				if (SHideNomal[SubMenu->nField[j].nSHideNomalNumber][i].bRect && SHideNomal[SubMenu->nField[j].nSHideNomalNumber][i].nType == FT_NOMAL_PUT_CENTER)
				{
					if (!SHideNomal[SubMenu->nField[j].nSHideNomalNumber][i].bShow)
						if (bSound != type)
						{
							MP3(SN_TOUCH_START);
							bSound = type;
						}

					static int tt;
					static int de = 10;

					if (tt)
					{
						de++;
						if (de > 12)
						{
							tt = !tt;
						}
					}
					else
					{
						de--;
						if (de < 3)
						{
							tt = !tt;
						}
					}

					Spr* s = &spr[SHideNomal[SubMenu->nField[j].nSHideNomalNumber][i].nImageNumber];
					lenght = (spr[SubMenu->nImageNumber].xl - s->xl) / 2;

					PutCompressedImageFX(SubMenu->x + lenght + s->ox, SubMenu->y + SHideNomal[SubMenu->nField[j].nSHideNomalNumber][i].y + s->oy, s, de, 2);
				}
				else
				{
					if (bSound == type)
						bSound = 0;
				}

				if (SHideNomal[SubMenu->nField[j].nSHideNomalNumber][i].bShow || SubMenu->nField[j].nType == FT_HIDE_NOMAL_GROUP_PUT) //ONE   üũ  Ȱ͸
				{
					switch (SHideNomal[SubMenu->nField[j].nSHideNomalNumber][i].nType)
					{
					case FT_TEXT_PUT:
						FieldTypeTextPut(SubMenu->x, SubMenu->y, SHideNomal[SubMenu->nField[j].nSHideNomalNumber][i].x, SHideNomal[SubMenu->nField[j].nSHideNomalNumber][i].y, SHideNomal[SubMenu->nField[j].nSHideNomalNumber][i].temp);
						break;
					case FT_TEXT_PUT_CENTER:
						k = 0;
						LenghtTotal = 0;
						//   ڿ   ü   ̸
						while ((SHideNomal[SubMenu->nField[j].nSHideNomalNumber][i].temp[k]) != '\0')
						{
							code = SHideNomal[SubMenu->nField[j].nSHideNomalNumber][i].temp[k];
							if (code == 32)
								LenghtTotal += 4;
							else
							{
								code -= 27;
								LenghtTotal += spr[code].xl;
							}
							k++;
						}
						//  ߰        ϱ   ǥ
						lenght = (spr[SubMenu->nImageNumber].xl - LenghtTotal) / 2;
						FieldTypeTextPut(SubMenu->x, SubMenu->y, lenght, SHideNomal[SubMenu->nField[j].nSHideNomalNumber][i].y, SHideNomal[SubMenu->nField[j].nSHideNomalNumber][i].temp);
						break;

					case FT_NOMAL_PUT:
						FieldTypeNomalPut(SubMenu->x, SubMenu->y, SHideNomal[SubMenu->nField[j].nSHideNomalNumber][i].x, SHideNomal[SubMenu->nField[j].nSHideNomalNumber][i].y, SHideNomal[SubMenu->nField[j].nSHideNomalNumber][i].nImageNumber);
						break;
					case FT_NOMAL_PUT_CENTER:
						lenght = (spr[SubMenu->nImageNumber].xl - spr[SHideNomal[SubMenu->nField[j].nSHideNomalNumber][i].nImageNumber].xl) / 2;
						FieldTypeNomalPut(SubMenu->x, SubMenu->y, lenght + 1, SHideNomal[SubMenu->nField[j].nSHideNomalNumber][i].y, SHideNomal[SubMenu->nField[j].nSHideNomalNumber][i].nImageNumber);
						break;
					}
				}
			}
			break;
		default: //020515 lsw
		{
			StartMenuSubProcessType2(SubMenu, i, j, k);
		}
		break;
		}
	}
}

#define MAX(x,y)	(x>y)? x : y 


extern bool g_ControlKeyOn;

void start_KeyProc()
{

	static int flag = 0;
	static int flag2 = 0;
	static char flag3 = 0;
	int index;
	int comp = 0;

	int y, k, group = 5;


	if ((isKeyDown(SDL_SCANCODE_LCTRL))
		|| isKeyDown(SDL_SCANCODE_RCTRL))
	{
		g_ControlKeyOn = true;
	}
	else
	{
		g_ControlKeyOn = false;
	}

	if (isKeyDown(SDL_SCANCODE_TAB))
	{
		if (flag != 1) flag = 1;
		else if (flag == 1) return;
	}
	else if (flag == 1) { flag = 0; comp = 1; }

	if (isKeyDown(SDL_SCANCODE_RETURN))
	{
		if (flag != 2) flag = 2;
		else if (flag == 2) return;
	}
	else if (flag == 2) { flag = 0; comp = 2; }

	if (isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		if (flag != 3) flag = 3;
		else if (flag == 3) return;
	}
	else if (flag == 3) { flag = 0; comp = 3; }

	if (!flag && !comp) return;

	for (int x = 0; x < MAX_START; x++)
	{
		index = ClosemenuWhenLogin[x];

		if (EWndMgr.IsFocus(HWND_1) ||//021001 lsw
			EWndMgr.IsFocus(HWND_2) ||
			EWndMgr.IsFocus(HWND_3)) {
			return;
		}

		if (SMenu[index].bActive)
		{
			if (y_MenuFocus && y_MenuFocus != index) continue;
			switch (MAX(flag, comp))
			{
			case 2:	/*����Ű*/
			{
				switch (index)
				{
				case MN_MAKECHARACTER_ABILITY:
					if (comp)
					{
						SMenu[MN_MAKECHARACTER_ABILITY].nField[4].fLButtonDown = FALSE;
						SMenu[MN_MAKECHARACTER_ABILITY].nField[4].fCheakFlag = TRUE;
					}
					else	SMenu[MN_MAKECHARACTER_ABILITY].nField[4].fLButtonDown = TRUE;
					return;

				case MN_YESNO:	if (comp)
				{
					SMenu[MN_YESNO].nField[0].fCheakFlag = TRUE;
					SMenu[MN_YESNO].nField[0].fLButtonDown = FALSE;
				}
							 else SMenu[MN_YESNO].nField[0].fLButtonDown = TRUE;
					return;

				case MN_SYSTEM_LOGOUT: if (comp)
				{
					SMenu[MN_SYSTEM_LOGOUT].nField[0].fCheakFlag = true;
					SMenu[MN_SYSTEM_LOGOUT].nField[0].fLButtonDown = false;
				}
									 else
				{
					SMenu[MN_SYSTEM_LOGOUT].nField[0].fLButtonDown = true;
				}
									 return;

				case MN_OK_MESSAGEBOX:
				case MN_OKCANCEL_MESSAGEBOX:	if (comp)
				{
					SMenu[index].nField[2].fCheakFlag = true;
					SMenu[index].nField[2].fLButtonDown = false;
				}
										   else
				{
					SMenu[index].nField[2].fLButtonDown = true;
				}
										   return;

				case MN_MAINSTART_BASIC:	if (comp)
				{
					SMenu[MN_MAINSTART_BASIC].nField[1].fCheakFlag = TRUE;
					SMenu[MN_MAINSTART_BASIC].nField[1].fLButtonDown = FALSE;
				}
									   else SMenu[MN_MAINSTART_BASIC].nField[1].fLButtonDown = TRUE;
					return;

				case MN_LOGON:
				case MN_LOGIN: if (comp)
				{
					SMenu[index].nField[0].fCheakFlag = TRUE;
					SMenu[index].nField[0].fLButtonDown = FALSE;
				}
							 else SMenu[index].nField[0].fLButtonDown = TRUE;
					return;

				case MN_SELECT: if (comp)
				{
					SMenu[MN_MAINSTART_START].nField[1].fCheakFlag = true;
					SMenu[MN_MAINSTART_START].nField[1].fLButtonDown = false;
				}
							  else SMenu[MN_MAINSTART_START].nField[1].fLButtonDown = true;
					return;

				case MN_LOCATION:	if (comp)
				{
					SMenu[MN_MAINSTART_START].nField[3].fCheakFlag = true;
					SMenu[MN_MAINSTART_START].nField[3].fLButtonDown = false;
				}
								else SMenu[MN_MAINSTART_START].nField[3].fLButtonDown = true;
					return;

				case MN_SELECT_CHARACTER:
				{
					if (comp)
					{
						SMenu[MN_SELECT_CHARACTER].nField[0].fCheakFlag = true;
						SMenu[MN_SELECT_CHARACTER].nField[0].fLButtonDown = false;
					}
					else
					{
						SMenu[MN_SELECT_CHARACTER].nField[0].fLButtonDown = true;
					}
				}return;

				default:
				{
					for (y = 0; y < SMenu[index].nFieldCount; y++)
					{
						if (SMenu[index].nField[y].nImageNumber == 64 || (SMenu[index].nField[y].nImageType == RESET_JOB_ETC && SMenu[index].nField[y].nImageNumber == 10))
						{
							if (comp)
							{
								SMenu[index].nField[y].fCheakFlag = TRUE;
								SMenu[index].nField[y].fLButtonDown = FALSE;
								return;
							}
							SMenu[index].nField[y].fLButtonDown = TRUE;
							return;
						}
					}
				}break;
				}
			}break;
			case 3:	/* ESC key */
			{
				switch (index)
				{
				case MN_YESNO:	if (comp)
				{
					SMenu[MN_YESNO].nField[1].fCheakFlag = TRUE;
					SMenu[MN_YESNO].nField[1].fLButtonDown = FALSE;
				}
							 else SMenu[MN_YESNO].nField[1].fLButtonDown = TRUE;
					return;

					//							case MN_LOGIN :	if(comp)
					//											{
					//												SMenu[MN_LOGIN].nField[2].fCheakFlag=TRUE;
					//												SMenu[MN_LOGIN].nField[2].fLButtonDown=FALSE;
					//											}
					//											else SMenu[MN_LOGIN].nField[2].fLButtonDown=TRUE;
					//											return;
				case MN_SYSTEM_LOGOUT: if (comp)
				{
					SMenu[MN_SYSTEM_LOGOUT].nField[1].fCheakFlag = true;
					SMenu[MN_SYSTEM_LOGOUT].nField[1].fLButtonDown = false;
				}
									 else
				{
					SMenu[MN_SYSTEM_LOGOUT].nField[1].fLButtonDown = true;
				}
									 return;
				case MN_MAINSTART_START:	if (comp)
				{
					SMenu[MN_MAINSTART_START].nField[2].fCheakFlag = TRUE;
					SMenu[MN_MAINSTART_START].nField[2].fLButtonDown = FALSE;
				}
									   else SMenu[MN_MAINSTART_START].nField[2].fLButtonDown = TRUE;
					return;

				case MN_LOGON:	if (comp)
				{
					SMenu[MN_LOGON].nField[1].fCheakFlag = TRUE;
					SMenu[MN_LOGON].nField[1].fLButtonDown = FALSE;
				}
							 else SMenu[MN_LOGON].nField[1].fLButtonDown = TRUE;
					return;

				case MN_INFO:
				{
					SMenu[index].bActive = false;
					::CallLoginMenu();
				}return;

				default:
				{
					if (index == MN_MAKECHARACTER_GENDER)
					{
						break;
					}
					for (y = 0; y < SMenu[index].nFieldCount; y++)
					{
						if (SMenu[index].nField[y].nImageNumber == 65 || SMenu[index].nField[y].nImageNumber == 33 || SMenu[index].nField[y].nImageNumber == 837)
						{
							if (comp)
							{
								SMenu[index].nField[y].fCheakFlag = TRUE;
								SMenu[index].nField[y].fLButtonDown = FALSE;
								return;
							}
							SMenu[index].nField[y].fLButtonDown = TRUE;
							return;
						}
					}
				}break;		// �Ϻη�...
				}
			}break;
			case 1:/* TAB */
			{
				if (flag == 1)
				{
					if (index == MN_SELECT_CHARACTER)
					{
						for (int a = 4; a <= 7; a++)
						{
							if (SMenu[index].nField[a].nType == FT_NOMAL_PUT)
							{
								int temp;
								if (a == 7) temp = 4;
								else temp = a + 1;
								SMenu[index].nField[temp].fCheakFlag = true;
								return;
							}
						}
					}
					if (index == MN_MAKECHARACTER_NAME)
					{
						SMenu[index].nField[10].fCheakFlag = TRUE; // ����Ʈ �ڽ� ����
						return;
					}

					if (index == MN_SELECT_NATION)
					{
						//< kjy-040325
						if (g_GameInfo.preventSelectYilse == 0)
						{
							switch (SMenu[index].work)
							{
							case 0:
							case 100:	SMenu[index].nField[8].fCheakFlag = true; break;

							case N_VYSEUS: SMenu[index].nField[8].fCheakFlag = true; break;
							case N_ZYPERN: SMenu[index].nField[10].fCheakFlag = true; break;
							case N_YILSE: SMenu[index].nField[9].fCheakFlag = true; break;
							}
							return;
						}
						else
						{
							switch (SMenu[index].work)
							{
							case 0:
							case 100:	SMenu[index].nField[9].fCheakFlag = true; break;

							case N_VYSEUS: SMenu[index].nField[9].fCheakFlag = true; break;
							case N_ZYPERN: SMenu[index].nField[10].fCheakFlag = true; break;
								//case N_YILSE : SMenu[index].nField[9].fCheakFlag = true; break;
							}
							return;
						}
						//> kjy-040325 
					}

					for (y = 0; y < SMenu[index].nFieldCount; y++)
					{
						if (SMenu[index].nField[y].nType == FT_HIDE_NOMAL_ONE_PUT)
						{
							for (k = SMenu[index].nField[y].nSHideNomalStart; k < SMenu[index].nField[y].nSHideNomalStart + SMenu[index].nField[y].nShideNomalPlus; k++)
							{
								if (SHideNomal[SMenu[index].nField[y].nSHideNomalNumber][k].bShow)
								{
									group = SMenu[index].nField[y].nWillDo;
									SHideNomal[SMenu[index].nField[y].nSHideNomalNumber][k].bCheak = FALSE;
									k++;
									if (k == SMenu[index].nField[y].nSHideNomalCount)
									{
										SMenu[index].nField[y].nSHideNomalStart = 0;
										SMenu[index].nField[group].nSHideNomalStart = 0;
										k = 0;
									}
									SHideNomal[SMenu[index].nField[y].nSHideNomalNumber][k].bCheak = TRUE;
									if (SMenu[MN_MAKECHARACTER_BASIC].bActive)
										SCharSource.nCharacterData[SMenu[index].nField[y].nSHideNomalNumber] = k;

									if (k == SMenu[index].nField[y].nSHideNomalStart + SMenu[index].nField[y].nShideNomalPlus)
									{
										SMenu[index].nField[y].nSHideNomalStart += SMenu[index].nField[y].nShideNomalPlus;
										SMenu[index].nField[group].nSHideNomalStart += SMenu[index].nField[group].nShideNomalPlus;
									}
									return;
								}
							}
							if (SMenu[MN_MAKECHARACTER_BASIC].bActive)
							{
								SHideNomal[SMenu[index].nField[y].nSHideNomalNumber][SMenu[index].nField[group].nSHideNomalStart].bCheak = TRUE;
								SCharSource.nCharacterData[SMenu[index].nField[y].nSHideNomalNumber] = SMenu[index].nField[group].nSHideNomalStart;
								return;
							}
							break;
						}
					}
				}
			}break;
			}
		}
	}
}


void LoadChImageNumber(char* filename, char** buf, Spr Ani[][80], int max_ani[7])
{
    SDL_RWops* fp = NULL;
    short no;
    unsigned int sprfilelength, size; // DWORD -> unsigned int
    unsigned int nOff = 0;            // DWORD -> unsigned int
    Spr* s;

    // [Android] 1. 路徑處理
    std::string path = filename;
    if (path.find("./") == 0) path = path.substr(2);

    // [Android] 2. 開啟檔案
    fp = SDL_RWFromFile(path.c_str(), "rb");
    if (fp == NULL) {
        //SDL_Log("LoadChImageNumber: Not found %s", path.c_str());
        return;
    }

    // 讀取 Header 中的數量
    SDL_RWread(fp, &no, 2, 1);

    // 跳過 Offset 表
    SDL_RWseek(fp, no * 4, RW_SEEK_CUR);

    // [Android] 3. 計算有效資料長度
    Sint64 totalSize = SDL_RWsize(fp);
    // 原始公式：總長 - (Offset表1) - (Offset表2?) - Header
    sprfilelength = (unsigned int)(totalSize - no * 4 * 2 - 2);

    // 重新分配外部緩衝區
    MemFree(*buf);
    MemAlloc(*buf, sprfilelength);

    // 開始讀取動作序列
    // a: 動作類型 (1~6)
    // b: 該動作的幀數
    for (int a = 1; a < 7; a++)
    {
        for (int b = 1; b <= max_ani[a]; b++)
        {
            s = &Ani[a][b];

            // 讀取區塊大小 (Block Size)
            SDL_RWread(fp, &size, 4, 1);

            if (size == 0)
            {
                s->xl = 0;
                s->yl = 0;
                s->size = 0;
                s->img = NULL;
                continue;
            }

            // 讀取 Sprite 屬性
            SDL_RWread(fp, &(s->xl), sizeof(short), 1);
            SDL_RWread(fp, &(s->yl), sizeof(short), 1);
            SDL_RWread(fp, &(s->ox), sizeof(short), 1);
            SDL_RWread(fp, &(s->oy), sizeof(short), 1);
            SDL_RWread(fp, &(s->size), sizeof(unsigned int), 1); // Spr Data Size

            // [Android] 4. 安全檢查
            // 如果數據超出緩衝區，記錄錯誤並安全退出函式，不要直接 exit(0)
            if (nOff + s->size >= sprfilelength)
            {
                SDL_Log("LoadChImageNumber Error: Buffer Overflow in %s (Ani:%d Frame:%d)", path.c_str(), a, b);
                // JustMsg(lan->OutputMessage(5, 60), filename, a + 1, b);
                SDL_RWclose(fp);
                return;
            }

            // [Android] 5. 讀取圖片數據到緩衝區
            // *buf 是緩衝區起始位置，nOff 是目前的寫入偏移量
            SDL_RWread(fp, *buf + nOff, 1, s->size);

            // 設定 Spr 的 img 指標
            s->img = *buf + nOff;

            convert565to555(s); // 維持像素格式轉換

            // 更新偏移量 (注意：這裡是加 size，即區塊總大小，而非 s->size)
            nOff += size;
        }
    }

    // [Android] 6. 關閉檔案
    SDL_RWclose(fp);

    // 建立 Atlas
    // 注意：這裡傳入原始 filename，因為 CreateAtlas 內部可能也會做路徑處理，或者作為 Key 使用
    g_SDLInfo.CreateAtlasForChImageNumber_CPP(filename, &Ani[0][0], max_ani, 7, 80);
}

void LoadChImage()
{
	int max_ani[2][7] = { { 0,18,18,19,32,22,22 },
							{ 0,46,60,35,30,39,52 }, };

	int max_ani2[7] = { 0,55,60,35,30,39,52 };


	//	LoadChImageNumber( "./character/armor_befor.ani", &ani_buf[0], ch_armor_image_befor, max_ani[0] );
	LoadChImageNumber("./character/armor_after.ani", &ani_buf[1], ch_armor_image_after, max_ani[1]);
	//	LoadChImageNumber( "./character/body_befor.ani" , &ani_buf[2], ch_body_image_befor , max_ani[0] );
	LoadChImageNumber("./character/body_after.ani", &ani_buf[3], ch_body_image_after, max_ani[1]);
	//	LoadChImageNumber( "./character/line_befor.ani" , &ani_buf[4], ch_line_image_befor , max_ani[0] );
	LoadChImageNumber("./character/line_after.ani", &ani_buf[5], ch_line_image_after, max_ani[1]);

	//	LoadChImageNumber( "./character/armor_befor_b.dat", &ani_buf[6], ch_armor_image_befor_b, max_ani[0] );
	LoadChImageNumber("./character/armor_after_b.dat", &ani_buf[7], ch_armor_image_after_b, max_ani2);
	//	LoadChImageNumber( "./character/body_befor_b.dat" , &ani_buf[8], ch_body_image_befor_b , max_ani[0] );
	LoadChImageNumber("./character/body_after_b.dat", &ani_buf[9], ch_body_image_after_b, max_ani2);
	//	LoadChImageNumber( "./character/line_befor_b.dat" , &ani_buf[10], ch_line_image_befor_b , max_ani[0] );
	LoadChImageNumber("./character/line_after_b.dat", &ani_buf[11], ch_line_image_after_b, max_ani2);


}

void FreeChImage()
{
	int i;
	for (i = 0; i < 12; i++)
	{
		MemFree(ani_buf[i]);
	}
}

void MakeCharacterAbility()
{
	g_GoDice = false;
	g_GoDice2 = false;

	CallServer(CMD_CREATE_ABILITY);

	for (int i = 0; i < 5; i++)	nDiceNumber[i] = 1;
	return;
	/*
		for(int i=STR; i<=LUCK; i++)
		{
			SCharSource.nCharacterAbility[i] = 10;
			SCharSource.nCharacterAbility[i] += ran(3)+1;
		}
		for( i=0; i<5; i++ )	nDiceNumber[i] = 1;*/
}


int ImgToAbility(int num_to_img[10], int img)
{
	for (int i = 0; i < 11; i++)
		if (num_to_img[i] == img) return i;

	return 0;
}

void DoLButtonDoubbleClickOfStartMenu(int i, int j)
{
	SMENU* SubMenu = &SMenu[i];
	switch (SMenu[i].nField[j].nType)
	{
	case FT_HIDE_NOMAL_ONE_PUT: {
		for (int y = 0; y < SMenu[i].nFieldCount; y++)
		{
			if (SMenu[i].nField[y].nImageNumber == 64 || (SMenu[i].nField[y].nImageType == RESET_JOB_ETC && SMenu[i].nField[y].nImageNumber == 10))
			{
				SMenu[i].nField[y].fCheakFlag = true;
				break;
			}
		}
		break;
	}
	case FT_SELECT_NATION: {
		SubMenu->work = SubMenu->nField[j].nWillDo;
		SubMenu->nField[4].fCheakFlag = true;
		break;
	}
	case FT_NOMAL_PUT:
	case FT_HIDE_SPECIAL_WILLDO_AUTO_PUT:
		switch (SMenu[i].nField[j].nSpecialWillDo)
		{
		case SWD_CHARACTER_SELECT:
		{
			for (int a = 4; a <= 7; a++)
			{
				SMenu[i].nField[a].nType = FT_HIDE_SPECIAL_WILLDO_AUTO_PUT;
			}
			SMenu[i].nField[j].nType = FT_NOMAL_PUT;
			character_active = SMenu[i].nField[j].nWillDo;
			SCharSource = LoadCh[character_active];

			SMenu[i].nField[0].fCheakFlag = true;	// ok button on
		}break;
		}
		break;
	case FT_HIDE_SPECIAL_WILLDO_AUTO_PUT_2:
		switch (SMenu[i].nField[j].nSpecialWillDo)
		{
		case SWD_WILLDO_WORK:
			if (CheckNationCity(SMenu[i].nField[j].nWillDo) == OK)
			{
				SMenu[i].work = SMenu[i].nField[j].nWillDo;
				SMenu[i].key = SMenu[i].nField[j].x * 1000 + SMenu[i].nField[j].y;		// ��ǥ�Ѱ� �ٶ�
				StartPosition = SMenu[i].work;
				SMenu[i].nField[0].fCheakFlag = true;
			}
			else
			{
				MP3(SN_WARNING);
			}
			break;
		}
		break;

	}
}


void SetDamageOfHero(SCharacter* ch)
{
	int level_to_para[5] = { 10, 20, 40, 70, 90 };
	int level_count[6] = { 7,3,7,5,5,3 };

	int level_dmg[6][7][2] =
	{
		{ {15,2}, {10,3}, {6,5}, {5,6}, {3,10}, {2,15}, {1,30}, },
		{ {7,5}, {5,7}, {1,35}, },
		{ {20,2}, {10,4}, {8,5}, {5,8}, {4,10}, {2,20}, {1,40}, },
		{ {15,3}, {9,5}, {5,9}, {3,15}, {1,45}, },
		{ {25,2}, {10,5}, {5,10}, {2,25}, {1,50}, },
		{ {11,5}, {5,11}, {1,55}, },
	};
	int luck_dmg[6] = { 30, 60, 90, 140, 180, 190 };

	int lv = 5;				// level�� ���� ����� ���� ����� ��
	int i;
	for (i = 0; i < 5; i++)
	{
		if (ch->nLevel <= level_to_para[i])
		{
			lv = i;
			break;
		}
	}

	int luck_count = 7; // �ִ� 7���� ��ȸ�� �ش�.
	for (i = 0; i < 6; i++)
	{
		if (ch->nCharacterAbility[LUCK] < luck_dmg[i])
		{
			luck_count = i + 1;
			break;
		}
	}


	int temp;
	int min = 100;
	for (i = 0; i < luck_count; i++)		// �ִ� ��ȸ�� �༭ ���� ���� ���� �̾Ƴ�
	{
		temp = rand() % level_count[lv];
		if (temp < min) min = temp;
	}

	ch->dmg_count = level_dmg[lv][min][0];		// ���!
	ch->dmg_max = level_dmg[lv][min][1];
}

//extern void LoadManList();
extern void LoadMapInfo(int lang);



void InitStartMenu()
{	// thai2 YGI
#ifdef _DEBUG // finito 060507
	if (GetSysInfo(SI_GAME_MAKE_MODE) && GetSysInfo(SI_NOT_CONNECT_MODE))
	{	//< CSD-030324
		LoadFilterWord();
	}	//> CSD-030324
#endif
	//LoadManList();
	///////////////////////////////////////////////////////
	//RegistProgram();
	///////////////////////////////////////////////////////
	MP3(SN_START, 1);
	g_GoDice = false;
	g_GoDice2 = false;

	ChatMgr.SetViewChat(CHATTYPE_NORMAL, true);//020702 lsw
	ChatMgr.SetViewChat(CHATTYPE_GUILD, true);//021001 lsw
	ChatMgr.SetViewChat(CHATTYPE_PARTY, true);// Finito
	ChatMgr.SetViewChat(CHATTYPE_WHISPER, true);//021001 lsw

	SCharSource1.body_r = SCharSource.body_r = -1;

	LoadChImage();
	// 020701 YGI
	LoadOpenningSpr("./data/openning.dat", &open_spr_buf, open_spr, 75);

	LoadOpenningSpr("./data/face_man_b.dat", &facebuf_man_b, spr_face_man_b, FACE_MAX);			// ĳ���� �� �̹��� �ε� ����,
	LoadOpenningSpr("./data/face_woman_b.dat", &facebuf_woman_b, spr_face_woman_b, FACE_MAX);			// ĳ���� �� �̹��� �ε� ����,
	LoadOpenningSpr("./data/face_man_m.dat", &facebuf_man_m, spr_face_man_m, FACE_MAX);			// ĳ���� �� �̹��� �ε� ����,
	LoadOpenningSpr("./data/face_woman_m.dat", &facebuf_woman_m, spr_face_woman_m, FACE_MAX);			// ĳ���� �� �̹��� �ε� ����,
	//	0906 kkh �߰� 
	LoadOpenningSpr("./data/face_man_b_add.dat", &facebuf_man_b_dash, spr_face_man_b_dash, ADD_CHA_PIC);			// ĳ���� �� �̹��� �ε� ����,
	LoadOpenningSpr("./data/face_woman_b_add.dat", &facebuf_woman_b_dash, spr_face_woman_b_dash, ADD_CHA_PIC);			// ĳ���� �� �̹��� �ε� ����,
	LoadOpenningSpr("./data/face_man_m_add.dat", &facebuf_man_m_dash, spr_face_man_m_dash, ADD_CHA_PIC);			// ĳ���� �� �̹��� �ε� ����,
	LoadOpenningSpr("./data/face_woman_m_add.dat", &facebuf_woman_m_dash, spr_face_woman_m_dash, ADD_CHA_PIC);			// ĳ���� �� �̹��� �ε� ����,

	LoadOpenningSpr("./data/alphabet.dat", &buf_alphabet, spr_alphabet, 36);			// 英文字母圖片

	LoadMenuStr();
	LoadMapInfo(system_info.lang);

#ifdef _DEBUG // finito 060507
	//if (GetSysInfo(SI_GAME_MAKE_MODE))
		//guild_mark_main.LoadBMP("./temp.bmp");
#endif

	if (!LoadMenuImage()) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Failed to load menu image");
	}

}

// 020701 YGI
void LoadOpenningSpr(char* filename, BYTE** buf, Spr* temp_spr, int nMaxCount)
{
    short no;
    unsigned int sprfilelength, size; // DWORD -> unsigned int (32-bit)
    unsigned int nOff = 0;            // DWORD -> unsigned int
    Spr* s;

    // [Android] 1. 路徑處理：移除 ./
    std::string path = filename;
    if (path.find("./") == 0) path = path.substr(2);

    // [Android] 2. 開啟檔案
    SDL_RWops* fp = SDL_RWFromFile(path.c_str(), "rb");
    if (fp == NULL) {
        SDL_Log("LoadOpenningSpr: Not found %s", path.c_str());
        return;
    }

    // 讀取數量
    SDL_RWread(fp, &no, 2, 1);

    // 安全檢查：數量上限
    if (nMaxCount)
    {
        if (no > nMaxCount) {
            SDL_Log("Error Image count [%s] (Read: %d, Max: %d)", path.c_str(), no, nMaxCount);
            // Error("Error Image count [%s]", filename);
            SDL_RWclose(fp);
            return;
        }
    }

    // [Android] 3. 跳過 Offset 表
    // fseek -> SDL_RWseek
    SDL_RWseek(fp, no * 4, RW_SEEK_CUR);

    // [Android] 4. 計算檔案大小
    // _filelength -> SDL_RWsize
    Sint64 totalFileSize = SDL_RWsize(fp);

    // 原始公式：總長度 - (no * 4 * 2) - 2
    // 假設結構是 [Count(2)] + [OffsetTable1] + [OffsetTable2] + [Data]
    sprfilelength = (unsigned int)(totalFileSize - no * 4 * 2 - 2);

    // 記憶體重分配
    // 假設 SAFE_DELETE_ARRAY 是 delete[] 的巨集
    SAFE_DELETE_ARRAY(*buf);

    // 安全檢查：避免分配 0 或過大記憶體
    if (sprfilelength > 0 && sprfilelength < 200 * 1024 * 1024) {
        *buf = new BYTE[sprfilelength];
    } else {
        SDL_Log("LoadOpenningSpr Error: Invalid file length %u in %s", sprfilelength, path.c_str());
        SDL_RWclose(fp);
        return;
    }

    for (int a = 0; a < no; a++)
    {
        s = &temp_spr[a];

        // 讀取區塊大小 (4 bytes)
        SDL_RWread(fp, &size, 4, 1);

        if (size == 0)
        {
            s->xl = 0;
            s->yl = 0;
            s->size = 0;
            s->img = NULL;
            continue;
        }

        // 讀取 Sprite 屬性 (逐一讀取，無對齊問題)
        SDL_RWread(fp, &(s->xl), sizeof(short), 1);
        SDL_RWread(fp, &(s->yl), sizeof(short), 1);
        SDL_RWread(fp, &(s->ox), sizeof(short), 1);
        SDL_RWread(fp, &(s->oy), sizeof(short), 1);
        SDL_RWread(fp, &(s->size), sizeof(unsigned int), 1);

        // [Android] 5. 邊界檢查與錯誤處理
        if (nOff + s->size > sprfilelength) // 改為 > 比較安全，或者 >=
        {
            SDL_Log("LoadOpenningSpr Error: Buffer Overflow in %s. Off:%u + Size:%u > Len:%u",
                    path.c_str(), nOff, s->size, sprfilelength);

            // JustMsg(lan->OutputMessage(6, 60), filename);
            // exit(0); // [禁止] 絕對不能在 Android 上呼叫 exit(0)

            SDL_RWclose(fp);
            return;
        }

        // 讀取圖片數據到外部緩衝區 (*buf)
        SDL_RWread(fp, *buf + nOff, 1, s->size);

        // 設定指標
        s->img = (char*)*buf + nOff;

        convert565to555(s); // 保持格式轉換

        nOff += size;
    }

    // [Android] 6. 關閉檔案
    SDL_RWclose(fp);

    // 建立 Atlas
    // 注意：傳入原始 filename 作為 Key，因為 Texture Cache 可能用它來識別
    g_SDLInfo.CreateAtlasFromLoadedData(filename, temp_spr , no);
}



void FreeOpenningSpr(BYTE** buf)
{
	SAFE_DELETE_ARRAY(*buf);
}


int CalCreateMoney(const SCharacter* s)
{
	switch (s->nCharacterData[SPELL])
	{
	case WARRIOR:	return 1000;
	case THIEF:	return 1000;
	case ARCHER:	return 1000;
	case WIZARD:	return 1000;
	case PRIEST:	return 1000;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////
//
void LoadJobExplain()
{
	FILE* fp;
	fp = Fopen("./data/job_ex.txt", "rt");
	if (fp)
	{
		for (int i = 0; i < 7; i++)
		{
			int j = 0;
			while (j < 6)
			{
				fgets(job_explain[i][j], 1000, fp);
				EatRearWhiteChar(job_explain[i][j]);
				if (job_explain[i][j][0] == ';') continue;
				j++;
			}
		}
		fclose(fp);
	}
}

void SetJobExplainDetect(int job, int width, char explain[5][11][200], WORD color[5][11])
{
	//char job_explain[7][5][1000];
	WORD c[] = { RGB16(255, 0, 0), RGB16(255, 255, 0), FONT_COLOR_NUMBER, RGB16(255, 255, 0), FONT_COLOR_NUMBER, FONT_COLOR_NUMBER };
	WORD c2[] = { RGB16(255, 0, 0), FONT_COLOR_NUMBER, RGB16(255, 255, 0), FONT_COLOR_NUMBER, RGB16(255, 255, 0), RGB16(0, 255, 0) };
	static int old_job = -1;
	if (job == old_job) return;

	old_job = job;

	char* ex = job_explain[job][0];
	int ct = 0;
	int count = 0;
	for (int i = 0; i < 5; i++) for (int j = 0; j < 11; j++) explain[i][j][0] = NULL;
	WORD* cl;
	if (!job) cl = c;
	else cl = c2;

	while (ex)
	{
		for (int i = 0; i < 11; i++)
		{
			color[ct][i] = cl[count];
			char* ex2 = DivideStringByRect(width, ex, 1);
			if (!ex2 || ex2 == ex)
			{
				count++;
				strcpy(explain[ct][i], ex);
				if (count >= 6) return;
				ex = job_explain[job][count];
			}
			else
			{
				int len = ex2 - ex;
				strncpy(explain[ct][i], ex, len);
				explain[ct][i][len-1] = NULL;
				ex = ex2;
			}
		}
		ct++;
	}
}


void LoadNationExplain()
{
	FILE* fp;
	fp = Fopen("./data/nation_ex.txt", "rt");
	if (fp)
	{
		for (int i = 0; i < 7; i++)
		{
			int j = 0;
			while (j < 11)
			{
				fgets(nation_explain[i][j], 1000, fp);
				EatRearWhiteChar(nation_explain[i][j]);
				if (nation_explain[i][j][0] == ';') continue;
				j++;
			}
		}
		fclose(fp);
	}
}

void SetNationExplainDetect(int nation, int width, char explain[7][11][200], WORD color[7][11])
{
	nation -= 2;
	if (nation < 0) nation = 0;
	WORD c[] = { RGB16(255, 0, 0), RGB16(255, 255, 0), FONT_COLOR_NUMBER, FONT_COLOR_NUMBER, FONT_COLOR_NUMBER, FONT_COLOR_NUMBER, FONT_COLOR_NUMBER, FONT_COLOR_NUMBER, FONT_COLOR_NUMBER, FONT_COLOR_NUMBER, FONT_COLOR_NUMBER };
	WORD c2[] = { RGB16(255, 0, 0), RGB16(255, 255, 0), FONT_COLOR_NUMBER, RGB16(255, 255, 0), FONT_COLOR_NUMBER, RGB16(255, 255, 0), FONT_COLOR_NUMBER, RGB16(255, 255, 0), FONT_COLOR_NUMBER, FONT_COLOR_NUMBER, FONT_COLOR_NUMBER };
	static int old_nation = -1;
	if (nation == old_nation) return;

	old_nation = nation;

	char* ex = nation_explain[nation][0];
	int ct = 0;
	int count = 0;
	for (int i = 0; i < 7; i++) for (int j = 0; j < 11; j++) explain[i][j][0] = NULL;
	WORD* cl;
	if (!nation) cl = c;
	else cl = c2;

	while (ex)
	{
		for (int i = 0; i < 11; i++)
		{
			color[ct][i] = cl[count];
			char* ex2 = DivideStringByRect(width, ex, 1);
			if (!ex2 || ex2 == ex)
			{
				count++;
				strcpy(explain[ct][i], ex);
				if (count >= 11) return;
				ex = nation_explain[nation][count];
			}
			else
			{
				int len = ex2 - ex;
				strncpy(explain[ct][i], ex, len);
				explain[ct][i][len-1] = NULL;
				ex = ex2;
			}
		}
		ct++;
	}
}


void OpenSelectNationMenu()
{
	for (int k = 0; k < MAX_START; k++) SMenu[ClosemenuWhenLogin[k]].bActive = FALSE;
	SMenu[MN_MAKECHARACTER_BASIC].bActive = TRUE;
	SMenu[MN_NATION_EXPLAIN].bActive = true;
	SMenu[MN_SELECT_NATION].bActive = true;
	SMenu[MN_SELECT_NATION].work = 100;		// �ϴ� �߸���
	//SMenu[MN_RESET_JOB_EXPLAIN].bActive = true;
	MP3(SN_MENU_OPEN);
	y_MenuFocus = 0;
}

#ifdef LIGHT_VERSION_
#define __CHECK_LIGHT_VERSION__	return 0;
#else
#define __CHECK_LIGHT_VERSION__
#endif

int CheckNationCity(int city)
{
	switch (city)
	{
	case SC_BAISUS_IMPEL:	__CHECK_LIGHT_VERSION__
		if (SCharacterData.nation == N_VYSEUS && SCharacterData.nLevel >= 41) return OK;
		break;
	case SC_IRAMUS:	__CHECK_LIGHT_VERSION__
		if (SCharacterData.nation == N_VYSEUS && SCharacterData.nLevel >= 31) return OK;
		break;
	case SC_HELTERNT_VILLAGE:	if (SCharacterData.nation == N_VYSEUS) return OK;
		break;
	case SC_DIPH_UBEN:	__CHECK_LIGHT_VERSION__
		if (SCharacterData.nation == N_ZYPERN && SCharacterData.nLevel >= 41) return OK;
		break;
	case SC_TYREN:	__CHECK_LIGHT_VERSION__
		if (SCharacterData.nation == N_ZYPERN && SCharacterData.nLevel >= 31) return OK;
		break;
	case SC_RENUS:	if (SCharacterData.nation == N_ZYPERN) return OK;
		break;
	case SC_BARANTAN:	if (SCharacterData.nation == N_YILSE) return OK;
		break;

	default: return NOT;
	}
	if (SCharacterData.nation == N_NEUTRAL) return OK;
	return NOT;
}