#include "stdafx.h"
#include "Hangul.h"
#include "dragon.h"
#include "SmallMenuSet.h"
#include "Menu.h"
#include "SmallMenu.h"
#include "Hong_Sprite.h"
#include "stdio.h"
#include "String.h"
#include "convert565to555.h"
#include "Hong_Sub.h"
#include "tool.h"
#include "MenuSet.h"
#include "itemtable.h"
#include "skill.h"
#include "Effect.h"
#include "word.h"
#include <MATH.H>
#include "char.h"

extern void MouseClickPosition(int*,int*);


CPcWord pc_word[MAX_PCWORD];			// PC�� ���� ������ �߿��� �ܾ�
CPcWord npc_word[MAX_PCWORD];			// NPC�� ����� ����Ʈ
CPcWord random_npc_word[MAX_PCWORD];	// NPC�� Random���� ����� ����Ʈ
char white_str[20][6]; 

// 假設這兩個全域變數或成員變數已被轉換
std::vector<std::string> cmd(5);
static int start = 0;//lsw
static int start2 = 0;//lsw
static int start3 = 0;//lsw

char sell_text[10][10];		//lsw
char buy_text[10][10];		//lsw
char repair_text[10][10];	//lsw

//������: ���� ���� ���� ���� �Լ�
//�ӽ� �����: Test�� ���� �ӽ������� ������� variables... and etc. 

////////////////////////////////////////////////////////////////////////////////
//		Function isThatMyItem
//		Input: name of item
//		Output: return true or false depense on owning the inputed item or not.
////////////////////////////////////////////////////////////////////////////////

bool isThatMyItem( char *item_name )
{
	for( int a=0; a<3; a++ )
		for( int b=0; b<3; b++ )
			for( int c=0; c<8; c++ )
			{
				int item_no = InvItemAttr[a][b][c].item_no;
				if( item_no )
				{
					char *my_item_name = GetItemName( item_no );
					if( !strcmp( item_name, my_item_name ) ) return true;
				}
			}

	return false;
}

////////////////////////////////////////////////////////////////////////////
//		Function amountItem
//		Input: name of item
//		Output: number of items user has.		
////////////////////////////////////////////////////////////////////////////
int amountItem( char *item_name )
{
	int count = 0;
	for( int a=0; a<3; a++ )
		for( int b=0; b<3; b++ )
			for( int c=0; c<8; c++ )
			{
				int item_no = InvItemAttr[a][b][c].item_no;
				if( item_no )
				{
					char *my_item_name = GetItemName( item_no );
					if( !strcmp( item_name, my_item_name ) ) count++;
				}
			}

	return count;
}

////////////////////////////////////////////////////////////////////////////////////
// Function itemNameToNumber
// Input: The name of item.
// Output: The number of item.
////////////////////////////////////////////////////////////////////////////////////
int itemNameToNumber( char *item_name )
{
/*
#define ITEM_Plant_MAX		43
#define ITEM_Mineral_MAX	17
#define ITEM_Herb_MAX		63
#define ITEM_Cook_MAX		57	
#define ITEM_Potion_MAX		64	
#define ITEM_Tool_MAX		49
#define ITEM_Weapon_MAX		118
#define ITEM_Disposable_MAX	53	
#define ITEM_Armor_MAX		122
#define ITEM_Accessory_MAX	177	
#define ITEM_Etc_MAX		184		
*/
	int item_max[] = {	ITEM_Plant_MAX, ITEM_Mineral_MAX	,
						ITEM_Herb_MAX, ITEM_Cook_MAX		,
						ITEM_Potion_MAX	, ITEM_Tool_MAX		,
						ITEM_Weapon_MAX	, ITEM_Disposable_MAX	,
						ITEM_Armor_MAX	,ITEM_Accessory_MAX	,
						ITEM_Etc_MAX		, 
	};

	for( int i=0; i<11; i++ )
	{
		for( int j=0; j<item_max[i]; j++ )
		{
			int item_no = i*1000+j;
			char *source_item_name = GetItemName( item_no );
			if( !strcmp( item_name, source_item_name ) ) return item_no;
		}
	}
	return 0;
}


//ReqItemValue( item_no );
//////////////////////////////////////////////////////////////////////
//		Function itemPrice		
//		return : Price of one. 
//		input : Name of Item.
//////////////////////////////////////////////////////////////////////
DWORD itemPriceWhenSell( char *item_name)
{
	DWORD price = 0;
	for( int a=0; a<3; a++ )
		for( int b=0; b<3; b++ )
			for( int c=0; c<8; c++ )
			{
				int item_no = InvItemAttr[a][b][c].item_no;
				if( item_no )
				{
					char *my_item_name = GetItemName( item_no );
					if( !strcmp( item_name, my_item_name ) ) 
					{
						return ( GetItemValueSell( InvItemAttr[a][b][c] ));
					}
				}
			}
			return 0; // The user doesn't have this item.
}

