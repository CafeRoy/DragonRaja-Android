// menu.cpp
#include "stdafx.h"
#include "Hangul.h"
#include "dragon.h"
#include "Menu.h"
#include "Hong_Sprite.h"
#include "SmallMenuSet.h"
#include "stdio.h"
#include "directsound.h"
#include "Char.h"
#include "SmallMenu.h"
#include "Hong_Sub.h"
#include "Effect.h"
#include "map.h"
#include "tool.h"
#include "MenuSet.h"
#include "skill.h"
#include "Item.h"
#include "MenuNetwork.h"
#include "MouseCursor.h"
#include "gameproc.h"
#include "kh_cpp.h"
#include "chardatatable.h"
#include "QuestLog.h"


void CallKhMenuProc( int menu, int field )		// ȭ�� ��� // �Ŵ��� �߻��ϴ� ���̶� ���� ���� ���� �ʿ� ���� 
{
	SMENU *SubMenu = &SMenu[menu];
	int j = field;
	int i = menu;

	switch( SMenu[menu].nField[field].nType )
	{

		case FT_QUEST_SCROLL : {							
									// ����Ʈ ���̺� �����͸� �о� �´�.........................................
									if( quest_text[0].name[0] == 0 ) {
										LoadQuestTable("./data/quest.txt","./data/questpic.txt","./data/questpic.bin");
									}
									// ���콺�� ���������� Ÿ��ȭ ��ǥ���� ������ �־� �װ��� ������ �ִ´�. ...
									int quest_num1 = quest_num[SubMenu->work];
									// �׸� ��� ��ƾ 
									//�׸��� ���� �ۿ� ���� ����� 
									//FieldTypeNomalPut( SubMenu->x, SubMenu->y, 57, 34, quest_num1+2, SubMenu->nField[j].nImageType );
									//�ϳ��� �������� �����Ƿ� FieldTypeNomalPut( SubMenu->x, SubMenu->y, 80, 70, 6+����Ʈ ��ȣ, SubMenu->nField[j].nImageType );
									
									//  ���콺�� �簢�� �ȿ��� ��������.......................................................
									if( SubMenu->nField[j].fLButtonDown ){
										FieldTypeNomalPut( SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType );
									}
									// ���콺�� �簢�� ���� �ö� ����......................................................... 
									else if( SubMenu->nField[j].fRectMouse ){
										FieldTypeNomalPut( SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType );
									}
									
									// ���ڸ� ��´�. 
									Hcolor(40,30,20);
									
									// ����Ʈ �׸� �ҷ����� 
									if(quest_text[quest_num1-1].picnum ){
										Spr *s = &quest_text[quest_num1-1].sq;
										if( !s->img )
											LoadSpriteOnlyOne( s,quest_text[quest_num1-1].picture);
											PutCompressedImage(SubMenu->x+57+63, SubMenu->y+34+80, s );
									}

									//���࿡ ������ ����Ʈ �϶� 
									if(end_quest[SubMenu->work]){
										Hcolor( 0,0,200);
										HprintBold( SubMenu->x+70,SubMenu->y+110,RGB(250,100,80),RGB(80,30,250),lan->OutputMessage(6,54));//010215 lsw
										
									}
									//����Ʈ ���°?
									Hprint2( SubMenu->x+50,SubMenu->y+50, g_DestBackBuf, "%d", SubMenu->work+1 );
									//��¥ ����Ʈ ��ȣ 
									//Hprint2( SubMenu->x+150,SubMenu->y+50, g_DestBackBuf, "QUEST %d", quest_text[quest_num1].num );
									
									//����Ʈ 
									Hcolor(11,11,11);
									if(quest_num1 == 0){
										// ����Ʈ�� ������ �ƴ� �����϶� 	
										Hprint2( SubMenu->x+230, SubMenu->y+70, g_DestBackBuf, "%s", lan->OutputMessage(6,55) );//;sw
									}
									else {
										HprintBold( SubMenu->x+230, SubMenu->y+50,RGB(200,200,200),RGB(80,80,80), quest_text[quest_num1].name);
										//Hprint2( SubMenu->x+230, SubMenu->y+50, g_DestBackBuf, "%s",quest_text[quest_num1].name );
										
										RectTextPut( SubMenu->x+230, SubMenu->y+100, 110, quest_text[quest_num1].ch );
									}
									
									//page �� 
									Hcolor(77,208,109);
									Hprint2( SubMenu->x+105,SubMenu->y+200, g_DestBackBuf, "CURRENT PAGE %d", SubMenu->work+1 );
									Hprint2( SubMenu->x+230,SubMenu->y+200, g_DestBackBuf, "TOTAL PAGE %d",  QUEST_COUNT);
						
									break;
							   }
						
		case FT_QUEST_OK : {
									;
									if( SubMenu->nField[j].fLButtonDown ){
										FieldTypeNomalPut( SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType );
									}
									else if( SubMenu->nField[j].fRectMouse ){
										FieldTypeNomalPut( SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType );
									}
									break;
							   }
		
		case FT_QUEST_INDEX_SCROLL : { //  ���콺�� �簢�� �ȿ��� ��������.......................................................
										//Hprint2( SubMenu->x+38, 150, g_DestBackBuf, "%d", quest_text[quest_num[quest_num1]].num );
										;
										// ���� �������� ��Ż �������� ���� ��� right ��ư�� �� ������ �Ѵ�.
										// ���� �������� ó���� ��� left ��ư�� �� ������ �Ѵ�.
										// //�̶� ����� MP3( SN_WARNING );
										if( SubMenu->nField[j].fLButtonDown ){
											FieldTypeNomalPut( SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nImageNumber, SubMenu->nField[j].nImageType );
										}
						
										// ���콺�� �簢�� ���� �ö� ����......................................................... 
										else if( SubMenu->nField[j].fRectMouse ){
											FieldTypeNomalPut( SubMenu->x, SubMenu->y, SubMenu->nField[j].x, SubMenu->nField[j].y, SubMenu->nField[j].nRectImage, SubMenu->nField[j].nImageType );
										}
										break;
									 }
		
		case FT_QUEST_INDEX : {
							int x = SubMenu->x+SubMenu->nField[field].x;
							int y = SubMenu->y+SubMenu->nField[field].y;
							QuestLog* pQuestLog = new QuestLog;
							vector<Quest>::iterator iter;
							for (iter = pQuestLog->m_Quests.begin(); iter != pQuestLog->m_Quests.end(); ++iter)
							{
								Quest quest = (*iter);
								Hcolor( RGB16( 0, 0, 255 ));
								Hprint2( x ,y,  NULL, "%s", quest.strName.c_str() );	
								y+=20;
							}

									// ���콺 ��ǥ���� Ÿ��ȭ �Ѵ�.(Y ����..) ..................................
							/*		;
									int max_quest_one_page = SubMenu->key;
									int page = SubMenu->work;
									int	tempY = ( g_pointMouseY - ( SubMenu->y+40) ) / QUEST_TEXT_DEVISION;
									
									// ����Ʈ ���̺� �����͸� �о� �´�.
									if( quest_text[0].name[0] == 0 ) 
									{ 
										LoadQuestTable("./data/quest.txt","./data/questpic.txt","./data/questpic.bin");
									}
								
									//����Ʈ�� �Ѿ� �´� 
									//�׸��� ���� �Ʒ��� ������.
									//quest_num[QUEST_COUNT] = QUEST_GIVE;
									// ���࿡ �簢���� ������� -> 
									//	����Ʈ �ε��� ��� ==> ��ȣ�� �̸� ���
									//	1. ���� �������� 7�� ������ �������� 7�� ���.
									//	2. ���� ���� �� ����Ʈ ����, �׸��� ���������� 14�� ���
									//	3. �������� �ѱ�� �� �ѱ䵥 ���� ���( ��������  page ������ ����ִ� )
									//	4. ���� ����Ʈ ��ȣ�� �������� ���������� ����Ҽ� �ִ� ����Ʈ�� ���̴�.
									//		( page * max_quest_one_page )
									//	5. ���콺�� ������ ���� ���� �ٲ��ش�.
									
									int rect_mouse = SubMenu->nField[j].fRectMouse;
									int willdo = SubMenu->nField[j].nWillDo;
									int PGCT = max_quest_one_page*page; // �� ������ ó��..

									Hcolor(77,208,109);
									Hprint2( SubMenu->x+105, SubMenu->y+200, g_DestBackBuf, "CURRENT PAGE %d", (PGCT/SubMenu->key) +1);
									Hprint2( SubMenu->x+230, SubMenu->y+200, g_DestBackBuf, "TOTAL PAGE %d",  (QUEST_COUNT-1)/SubMenu->key+1);
									Hcolor(11,11,11);
								
									for( int a = 0; a<max_quest_one_page/2; a++ )
									{
										int i= PGCT+a+willdo*max_quest_one_page/2;
										if( i > QUEST_COUNT-1 ) break;
										// �׸� ��� 
										if( willdo == 0 ){
											if( i == tempY+PGCT && rect_mouse ) {
												if(end_quest[i]){Hcolor( 240,0,0);}
												else Hcolor( 240,224,208);
											}
											else Hcolor(11,11,11);
											//Hprint2(SubMenu->x+38 , 150, g_DestBackBuf, "%d", quest_text[quest_num].num );
											Hprint2( SubMenu->x+60, 140+(a*QUEST_TEXT_DEVISION), g_DestBackBuf, "%d", i+1 );
											//RectTextPut( SubMenu->x+80,SubMenu->y+40+(a*QUEST_TEXT_DEVISION), 110, quest_text[quest_num[i]].name );
											if(quest_num[i] == 0){
												//����Ʈ�� �����϶� ó�� 
												Hprint2( SubMenu->x+80,SubMenu->y+40+(a*QUEST_TEXT_DEVISION), g_DestBackBuf, lan->OutputMessage(6,55) );//010215 lsw
											}
											else {
												RectTextPut( SubMenu->x+80,SubMenu->y+40+(a*QUEST_TEXT_DEVISION), 110, quest_text[quest_num[i]].name );
											}
										}
										else
										{
											if( a == tempY && rect_mouse(g_pointMouseX > SubMenu->x+210 && g_pointMouseX < SubMenu->x+340)) {
												if(end_quest[i]){Hcolor( 240,0,0);}
												else Hcolor( 240,224,208);
											}
											else Hcolor(11,11,11);
											Hprint2( SubMenu->x+230, 140+(a*QUEST_TEXT_DEVISION), g_DestBackBuf, "%d", i+1 );
											//RectTextPut( SubMenu->x+250,SubMenu->y+40+(a*QUEST_TEXT_DEVISION), 110, quest_text[quest_num[i]].name );
											if(quest_num[i] == 0){
												//����Ʈ�� �����϶� ó��
												Hprint2( SubMenu->x+250,SubMenu->y+40+(a*QUEST_TEXT_DEVISION), g_DestBackBuf, lan->OutputMessage(6,55) );//010215 lsw
											}
											else {
												RectTextPut( SubMenu->x+250,SubMenu->y+40+(a*QUEST_TEXT_DEVISION), 110, quest_text[quest_num[i]].name );
											}
											
										}
										
									}								 
								*/
																
									break;
							   }
		
		case FT_COLOR_CHANGE_GAUGE :	
			{ 
				if( !SubMenu->CheakType )
				{
					if( g_Menu_Variable.m_stRGB )
					{
						delete g_Menu_Variable.m_stRGB;
					}

					g_Menu_Variable.m_stRGB = new k_char_rgb;
					SubMenu->CheakType = 1;
				}

				if(SMenu[i].key == 1 && j == 4){
					FieldTypeNomalPut(SMenu[i].x, SMenu[i].y, 0, 0, 1, SMenu[i].nImageType);
				}

				int start, end; 
				int *pGab;
				switch( j )			// field ��ȣ�� 4������ ����
				{
					case 4 : switch(SMenu[MN_CHARACTER_COLOR_CHANGE].key)
							 {
								case 0: pGab= &SCharacterData.body_r; break;
								case 1: pGab= &SCharacterData.cloth_r; break;
							 }
							 break;
					case 5 : switch(SMenu[MN_CHARACTER_COLOR_CHANGE].key)
							 {
								case 0: pGab = &SCharacterData.body_g; break;
								case 1: pGab = &SCharacterData.cloth_g; break;
							 }
							 break;
					case 6 : switch(SMenu[MN_CHARACTER_COLOR_CHANGE].key)
							 {
								case 0: pGab = &SCharacterData.body_b; break;
								case 1: pGab = &SCharacterData.cloth_b; break;
							 }
							 break;
				}

				// ���ڷ� ǥ���ϱ�
				Hprint2( SubMenu->x+SubMenu->nField[j].x,SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf, "%d", (*pGab) );
				
				int width = 98;
				// ����� ǥ���ϱ�
				int gab= (int)((*pGab)/ 255.0f*width+0.5f);
				end = gab;
				start = 75;		// ���밡 �����ϴ� x ��ǥ

				if(SMenu[i].nField[j].fLButtonDown)	 
					end = g_pointMouseX - (SMenu[i].x+start);
				if(end < 0 ) 
				{
					gab--;
					gab = std::max( gab, 0 );
				}
				else if(end > width) 
				{
					gab++;
					gab = std::min( gab, width );
				}
				else gab = end;

				for(int x=start;x<gab+start;x++)
					FieldTypeNomalPut(SMenu[i].x, SMenu[i].y, x, SMenu[i].nField[j].y+4, SMenu[i].nField[j].nImageNumber);

				(*pGab) = (int) (gab/(float)width * 255.0f + 0.5);
				
				static int old_gab[2][3];
				if( old_gab[SMenu[MN_CHARACTER_COLOR_CHANGE].key][j-4] != gab )
				{
					Hero->bodycolor = ReturnBlendRGB( SCharacterData.body_r, SCharacterData.body_g, SCharacterData.body_b );
					Hero->clothcolor= ReturnBlendRGB( SCharacterData.cloth_r, SCharacterData.cloth_g, SCharacterData.cloth_b );
					old_gab[SMenu[MN_CHARACTER_COLOR_CHANGE].key][j-4] = gab;
				}
				break;
			}
		case FT_MANTLE_COLOR_CHANGE_GAUGE :	
			{ 
				if( !SubMenu->CheakType )
				{
					if( g_Menu_Variable.m_stRGB )
					{
						delete g_Menu_Variable.m_stRGB;
					}

					g_Menu_Variable.m_stRGB = new k_char_rgb;
					SubMenu->CheakType = 1;
				}

				int start, end; 
				int *pGab;
				switch( j )			// field ��ȣ�� 4������ ����
				{
					case 3: 
						pGab = &SCharacterData.mantle_r;
						break;
					case 4:
						pGab = &SCharacterData.mantle_g;
						break;
					case 5 :
						pGab = &SCharacterData.mantle_b;
						break;
				}

				// ���ڷ� ǥ���ϱ�
				Hprint2( SubMenu->x+SubMenu->nField[j].x,SubMenu->y+SubMenu->nField[j].y, g_DestBackBuf, "%d", (*pGab) );
				
				int width = 98;
				// ����� ǥ���ϱ�
				int gab= (int)((*pGab)/ 255.0f*width+0.5f);
				end = gab;
				start = 75;		// ���밡 �����ϴ� x ��ǥ

				if(SMenu[i].nField[j].fLButtonDown)	 
					end = g_pointMouseX - (SMenu[i].x+start);
				if(end < 0 ) 
				{
					gab--;
					gab = std::max( gab, 0 );
				}
				else if(end > width) 
				{
					gab++;
					gab = std::min( gab, width );
				}
				else gab = end;

				for(int x=start;x<gab+start;x++)
					FieldTypeNomalPut(SMenu[i].x, SMenu[i].y, x, SMenu[i].nField[j].y+4, SMenu[i].nField[j].nImageNumber);

				(*pGab) = (int) (gab/(float)width * 255.0f + 0.5);

				static int old_gab[2][3];
				if( old_gab[SMenu[MN_MANTLE_COLOR_CHANGE].key][j-3] != gab )
				{
					Hero->mantlecolor = ReturnBlendRGB( SCharacterData.mantle_r, SCharacterData.mantle_g, SCharacterData.mantle_b );
					old_gab[SMenu[MN_MANTLE_COLOR_CHANGE].key][j-3] = gab;
				}
				break;
			}
		case FT_SKIN_MONEY : 
			{
				// ���� ���
				if( SubMenu->key )
					FieldTypeNomalPut(SMenu[i].x, SMenu[i].y, 0, 0, 12, SMenu[i].nField[j].nImageType );
				
				// �� ���
				if( !SubMenu->work ) break;
				FieldTypeNomalPut(SMenu[i].x, SMenu[i].y, SMenu[i].nField[j].x, SMenu[i].nField[j].y, SMenu[i].nField[j].nImageNumber,SMenu[i].nField[j].nImageType);

				SetHangulAlign( TA_RIGHT );
				Hprint2( SubMenu->x+SubMenu->nField[j].rCheakBox.right,SubMenu->y+SubMenu->nField[j].rCheakBox.top+1, g_DestBackBuf, "%d", NPC_Lev_Ref[Hero->lv].nDyeingPrice);

				break;
			}

		case FT_VIEW_RUNE_LEFT :
			{
				int &count = SubMenu->nField[j].nSHideNomalNumber;
				if( !SubMenu->CheakType )
				{
					//SubMenu->CheakType = 1;
					if( !g_Menu_Variable.m_pRune )
					{
						g_Menu_Variable.m_pRune = new CRuneMenu;
					}
					else
					{
						g_Menu_Variable.m_pRune->m_nQuestStep = 0;
						g_Menu_Variable.m_pRune->m_nSelectRune = 0;
					}
					CallServer( CMD_GET_GHOST_QUEST_INFO );
					count = 0;
					SubMenu->nField[3].m_lpTemp= NULL;
				}

				SubMenu->nField[j].nSHideNomalCount = SCharacterData.m_nQuestStep+1;

				g_Menu_Variable.m_pRune->m_nQuestStep = SubMenu->nField[j].nSHideNomalStart;
				int step = g_Menu_Variable.m_pRune->m_nQuestStep;
				int rune_count = SCharacterData.m_nSelectRune;

				if( step == SCharacterData.m_nQuestStep )
				{
					if( rune_count == 24 )		// �� á�� ��� 
					{
						int effect_def[] = { RUNE_EFFECT1, RUNE_EFFECT2, RUNE_EFFECT3, RUNE_EFFECT4 };
						FieldTypeNomalPut( SMenu[i].x, SMenu[i].y, 48, 163, 64, RUNE_IMG );		// ���� ��� �ֱ�
						int x = SubMenu->x+163; 
						int y = SubMenu->y+250;
						static int count1[] = { 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5 };
						static int count2[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 8, 9, 10 };				
						if( count < 10 && count1[count] < 5 )
						{
							Spr *s = GetSprOfMenu( RUNE_EFFECT_DEFAULT, count1[count] );
							if( s && s->img )
							{
								PutCompressedImageFX( x, y, s, 17, 2 );
								count ++;
							}
							else count = 10;
						}
						else if( count2[count-10] < 10 )
						{
							Spr *s = GetSprOfMenu( effect_def[step], count2[count-10] );
							if( s && s->img )
							{
								PutCompressedImageFX( x, y, s, 17, 2 );
								count++;
							}
						}
						else
						{
							// ������ �͸� ��� �ش�.
							Spr *s = GetSprOfMenu( effect_def[step], 9 );
							if( s && s->img )
							{
								PutCompressedImageFX( x, y, s, 17, 2 );
							}
						}
						g_Menu_Variable.m_pRune->m_nSelectRune = 0;
					}
					else
					{
						for( int a=1; a<=rune_count; a++ )
						{
							FieldTypeNomalPut( SMenu[i].x, SMenu[i].y, 
								g_Menu_Variable.m_pRune->m_stRune[a].x,
								g_Menu_Variable.m_pRune->m_stRune[a].y,
								g_Menu_Variable.m_pRune->m_stRune[a].m_nLeftRuneImage,
								RUNE_IMG );
						}
					}
				}
				if( step < SCharacterData.m_nQuestStep )
				{
					for( int a=1; a<MAX_RUNE; a++ )
					{
						FieldTypeNomalPut( SMenu[i].x, SMenu[i].y, 
							g_Menu_Variable.m_pRune->m_stRune[a].x,
							g_Menu_Variable.m_pRune->m_stRune[a].y,
							g_Menu_Variable.m_pRune->m_stRune[a].m_nLeftRuneImage,
							RUNE_IMG );
					}
				}

				int step2image[] = { 0, 61, 62, 63 };
				if( step2image[step] )
				{
					FieldTypeNomalPut( SMenu[i].x, SMenu[i].y, 133, 105, step2image[step], RUNE_IMG );
				}
				break;
			}
		case FT_VIEW_RUNE_RIGHT :
			{
				int step = g_Menu_Variable.m_pRune->m_nQuestStep;
				int &rune = g_Menu_Variable.m_pRune->m_nSelectRune;
				
				static int old_step = step;
				if( old_step != step ) 
				{
					if( step == SCharacterData.m_nQuestStep )
						rune =SCharacterData.m_nSelectRune;
					else 
						rune = 24;

					old_step = step;
				}
				if( !g_Menu_Variable.m_clsRuneExplain ) 
					g_Menu_Variable.m_clsRuneExplain = new CRuneExplain;

				char *lpszText = (char*)SMenu[i].nField[3].m_lpTemp;
				static char *old_point = lpszText;
				if( old_point != SMenu[i].nField[3].m_lpTemp )
				{				
					old_point = lpszText;
					int nWidth = SMenu[i].nField[3].nRectImage;
					if( lpszText )
					{
						int &page_status = SMenu[i].work;
						int line = SMenu[i].key;
						WORD page;
						WORD page_max;
						GetWORDOfDWORD( (DWORD)page_status, page, page_max );
						
						page = 0;
						int line_max = GetLineRectTextPut( nWidth, lpszText );
						if( line )
						{
							page_max = (line_max-1) / line + 1;
						}
						else page_max = 1;
						page_status = (int )MAKELONG( page, page_max );
					}
				}

				if( rune ) 
				{
					FieldTypeNomalPut( SMenu[i].x, SMenu[i].y, 
						SMenu[i].nField[j].x,SMenu[i].nField[j].y,
						g_Menu_Variable.m_pRune->m_stRune[rune].m_nRightRuneImage,
						RUNE_IMG );

					SubMenu->nField[3].m_lpTemp= g_Menu_Variable.m_clsRuneExplain->m_szExplain[step][rune];
					Hcolor( FONT_COLOR_YELLOW );
					Hprint2( SMenu[i].x+SMenu[i].nField[j].x+50, SMenu[i].y+SMenu[i].nField[j].y+23, 
						NULL, "%s", g_Menu_Variable.m_clsRuneExplain->m_szTitle[step][rune] );
				}
				else if( step == SCharacterData.m_nQuestStep && SCharacterData.m_nSelectRune == 24 )
				{
					FieldTypeNomalPut( SMenu[i].x, SMenu[i].y, 
							SMenu[i].nField[j].x,SMenu[i].nField[j].y, step+25,		// �� �̹��� ��ȣ�� 28������
							RUNE_IMG );

					g_Menu_Variable.m_clsRuneExplain->LoadLuneText2( step );
					SubMenu->nField[3].m_lpTemp = (void *)g_Menu_Variable.m_clsRuneExplain->GetTitleExplain( step );
					Hcolor( FONT_COLOR_YELLOW );
					Hprint2( SMenu[i].x+SMenu[i].nField[j].x+50, SMenu[i].y+SMenu[i].nField[j].y+23, 
						NULL, "%s", g_Menu_Variable.m_clsRuneExplain->m_szTitle[step][rune] );
				}
				else 
				{
					SubMenu->nField[3].m_lpTemp = 0;
				}
				break;
			}		
		
		default: break;
	}
}




