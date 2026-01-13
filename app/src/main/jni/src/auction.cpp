// Auction.cpp: implementation of the CAuction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Auction.h"
#include "Menu.h"
#include "dragon.h"
#include "gameproc.h"
#include "MenuSet.h"
#include "Chat.h"
#include "Skill.h"
#include "CurrentMsgMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CAuction Auction;

const SUPERMONEY SM1(BOND_MONEY_1000	,1000000000);
const SUPERMONEY SM2(BOND_MONEY_500		,500000000);
const SUPERMONEY SM3(BOND_MONEY_100		,100000000);
const SUPERMONEY SM4(BOND_MONEY_50		,50000000);
const SUPERMONEY SM5(BOND_MONEY_10		,10000000);
const SUPERMONEY SM6(BOND_MONEY_5		,5000000);

const SUPERMONEY aBondMoney[BOND_MONEY_MAX] = {SM1,SM2,SM3,SM4,SM5,SM6};


CAuction::CAuction()
{
	Clear();
}

CAuction::~CAuction()
{
	Clear();
}

void CAuction::Clear()
{
	ClearFK();
	::memset(&m_SearchList,0,sizeof(SEARCHRESULTLIST));
	m_iWaitPageRefresh = 0;
	m_iBuyerViewPage = 0;
	m_iSellerViewPage = 0;
	m_iResultViewPage = 0;
	m_iDeleteItemIndex= 0;
	m_iBuyItemIndex = 0;
	m_iTakeItemIndex= 0;

	this->m_nPeriod = 0;

	strcpy(m_szKeyWord,"");
	strcpy(m_szMerchantName,"");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�����ڰ� ������ �˻� ����
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAuction::SendCMD_MERCHANT_BUY_LIST_REQUEST(const int iIndex, const int iNeedIndexOver)//����Ʈ ��û(�����ڰ� ���°�//�˻��Ⱑ ȣ��
{
	//soto-030611 ��޼���.
//	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,82));
//	return;
	
	

	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,500));
	//��Ŷ ������
	t_packet p;
	p.h.header.type = CMD_MERCHANT_BUY_ITEM_SEARCH;
	p.h.header.size = sizeof(SEARCHPACKETCLIENT);
		
	if(m_bChkKeyWord)
	{
		strcpy(p.u.SearchPacketClient.szKeyWord,m_szKeyWord);
	}
	else
	{
		strcpy(p.u.SearchPacketClient.szKeyWord,"");
	}

	if(m_bChkMerchantName)
	{
		strcpy(p.u.SearchPacketClient.szMerchant,m_szMerchantName);
	}
	else
	{
		strcpy(p.u.SearchPacketClient.szMerchant,"");
	}	
	p.u.SearchPacketClient.nPeriod  = m_nPeriod;
	p.u.SearchPacketClient.iIndex	= iIndex;
	p.u.SearchPacketClient.iKey		= iNeedIndexOver;

	m_iWaitPageRefresh = 1;

	::QueuePacket(&p,1);
	::CloseAllMenu();

	//>soto-030511 ���� �ؾ���.	
}

void CAuction::RecvCMD_MERCHANT_BUY_LIST_REQUEST_RESULT(t_packet &p)//����Ʈ ���(�����ڰ� ���°�
{
	Clear();
	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,502));
	m_SearchList = p.u.SearchResultList;
	m_iBuyerViewPage  = (p.u.SearchResultList.iKey)?AUCTION_PAGE_MIN:AUCTION_PAGE_MAX;
	m_iWaitPageRefresh = 0;

	::CallSmallMenu(MN_MERCHANT_BUY);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�����ڰ� ������ �˻� �ϱ�
////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�����ϱ� ����
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAuction::SendCMD_MERCHANT_ITEM_BUY(SEARCHRESULT &SR)//�����ϱ�(�����ڰ� ����
{
	if(!strcmp( SR.szSellerName,Hero->name))
	{
		::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,503));
		return;
	}

	int iChatTarget = CHAT_TARGET_NORMAL;
	if(!SR.iSellValue)
	{
		::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,504));
		iChatTarget = ::InsertWisperList(SR.szSellerName);
	}

	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,505));

	t_packet p;
	p.h.header.type = CMD_MERCHANT_ITEM_BUY;
	p.h.header.size = sizeof(MERCHANT_ITEM_BUY);

	p.u.MerchantItemBuy.iIndex = SR.iIndex;//������ �������� �ε��� ��
	p.u.MerchantItemBuy.iCn = Hero->id;//Ŀ�ؼ� ��
	strcpy(p.u.MerchantItemBuy.szSellerName,SR.szSellerName);
	strcpy(p.u.MerchantItemBuy.szBuyerName,Hero->name);//������ �̸�
	p.u.MerchantItemBuy.dwSellValue = SR.iSellValue;//�Ǹűݾ��� 20���� �ѱ��� �ʽ��ϴ�.
	p.u.MerchantItemBuy.SellItem	= SR.m_ResultItem;
	p.u.MerchantItemBuy.iKey = 0;
	::QueuePacket(&p,1);
	CloseAllMenu();
	if(!SR.iSellValue)
	{
		::SetChatMode(CM_MESSAGE);
		::SetChatTarget(iChatTarget);
	}
}

void CAuction::RecvCMD_MERCHANT_ITEM_BUY_RESULT(t_packet &p)//�����ϱ�(�����ڰ� ����
{//���Ű���� �´�
	//�˾Ƽ� ó�� �Ѥ�;.//iKey ������ �Ǻ� �մϴ�.//�������⸦ �ٶ�� ���� //���� �� �� �ִ� ����
	Clear();
	const int iResult = p.u.MerchantItemBuy.iKey;
	switch(iResult)
	{
	case IS_END_WAIT_TAKE://���� �Ϸ� ����
		{
			::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,506));
		}break;
	case IS_END_ALL_RIGHT://������ ���³�.. ���� �Ұ��� �߳�����
		{
			::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,507));
		}break;
	default://
		{
			::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,508),iResult);
		}break;
	/*
	case IS_END_DELETE_COMPLETE:
	case IS_END_DELETING:
		{
			COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE,"���Կ� ���� �߽��ϴ�. �Ǹ��ڰ� ����� �ֽ��ϴ�.");
		}break;
	case IS_END_BUYING:
		{
			COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE,"�������� �������Դϴ�.");
		}break;
	*/
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�����ϱ� ��
////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���� �Ǹ��ϴ� ����Ʈ ��û ����
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAuction::SendCMD_MERCHANT_SELL_LIST_REQUEST(const int iIndex, const int iNeedIndexOver)//����Ʈ ��û(�Ǹ��ڰ� ���°�
{	
	/*if (!::IsMerchant())
	{
		::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,509));
		return;
	}*/
	if(iIndex && !iNeedIndexOver)
	{
		if(AUCTION_PAGE_MIN == GetSellerViewPage())
		{
			for(int xx = 0; MAX_VIEW_ABLE_PER_PAGE > xx; xx++)
			{
				if(!m_SearchList.ResultList[xx].m_ResultItem.item_no)
				{
					::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,510));
					return;
				}
			}		
		}
	}

	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,511));
	t_packet p;
	p.h.header.type = CMD_MERCHANT_SELL_ITEM_INFO_REQUEST;
	p.h.header.size = sizeof(SELLERITEMREQUEST);
	p.u.SellerItemRequest.iCn = Hero->id;
	strcpy(p.u.SellerItemRequest.szName,Hero->name);

	p.u.SellerItemRequest.iIndex = iIndex;
	p.u.SellerItemRequest.iKey = iNeedIndexOver;
	m_iWaitPageRefresh = 1;
	::QueuePacket(&p,1);
}

