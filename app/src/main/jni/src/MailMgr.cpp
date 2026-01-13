// MailMgr.cpp: implementation of the CMailMgr class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Chat.h"
#include "Hangul.h"
#include "menu.h"
#include "dragon.h"
#include "Hong_Sub.h"
#include "Map.h"
#include "CharDataTable.h"
#include "TeamBattle.h"
#include "GameProc.h"
#include "LineCommand.h"
#include "Tool.h"
#include "Hong_Sprite.h"
#include "directsound.h"
#include "SmallMenu.h"
#include "TextOutMgr.h"
#include "MailMgr.h"
#include "menudef2.h"
#include "network.h"
//////////////////////////////////////////////////////////////////////
// Construction / Destruction
//////////////////////////////////////////////////////////////////////
//LC_GUILD_MAIL �̰� ��� ���� ���°Ŵ� ���� ~
/* ETC
200 : "����Ÿ�Կ� ������ �ֽ��ϴ�."
201 : "�޴� ����� ������ �ֽʽÿ�."
202 : "���� ������ �Է��� �ֽʽÿ�."
203 : "���� ������ �Է��� �ֽʽÿ�."
204 : "������ '%s'�� ����Ǿ����ϴ�."
205 : "���� ���忡 ���� �߽��ϴ�."
*/
CMailMgr g_MailMgr;
CMailMgr::CMailMgr()
{
	Clear();
}

CMailMgr::~CMailMgr()
{
	Clear();
	ClearTempData();
}

bool CMailMgr::Send(const int iType, const char* szTarget, const char *szTitle, const char *szBody)//������
{
	switch(iType)
	{
	case GUILD_MAIL:
		{
			if( !CheckGuildCode(Hero->GetGuildCode()) ) //����ڵ尡 ����
			{	// �޽��� �ڽ� ���
				Kein_PutMessage( KM_FAIL, kein_GetMenuString( 79 ));
				return false;		// ��带 ������ �־�� ��� ������ ������ �ִ�.
			}
		}break;
	case NORMAL_MAIL:
	case GAMEMASTER_MAIL:
	case NORMAL_MAIL_REPLY:
	case GAMEMASTER_MAIL_REPLY:
		{
		}break;
	default:
		{
			::AddCurrentStatusMessage( 250, 100, 100, lan->OutputMessage(5,200) );
			return false;//Ÿ���� Ʋ�ȼ�
		}break;
	}
	
	const int iTargetLen	= ::strlen(szTarget);
	const int iTitleLen		= ::strlen(szTitle);
	const int iBodyLen		= ::strlen(szBody);

	if( (GUILD_MAIL != iType) && !iTargetLen)
	{
		::AddCurrentStatusMessage( 250, 100, 100, lan->OutputMessage(5,201) );
		
		return false;
	}

	if(	!iTitleLen)
	{
		::AddCurrentStatusMessage( 250, 100, 100, lan->OutputMessage(5,202));
		return false;
	}

	if(	!iBodyLen)
	{
		::AddCurrentStatusMessage( 250, 100, 100, lan->OutputMessage(5,203));
		return false;
	}

	//������ �����ϴ�.
#ifdef _DEBUG
    AddCurrentStatusMessage( 250, 100, 100, "Mail Send");
#endif

	//������ ��������~.. 
	{	//< CSD-030324
//		ConvString(szTitle);
//		ConvString(szBody);
		t_packet p;

		p.h.header.type = CMD_MAIL_SEND;
		
		p.h.header.size = sizeof( MAILSEND ) - 1000 + ::strlen( szBody )+1;

		p.u.Hwoa.rang.MailSend.iType = iType;
		
		::strcpy(p.u.Hwoa.rang.MailSend.szTarget, szTarget);
		
		if(GUILD_MAIL == iType)
		{
			::strcpy(p.u.Hwoa.rang.MailSend.szTarget, " ");
		}
		
		::strcpy(p.u.Hwoa.rang.MailSend.szSender, Hero->name);
		::strcpy(p.u.Hwoa.rang.MailSend.szTitle, szTitle);
		::strcpy(p.u.Hwoa.rang.MailSend.szBody, szBody );
		
		::QueuePacket( &p, 1 );
	}
	return true;
}

