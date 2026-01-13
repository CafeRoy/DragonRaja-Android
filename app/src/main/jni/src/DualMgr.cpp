// DualMgr.cpp: implementation of the CDualMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "DualMgr.h"

#include "char.h"
#include "Effect.h"
#include "network.h"
#include "CharDataTable.h"
#include "SmallMenuSet.h"
#include "SymbolMgr_Client.h"
#include "Ability.h"
extern CHARACTERLIST g_CharacterList;
extern int UseItemByRbutton(POS pos_s, ItemAttr &item_attr);
extern CSymbolMgr_Client	g_SymBolMgr;
extern	ItemAttr InvItemAttr[3][3][8];
//class	CSymbolMgr_Client;
extern void SetItemPos(int type, int p1, int p2, int p3, POS* IP_set);
CDualMgr g_mgrDual;
///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CDualMgr::CDualMgr()
{
	m_nPara = 0;
	m_nPosX = 0;
	m_nPosY = 0;
}

CDualMgr::~CDualMgr()
{
	
}

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

void CDualMgr::Load(const char* pFile)
{
    std::string path = pFile;
    if (path.find("./") == 0) path = path.substr(2);

    // 1. 使用 SDL_RWops 開啟檔案 (相容 Android Assets)
    SDL_RWops* rw = SDL_RWFromFile(path.c_str(), "rb");
    if (!rw) {
        SDL_Log("CDualMgr::Load Failed to open: %s", pFile);
        return;
    }

    // 2. 取得檔案大小並讀取到記憶體
    Sint64 fileSize = SDL_RWsize(rw);
    if (fileSize <= 0) {
        SDL_RWclose(rw);
        return;
    }

    // 準備緩衝區 (+1 是為了放字串結尾符 \0)
    std::vector<char> buffer(fileSize + 1);
    SDL_RWread(rw, buffer.data(), fileSize, 1);
    buffer[fileSize] = '\0'; // 確保是有效的 C-String
    SDL_RWclose(rw);

    // 3. 使用 stringstream 包裝緩衝區，這樣就可以用 getline 了
    std::stringstream ss(buffer.data());
    std::string strLine;
    int i = 0, j = 0;

    // 4. 解析邏輯 (保持原樣，僅微調)
    // 注意：原本的 fin.widen('\n') 在這裡不需要，直接用預設行為即可
    while (std::getline(ss, strLine))
    {
        // 移除 Windows 可能殘留的 \r (Carriage Return)
        if (!strLine.empty() && strLine.back() == '\r') {
            strLine.pop_back();
        }

        if (strLine.empty()) continue; // 跳過空行

        int nBegin = 0;
        int nEnd = strLine.find("\t", 0);

        // 如果找不到 tab，這一行可能有問題，先跳過防崩潰
        if (nEnd == std::string::npos) continue;

        int new_i = atoi(strLine.substr(nBegin , nEnd - nBegin).c_str());

        // 安全檢查：防止 i 超出 m_aBank 的範圍 (假設是 [5][15])
        if (new_i >= 5) {
            SDL_Log("CDualMgr::Load Warning: Index i (%d) out of bounds", new_i);
            continue;
        }

        if (i != new_i)
        {
            i = new_i;
            j = 0;
        }

        // 安全檢查：防止 j 超出範圍
        if (j >= 15) {
            // 防止這一行寫入導致溢位，讀下一行
            continue;
        }

        // --- 以下邏輯保持原樣 ---

        // 第一欄：Class Index (已經讀過了，直接賦值)
        m_aBank[i][j].nBefore = i;

        // 第二欄：nAfter
        nBegin = nEnd + 1;
        nEnd = strLine.find("\t", nBegin);
        if (nEnd == std::string::npos) continue;
        m_aBank[i][j].nAfter = atoi(strLine.substr(nBegin , nEnd - nBegin).c_str());

        // 第三欄：nStep
        nBegin = nEnd + 1;
        nEnd = strLine.find("\t", nBegin);
        if (nEnd == std::string::npos) continue;
        m_aBank[i][j].nStep = atoi(strLine.substr(nBegin , nEnd - nBegin).c_str());

        // 第四欄：nLimit
        nBegin = nEnd + 1;
        nEnd = strLine.find("\t", nBegin);
        if (nEnd == std::string::npos) continue;
        m_aBank[i][j].nLimit = atoi(strLine.substr(nBegin , nEnd - nBegin).c_str());

        // 第五欄：strName
        nBegin = nEnd + 1;
        nEnd = strLine.find("\t", nBegin);
        if (nEnd == std::string::npos) continue; // 這裡如果沒有 Tab 可能名字是最後一個欄位？
        m_aBank[i][j].strName = std::string(strLine, nBegin , nEnd - nBegin);

        // 第六欄：strExplain (讀到行尾)
        nBegin = nEnd + 1;
        if (nBegin < strLine.length()) {
            m_aBank[i][j].strExplain = std::string(strLine, nBegin , strLine.length() - nBegin);
        } else {
            m_aBank[i][j].strExplain = "";
        }

        ++j;
    }
}