void CAuction::RecvCMD_MERCHANT_SELL_LIST_REQUEST_RESULT(t_packet &p)//����Ʈ ���(�Ǹ��ڰ� ���°�
{
	Clear();

	/*if (!::IsMerchant())
	{
		::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,509));
		return;
	}*/

	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,512));
	m_SearchList = p.u.SearchResultList;
	if(p.u.SearchResultList.iKey)
	{
		m_iSellerViewPage  = AUCTION_PAGE_MIN;
	}
	else
	{
		if(m_SearchList.ResultList[0].m_ResultItem.item_no)
		{
			m_iSellerViewPage = AUCTION_PAGE_MIN;
		}
		if(m_SearchList.ResultList[6].m_ResultItem.item_no)
		{
			m_iSellerViewPage = AUCTION_PAGE_MIN+1;
		}
		if(m_SearchList.ResultList[12].m_ResultItem.item_no)
		{
			m_iSellerViewPage = AUCTION_PAGE_MIN+2;
		}
		if(m_SearchList.ResultList[18].m_ResultItem.item_no)
		{
			m_iSellerViewPage = AUCTION_PAGE_MAX;
		}
	}
	m_iWaitPageRefresh = 0;
	::CallSmallMenu(MN_MERCHANT_SELL_INFO);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���� �Ǹ��ϴ� ����Ʈ ��û ��
////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�ŷ��� �Ǹ���� ����
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAuction::SendCMD_MERCHANT_SELL_ITEM_DELETE()//�Ǹ� �������� ����
{
	t_packet p;
	p.h.header.type = CMD_MERCHANT_SELL_ITEM_DELETE;
	p.h.header.size = sizeof(SELLERITEMDELETE);
	
	SEARCHRESULT *SR = NULL;
	int i;
	for(i = 0; 24 > i; i++)
	{
		if(m_iDeleteItemIndex == m_SearchList.ResultList[i].iIndex )
		{
			SR = &m_SearchList.ResultList[i];
			break;
		}
	}
	if( !m_iDeleteItemIndex || 24 == i )
	{
		::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,513));
		return;
	}
	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,514));
	p.u.SellerItemDelete.iIndex = SR->iIndex;//�ǸŹ�ǰ�� �ε���
	p.u.SellerItemDelete.iCn	= Hero->id;
	strcpy(p.u.SellerItemDelete.szName,SR->szSellerName);	
	p.u.SellerItemDelete.dwSellValue  = SR->iSellValue;
	p.u.SellerItemDelete.SellItem  = SR->m_ResultItem;
	p.u.SellerItemDelete.iKey = 0;
	::QueuePacket(&p,1);
}

void CAuction::RecvCMD_MERCHANT_SELL_ITEM_DELETE_RESULT(t_packet &p)//�Ǹ� �������� ����
{
	Clear();
	if(IS_END_DELETE_COMPLETE == p.u.SellerItemDelete.iKey)
	{//���������� ���� �޾ҽ��ϴ�.
		::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,515));
	}
	else
	{
		::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,516));
	}
	SendCMD_MERCHANT_SELL_LIST_REQUEST();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�ŷ��� �Ǹ���� ��
////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�ŷ��� ����ϱ� ����
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAuction::SendCMD_MERCHANT_SELL_ITEM_REGISTER()//�Ǹ� �������� ���
{//�Ǹž������� ��� �մϴ�.
	t_packet p;
	p.h.header.type = CMD_MERCHANT_SELL_ITEM_REGISTER;
	p.h.header.size = sizeof(SELLERITEMREGISTER);
	
	p.u.SellerItemRegister.iCn = Hero->id;
	strcpy(p.u.SellerItemRegister.szName,Hero->name);	

	p.u.SellerItemRegister.pos		= m_SellItemPos;//pos ��
	p.u.SellerItemRegister.SellItem	= m_SellItemAttr;//������ ������
	
	p.u.SellerItemRegister.dwSellValue = m_iSellValue;
//	if(!m_iSellValue)
//	{
//		COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE,"�����ŷ��� ��� �Ǿ����ϴ�.");
//		COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE,"������ �������� �ʴ� ����Դϴ�.");
//		return;
//	}
	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,517));
	//��Ŷ �� �����ؼ� ������
	::QueuePacket(&p,1);
}

