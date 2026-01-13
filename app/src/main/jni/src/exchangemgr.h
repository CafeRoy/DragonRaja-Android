// ExchangeMgr.h: interface for the CExchangeMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXCHANGEMGR_H__9E46C0B2_FD86_4EBD_B107_B9D8B67B0669__INCLUDED_)
#define AFX_EXCHANGEMGR_H__9E46C0B2_FD86_4EBD_B107_B9D8B67B0669__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum eVersusInterFaceFieldNo
{
	VS_LEFT_DISPLAY_FIELD_NO  = 0,
	VS_RIGHT_DISPLAY_FIELD_NO  = 1,
};

enum ExchangeType
{
	EXCHANGE_TYPE_NO = 0,//�ŷ��ƴ� 
	EXCHANGE_TYPE_NORMAL = 1,		//���� �ŷ�
	EXCHANGE_TYPE_MERCHANT_DIRECT = 2,	//���ΰ� ���� �ŷ�
};

enum ExchangeStateType
{
	EXCHANGE_STATE_READY = 0,//�ŷ� ��� ����
	EXCHANGE_STATE_EXCHANGE_NOW  ,	//�ŷ��� ���� GetExchangeState���� ��
};

class CExchangeMgr  
{
public:
	CExchangeMgr();
	virtual ~CExchangeMgr();
private:
	int m_iExchageType;//�ŷ� Ÿ��

	int m_iExchageServerConformState;//���� ���������㰡 �ް� �ִ� ���¾�
	int m_iExchageState;//�ŷ����� ����
	int m_iExchageTargetId;//�ŷ����� ����� cn ��
	bool m_bMySideConformState;//������ �ŷ� Ok ������ ���´°�.(�ŷ��������̽� �����ų� ������ ������ 0 ���� ����� ���.
	bool m_bOtherSideConformState;//������ �ŷ� Ok ������ ���´°�.(�ŷ��������̽� �����ų� ������ ������ 0 ���� ����� ���.
	char m_szTargetName[31];
	
public:
	bool	m_bIsIAmMerchant;
	int		GetExchangeType()const{return m_iExchageType;}// �ŷ� ����� cn�� ����
	void	SetExchangeType(const int iType){m_iExchageType = iType;}// �ŷ� ����� cn�� ����

	int		GetExchangeTargetId()const{return m_iExchageTargetId;}// �ŷ� ����� cn�� ����
	void	SetExchangeTargetId(const int iCn)
	{
		LPCHARACTER	pCh = ::FindCharacter( &g_CharacterList, iCn);
		if( pCh )
		{
			m_iExchageTargetId = iCn;
			SetExchangeTargetName( pCh->name );
		}
		else
		{
			::CallServer( CMD_EXCHANGE_CANCEL );
		}
	}// �ŷ� ����� cn�� ����

	void 	SetOtherSideConformState(const bool bIsOkSign)	{m_bOtherSideConformState = bIsOkSign;}
	void	SetMySideConformState(const bool bIsOkSign)		{m_bMySideConformState = bIsOkSign;}

	bool	IsOtherSideConformStateOk()const	{return m_bOtherSideConformState;}
	bool	IsMySideConformStateOk()const	{return m_bMySideConformState;}

	void	SetExchangeTargetName( const char* szName )
	{
		::sprintf(m_szTargetName,"%s",szName);
	}
	char*	GetExchangeTargetName()
	{
		return m_szTargetName;
	}
	
	void	SetExchangeState(const int iValue){m_iExchageState = iValue;}
	int		GetExchangeState()const{return m_iExchageState;}//now Exchage State

	void	SendExchangeResult();
	void	SendExchangeStateSign(const bool bOkSign);//030108 lsw
	void	CallExchangeMenu(const bool bIsRecvSide, const int iExchageType);

	void Clear();
	void ClearItemData();

};
extern CExchangeMgr ExchangeMgr;

const int MAX_EXCHANGE_ITEM = 20;
extern DefaultItem item_give[MAX_EXCHANGE_ITEM];
extern DefaultItem item_recv[MAX_EXCHANGE_ITEM];

extern bool IsExchangeNow();

extern void SendExchangeItemWithOtherCh( short int you_id ,const int iExchangeType);
extern void RecvExchangeItemReq( k_exchange_item_start *exchange_item );
extern void SendExchangeResult();		// ���� ���ۿ� ���� ���� �޽��� ó��
extern void RecvExchangeItemOk( k_exchange_item_start *exchange_item );
extern void RecvExchangeItemResult( int ret );		// ������ ��ȯ�� ���� �غ� (���� ���ۿ� ���� ��� )
extern void RecvExchangeCancel();
extern void RecvExchange();
extern void RecvExchangeEach( const K_ITEM *lpItem );
extern void SendExchangeItemEach( ItemAttr &item, POS &pos, short int you_id );
extern void SendExchangeItemDel( POS pos, short int you_id );
extern void SetExchageItemData(const int iExchangeIndex,ItemAttr &item, POS &pos,const bool bSendToServer = false);//������ ����Ÿ ����
extern void CheckFaultExchageItemList();
extern void DisplayExchangeItemList(const int iWillDo, const int iIsRectOn);
extern void DeleteItemAtExchangeGiveItemList(const int iWillDo);
#endif // !defined(AFX_EXCHANGEMGR_H__9E46C0B2_FD86_4EBD_B107_B9D8B67B0669__INCLUDED_)