bool DoButtonCheckOfMenu_kh( int i, int j )		// ��ư Ŭ����
{
	switch( SMenu[i].nField[j].nType )
	{
	
	case FT_QUEST_SCROLL : {
								; 
								int type = SMenu[i].nField[j].nWillDo;
								
								if( type )		// right
								{
									SMenu[i].work++;
									if( SMenu[i].work > QUEST_COUNT-1 ) 
									{
										SMenu[i].work = QUEST_COUNT-1;
										MP3( SN_WARNING );
									}
									// max ó���� ����� �Ѵ�. ������ ÷ ���� ������ ����Ʈ ��û 
								
								}
								else			// left
								{
									SMenu[i].work--;
									if( SMenu[i].work < 0 ) 
									{
										SMenu[i].work = 0;
										MP3( SN_WARNING );
									}
								}
								break;
							   }

		case FT_QUEST_OK : {
							CallSmallMenu(MN_QUEST_INDEX); 
							SendReqQuestIndex( 0 );
							break;
							}

		case FT_QUEST_INDEX_SCROLL : {
									; 
									int page = SMenu[i].work;
									int max_quest_one_page = SMenu[i].key;
									int type = SMenu[i].nField[j].nWillDo;
									int PGCT = max_quest_one_page*page; // �� ������ ó��..
									
									if( QUEST_COUNT-1 <= SMenu[i].work) break;
												
									
									if( type )		// right
									{
										
										SMenu[i].work++;

										// max ó��
										if( SMenu[i].work  > (QUEST_COUNT-1) / max_quest_one_page  ) {
											SMenu[i].work = (QUEST_COUNT-1) / max_quest_one_page ;
											MP3( SN_WARNING );
										}
										if((PGCT/SMenu[i].key) == (QUEST_COUNT-1)/SMenu[i].key){
											SendReqQuestIndex( SMenu[i].work );		//����Ʈ �θ���		// �ڷ� ���ȴٰ� ������ ����, // 0810 YGI
										}									
									}
									else			// left
									{
										
										SMenu[i].work--;
										if( SMenu[i].work < 0 ) 
										{
											SMenu[i].work = 0;
											MP3( SN_WARNING );
										}
										
									}
									break;
									}
		
		case FT_QUEST_INDEX : {
								int PGCT = SMenu[i].work*SMenu[i].key;	// �� ������ ó��..
								int clickY = ( g_pointMouseY - ( SMenu[i].y+ 40 ) ) / QUEST_TEXT_DEVISION;
								//  ���콺 ���� ���ǰ��� �Ѿ� �´�. 
								//GetLineRectTextPut( )
								int willdo = SMenu[i].nField[j].nWillDo;
								
								if( willdo == 0 )
								{
									if( QUEST_COUNT > clickY+PGCT){
										SMenu[MN_QUEST].bActive = true;
										SMenu[MN_QUEST].work = clickY+PGCT; 
										SMenu[MN_QUEST].nTemp = MN_QUEST_INDEX;
										SMenu[MN_QUEST_INDEX].bActive = false;
										y_MenuFocus = MN_QUEST;
									}
								}
								else 
								{
									if( QUEST_COUNT > clickY+3+PGCT){
										SMenu[MN_QUEST].bActive = true;
										SMenu[MN_QUEST].work = clickY+(SMenu[i].key/2)+PGCT; 
										SMenu[MN_QUEST].nTemp = MN_QUEST_INDEX;
										SMenu[MN_QUEST_INDEX].bActive = false;
										y_MenuFocus = MN_QUEST;
									}
								}

								break;
							   }
		case FT_COLOR_CHANGE_GAUGE :
			{

				break;
			}
		case FT_MANTLE_COLOR_CHANGE_GAUGE :
			{

				break;
			}
		case FT_VIEW_RUNE_LEFT :
			{
				int step = g_Menu_Variable.m_pRune->m_nQuestStep;
				int rune_count = SCharacterData.m_nSelectRune;

				RECT rect;
				if( step == SCharacterData.m_nQuestStep && rune_count == 24 ) break;

				for( int a=1; a<=rune_count; a++ )
				{
					SetRect( &rect, g_Menu_Variable.m_pRune->m_stRune[a].x, g_Menu_Variable.m_pRune->m_stRune[a].y,
						g_Menu_Variable.m_pRune->m_stRune[a].x+38, g_Menu_Variable.m_pRune->m_stRune[a].y+37 );
					if( MouseInRectCheak( SMenu[i].x, SMenu[i].y, rect ) )
					{
						g_Menu_Variable.m_pRune->m_nSelectRune = a;
						break;
					}
				}
				break;
			}
			
		default: 
			return false;
			break;
	}
	return true;
}