void CAuction::RecvCMD_MERCHANT_SELL_ITEM_REGISTER_RESULT(t_packet &p)//�Ǹ� �������� ���
{
	Clear();
	//�� ����Ʈ�� �ٽ� ��û �ϴ°��� �ֽ��ϴ�.
	if(1==p.u.SellerItemRegister.iKey)
	{
		::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,518));
	}
	else
	{
		::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,519));
	}
	SendCMD_MERCHANT_SELL_LIST_REQUEST();//�ٽ� ���� �Ǹ� ��� ����Ʈ�� ��û�մϴ�.
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�ŷ��� ����ϱ� ��
////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�ŷ� ����� ����Ʈ �޾ƿ��� ����
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAuction::SendCMD_MERCHANT_RESULT_LIST_REQUEST(const int iIndex, const int iNeedIndexOver)
{
	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,520));
	t_packet p;
	p.h.header.type = CMD_MERCHANT_RESULT_LIST_REQUEST;
	p.h.header.size = sizeof(SELLERITEMREQUEST);
	p.u.SellerItemRequest.iCn = Hero->id;
	strcpy(p.u.SellerItemRequest.szName,Hero->name);
	p.u.SellerItemRequest.iIndex = iIndex;
	p.u.SellerItemRequest.iKey = iNeedIndexOver;
	m_iWaitPageRefresh = 1;
	::QueuePacket(&p,1);
}

void CAuction::RecvCMD_MERCHANT_RESULT_LIST_REQUEST_RESULT(t_packet &p)
{
	Clear();
	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,521));
	m_SearchList = p.u.SearchResultList;
	m_iResultViewPage = (p.u.SearchResultList.iKey)?AUCTION_PAGE_MIN:AUCTION_PAGE_MAX;
	m_iWaitPageRefresh = 0;
	::CallSmallMenu(MN_MERCHANT_RESULT_TAKE);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�ŷ� ����� ����Ʈ �޾ƿ��� ��
////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�ŷ� ����� ã�� ����
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAuction::SendCMD_MERCHANT_RESULT_TAKE(SEARCHRESULT &SR)
{
	t_packet p;
	p.h.header.type = CMD_MERCHANT_RESULT_TAKE;
	p.h.header.size = sizeof(MERCHANTRESULTTAKE);
	if(!SR.iIndex)
	{
		::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,524));
		return;
	}
	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,523));
	p.u.MerchantResultTake.iIndex	=	SR.iIndex;
	p.u.MerchantResultTake.iCn		=	Hero->id;
	strcpy(p.u.MerchantResultTake.szMyName,Hero->name);
	strcpy(p.u.MerchantResultTake.SellerName,SR.szSellerName);
	strcpy(p.u.MerchantResultTake.BuyerName,SR.szBuyerName);
	
	p.u.MerchantResultTake.dwSellValue	= SR.iSellValue;
	p.u.MerchantResultTake.SellItem		= SR.m_ResultItem;
	p.u.MerchantResultTake.iSellerTake	= 0;
	p.u.MerchantResultTake.iBuyerTake	= 0;
	p.u.MerchantResultTake.iKey			= 0;

	//�ε��� ��ȣ�����ؼ� �ڷ� ����
	::QueuePacket(&p,1);
}

void CAuction::RecvCMD_MERCHANT_RESULT_TAKE_RESULT(t_packet &p)
{
	SendCMD_MERCHANT_RESULT_LIST_REQUEST();
	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,525));
	Clear();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�ŷ� ����� ã�� ��
////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CAuction::SetSellItem(const POS &pos,const ItemAttr Item)
{
	if(INV != pos.type) 
	{
		//COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE , lan->OutputMessage(2,530));//�κ��丮 ������ ���� �ʼ�..
		return 0;
	}
	if(!Item.item_no)
	{
		return 0;
	}
	m_SellItemPos = pos;
	m_SellItemAttr = Item;
	return 1;
}

int CAuction::SetBondItem(const POS &pos,const ItemAttr Item)
{
	if(INV != pos.type) 
	{
		COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE , lan->OutputMessage(2,530));//�κ��丮 ������ ���� �ʼ�..
		return 0;
	}
	if(!Item.item_no)
	{
		return 0;
	}
	m_BondItemPos = pos;
	m_BondItemAttr = Item;
	return 1;
}