void CDualMgr::Save(const char* pFile)
{
	// 1. 建立並開啟輸出檔案流 (ofstream)
		// std::ios::out 確保以寫入模式開啟檔案
		// 如果檔案不存在會建立新檔案；如果存在會清空內容
	ofstream fout(pFile, ios::out);

	// 檢查檔案是否成功開啟
	if (!fout.is_open())
	{
		// 處理錯誤，例如拋出例外或印出錯誤訊息
		cerr << "Error: Could not open file for writing: " << pFile << endl;
		return;
	}

	// 2. 遍歷 m_aBank 陣列
	// 假設 m_aBank 的維度是 [5][15] (根據原本的註解)
	for (int i = 0; i < 5; ++i)
	{
		// 迴圈遍歷 m_aBank[i] 中的元素
		for (int j = 0; j < 15; ++j)
		{
			// 檢查是否為一個有效的資料項目 (這裡需要一個判斷依據)
			// 由於我們不知道 m_aBank 結構中是否有專門的標記，
			// 這裡假設只要 nAfter 或 strName 非空/非零，就是有效資料。
			// 為了簡化和確保寫入，這裡假設 m_aBank[i][j] 都是有效的。

			// 根據載入時的格式，一行資料應包含 6 個欄位，以 \t 分隔：
			// nBefore (i) \t nAfter \t nStep \t nLimit \t strName \t strExplain

			// nBefore (載入時的第一欄，即外層索引 i)
			fout << i << "\t";

			// nAfter (轉職後的職業)
			fout << m_aBank[i][j].nAfter << "\t";

			// nStep (職業階段/等級)
			fout << m_aBank[i][j].nStep << "\t";

			// nLimit (使用等級限制)
			fout << m_aBank[i][j].nLimit << "\t";

			// strName (技能/專長名稱)
			fout << m_aBank[i][j].strName << "\t";

			// strExplain (技能/專長描述 - 這是最後一欄，後面是換行)
			fout << m_aBank[i][j].strExplain << endl; // 使用 endl 寫入換行符

			// **注意:** 如果資料在 m_aBank 中是以「組」的方式儲存 (例如 i 相同為一組)，
			// 並且 j 是不連續的 (只有有效資料會用到 j)，
			// 則這個簡單的 for 迴圈可能需要調整，
			// 但基於原 Load 函式的結構，簡單的雙重迴圈是最直接的寫入方式。
		}
	}

	// 3. 關閉檔案流
	fout.close();
}

string CDualMgr::GetClassName(int nBefore, int nAfter, int nStep, int nLevel) const
{
	switch (nStep)
	{
    case 0: return (nLevel >= 100) ? "Crossing Class":m_aBank[nBefore][(nLevel - 1)/10].strName;
    case 1:
    case 2:
    case 3: 
    case 4: return m_aBank[nBefore][nAfter + 10].strName;
    case 5: return "Dragon Slayer";
	}
	
	return "";
}

string CDualMgr::GetDulaExplain(int nBefore, int nAfter) const
{
	return m_aBank[nBefore][nAfter + 10].strExplain; 
}
void CDualMgr::SendResetSymbolReady(LPCHARACTER pMaster, BYTE nPara, BYTE nX, BYTE nY)
{
	if (pMaster == NULL)  return;

	POS pos;
	// ������ų ������ ȹ�� ������ ��ġ ���ϱ�
	::SetItemPos(INV, nPara, nY, nX, &pos);
	// ������ų ������ ȹ�� ������ Ȯ��
	ItemAttr& rItemAttr = InvItemAttr[nPara][nY][nX];

	if (rItemAttr.item_no == 10217) {
		m_nPara = nPara;
		m_nPosX = nX;
		m_nPosY = nY;
		::CallDualInterFace(MT_DUAL_RESET_SYMBOL);
	}
}