DWORD itemPriceWhenBuy( char *item_name)
{
	DWORD price = 0;
	for( int a=0; a<3; a++ )
		for( int b=0; b<3; b++ )
			for( int c=0; c<8; c++ )
			{
				int item_no = InvItemAttr[a][b][c].item_no;
				if( item_no )
				{
					char *my_item_name = GetItemName( item_no );
					if( !strcmp( item_name, my_item_name ) ) 
					{
						return ( (DWORD)GetItemValueBuy( InvItemAttr[a][b][c].item_no ));
					}
				}
			}
			return 0; // The user doesn't have this item.
}



//	price = GetItemAttr(t.item_no, VALUE);
//	price = (int)( (float)price/d_max*(d_max - d_curr)*DUR_RATIO +.5 );



///////////////////////////////////////////////////////////////////////////
//	Erase usless words from input.
//  return the inputed sentence without usless words. 
///////////////////////////////////////////////////////////////////////////
char* getRidOfStuff( char *text )
{
	if( !start2 )
	{
		for(int ii=0;ii<20; ii++)
		{
		
			strcpy(white_str[ii],lan->OutputMessage(7,ii+211));//lsw
		}
		start2 = 1;
	}

	for( int i=0; i<20; i++ )
	{
		if( white_str[i] )
		{
			char *ptr;
			while( ptr = strstr( text, white_str[i] ) ) 
			{
				int len = strlen( white_str[i] );
				for( int j=0; j<len; j++ ) 
					*( ptr+j ) = ' ';
			}
		}
		else break;
	}						/// ���ʿ��� �ܾ ���ִ� �۾�
	return text;
}


////////////////////////////////////////////////////////////////////////////////
//	This function finds out what kind of operation does it need.
//	And return integers depense on the result.
////////////////////////////////////////////////////////////////////////////////
int whichCommand( char *text )
{
	if( !start3 )
	{
		int ii;
		for(ii=0;ii<10; ii++)
		{
			
		strcpy(sell_text[ii],lan->OutputMessage(7,ii+181));//lsw
		}
		
		for(ii=0;ii<10; ii++)
		{
			
		strcpy(buy_text[ii],lan->OutputMessage(7,ii+191));//lsw
		}

		for(ii=0;ii<10; ii++)
		{
			
		strcpy(repair_text[ii],lan->OutputMessage(7,ii+201));//lsw
		}
		start3 = 1;
	}

	int i;
	for( i=0; i<20; i++ )
	{
		if( (sell_text[i]) ) 
		{
			char *ptr = strstr( text, sell_text[i] );
			if( ptr ) return 1; // selling command return 1.
		}
	}

	for( i=0; i<20; i++ )
	{
		if( (buy_text[i]) )
		{
			char *ptr = strstr( text, buy_text[i] );
			if( ptr ) return 2; // buying command return 2.
		}
	}

	for( i=0; i<20; i++ )
	{
		if( (repair_text[i]) )
		{
			char *ptr = strstr( text, repair_text[i] );
			if( ptr ) return 3; // repair command return 3.
		}
	}
	return 0; // invalid command return 0.
}// End of function, whichCommand.

////////////////////////////////////////////////////////////////////////////
//	������
//  Function makeDeal: ���� system
//  void makeDeal(parameters) // inputed line from chat. 
//  No return value.
////////////////////////////////////////////////////////////////////////////

int makeDeal( char* lpStr) //�޴����� ����� ��� �ý���
{	
	char* text = getRidOfStuff(lpStr); //get a text line without crap.
	int operation = whichCommand(text); //which command do we need to excute.
	char *word;
	word = strtok( text, " \n");
	char *name = NULL;
	int amount = 0;
	int costs = 0;
	float dum = (0.8f + ((rand()%5)/10.0f)) ;
		

	////////////////// Read in the name of item ///////////////////////
		switch( operation )
		{
			case 1 :	// command sell.
				{
					if( isThatMyItem( word ) )
					{
						char *temp_word;
						temp_word = strtok( NULL, " \n");
						amount = atoi( temp_word );
						if( amount <= 0 ) return -200;// You don't have anything with you. 

						temp_word = strtok( NULL, " \n");
						costs = atoi( temp_word);

						if( costs <= 0 ) return -300;// Are you out of your mind?
													 // You don't want money for that item?
						if(costs <= (amount * (itemPriceWhenSell(word))*dum))
							return 0; // The deal is made.
						else
							return -100; // The deal wasn't good. Nah... I don't want to buy.
					}
					else 
						return -400; // user doesn't have this item. You don't have this item.					
				}
			case 2 :	// command buy.
				{
					//Need function that check inventory of NPC
					// 
					//	������ ���� ����Ʈ ��Ź~
					//	receive �ϴ� ������ npc�� ��ȭ�� ó���Ѵ�.
					//	npc�� event_no�� ���� �������� ����.
					//  �޴� ���� ������ �޾ƿ´�. ( true :������ �� �������� �����Ұ�� )
				//	else 
					return -500; // NPC doesn't have this item.
								 // Sorry NPC doesn't have this item.
				}

			case 3 :	// command repair.
				{
					/*if( isThatMyItem( word ) )
					{
						(char*)costs = strtok( text, "\n");

						if(cost <= /*function which returns the price of repair/ * dum)
						{
							return 0; // good deal!
							break;
						}
						else
						{
							return -100; // The deal was not good. You have to pay more...
							break;
						}
					}
					else return -400; //Hey you don't have this item. */ 
					return -400;
				}

			default : return -600; // this command is not availiable.
								// what do yo want?
		}
}