int	CAuction::IncBuyerViewPage() 
{
	SEARCHRESULT *SR = GetSearchList().ResultList;
	
	if(!m_iWaitPageRefresh && AUCTION_PAGE_MAX == GetBuyerViewPage())//3�� �Ǿ����� ���� ����Ʈ�� ���̴�..
	{	//����Ʈ ��û �������� ���� �Ծ�� �ϰ�

		const int iSendIndex = SR[MAX_SEARCH_RESULT_LIST-1].iIndex;
		if(iSendIndex)//����Ʈ�� �� ���ִ�.. �׷��߸� ���� �������� ��û����
		{
			SendCMD_MERCHANT_BUY_LIST_REQUEST(iSendIndex,true);
		}
		else
		{
			//COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,510));
			COMMENT ::AddCurrentStatusMessage(255,255,255,"ù ��� ���� �ٽ� ��û�մϴ�.");
			SendCMD_MERCHANT_BUY_LIST_REQUEST(0,true);	
		}
	}
	else
	{
		int xx =0;
		for(xx = 0; MAX_SEARCH_RESULT_LIST > xx ; xx++)
		{
			if(!SR[xx].iIndex){break;}//��� �߿� �ε����� 0�� �ڸ�ã��
		}

		if((xx)/MAX_VIEW_ABLE_PER_PAGE > GetBuyerViewPage())//���� �������� �������� 0���� ���� ������ -1�� ���ذ��̴�
		{
			m_iBuyerViewPage++;//���� ��������
		}
	}
	return 0;
}

int	CAuction::DecBuyerViewPage() 
{
	SEARCHRESULT *SR = GetSearchList().ResultList;
	if(!m_iWaitPageRefresh && AUCTION_PAGE_MIN >=GetBuyerViewPage())//0������ �϶� �ѹ��� Dec��Ű��
	{
		if(SR[0].iIndex)//������ �����µ� �ε����� 0�̸�ȵȴ�
		{
			SendCMD_MERCHANT_BUY_LIST_REQUEST(SR[0].iIndex,false);//��� ��û�� �ϴµ�
		}
		else
		{
			COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,510));
			COMMENT ::AddCurrentStatusMessage(255,255,255,"ù ��� ���� �ٽ� ��û�մϴ�.");
			SendCMD_MERCHANT_BUY_LIST_REQUEST(0,true);				
		}
	}
	else
	{
		m_iBuyerViewPage--;//���� ��������
	}
	return 0;
}

int	CAuction::IncResultViewPage() 
{
	SEARCHRESULT *SR = GetSearchList().ResultList;
	
	if(!m_iWaitPageRefresh && AUCTION_PAGE_MAX == GetResultViewPage())//3�� �Ǿ����� ���� ����Ʈ�� ���̴�..
	{	//����Ʈ ��û �������� ���� �Ծ�� �ϰ�

		const int iSendIndex = SR[MAX_SEARCH_RESULT_LIST-1].iIndex;
		if(iSendIndex)//����Ʈ�� �� ���ִ�.. �׷��߸� ���� �������� ��û����
		{
			SendCMD_MERCHANT_RESULT_LIST_REQUEST(iSendIndex,true);
		}
		else
		{
			COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,510));
			COMMENT ::AddCurrentStatusMessage(255,255,255,"ù ��� ���� �ٽ� ��û�մϴ�.");
			SendCMD_MERCHANT_RESULT_LIST_REQUEST(0,true);	
		}
	}
	else
	{
		int xx =0;
		for(xx = 0; MAX_SEARCH_RESULT_LIST > xx ; xx++)
		{
			if(!SR[xx].iIndex){break;}//��� �߿� �ε����� 0�� �ڸ�ã��
		}

		if((xx)/MAX_VIEW_ABLE_PER_PAGE > GetResultViewPage())//���� �������� �������� 0���� ���� ������ -1�� ���ذ��̴�
		{
			m_iResultViewPage++;//���� ��������
		}
	}
	return 0;
}