void CDualMgr::SendResetSymbol(LPCHARACTER pMaster, int Select)
{
	if (pMaster == NULL)  return;

	ItemAttr& rItemAttr = InvItemAttr[m_nPara][m_nPosY][m_nPosX];

	if (rItemAttr.item_no != 10217) {
		return;
	}

	t_packet packet;
	packet.h.header.type = CMD_RESET_SYMBOL;
	packet.h.header.size = sizeof(t_client_dual_change);
	packet.u.dual.client_dual_change.idMaster = static_cast<WORD>(pMaster->id);
	packet.u.dual.client_dual_change.nPara = m_nPara;
	packet.u.dual.client_dual_change.nPosX = m_nPosX;
	packet.u.dual.client_dual_change.nPosY = m_nPosY;
	packet.u.dual.client_dual_change.nNext = Select;
	::QueuePacket(&packet, 1);
}

void CDualMgr::SendDualEnable(LPCHARACTER pMaster, BYTE nPara, BYTE nX, BYTE nY)
{
	if (pMaster == NULL)  return;
	
	POS pos;
	// ������ų ������ ȹ�� ������ ��ġ ���ϱ�
	::SetItemPos(INV, nPara, nY, nX, &pos);
	// ������ų ������ ȹ�� ������ Ȯ��
	ItemAttr& rItemAttr = InvItemAttr[nPara][nY][nX];
	
	if (rItemAttr.item_no == 4028)
	{ // Packet ����
		t_packet packet;
		packet.h.header.type = CMD_DUAL_ENABLE;
		packet.h.header.size = sizeof(t_client_dual_enable);
		packet.u.combat.client_combat_obtain.idMaster = static_cast<WORD>(pMaster->id);
		packet.u.combat.client_combat_obtain.nPara = nPara;
		packet.u.combat.client_combat_obtain.nPosX = nX;
		packet.u.combat.client_combat_obtain.nPosY = nY;
		::QueuePacket(&packet, 1);  
	} 
}

void CDualMgr::SendDualChange(LPCHARACTER pMaster, BYTE nNext)
{
	if (pMaster == NULL)  return;
	
	POS pos;
	// ������ų ������ ȹ�� ������ ��ġ ���ϱ�
	::SetItemPos(INV, m_nPara, m_nPosY, m_nPosX, &pos);
	// ������ų ������ ȹ�� ������ Ȯ��
	ItemAttr& rItemAttr = InvItemAttr[m_nPara][m_nPosY][m_nPosX];
	
	if (rItemAttr.item_no == 4028)
	{ // Packet ����
		t_packet packet;
		packet.h.header.type = CMD_DUAL_CHANGE;
		packet.h.header.size = sizeof(t_client_dual_change);
		packet.u.dual.client_dual_change.idMaster = static_cast<WORD>(pMaster->id);
		packet.u.dual.client_dual_change.nPara = m_nPara;
		packet.u.dual.client_dual_change.nPosX = m_nPosX;
		packet.u.dual.client_dual_change.nPosY = m_nPosY;
		packet.u.dual.client_dual_change.nNext = nNext;
		::QueuePacket(&packet, 1);  
	}
}

void CDualMgr::SendDualDivide(LPCHARACTER pMaster, BYTE nNext)
{
	if (pMaster == NULL)  return;
	
	t_packet packet;
	packet.h.header.type = CMD_DUAL_DIVIDE;
	packet.h.header.size = sizeof(t_client_dual_divide);
	packet.u.dual.client_dual_divide.idMaster = static_cast<WORD>(pMaster->id);
	packet.u.dual.client_dual_divide.nNext = nNext;
	::QueuePacket(&packet, 1);  
}

void CDualMgr::RecvDualEnable(t_server_dual_enable* pPacket)
{
	LPCHARACTER pDual = ::ReturnCharacterPoint(pPacket->idMaster);
	if (pDual == NULL)  return;
	
	const BYTE nPara = pPacket->nPara;
	const BYTE nX = pPacket->nPosX;
	const BYTE nY = pPacket->nPosY;
	// ������ų ������ ȹ�� ������ ��ġ ���ϱ�
	POS pos;
	::SetItemPos(INV, nPara, nY, nX, &pos);
	// ������ų ������ ȹ�� ������ Ȯ��
	ItemAttr& rItemAttr = InvItemAttr[nPara][nY][nX];
	
	if (rItemAttr.item_no == 4028)
	{ // Packet ����
		m_nPara = nPara;
		m_nPosX = nX;
		m_nPosY = nY;
		::CallDualInterFace(MT_DUAL_CHANGE);
	}
}