int max_pc_word;			// ���� PC�� ����Ҽ��ִ� ������ ��
int max_npc_word;			// ���� NPC�� ����Ҽ��ִ� ������ ��
int max_random_npc_word;		// ���� NPC�� random���� ����Ҽ� �ִ� �����

// 假設 LPCHARACTER, whoIsClose, CLOSE_ENOUGH, HUMAN 等已定義
// 為了重寫這個函式，我們需要簡化它們，假設它們是可用的。

void languageSystem(const std::string &lpStr) {
  int getDistance = 0;                        // distance from the nearest NPC.
  LPCHARACTER ch = whoIsClose(&getDistance);  // nearest NPC.

  // 當玩家與 NPC 距離夠近時
  if (!ch || getDistance > CLOSE_ENOUGH) {
    return;
  }

  // 檢查 NPC 是否為人類
  if (ch->nRace != HUMAN) {
    return;
  }

  // 當 cmd 向量還未初始化時，填充它
  if (!start) {
    for (int ii = 0; ii < 5; ++ii) {
      // 使用 std::string 賦值，避免 sprintf
      cmd[ii] = lan->OutputMessage(4, ii + 171);
      //SDL_Log("[%s] %s", cmd[ii].c_str(), lpStr.c_str());
    }
    start = true;
  }

  // 迴圈檢查玩家輸入字串是否包含任何命令
  bool found_match =
      std::any_of(cmd.begin(), cmd.end(), [&](const std::string &command) {
        // 使用 std::search 進行不區分大小寫的子字串搜尋
        auto it = std::search(lpStr.begin(), lpStr.end(),  // 在 lpStr 中搜尋
                              command.begin(),
                              command.end(),  // 搜尋 command 這個子字串
                              // 自訂的比較規則：比較兩個字元的小寫版本是否相等
                              [](unsigned char ch1, unsigned char ch2) {
                                return std::tolower(ch1) == std::tolower(ch2);
                              });
        // 如果 it 不是 lpStr.end()，代表找到了匹配的子字串
        return it != lpStr.end();
      });

  if (found_match) {
    SendEvent(EVENT_TYPE_NPC, ch->id);
  } 
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	Function CheckNpcTalk()
//	Player�� NPC��ó�� ������ ������ NPC�� Player���� �����Ǵ�. 
////////////////////////////////////////////////////////////////////////////////////////////////
void CheckNpcTalk( )
{
	return;
	int realNPC = 0;
	char* fileRandomNPCTalk = "./data/randomTalk.dat";
	int currentPosition;
	LPCHARACTER target = whoIsClose(&currentPosition); //returns closest NPC and the distance.
	
	if (target)
	{
		if (target->nRace != HUMAN)	
		{	//< CSD-030419
			int answerRate = (rand()%((int)(REPLY_RATE))); // Currently 5% // 0905 kkh ���� 
			
			if (!max_random_npc_word)
			{
				replyTable(fileRandomNPCTalk, random_npc_word, max_random_npc_word);
			}

			int whichLine = (rand()%max_random_npc_word);
			char* getReply = random_npc_word[whichLine].ReturnWords();
			
			if (getReply && answerRate ==0 && currentPosition < CLOSE_ENOUGH) 
			{
				strcpy( target->szChatMessage, getReply );
				target->ChatDelay = CHAT_DELAY_70; 
			}	
		}	//> CSD-030419
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////
//	���糡... Ha Ha ha ^o^
//	Function replyTable()
//		This function Initialize the data into the linked list. 
//		
//////////////////////////////////////////////////////////////////////////////////////////////
void replyTable(char* getFile, CPcWord getWord[], int &max)
{	
	int whichOperation = 0;
	char *getNPCWord;
	char buf[512];
	char *token;
	char delemeter[] = "\t\n";

	FILE *fp;
	
	fp = Fopen(getFile, "rt");
	if(fp)
	{
			max = 0;
//			for(int i = 0; (i < NUM_NPCWORD && !feof(fp)); i++)
			while(fgets( buf, 511, fp ) != NULL )
			{
//				fgets( buf, 511, fp );
				//getWord[i].AddNode( getNPCWord, getOperation );
				token = strtok( buf, delemeter);
				whichOperation = atoi(token);
				token = strtok( NULL, delemeter );
				//token = strtok( buf, "\n");
				getNPCWord = token;
				getWord[max].AddNode(getNPCWord, whichOperation);
				max++;
			}
			fclose(fp);
	}
	
	
}//End of fn() replyTable
//////////////////////////////////////////////////////////////////////////////////////////////
//	Function *ReplyNpc(char)
//		I need data set, pc_word[100] and npc_word[100].
//		They are 100 nodes which include a character.
//		When player asked something NPC answers them...
//		I don't think this will work. Why? if the pc_word = "whaz"		a.com(
//////////////////////////////////////////////////////////////////////////////////////////////
char *ReplyNpc( char *str, CPcWord pc[], CPcWord npc[] )
{
	int random_cmd[100] = {0,};

	for( int i=0; i<max_pc_word; i++ )
	{
		int cmd = pc[i].CompareWord( str );
		if( cmd != -1 )
		{
			int count = 0;
			for( int j=0; j<max_npc_word; j++ )
			{
				if( npc[j].ReplyWords( cmd ) ) 
				{
					random_cmd[count++] = j;
				}
			}
			if( count )
			{
				count = rand()%count;
				return npc[random_cmd[count]].ReturnWords();
			}
		}
	}

	return NULL;
}



///////////////////////////////////////////////////////////////////////////////////////
//Class CPcWord.
///////////////////////////////////////////////////////////////////////////////////////
 
CPcWord::CPcWord()
{
	word = NULL;
	cmd = -1;
}

CPcWord::~CPcWord()
{
	DeleteNode();
}

void CPcWord::AddNode( char *str, int num ) // �߰��� �ܾ�� ��� ����
{
	if( word ) delete word;
	int len = strlen( str );
	word = new char[len+1];
	strcpy( word, str );

	cmd = num;
}

void CPcWord::DeleteNode( )
{
	if( word ) delete word;
	cmd = -1;
}
	
int CPcWord::CompareWord( const char *target_str )
{
	if( strstr( target_str, word ) ) return cmd;
	else return -1;
}

char* CPcWord::ReplyWords( int num )
{
	if( num == cmd ) return word;
	else return NULL;
}

char* CPcWord::ReturnWords( )
{ 
	return word;
}





//////////////////////////////////////////////////////////////////////////////////////////////
//	Function whoIsclose(int*, int*)
//		���尡��� �Ÿ����ִ� NPC���� �Ÿ��� ���ϰ�, �� NPC�� return��.  
//
//////////////////////////////////////////////////////////////////////////////////////////////

LPCHARACTER whoIsClose(int* getDistance)
{	
	LPCHARACTER	ch = Hero->lpNext;
	//////This is for exchanging the value of dot position to tile///////
	POINT	hero_Position;

	int mx=0, my=0;
	MouseClickPosition( &mx, &my );
	hero_Position.x = mx/TILE_SIZE;
	hero_Position.y = my/TILE_SIZE;
	
	int distance=0;
	int currentPosition = 2000000;
	// change the above line to the sum of position where player clicked.

	LPCHARACTER target= NULL;

	// This while-roof finds the closest NPC and the distance between NPC and Player.
	while( ch )
	{
		/**********  �ӽ� ����� SPRITETYPE_NPC  **********************************/
//		if( ch->type == SPRITETYPE_NPC ) // make sure ch is NPC.
		if( ch->type == SPRITETYPE_MONSTER ) // make sure ch is NPC.
		/****************************************************************************/
		{
			POINT	npc_Position;
			npc_Position.x = ch->x/TILE_SIZE;
			npc_Position.y = ch->y/TILE_SIZE;
			distance = abs(hero_Position.x-npc_Position.x)+abs(hero_Position.y-npc_Position.y);//(int)sqrt(pow(hero_Position.x-npc_Position.x,2)+pow(hero_Position.y-npc_Position.y,2));
			
			if( currentPosition > distance ) 
			{
				target = ch;
				currentPosition = distance;
			}
		}
		ch = ch->lpNext;
	}
	*getDistance = currentPosition;
	return target;
}