int	CAuction::DecResultViewPage() 
{
	SEARCHRESULT *SR = GetSearchList().ResultList;
	if(!m_iWaitPageRefresh && AUCTION_PAGE_MIN >=GetResultViewPage())//0������ �϶� �ѹ��� Dec��Ű��
	{
		if(SR[0].iIndex)//������ �����µ� �ε����� 0�̸�ȵȴ�
		{
//<! BBD 040303 �����϶��� �̻��� ����
			if(m_iResultViewPage <= 0)
			{
				m_iResultViewPage = 0;
				return 0;
			}
			else
			{
				SendCMD_MERCHANT_RESULT_LIST_REQUEST(SR[0].iIndex,false);//��� ��û�� �ϴµ�
			}
//> BBD 040303 �����϶��� �̻��� ����
		}
		else
		{
			COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,510));
			COMMENT ::AddCurrentStatusMessage(255,255,255,"ù ��� ���� �ٽ� ��û�մϴ�.");
			SendCMD_MERCHANT_RESULT_LIST_REQUEST(0,true);				
		}
	}
	else
	{
		m_iResultViewPage--;//���� ��������
	}
	return 0;
}

int	CAuction::IncSellerViewPage()
{
	SEARCHRESULT *SR = GetSearchList().ResultList;
	
	if(!m_iWaitPageRefresh && AUCTION_PAGE_MAX == GetSellerViewPage())//3�� �Ǿ����� ���� ����Ʈ�� ���̴�..
	{	//����Ʈ ��û �������� ���� �Ծ�� �ϰ�

		const int iSendIndex = SR[MAX_SEARCH_RESULT_LIST-1].iIndex;
		if(iSendIndex)//����Ʈ�� �� ���ִ�.. �׷��߸� ���� �������� ��û����
		{
			SendCMD_MERCHANT_SELL_LIST_REQUEST(iSendIndex,true);
		}
		else
		{
			COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,510));
			COMMENT ::AddCurrentStatusMessage(255,255,255,"ù ��� ���� �ٽ� ��û�մϴ�.");
			SendCMD_MERCHANT_SELL_LIST_REQUEST(0,true);	
		}
	}
	else
	{
		int xx =0;
		for(xx = 0; MAX_SEARCH_RESULT_LIST > xx ; xx++)
		{
			if(!SR[xx].iIndex){break;}//��� �߿� �ε����� 0�� �ڸ�ã��
		}

		if((xx)/MAX_VIEW_ABLE_PER_PAGE > GetSellerViewPage())//���� �������� �������� 0���� ���� ������ -1�� ���ذ��̴�
		{
			m_iSellerViewPage++;//���� ��������
		}
	}
	return 0;
}

int	CAuction::DecSellerViewPage()
{
	SEARCHRESULT *SR = GetSearchList().ResultList;
	if(!m_iWaitPageRefresh && AUCTION_PAGE_MIN >=GetSellerViewPage())//0������ �϶� �ѹ��� Dec��Ű��
	{
		if(SR[0].iIndex)//������ �����µ� �ε����� 0�̸�ȵȴ�
		{
			SendCMD_MERCHANT_SELL_LIST_REQUEST(SR[0].iIndex,false);//��� ��û�� �ϴµ�
		}
		else
		{
			COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,510));
			COMMENT ::AddCurrentStatusMessage(255,255,255,"ù ��� ���� �ٽ� ��û�մϴ�.");
			SendCMD_MERCHANT_SELL_LIST_REQUEST(0,true);
		}
	}
	else
	{
		m_iSellerViewPage--;//���� ��������
	}
	return 0;
}

int CAuction::SendCMD_MERCHANT_EXCHANGE_LIST_REQUEST(std::string szSellerName,const int iIndex, const int iNeedIndexOver,const bool bIsDirectExchange)
{//����Ʈ ��û
	#ifdef KOREA_LOCALIZING_ 
	{	//< CSD-TW-030625
		LPCHARACTER pch = ::ExistHe(szSellerName);//�� �þ߿� �ִ� ����̾�?
	//	if(!pch){return 0;}	//����� �� �þ߿� �ֱ���.
		if(!strcmp(szSellerName,Hero->name))
		{
			COMMENT ::AddCurrentStatusMessage(255,255,255,"�ڱ��ڽ��� ����� ��û �� �� �����ϴ�.");
			return 0;
		}

		t_packet p;
		p.h.header.type = CMD_MERCHANT_DIRECT_EXCHANGE_LIST_REQUSET;
		p.h.header.size = sizeof(MERCHANTEXCHANGEREQUEST);

		p.u.MerchantExchangeRequest.iIndex	=	iIndex;
		p.u.MerchantExchangeRequest.iCn		=	Hero->id;
		::strcpy(p.u.MerchantExchangeRequest.szMyName,Hero->name);
		::strcpy(p.u.MerchantExchangeRequest.szSellerName,szSellerName);
		
		p.u.MerchantExchangeRequest.iKey	= iNeedIndexOver;
	//	p.u.MerchantExchangeRequest.iKey2	= bIsDirectExchange;
		::QueuePacket(&p,1);
	}	//> CSD-TW-030625
#endif
	return 0;
}