void CMailMgr::SendDelete(const int iCallType)//���� ���õ� ����Ʈ�� ����ϴ�.
{//Force �� 1 �̰� �ε����� �Ѿ���� ������ �����ϴ�.
	
	t_packet p;
	p.h.header.type = CMD_MAIL_DELETE;
	p.h.header.size = sizeof( MAILDELETE );
	::strcpy(p.u.Hwoa.rang.MailDelete.szName, Hero->name);

	ZeroMemory( p.u.Hwoa.rang.MailDelete.aiMailIndex,sizeof(int)*MAX_MAIL_PAGE);

	bool bIsCanSend = false;
	if(iCallType)//�����ִ� ������
	{
		p.u.Hwoa.rang.MailDelete.aiMailIndex[0]  = GetTempMailIndex();
		bIsCanSend = true;
	}
	else
	{
		for(int i = 0; MAX_MAIL_PAGE > i ; i++)
		{
			if(m_MailList[i].bIsSelect)//����Ʈ �ȳ���
			{
				p.u.Hwoa.rang.MailDelete.aiMailIndex[i] = GetMailIndex(i);
				bIsCanSend = true;
			}
		}
	}
	if(bIsCanSend)
	{
		::QueuePacket( &p, 1 );
		SendReqMailList(m_iNowViewType,GetNowViewPageNo());
	}
}//�����(����Ʈ ��ȣ�� �����ϴ�.)

void CMailMgr::SendRepair()//���� ���õ� ����Ʈ�� �����մϴ�.
{//Force �� 1 �̰� �ε����� �Ѿ���� ������ �����ϴ�.
	t_packet p;
	p.h.header.type = CMD_MAIL_REPAIR;
	p.h.header.size = sizeof( MAILDELETE );
	::strcpy(p.u.Hwoa.rang.MailDelete.szName, Hero->name);

	ZeroMemory( p.u.Hwoa.rang.MailDelete.aiMailIndex,sizeof(int)*MAX_MAIL_PAGE);

	bool bIsCanSend = false;

	for(int i = 0; MAX_MAIL_PAGE > i ; i++)
	{
		if(m_MailList[i].bIsSelect)//����Ʈ �ȳ���
		{
			p.u.Hwoa.rang.MailDelete.aiMailIndex[i] = GetMailIndex(i);
			bIsCanSend = true;
		}
	}

	if(bIsCanSend)
	{
		::QueuePacket( &p, 1 );
		SendReqMailList(m_iNowViewType,GetNowViewPageNo());
	}
}//�����(����Ʈ ��ȣ�� �����ϴ�.)

void CMailMgr::RequestBody(const int iMailIndex)//�ش� ������ ���� �ޱ�
{
//COMMENT	::AddCurrentStatusMessage( 250, 100, 100, "Req Mail Body, INDEX [%d]",iMailIndex);
//	ConvString(szTitle);
//	ConvString(szBody);
	t_packet p;

	p.h.header.type = CMD_MAIL_REQ_BODY;
	
	p.h.header.size = sizeof( REQESTMAILBODY );
	
	p.u.Hwoa.rang.ReqestMailBody.iMailIndex = iMailIndex;
	::strcpy(p.u.Hwoa.rang.ReqestMailBody.szName, Hero->name);

//	aiMailIndex[MAX_MAIL_PAGE];
	::QueuePacket( &p, 1 );
}

