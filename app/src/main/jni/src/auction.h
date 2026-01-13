// Auction.h: interface for the CAuction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUCTION_H__65C7ED7E_9020_467F_AB76_8528EE975EC8__INCLUDED_)
#define AFX_AUCTION_H__65C7ED7E_9020_467F_AB76_8528EE975EC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ItemTable.h"
#include "network.h"

enum BondMoneyNo
{
	BOND_MONEY_1000 = 10219,	//10�� ũ�� ����	
	BOND_MONEY_500	= 10220,	//5�� ũ�� ����	
	BOND_MONEY_100	= 10221,	//1�� ũ�� ����	
	BOND_MONEY_50	= 10222,	//5õ�� ũ�� ����	
	BOND_MONEY_10	= 10223,	//1õ�� ũ�� ����	
	BOND_MONEY_5	= 10224,	//5�鸸 ũ�� ����	
};

typedef struct SUPERMONEY
{
	int iItemNo;
	int iValue; 
	SUPERMONEY(const int iInput,const int iInput2)
	{
		iItemNo =iInput;
		iValue	=iInput2;
	}
}t_SuperMoney,*lpSUPERMONEY;

const int BOND_MONEY_MAX = 6;
extern const SUPERMONEY aBondMoney[BOND_MONEY_MAX];

enum FKType
{
	FK_RARE = 1,
	FK_LV ,
	FK_TACTIC ,
	FK_WEAR ,
};

enum ItemType
{
	NORMAL_ITEM	= 0,
	RARE_ITEM	= 1,
	HIGH_ITEM	= 2,
	DYNAMIC_RARE_ITEM	= 3,
	SET_ITEM	= 4
};

class CAuction  
{
	enum IsEndType
	{
		IS_END_ALL_RIGHT	= 0,	//���� �� �� �ִ� ����
		IS_END_DELETING		= 1,	//������
		IS_END_BUYING		= 3,	//������
		IS_END_WAIT_TAKE	= 4	,	//�������⸦ �ٶ�� ����
		IS_END_DELETE_COMPLETE = 5,	// BBD 040303
		IS_END_GIVING		= 6,	// BBD 040303 �������μ����� ���ִ�.
	};

	enum ItemLevelType
	{
		LVMIN		= 0,
		LV10TO20	= 1,
		LV20TO30	= 2,
		LV30TO40	= 3,
		LV40TO50	= 4,
		LV50TO60	= 5,
		LV60TO70	= 6,
		LV70TO80	= 7,
		LV80TO90	= 8,
		LV90TO100	= 9,
		LV100TO110	= 10,
		LV110TO120	= 11,
		LV120TO130	= 12,
		LV130TO140	= 13,
		LV140TO150	= 14,
		LVMAX		= 15
	};
public:
	CAuction();
	virtual ~CAuction();
private://�˻���� ����
	int	m_iFKRareType;
	int	m_iFKLevel;
	int	m_iFKTacticType;
	int m_iFKWearType;
	
	int			m_iDeleteItemIndex;//��ü �ε���(SQL)
	int			m_iBuyItemIndex;//��ü �ε���(SQL)
	int			m_iTakeItemIndex;
	int			m_iSellValue;
	POS			m_SellItemPos;
	ItemAttr	m_SellItemAttr;

	int	m_iWaitPageRefresh;

	int	m_iBuyerViewPage;
	int	m_iSellerViewPage;
	int	m_iResultViewPage;

	POS			m_BondItemPos;
	ItemAttr	m_BondItemAttr;

private://�˻������
	SEARCHRESULTLIST m_SearchList;//����ü ���ΰ� �迭�Դϴ�.
	void Clear();
public://�Ǹ� ������ ��Ͽ�
	int	GetBuyerViewPage()const{return m_iBuyerViewPage;}
	int	GetSellerViewPage()const{return m_iSellerViewPage;}
	int	GetResultViewPage()const{return m_iResultViewPage;}

	int	IncBuyerViewPage() ;
	int	IncSellerViewPage();
	int	IncResultViewPage() ;