int CAuction::RecvCMD_MERCHANT_EXCHANGE_LIST_REQUEST_RESULT(t_packet &p)
{//����Ʈ ����
	Clear();
	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,527));
	
	m_SearchList = p.u.SearchResultList;
	m_iBuyerViewPage  = ((p.u.SearchResultList.iKey)?AUCTION_PAGE_MIN:AUCTION_PAGE_MAX);
	m_iWaitPageRefresh = 0;
	::CallSmallMenu(MN_MERCHANT_DIRECT_EXCHAGE_LIST);
//	::CallSmallMenu(MN_MERCHANT_BUY);
	
	return 0;
}

void CAuction::SendCMD_EXCHANGE_BOND_MONEY()//021126 lsw
{
	if(!Auction.GetBondItemAttr())
	{
		return;
	}
	t_packet p;
	p.h.header.type =	CMD_EXCHANGE_BOND_MONEY;
	p.h.header.size =	sizeof(t_ExchangeBondMoney);
	p.u.Hwoa.rang.ExBondMoney.iCn			= Hero->id;
	p.u.Hwoa.rang.ExBondMoney.pos			= Auction.GetBondItemPos();
	p.u.Hwoa.rang.ExBondMoney.BondMoneyItem	= *Auction.GetBondItemAttr();
	p.u.Hwoa.rang.ExBondMoney.iKey = 0;
	ClearBondItem();
	::QueuePacket( &p, 1 );
}

void CAuction::IncPeriod(int nInc)
{
	m_nPeriod += nInc;
	if(m_nPeriod > 2)m_nPeriod = 2;
}

void CAuction::DecPeriod(int nDec)
{
	m_nPeriod -= nDec;
	if(m_nPeriod < 0)m_nPeriod = 0;
}
//////////////////////////////////////////////////////////////////////////////////////
// BBD 040303	��õƮ ��� ���̺��� �̿��ϴ� �Լ��� ����							//
//////////////////////////////////////////////////////////////////////////////////////
//â�� ���ڸ��� ��ϵǾ� �ִ� ����Ʈ�� ��û�Ѵ�
void CAuction::SendCMD_MERCHANT_BACKUP_LIST_REQUEST(const int iIndex, const int iNeedIndexOver)
{
	Clear();	// Ȥ�� �𸣴� ����Ʈ�� ���������� ��û�Ѵ�
	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,520));
	SELLERITEMREQUEST	data;

	data.iCn = Hero->id;
	strcpy(data.szName,Hero->name);
	data.iIndex = iIndex;
	data.iKey = iNeedIndexOver;
	m_iWaitPageRefresh = 1;

	SendDirectDB(CMD_MERCHANT_BACKUP_LIST_REQUEST, &data, sizeof(data));
}
// ��û�� ����Ʈ�� �ް� ����Ѵ�.
void CAuction::RecvCMD_MERCHANT_BACKUP_LIST_RESPONSE(t_packet &p)
{
	Clear();	// Ȥ�� �𸣴� ����Ʈ�� ������ �޾Ƽ� �������
	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,521));

	memcpy(&m_SearchList, p.u.data, sizeof(m_SearchList));

	m_iResultViewPage = (m_SearchList.iKey)?AUCTION_PAGE_MIN:AUCTION_PAGE_MAX;
	m_iWaitPageRefresh = 0;
	::CallSmallMenu(MN_MERCHANT_BACKUP_MAIN);
}