void CMailMgr::RecvBody(t_packet *p)//�ش� ������ ���� �ޱ�
{
//COMMENT	::AddCurrentStatusMessage( 250, 100, 100, "Recv Mail Body");
	ClearTempData();
	
	m_TempMailData;//����� ������ �ϰ� ��� �ݽô�.
	m_TempMailBody;//�ް� ���� �ӽ� ����

	LPREQESTMAILBODYRESULT pRMBR = &p->u.Hwoa.rang.ReqestMailBodyResult;

	m_TempMailData.iIndex = pRMBR->iMailIndex;

	m_TempMailData.iType = pRMBR->iType;
	m_TempMailData.bIsRead = true;
	
	::EatRearWhiteChar(pRMBR->szSender);
	::strcpy(m_TempMailData.szSender, pRMBR->szSender);

	::EatRearWhiteChar(pRMBR->szReceiver);
	::strcpy(m_TempMailData.szReceiver,pRMBR->szReceiver);

	::EatRearWhiteChar(pRMBR->szTitle);
	::strcpy(m_TempMailData.szTitle,pRMBR->szTitle);
	
	::EatRearWhiteChar(pRMBR->szBody);
	::strcpy(m_TempMailBody.szBody, pRMBR->szBody);

	m_TempMailData.date = pRMBR->date;
	
	::CallSmallMenu(MN_MAIL_READ);
}

void CMailMgr::SaveMailToLocal()//���õ� ���� ���� �ϱ�.
{

//COMMENT	::AddCurrentStatusMessage( 250, 100, 100, "Mail Save To Pc");
	/*const char szMailSaveFolder[] = "/MyMail";
	char szTargetFolder[MAX_PATH] = {0,};

	::sprintf(szTargetFolder,"%s%s",GetCurrentWorkingDirectory(),szMailSaveFolder);

    std::__fs::filesystem::create_directory(szTargetFolder);
	
	//���� ���̺� �մϴ�.
	
	char szTime[MAX_PATH] = {0,};
	GetTempMailDate(false,szTime);

	char szMailName[MAX_PATH] = {0,};
	
	static int iCt = 0;
	::sprintf(szMailName,"%s/From %s %s(%d).txt",
		szTargetFolder,GetTempMailSender(),	szTime,iCt++);
	
	FILE *fp = ::fopen(szMailName, "wt");
	if(fp)
	{
		::fprintf(fp,"Title:\n");
		::fprintf(fp,"%s\n",GetTempMailTitle());
		::fprintf(fp,"\n");

		::fprintf(fp,"From:" );
		::fprintf(fp,"	%s\n",GetTempMailSender());
		::fprintf(fp,"\n");

		::fprintf(fp,"Date:" );
		char szTime[MAX_PATH] = {0,};
		GetTempMailDate(true,szTime);
		::fprintf(fp,"	%s\n",szTime);
		::fprintf(fp,"\n");

		::fprintf(fp,"%s\n",GetTempMailBody());
		::fclose(fp);
		::AddCurrentStatusMessage( FONT_COLOR_RARE_MAIN, lan->OutputMessage(5,204),szTargetFolder);
		
	}
	else
	{
		::AddCurrentStatusMessage( FONT_COLOR_SOLID_RED, lan->OutputMessage(5,205));
	}*/
}

void CMailMgr::ClearTempData()
{
	ZeroMemory(&m_TempMailData, sizeof(MAILDATA));
	ZeroMemory(&m_TempMailBody, sizeof(MAILBODY));
}

void CMailMgr::Clear()
{
	SetExistNotRead(false);
	m_iNowViewType = MVT_RECV_LIST;
	ZeroMemory(m_MailList, sizeof(INTERFACEMAILDATA)*MAX_MAIL_PAGE);
}

void CMailMgr::SendReqMailList(const int eViewType, const int iPageIndex)//�ε��� ������ ��
{
	if( 0 > iPageIndex || 5 <= iPageIndex)
	{
//COMMENT	::AddCurrentStatusMessage( 250, 100, 100, "Page Index Too Over/Down");
		return;
	}
	::CloseAllMenu();
	Clear();
	switch(eViewType)//eMAIL_VIEW_TYPE
	{
	case MVT_RECV_LIST:
		{
		}break;
	case MVT_DELETE_LIST:
		{
		}break;
	default:
		{
//COMMENT	::AddCurrentStatusMessage( 250, 100, 100, "View Type Error");
			return;
		}break;
	}

	t_packet packet;
	packet.h.header.type = CMD_MAIL_REQUEST_LIST;
	packet.h.header.size = sizeof(REQESTMAILLIST);
	
	packet.u.Hwoa.rang.ReqestMailList.iPageIndex = iPageIndex;
	packet.u.Hwoa.rang.ReqestMailList.iWantMailType = eViewType;
	
	::strcpy(packet.u.Hwoa.rang.ReqestMailList.szName,Hero->name);
	
	packet.u.Hwoa.rang.ReqestMailList.szName[MAX_NAME-1] = NULL;
	::QueuePacket(&packet,1);

//COMMENT	RecvMailList( (eMAIL_VIEW_TYPE)eViewType);
}