	int	DecBuyerViewPage() ;
	int	DecSellerViewPage();
	int	DecResultViewPage() ;

	void SetSellValue(const int iSellValue){m_iSellValue =iSellValue;}

	int SetSellItem(const POS &pos,const ItemAttr Item);	
	POS GetSellItemPos(){return m_SellItemPos;}
	ItemAttr *GetSellItemAttr(){return (m_SellItemAttr.item_no)?&m_SellItemAttr:NULL;}

	int SetBondItem(const POS &pos,const ItemAttr Item);	
	POS GetBondItemPos(){return m_BondItemPos;}
	ItemAttr *GetBondItemAttr(){return (m_BondItemAttr.item_no)?&m_BondItemAttr:NULL;}

	void ClearSellItem()
	{
		m_iSellValue = 0;
		memset(&m_SellItemPos,0,sizeof(POS));
		memset(&m_SellItemAttr,0,sizeof(ItemAttr));
	}

	void ClearBondItem()
	{
		memset(&m_BondItemPos,0,sizeof(POS));
		memset(&m_BondItemAttr,0,sizeof(ItemAttr));
	}
	
	void SetDeleteItemIndex(const int iIndex)
	{
		m_iDeleteItemIndex = iIndex;
	}
	
	void SetBuyItemIndex(const int iIndex)
	{
		m_iBuyItemIndex = iIndex;
	}

	void SetTakeItemIndex(const int iIndex)
	{
		m_iTakeItemIndex = iIndex;
	}	

	int GetDeleteItemIndex()
	{
		return m_iDeleteItemIndex;
	}
	int GetBuyItemIndex()
	{
		return m_iBuyItemIndex;
	}
	int GetTakeItemIndex()
	{
		return m_iTakeItemIndex;
	}
	
public:
	char m_szMerchantName[21];
	char m_szKeyWord[21];
	void DecPeriod(int nDec = 1);
	void IncPeriod(int nInc = 1);
	int m_nPeriod;
	bool m_bChkMerchantName;
	bool m_bChkKeyWord;
	SEARCHRESULTLIST GetSearchList()const{return m_SearchList;}
	void ClearFK()//FK���� �ʱ�ȭ ��ŵ�ϴ�.
	{
		m_iFKRareType	= NORMAL_ITEM;
		m_iFKLevel		= LVMIN;
		m_iFKTacticType = TACTICS_MIN;
		m_iFKWearType	= WEAR_ANYTHING;
	}

	void FKRareTypeInc()//�ƾ��� Ÿ�� ����
	{
		if(SET_ITEM <= m_iFKRareType){m_iFKRareType = NORMAL_ITEM;}//���� �븻 �������̰ų� �װŶ� ������
		else{ m_iFKRareType++;}
	}
	void FKRareTypeDec()
	{
		if(NORMAL_ITEM >= m_iFKRareType){m_iFKRareType = SET_ITEM;}//���� �븻 �������̰ų� �װŶ� ������
		else{ m_iFKRareType--;}
	}

	void FKLevelInc()//�ƾ��� ���� ����
	{
		if(LVMAX <= m_iFKLevel){m_iFKLevel = LVMIN;}//���� �븻 �������̰ų� �װŶ� ������
		else{ m_iFKLevel++;}
	}
	void FKLevelDec()
	{
		if(LVMIN >= m_iFKLevel){m_iFKLevel = LVMAX;}//���� �븻 �������̰ų� �װŶ� ������
		else{ m_iFKLevel--;}
	}

	void FKTacticTypeInc()
	{
		if(TACTICS_MAX <= m_iFKTacticType)
		{
			m_iFKTacticType = TACTICS_START;
		}
		else
		{
			m_iFKTacticType++;
		}
	}
	void FKTacticTypeDec()
	{
		if(TACTICS_START >= m_iFKTacticType)
		{
			m_iFKTacticType = TACTICS_MAX;
		}
		else
		{
			m_iFKTacticType--;
		}
	}