void CDualMgr::RecvDualChange(t_server_dual_change* pPacket)
{
	LPCHARACTER pDual = ::ReturnCharacterPoint(pPacket->idMaster);
	if (pDual == NULL)  return;
	
	if (pDual == Hero)
	{
		m_nPara = 0;
		m_nPosX = 0;
		m_nPosY = 0;
		
		pDual->aStepInfo[CLS_STEP] = 1;              // ��� Ŭ���� �ܰ� ���
		pDual->aStepInfo[DUAL_CLS] = pPacket->nDual; // ��� Ŭ���� ����
		SCharacterData.nCharacterAbility[FAME] = pPacket->dwFame; // ������ fame ���� �缳��
		SCharacterData.nCharacterAbility[STR] = pPacket->wStr;
		SCharacterData.nCharacterAbility[DEX] = pPacket->wDex;
		SCharacterData.nCharacterAbility[CON] = pPacket->wCon;
		SCharacterData.nCharacterAbility[WIS] = pPacket->wWis;
		SCharacterData.nCharacterAbility[INT_] = pPacket->wInt;
		SCharacterData.nCharacterAbility[CHA] = pPacket->wCha;
		SCharacterData.nCharacterAbility[MOVP] = pPacket->wMovep;
		SCharacterData.nCharacterAbility[ENDU] = pPacket->wEndu;
		SCharacterData.nCharacterAbility[MOR] = pPacket->wMor;
		SCharacterData.nCharacterAbility[LUCK] = pPacket->wLuck;
		SCharacterData.nCharacterAbility[WSPS] = pPacket->wWsPs;
		SCharacterData.LvUpPoint = pPacket->wPoint;
		
		
	}
	
	::InsertMagic(pDual, pDual, 443, -1, 0, 0, 0, 0);
}

void CDualMgr::RecvDualDivide(t_server_dual_divide* pPacket)
{
	LPCHARACTER pDual = ::ReturnCharacterPoint(pPacket->idMaster);
	if (pDual == NULL)  return;
	
	if (pDual == Hero)
	{
		
		++Hero->aStepInfo[CLS_STEP];                // ��� Ŭ���� �ܰ� ���
		Hero->aStepInfo[DUAL_CLS] = pPacket->nDual; // ��� Ŭ���� ����
		SCharacterData.nCharacterAbility[STR] = pPacket->wStr;
		SCharacterData.nCharacterAbility[DEX] = pPacket->wDex;
		SCharacterData.nCharacterAbility[CON] = pPacket->wCon;
		SCharacterData.nCharacterAbility[WIS] = pPacket->wWis;
		SCharacterData.nCharacterAbility[INT_] = pPacket->wInt;
		SCharacterData.nCharacterAbility[CHA] = pPacket->wCha;
		SCharacterData.nCharacterAbility[MOVP] = pPacket->wMovep;
		SCharacterData.nCharacterAbility[ENDU] = pPacket->wEndu;
		SCharacterData.nCharacterAbility[MOR] = pPacket->wMor;
		SCharacterData.nCharacterAbility[LUCK] = pPacket->wLuck;
		SCharacterData.nCharacterAbility[WSPS] = pPacket->wWsPs;
		SCharacterData.LvUpPoint = pPacket->wPoint;
		
	}
	
	::InsertMagic(pDual, pDual, 443, -1, 0, 0, 0, 0);
}

int CDualMgr::GetAbility(BYTE nType)
{	// ����Ƽ �ִ밪 ��������
	const int nClass = Hero->class_type;
	const int nStep = Hero->aStepInfo[CLS_STEP];
	CAbilityLimit* pLimit = g_mgrLimit.GetCell(nClass, nStep);
	
	switch (nType)
	{
    case STR:  return pLimit->GetStr();  // ��
    case CON:	 return pLimit->GetCon();  // ü��
    case DEX:	 return pLimit->GetDex();  // ��ø
    case WIS:	 return pLimit->GetWis();  // ����
    case INT_: return pLimit->GetInt();	 // ����
    case CHA:	 return pLimit->GetCha();  // �ŷ�
    case MOVP: return pLimit->GetMovp(); // �̵���
    case ENDU: return pLimit->GetEndu(); // �γ�
    case MOR:	 return pLimit->GetMor();  // ���
    case LUCK: return pLimit->GetLuck(); // ���
    case WSPS:
		{
			switch (SCharacterData.nCharacterData[SPELL])
			{
			case WIZARD_SPELL: return pLimit->GetWs(); // ������
			case PRIEST_SPELL: return pLimit->GetPs(); // ������
			}
		}
	}
	
	return 0;
}