void CMailMgr::SetInterFaceMailData(const int iMailListIndex, const bool bIsSelect , LPMAILDATA pMailData)
{
	if(!CheckIndex(iMailListIndex))
	{
		return;
	}

	m_MailList[iMailListIndex].MailData = *pMailData;
}

void CMailMgr::RecvMailList(t_packet *p)
{
//COMMENT	::AddCurrentStatusMessage( 250, 100, 100, "Call Mail Main Menu");
	Clear();	
	ClearTempData();
	
	LPMAILLISTRESULT pMLR = &p->u.Hwoa.rang.MailListResult;

	const int iViewType =  pMLR->iWantViewType;

	for(int i = 0; MAX_MAIL_PAGE > i; i++)
	{
		SetInterFaceMailData(i, false , &pMLR->MailData[i]);
	}

	switch(iViewType)//eMAIL_VIEW_TYPE
	{
	case MVT_RECV_LIST:
		{
			::SMenuReg(MN_MAIL_MENU, 400, 0, MAIL_MENU,0,11,0);

			::SMenuFTReg(MN_MAIL_MENU,0,FT_NONE, SWD_CALL_MAILMENU, //���� ��Ȱ��ȭ ��Ų��
			20,61,73,19, MAIL_MENU, 49, 49,MVT_RECV_LIST);

			::SMenuFTReg(MN_MAIL_MENU,1,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_MAILMENU, //���� Ȱ��ȭ ��Ų��
			98,61,72,19, MAIL_MENU, 50, 50,MVT_DELETE_LIST);

			//���� ����
			::SMenuFTReg(MN_MAIL_MENU,2,FT_NONE, SWD_MAIL_REPAIR, //��Ȱ��ȭ
			186,442 ,78,30, MAIL_MENU, 51, 52,MVT_DELETE_LIST);
		}break;
	case MVT_DELETE_LIST:
		{
			::SMenuReg(MN_MAIL_MENU, 400, 0, MAIL_MENU,1,11,0);

			::SMenuFTReg(MN_MAIL_MENU,0,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_CALL_MAILMENU, //���� Ȱ��ȭ ��Ų��
			20,61,73,19, MAIL_MENU, 49, 49,MVT_RECV_LIST);

			::SMenuFTReg(MN_MAIL_MENU,1,FT_NONE, SWD_CALL_MAILMENU, //���� ��Ȱ��ȭ ��Ų��
			98,61,72,19, MAIL_MENU, 50, 50,MVT_DELETE_LIST);

			//���� ����
			::SMenuFTReg(MN_MAIL_MENU,2,FT_HIDE_SPECIAL_WILLDO_AUTO_PUT, SWD_MAIL_REPAIR, //Ȱ��ȭ
			186,442 ,78,30, MAIL_MENU, 51, 52,MVT_DELETE_LIST);
		}break;
	default:
		{
//COMMENT	::AddCurrentStatusMessage( 250, 100, 100, "Mail List is Wrong!! Call Developer");
		}break;
	}
	m_iNowViewType = iViewType;
	m_iNowViewPageNo = pMLR->iWantViewPage;
	CallSmallMenu(MN_MAIL_MENU);
//	SMenu[MN_MAIL_MENU].bActive = true;
}

void CMailMgr::SendReqOtherPage(const int iPageFlag)
{
	switch(iPageFlag )
	{
	case SCROLL_UP://�����ִ� �ְ� �ε��� ���� ��û
		{
			SendReqMailList(m_iNowViewType, GetNowViewPageNo()+1);//�ε��� ������ ��
		}break;
	case SCROLL_DOWN://�����ִ� ������ �ε��� �Ʒ��� ��û
		{
			SendReqMailList(m_iNowViewType, GetNowViewPageNo()-1);//�ε��� ������ ��
		}break;
	}
}