	void FKWearTypeInc()
	{
		if(WEAR_TWO_HAND <= m_iFKWearType){m_iFKWearType = WEAR_ANYTHING;}//���� �븻 �������̰ų� �װŶ� ������
		else{ m_iFKWearType++;}
	}
	void FKWearTypeDec()
	{
		if(WEAR_ANYTHING >= m_iFKWearType){m_iFKWearType = WEAR_TWO_HAND;}//���� �븻 �������̰ų� �װŶ� ������
		else{ m_iFKWearType--;}
	}

	int GetFKRareType()const{return m_iFKRareType;}
	int GetFKLevel()const{return m_iFKLevel;}
	int GetFKTacticType()const{return m_iFKTacticType;}
	int GetFKWearType()const{return m_iFKWearType;}
//---->�����ڿ�
	void SendCMD_MERCHANT_BUY_LIST_REQUEST(const int iIndex = 0, const int iNeedIndexOver = 1);//����Ʈ ��û(�����ڰ� ���°�//�˻��Ⱑ ȣ��
	void RecvCMD_MERCHANT_BUY_LIST_REQUEST_RESULT(t_packet &p);//����Ʈ ���(�����ڰ� ���°�

	void SendCMD_MERCHANT_ITEM_BUY(SEARCHRESULT &SR);//�����ϱ�(�����ڰ� ����
	void RecvCMD_MERCHANT_ITEM_BUY_RESULT(t_packet &p);//�����ϱ�(�����ڰ� ����
//<----

//---->�Ǹ��ڿ�
	void SendCMD_MERCHANT_SELL_LIST_REQUEST(const int iIndex = 0, const int iNeedIndexOver = 1);//����Ʈ ��û(�Ǹ��ڰ� ���°�
	void RecvCMD_MERCHANT_SELL_LIST_REQUEST_RESULT(t_packet &p);//����Ʈ ���(�Ǹ��ڰ� ���°�

	void SendCMD_MERCHANT_SELL_ITEM_DELETE();//�Ǹ� �������� ����
	void RecvCMD_MERCHANT_SELL_ITEM_DELETE_RESULT(t_packet &p);//�Ǹ� �������� ����
	
	void SendCMD_MERCHANT_SELL_ITEM_REGISTER();//�Ǹ� �������� ���
	void RecvCMD_MERCHANT_SELL_ITEM_REGISTER_RESULT(t_packet &p);//�Ǹ� �������� ���
//<----

//---->�ŷ����
	void SendCMD_MERCHANT_RESULT_LIST_REQUEST(const int iIndex = 0, const int iNeedIndexOver =1);//
	void RecvCMD_MERCHANT_RESULT_LIST_REQUEST_RESULT(t_packet &p);

	void SendCMD_MERCHANT_RESULT_TAKE(SEARCHRESULT &SR);
	void RecvCMD_MERCHANT_RESULT_TAKE_RESULT(t_packet &p);
//<----

//---->�����ڰ� ���ΰ� �ŷ��� ��û
	int SendCMD_MERCHANT_EXCHANGE_LIST_REQUEST(std::string, const int iIndex, const int iNeedIndexOver, const bool bIsDirectExchange);
	int RecvCMD_MERCHANT_EXCHANGE_LIST_REQUEST_RESULT(t_packet &p);
//<----
	void SendCMD_EXCHANGE_BOND_MONEY();

//<! BBD 040303 ������̺���� ��Ϲ� ã��
	void SendCMD_MERCHANT_BACKUP_LIST_REQUEST(const int iIndex = 0, const int iNeedIndexOver = 1);
	void RecvCMD_MERCHANT_BACKUP_LIST_RESPONSE(t_packet &p);
	int IncPageBackupList();
	int DecPageBackupList();
	void SendCMD_MERCHANT_BACKUP_TAKE_REQUEST(SEARCHRESULT &SR);

//> BBD 040303 ������̺���� ��Ϲ� ã��
};
extern CAuction Auction;
#endif // !defined(AFX_AUCTION_H__65C7ED7E_9020_467F_AB76_8528EE975EC8__INCLUDED_)