int CAuction::IncPageBackupList()	// ������ Down ��ư Ŭ���� ȣ���
{
	SEARCHRESULT *SR = GetSearchList().ResultList;
	
	if(!m_iWaitPageRefresh && AUCTION_PAGE_MAX == GetResultViewPage())//3�� �Ǿ����� ���� ����Ʈ�� ���̴�..
	{	//����Ʈ ��û �������� ���� �Ծ�� �ϰ�

		const int iSendIndex = SR[MAX_SEARCH_RESULT_LIST-1].iIndex;
		if(iSendIndex)//����Ʈ�� �� ���ִ�.. �׷��߸� ���� �������� ��û����
		{
			SendCMD_MERCHANT_BACKUP_LIST_REQUEST(iSendIndex,true);
		}
		else
		{
			COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,510));
			COMMENT ::AddCurrentStatusMessage(255,255,255,"ù ��� ���� �ٽ� ��û�մϴ�.");
			SendCMD_MERCHANT_BACKUP_LIST_REQUEST(0,true);	
		}
	}
	else
	{
		int xx =0;
		for(xx = 0; MAX_SEARCH_RESULT_LIST > xx ; xx++)
		{
			if(!SR[xx].iIndex){break;}//��� �߿� �ε����� 0�� �ڸ�ã��
		}

		if((xx)/MAX_VIEW_ABLE_PER_PAGE > GetResultViewPage())//���� �������� �������� 0���� ���� ������ -1�� ���ذ��̴�
		{
			m_iResultViewPage++;//���� ��������
		}
	}
	return 0;
}

int CAuction::DecPageBackupList()	// ������ Up ��ư Ŭ���� ȣ���
{
	SEARCHRESULT *SR = GetSearchList().ResultList;
	if(!m_iWaitPageRefresh && AUCTION_PAGE_MIN >=GetResultViewPage())//0������ �϶� �ѹ��� Dec��Ű��
	{
		if(SR[0].iIndex)//������ �����µ� �ε����� 0�̸�ȵȴ�
		{
			if(m_iResultViewPage <= 0)	// �������� 0�̸�
			{
				m_iResultViewPage = 0;
				return 0;	// �ϰ͵� ��������
			}
			else
			{
				SendCMD_MERCHANT_BACKUP_LIST_REQUEST(SR[0].iIndex,false);//��� ��û�� �ϴµ�
			}
		}
		else
		{
			COMMENT ::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,510));
			COMMENT ::AddCurrentStatusMessage(255,255,255,"ù ��� ���� �ٽ� ��û�մϴ�.");
			SendCMD_MERCHANT_BACKUP_LIST_REQUEST(0,true);				
		}
	}
	else
	{
		m_iResultViewPage--;//���� ��������
	}
	return 0;
}

// ������ �������� ��û�Ѵ�
void CAuction::SendCMD_MERCHANT_BACKUP_TAKE_REQUEST(SEARCHRESULT &SR)	
{
	if(!SR.iIndex)
	{
		::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,524));
		return;
	}
	::AddCurrentStatusMessage(FONT_COLOR_WHITE,lan->OutputMessage(4,523));

	Clear();	// Ȥ�� �𸣴� ����Ʈ�� ���������� ��û�Ѵ�

	t_MerchantResultTake	data;

	data.iIndex	=	SR.iIndex;
	data.iCn		=	Hero->id;
	strcpy(data.szMyName,Hero->name);
	strcpy(data.SellerName,SR.szSellerName);
	strcpy(data.BuyerName,SR.szBuyerName);
	
	data.dwSellValue	= SR.iSellValue;
	data.SellItem		= SR.m_ResultItem;
	data.iSellerTake	= 0;
	data.iBuyerTake	= 0;
	data.iKey			= 0;

	m_iWaitPageRefresh = 1;
	// �ϴ� DB���� ������ ����
	SendDirectDB(CMD_MERCHANT_BACKUP_TAKE_REQUEST, &data, sizeof(t_MerchantResultTake));
}
//////////////////////////////////////////////////////////////////////////////////////
// BBD 040303	��õƮ ��� ���̺��� �̿��ϴ� �Լ��� ��								//
//////////////////////////////////////////////////////////////////////////////////////