void CMailMgr::SelectAll()
{
	for(int i = 0; MAX_MAIL_PAGE > i; i++)
	{
		SetSelected(i,true);
	}
}

void CMailMgr::SetTempMailType(const int iMailType)
{
	m_TempMailData.iType = iMailType;
}

void CMailMgr::SetTempMailReceiver(const char* szName)
{
	const int iNameLen = ::strlen(szName);
	if(iNameLen >= MAX_NAME)
	{
		return;
	}
	::strcpy(m_TempMailData.szReceiver,szName);
}

void CMailMgr::IncTempMailType()
{
	const int iMailType = GetTempMailType();
	switch(iMailType)
	{
	case NORMAL_MAIL:
		{
			if(Hero->GetGuildCode())
			{
				SetTempMailType(GUILD_MAIL);
			}
			else
			{
//COMMENT			::AddCurrentStatusMessage( 250, 100, 100, "You Don't Have Guild");
			}
		}break;
	case GUILD_MAIL:
		{//���� ������ ������ �ȵǰԲ� �� ó��
			if(Hero->IsCounselor())
			{
				SetTempMailType(GAMEMASTER_MAIL);
			}
			else
			{
				SetTempMailType(NORMAL_MAIL);
			}
		}break;
	case GAMEMASTER_MAIL:
		{
			SetTempMailType(NORMAL_MAIL);
		}break;
	case NORMAL_MAIL_REPLY:
	case GAMEMASTER_MAIL_REPLY:
		{//���ö��� ������ Ÿ���� ������ �ʽ��ϴ�.
		}break;
	default:
		{
			SetTempMailType(NORMAL_MAIL);
		}break;
	}
}
void CMailMgr::DecTempMailType()
{
	const int iMailType = GetTempMailType();
	switch(iMailType)
	{
	case NORMAL_MAIL:
		{
			if(Hero->IsCounselor())
			{
				SetTempMailType(GAMEMASTER_MAIL);
			}
			else
			{
				if(Hero->GetGuildCode())
				{
					SetTempMailType(GUILD_MAIL);
				}
				else
				{
					SetTempMailType(NORMAL_MAIL);
//COMMENT				::AddCurrentStatusMessage( 250, 100, 100, "You Don't Have Guild");
				}				
			}
		}break;
	case GUILD_MAIL:
		{//���� ������ ������ �ȵǰԲ� �� ó��
			SetTempMailType(NORMAL_MAIL);
		}break;
	case GAMEMASTER_MAIL:
		{
			if(Hero->GetGuildCode())
			{
				SetTempMailType(GUILD_MAIL);
			}
			else
			{
				SetTempMailType(NORMAL_MAIL);
//COMMENT			::AddCurrentStatusMessage( 250, 100, 100, "You Don't Have Guild");
			}			
		}break;
	case NORMAL_MAIL_REPLY:
	case GAMEMASTER_MAIL_REPLY:
		{//���ö��� ������ Ÿ���� ������ �ʽ��ϴ�.
		}break;
	default:
		{
			SetTempMailType(NORMAL_MAIL);
		}break;
	}
}

const int CMailMgr::GetTempMailIndex()
{
	return m_TempMailData.iIndex;
}

const int CMailMgr::GetTempMailType()
{
	switch(m_TempMailData.iType)
	{
	case NORMAL_MAIL:
	case GUILD_MAIL:
	case GAMEMASTER_MAIL:
	case NORMAL_MAIL_REPLY:
	case GAMEMASTER_MAIL_REPLY:
		{
			return m_TempMailData.iType;
		}break;
	default:
		{
			return NORMAL_MAIL;
		}break;
	}
	
	return NORMAL_MAIL;
}

char* CMailMgr::GetTempMailSender()
{
//	return "ȭ������";
	return m_TempMailData.szSender;
}

char* CMailMgr::GetTempMailTitle()
{
//	return "���� �����̷���~";
	return m_TempMailData.szTitle;
}

bool CMailMgr::GetTempMailDate(const bool bIsDetail,char *szTime)
{
	TIMESTAMP_STRUCT *pTS = &m_TempMailData.date;

	if(bIsDetail)
	{
		::sprintf(szTime,"%04d-%02d-%02d %02d:%02d:%02d",
			pTS->year,pTS->month,pTS->day,
			pTS->hour,pTS->minute,pTS->second);
	}
	else
	{
		::sprintf(szTime,"%02d-%02d-%02d",pTS->year%100,pTS->month,pTS->day);
	}
	return true;
}

char* CMailMgr::GetTempMailBody()
{
	return m_TempMailBody.szBody;
}


const int CMailMgr::GetMailIndex(const int iMailListIndex)
{
	if(!CheckIndex(iMailListIndex)){return 0;}
	return m_MailList[iMailListIndex].MailData.iIndex;
}

const bool CMailMgr::GetMailIsRead(const int iMailListIndex)
{
	if(!CheckIndex(iMailListIndex)){return 0;}
	return m_MailList[iMailListIndex].MailData.bIsRead;
}

const int CMailMgr::GetMailType(const int iMailListIndex)
{
	if(!CheckIndex(iMailListIndex)){return 0;}
	return m_MailList[iMailListIndex].MailData.iType;
}

const char* CMailMgr::GetMailSender(const int iMailListIndex)
{
//	return "ȭ������";
	if(!CheckIndex(iMailListIndex)){return "";}
	return m_MailList[iMailListIndex].MailData.szSender;
}

const char* CMailMgr::GetMailTitle(const int iMailListIndex)
{
//	return "���� �����̷���~";
	if(!CheckIndex(iMailListIndex)){return "";}
	return m_MailList[iMailListIndex].MailData.szTitle;
}

const bool CMailMgr::GetMailDate(const int iMailListIndex, const bool bIsDetail, char* szTime)
{
//	return "03-03-03";
	::strcpy(szTime,"");
	if(!CheckIndex(iMailListIndex)){return false;}
	if(!GetMailIndex(iMailListIndex)){return false;}
	
	TIMESTAMP_STRUCT *pTS = &m_MailList[iMailListIndex].MailData.date;

	if(bIsDetail)
	{
		::sprintf(szTime,"%04d-%02d-%02d %02d:%02d:%02d",
			pTS->year,pTS->month,pTS->day,
			pTS->hour,pTS->minute,pTS->second);
//		SQLUSMALLINT   hour;
//      SQLUSMALLINT   minute;
//    SQLUSMALLINT   second;
	}
	else
	{
		::sprintf(szTime,"%02d-%02d-%02d",pTS->year%100,pTS->month,pTS->day);
	}
	return true;
}

const bool CMailMgr::IsSelected(const int iMailListIndex)
{
	if(!CheckIndex(iMailListIndex)){return 0;}
	return m_MailList[iMailListIndex].bIsSelect;
}

const bool CMailMgr::IsRead(const int iMailListIndex)
{
	if(!CheckIndex(iMailListIndex))	{return false;}
	return m_MailList[iMailListIndex].MailData.bIsRead;
}

void CMailMgr::SetSelected(const int iMailListIndex, const bool bIsSelect)
{
//COMMENT	::AddCurrentStatusMessage( 250, 100, 100, "Set Check Button State");
	if(!CheckIndex(iMailListIndex))	{return;}
	if(!m_MailList[iMailListIndex].MailData.iIndex)
	{
//COMMENT	::AddCurrentStatusMessage( 250, 100, 100, "There Is No Mail So You Can't Select!");
		m_MailList[iMailListIndex].bIsSelect = false;	
		return;
	}
	m_MailList[iMailListIndex].bIsSelect = bIsSelect;	
}

void CMailMgr::RecvAlert()
{
	::MP3(63);
	::AddCurrentStatusMessage( FONT_COLOR_WHITE, lan->OutputMessage(4,24));
	
	SetExistNotRead(true);
}

void CMailMgr::RecvSendResult(int cmd_type)
{
	switch( cmd_type )
	{
	case CMD_SEND_MAIL_OK : MP3( SN_SEND_MAIL );
							AddCurrentStatusMessage( 255, 0, 255, lan->OutputMessage(5,51) );//010215 lsw
							break;
	case CMD_SEND_MAIL_FAIL:MP3( SN_WARNING );
							AddCurrentStatusMessage( 255, 0, 0,lan->OutputMessage(5,52));//010215 lsw
							break;
	}
}

inline bool IsMailMenuOpen()
{
	return (	SMenu[MN_MAIL_MENU	].bActive ||
				SMenu[MN_MAIL_WRITE	].bActive ||
				SMenu[MN_MAIL_READ	].bActive);
}

void Call_MAIL_WRITE_MENU(const char* szTargetName, const bool bIsReply)//���� ���� �޴� ȣ��
{
//COMMENT	::AddCurrentStatusMessage( 250, 100, 100, "Call Mail Write Menu.");
	SetChatMode(CM_COMMAND);//020720 lsw
	
	::CallSmallMenu(MN_MAIL_WRITE);//���ο��� ����Ʈ �ڽ��� Ŭ���� ��ŵ�ϴ�.

	EWndMgr.showInputBox(HWND_MAIL3);
	EWndMgr.showInputBox(HWND_MAIL2);
	EWndMgr.showInputBox(HWND_MAIL1);

	if(szTargetName)
	{
		EWndMgr.SetTxt(HWND_MAIL1,szTargetName);//Ŭ����� ���ķ� ���� �ؾ� �ϱ� ������

		SMenu[MN_MAIL_WRITE].CheakType = 2;
	}
	else
	{
		SMenu[MN_MAIL_WRITE].CheakType = 0;
	}

	g_MailMgr.ClearTempData();

	if(bIsReply)
	{//�����̸�
		switch(g_MailMgr.GetTempMailType())
		{
		case CMailMgr::GAMEMASTER_MAIL:
		case CMailMgr::GAMEMASTER_MAIL_REPLY:
			{
				g_MailMgr.SetTempMailType(CMailMgr::GAMEMASTER_MAIL_REPLY);
			}break;
		case CMailMgr::NORMAL_MAIL:
		case CMailMgr::GUILD_MAIL:
		case CMailMgr::NORMAL_MAIL_REPLY:
		default:
			{
				g_MailMgr.SetTempMailType(CMailMgr::NORMAL_MAIL_REPLY);
			}break;
		}

		if(Hero->IsCounselor())//���� ��ڸ� ��� ������ ���Ӹ����� ���Ϸ� ����
		{
			g_MailMgr.SetTempMailType(CMailMgr::GAMEMASTER_MAIL_REPLY);
		}
	}
	else
	{//�Ϲ� �����̸�
		g_MailMgr.SetTempMailType(CMailMgr::NORMAL_MAIL);

		if(Hero->IsCounselor())//���� ��ڸ� ��� ������ ���Ӹ����� ���Ϸ� ����
		{
			g_MailMgr.SetTempMailType(CMailMgr::GAMEMASTER_MAIL);
		}
	}
}

void RecvGuildMailDeleteResult( t_packet *p )
{
	int result = p->u.kein.default_char;
	switch( result )
	{
	case 0 : Kein_PutMessage( KM_FAIL, kein_GetMenuString(49) );
	}
}

void SendRegistFriend( const char *name )
{
	if (std::string(name).find('\'') != std::string::npos) {
		// 如果找到了單引號 (npos 以外的任何回傳值)，就 return
		return;
	}
	k_regist_friend data;
	data.nation = Hero->name_status.nation;
	strcpy( data.name1 , Hero->name );
	strcpy( data.name2, name );
	SendDirectDB( CMD_REGIST_FRIEND, &data, sizeof( k_regist_friend ) );
}

void RegistFriend(const char *szName)
{
	if(!szName || !::strlen(szName))
	{
//COMMENT ::AddCurrentStatusMessage( 250, 100, 100, "RegistFriend() Name Is Wrong");
		return;
	}

	int ret = g_Menu_Variable.m_FriendGrup->SerchName( szName );
	if( ret != -1 )
	{
		Kein_PutMessage( KM_FAIL, kein_GetMenuString( 102 ) ); // �̹� ����� ����Դϴ�.
	}
	else
	{
		SendRegistFriend( szName );		// ���� ���
	}
}