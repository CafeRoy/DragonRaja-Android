// SymbolMgr_Client.cpp: implementation of the CSymbolMgr_Client class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SymbolMgr_Client.h"
#include "Convert565to555.h"
#include "Hong_Sprite.h"
#include "Hong_Sub.h"
#include "MenuDef.h"
#include "TextOutMgr.h"
#include "Hangul.h"
#include "ItemTable.h"
#include "skill_lsw.h"
#include "CurrentMsgMgr.h"
#include "Menu.h"
#include "SDLRenderer.h"

extern	ItemAttr InvItemAttr[3][3][8];
extern	ItemAttr QuickItemAttr[7];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CSymbolMgr_Client	g_SymBolMgr;

#include <ctype.h> // for isspace
#include <string.h> // for strlen

/**
 * @brief 移除 C 字符串尾部的所有空白字符（空格、制表符、换行符等）。
 *
 * @param str 要操作的 C 字符串。
 * @return char* 返回传入的字符串指针 str。
 */
char* rtrim(char* str) {
	if (str == NULL) {
		return NULL;
	}

	size_t len = strlen(str);
	if (len == 0) {
		return str;
	}

	// 从字符串末尾开始向前遍历
	char* end = str + len - 1;

	// 当 end 指向空白字符且未越过起始位置时，继续向前移动
	while (end >= str && isspace((unsigned char)*end)) {
		end--;
	}

	// 如果 end 不再指向字符串结尾，说明有尾部空格被找到并越过了
	if (end < str + len - 1) {
		// 在新的字符串末尾（即最后一个非空白字符的下一个位置）放置终止符
		*(end + 1) = '\0';
	}

	return str;
}

CSymbolMgr_Client::CSymbolMgr_Client()
{
	memset(m_ItemSpr,0,sizeof(Spr) * SYMBOL_ITEM_MAX);
	m_nGrade	= 0;
	m_vtSymbolList.reserve(SYMBOL_ITEM_MAX);

	m_bTextItemMode = false;//soto-031031
	m_bTextAbilliyMode[0] = false;
	m_bTextAbilliyMode[1] = false;
	m_bBeta = false;

	m_nPutX	= 634;
	m_nPutY = 160;
}

CSymbolMgr_Client::~CSymbolMgr_Client()
{
	DestroyAllObject();
}

void CSymbolMgr_Client::LoadItemImage()
{
    SDL_RWops *fp = NULL; // 替換 FILE*
    char filename[MAX_FILENAME];

    for(int num = 0; num < 23; num++ )
    {
        // 1. 格式化檔名 (原始路徑)
        sprintf(filename, "./Itemdata/110%02d00.CSP", num+1 );

        // [Android] 2. 路徑修正：移除開頭的 ./
        std::string path = filename;
        if (path.find("./") == 0) path = path.substr(2);

        // [Android] 3. 使用 SDL_RWops 開啟檔案
        if( (fp = SDL_RWFromFile(path.c_str(), "rb" )) )
        {
            // [Android] 4. 讀取標頭資訊
            // 參數順序: (context, ptr, size, maxnum)
            SDL_RWread(fp, &m_ItemSpr[num].xl, sizeof( short ), 1);
            SDL_RWread(fp, &m_ItemSpr[num].yl, sizeof( short ), 1);
            SDL_RWread(fp, &m_ItemSpr[num].ox, sizeof( short ), 1);
            SDL_RWread(fp, &m_ItemSpr[num].oy, sizeof( short ), 1);
            SDL_RWread(fp, &m_ItemSpr[num].size, sizeof( unsigned int ), 1);

            // 5. 記憶體重分配 (保留您原本的函式)
            // 請確保 MemFree/MemAlloc 在 Android 上能正常運作 (例如底層是 free/malloc)
            MemFree( m_ItemSpr[num].img );
            MemAlloc( m_ItemSpr[num].img, m_ItemSpr[num].size );

            // [Android] 6. 讀取圖片數據
            // 這裡讀取 size 個 1 byte，確保回傳值是讀到的總 byte 數，比較安全
            SDL_RWread(fp, m_ItemSpr[num].img, 1, m_ItemSpr[num].size);

            // 7. 格式轉換 (維持原樣)
            convert565to555( &m_ItemSpr[num] );

            // [Android] 8. 關閉檔案
            SDL_RWclose(fp);
        }
        else
        {
            // (選用) 如果讀取失敗，可以在 Logcat 顯示錯誤
            SDL_Log("LoadItemImage Failed: %s", path.c_str());
        }
    }

    // 9. 建立圖集 (維持原樣)
    // 注意：這裡傳入的 filename 是迴圈最後一次產生的檔名 (Itemdata/1102300.csp)
    // 這是原本代碼的邏輯，這裡不做更動。
    g_SDLInfo.CreateAtlasFromLoadedData(filename, m_ItemSpr, 23);
}

void CSymbolMgr_Client::DestroyAllObject()
{
	for(int i = 0; i < SYMBOL_ITEM_MAX;i++)
	{
		if(m_ItemSpr[i].img)MemFree( m_ItemSpr[i].img );
	}
}

void CSymbolMgr_Client::PutSymbolItem()
{
	if(QuickItemAttr[5].item_no)//�������� �����Ҷ� ��´�.
	{
		m_nPutX	= SMenu[MN_ITEM].x + 164;//todo 134 + 10, 140 + 2
		m_nPutY = SMenu[MN_ITEM].y + 162;
		PutCompressedImage( m_nPutX, m_nPutY, &m_ItemSpr[QuickItemAttr[5].item_no-1] );
	}
}

void CSymbolMgr_Client::SetDualItem(int nItemNumber, int nLevel )
{
	//�ϴ����� ����ȴ�.
	QuickItemAttr[5].item_no = nItemNumber;
	QuickItemAttr[5].attr[0] = nLevel;

	m_nGrade = nLevel;
}


bool CSymbolMgr_Client::LoadTables()
{
    SDL_RWops* fp = NULL;
    char buf[1024] = {0,};

    // [Android] 路徑修正：移除開頭的 ./
    const char* strPathSymbolTxt = "Itemtable/Item_Symbol.txt";
    const char* strPathSymbolBin = "Itemtable/Item_Symbol.bin";

    m_bBeta = false;
    m_bTextItemMode = false; // Android 預設為二進位模式

    // ========================================================================
    // 1. 讀取 Item_Symbol (道具符號表)
    // ========================================================================

    // 優先嘗試讀取 BIN (Android 建議路徑)
    fp = SDL_RWFromFile(strPathSymbolBin, "rb");

#if defined(_DEBUG) && !defined(__ANDROID__)
    // PC Debug 模式下，如果 BIN 不存在，嘗試讀取 TXT
    if (!fp) {
        fp = SDL_RWFromFile(strPathSymbolTxt, "rb"); // 讀取文字檔
        if (fp) m_bTextItemMode = true;
    }
#endif

    if (!fp) {
        SDL_Log("Error: Failed to open %s or %s", strPathSymbolBin, strPathSymbolTxt);
        JustMsg("Item_Symbol.bin File not found");
        return false;
    }

    if (m_bTextItemMode)
    {
        // --- 文字模式讀取 (僅 PC Debug) ---
        Sint64 fileSize = SDL_RWsize(fp);
        std::string content; content.resize(fileSize);
        SDL_RWread(fp, &content[0], fileSize, 1);
        SDL_RWclose(fp);

        std::stringstream ss(content);
        std::string line;
        int i = 0;

        while (std::getline(ss, line)) {
            // 移除 \r
            if (!line.empty() && line.back() == '\r') line.pop_back();
            if (line.empty()) continue;
            if (i >= SYMBOL_ITEM_MAX) break;

            SYMBOL_ITEM Item;
            memset(&Item, 0, sizeof(Item));

            // 使用 stringstream 解析取代 strtok (更安全)
            std::stringstream lineSS(line);
            std::string token;

            // 依序讀取欄位
            if(!(lineSS >> Item.nItemNumber)) continue;

            if(!(lineSS >> token)) continue; // Name
            strncpy(Item.strName, token.c_str(), 63);
            rtrim(Item.strName);

            if(!(lineSS >> token)) continue; // HanName
            strncpy(Item.strHanName, token.c_str(), 63);
            rtrim(Item.strHanName);

            if(!(lineSS >> Item.n1StClass)) continue;
            if(!(lineSS >> Item.n2ndClass)) continue;

            // 跳過兩個可能的預留欄位 (對應原代碼的 tok = strtok(NULL,token))
            std::string dummy;
            lineSS >> dummy;
            lineSS >> dummy;

            if(!(lineSS >> Item.nRare[0])) continue;
            if(!(lineSS >> Item.nRare[1])) continue;
            if(!(lineSS >> Item.nRare[2])) continue;
            if(!(lineSS >> Item.nRare[3])) continue;
            if(!(lineSS >> Item.nRare[4])) continue;

            m_vtSymbolList.push_back(Item);
            i++;
        }
    }
    else
    {
        // --- 二進位模式讀取 (Android 主要路徑) ---
        for(int i = 0; i < SYMBOL_ITEM_MAX; i++)
        {
            SYMBOL_ITEM Item;
            memset(&Item, 0, sizeof(Item));

            char strName[65] = {0,};
            char strHanName[65] = {0,};
            int nRare[5] = {0,};

            // 注意：這裡必須嚴格遵守原始二進位結構順序
            if(SDL_RWread(fp, &Item.nItemNumber, sizeof(int), 1) == 0) break; // EOF check

            SDL_RWread(fp, strName, sizeof(char), 64);
            strncpy(Item.strName, strName, 64);

            SDL_RWread(fp, strHanName, sizeof(char), 64);
            strncpy(Item.strHanName, strHanName, 64);

            rtrim(Item.strName);
            rtrim(Item.strHanName);

            SDL_RWread(fp, &Item.n1StClass, sizeof(int), 1);
            SDL_RWread(fp, &Item.n2ndClass, sizeof(int), 1);

            SDL_RWread(fp, nRare, sizeof(int), 5);
            memcpy(Item.nRare, nRare, sizeof(int) * 5);

            m_vtSymbolList.push_back(Item);
        }
        SDL_RWclose(fp);
    }

    // ========================================================================
    // 2. 讀取 Item_Symbol_Ability (符號能力值)
    // ========================================================================

    for(int i = 0; i < SYMBOL_ATTR_COUNT - 1; i++)
    {
        VT_SYMBOL_RARE vtSymbolRare;
        char szFileName[MAX_PATH] = {0,};
        char szFileNameTxt[MAX_PATH] = {0,};

        // [Android] 路徑修正
        sprintf(szFileName, "Itemtable/Item_Symbol_Ability%d.bin", i + 1);
        sprintf(szFileNameTxt, "Itemtable/Item_Symbol_Ability%d.txt", i + 1);

        bool bTextAbilityMode = false;
        fp = SDL_RWFromFile(szFileName, "rb");

#if defined(_DEBUG) && !defined(__ANDROID__)
        if (!fp) {
           fp = SDL_RWFromFile(szFileNameTxt, "rb");
           if (fp) bTextAbilityMode = true;
       }
#endif

        // 檢查第一個檔案是否存在，如果不存在則報錯
        if (0 == i && NULL == fp) {
            SDL_Log("Error: Failed to open Ability file %s", szFileName);
            return false;
        }

        // 如果後續檔案不存在，則跳出迴圈 (非致命錯誤)
        if (!fp) break;

        SYMBOL_RARE SymbolRare;
        memset(&SymbolRare, 0, sizeof(SYMBOL_RARE));

        if (bTextAbilityMode)
        {
            // --- 文字模式 (PC Debug) ---
            Sint64 fileSize = SDL_RWsize(fp);
            std::string content; content.resize(fileSize);
            SDL_RWread(fp, &content[0], fileSize, 1);

            std::stringstream ss(content);
            std::string line;

            while(std::getline(ss, line))
            {
                // 移除 \r
                if (!line.empty() && line.back() == '\r') line.pop_back();
                if (line.empty()) continue;

                std::stringstream lineSS(line);
                int val = 0;
                bool ok = true;
                for(int j = 0; j < SYMBOL_ITEM_MAX; j++) {
                    if (lineSS >> val) {
                        SymbolRare.anRare[j] = val;
                    } else {
                        ok = false;
                        break;
                    }
                }
                if(ok) vtSymbolRare.push_back(SymbolRare);
            }
        }
        else
        {
            // --- 二進位模式 (Android) ---
            // 原始邏輯是不斷讀取直到 EOF
            // 每次讀取 SYMBOL_ITEM_MAX 個 int

            while(true)
            {
                // 一次讀取整排 (優化效能)
                size_t readCount = SDL_RWread(fp, SymbolRare.anRare, sizeof(int), SYMBOL_ITEM_MAX);

                if (readCount != SYMBOL_ITEM_MAX) {
                    break; // EOF 或 檔案不完整
                }

                vtSymbolRare.push_back(SymbolRare);
            }
        }

        SDL_RWclose(fp);
        m_vtRareValueList.push_back(vtSymbolRare);
    }

    // ========================================================================
    // 3. 存檔 (僅限 PC)
    // ========================================================================
#ifndef __ANDROID__
    // 在 Android 上，APK 是唯讀的，禁止寫入 Bin 檔案
    // 只有在 PC 端開發時才執行這些轉換/儲存操作
    SaveBinFile_Item_Symbol();
    SaveBinFile_Item_Symbol_Abilliy();
    SaveBinaryFiles();
#endif

    return true;
}

void CSymbolMgr_Client::Explain(int nX, int nY)
{
	
	const int nLineGab = DEFAULT_LINE_GAB;
	int nGab = 0;

	const int nWidth = TXT_BG_NORMAL_WIDTH;
	
	if(QuickItemAttr[5].item_no && m_nGrade) //������ �ѹ��� �־�� �����ϰ� �ִ°��̴�.
	{
		switch(m_nGrade) //�׷��̵忡 ���� ���� �ٲܼ� �ִ�.
		{
		case 1:
			TxtOut.Init(COLOR_BLUE,nWidth,5);
			break;
		case 2:
			TxtOut.Init(COLOR_RED,nWidth,5);
			break;
		case 3:
			TxtOut.Init(COLOR_GREEN,nWidth,5);
			break;
		case 4:
			TxtOut.Init(COLOR_BROWN,nWidth,5);
			break;
		case 5:
			TxtOut.Init(COLOR_BLACK,nWidth,5);
			break;
		}

		TxtOut.PutTextHeaderBGImg(nX,nY,nWidth);
		Hcolor(ConvertColor(255,0,255));

if(lang==0){
		char* str = "<Grade %d>";
		TxtOut.RcTXTOutB(nX, nY, nWidth, nGab, str, m_nGrade); str = "[%s]";
		Hcolor(ConvertColor(255, 255, 255)); nGab += nLineGab;
		TxtOut.RcTXTOutB(nX, nY + nGab, nWidth, nGab, str, m_vtSymbolList[QuickItemAttr[5].item_no - 1].strName);
}
else{
		char* str = "<Grade %d>";
		TxtOut.RcTXTOutB(nX,nY,nWidth,nGab,str,m_nGrade);str = "[%s]";
		Hcolor(ConvertColor(255,255,255));nGab += nLineGab;
		TxtOut.RcTXTOutB(nX, nY + nGab, nWidth, nGab, str, m_vtSymbolList[QuickItemAttr[5].item_no - 1].strHanName);
}
		nGab += nLineGab;
		
		for(int i = 0;i < SYMBOL_ATTR_COUNT;i++)
		{
			int		nRareNumber = this->GetRareNum(QuickItemAttr[5].item_no,i+1);
			
			if(!nRareNumber || MAX_ITEM_RARE <= nRareNumber)break;

			/*switch (ItemRare[nRareNumber].iExpMark / 10)
			{
			case 1://1�̸� +
				{
					::Hcolor(FONT_COLOR_RARE_PLUS_POINT);//��ȣ��
					TxtOut.RcTXTOutNoBG(nX,nY+nGab,nWidth,5,"%s + ",ItemRare[nRareNumber].ExplainAdd);// 1 : name
				}break;
			case 2://2�̸� -
				{
					::Hcolor(FONT_COLOR_RARE_MINUS_POINT);//��ȣ��
					TxtOut.RcTXTOutNoBG(nX,nY+nGab,nWidth,5,"%s - ",ItemRare[nRareNumber].ExplainAdd);// 1 : name
				}break;
			}*/
			
			switch(ItemRare[nRareNumber].iExpMark%10)
			{
			case 0:
				{
					::Hcolor(ConvertColor(0,255,255));
					TxtOut.RcTXTOut(nX,nY+nGab,nWidth,5,"%s      ",ItemRare[nRareNumber].ExplainAdd);
				}
				break;
			case 1:
				{	
					int nRareNum = RareValue(QuickItemAttr[5].item_no,i+1,m_nGrade);
					if(nRareNum)
					{
						::Hcolor(ConvertColor(0,255,255));
						TxtOut.RcTXTOut(nX,nY+nGab,nWidth,5,"%s       %d",ItemRare[nRareNumber].ExplainAdd,nRareNum);
					}
					
				}
				break;
			case 2:
				{
					
					int nRareNum = RareValue(QuickItemAttr[5].item_no,i+1,m_nGrade);
					if(nRareNum)
					{
						::Hcolor(ConvertColor(0,255,255));
						TxtOut.RcTXTOut(nX,nY+nGab,nWidth,5,"%s       %d%%",ItemRare[nRareNumber].ExplainAdd,nRareNum);
					}
				}
				break;
			case 3:
				{
					int nRareNum = RareValue(QuickItemAttr[5].item_no,i+1,m_nGrade);
					if(nRareNum)
					{
						::Hcolor(ConvertColor(0,255,255));
						TxtOut.RcTXTOut(nX,nY+nGab,nWidth,5,"%s       %dSec",ItemRare[nRareNumber].ExplainAdd,nRareNum);
					}
				}
				break;
			}			

			switch (ItemRare[nRareNumber].iExpMark / 10)
			{
			case 1://1�̸� +
			{
				::Hcolor(FONT_COLOR_RARE_PLUS_POINT);//��ȣ��
				TxtOut.RcTXTOutNoBG(nX, nY + nGab, nWidth, 5, "%s + ", ItemRare[nRareNumber].ExplainAdd);// 1 : name
			}break;
			case 2://2�̸� -
			{
				::Hcolor(FONT_COLOR_RARE_MINUS_POINT);//��ȣ��
				TxtOut.RcTXTOutNoBG(nX, nY + nGab, nWidth, 5, "%s - ", ItemRare[nRareNumber].ExplainAdd);// 1 : name
			}break;
			}
			nGab += nLineGab;
			TxtOut.RcTXTOut(nX, nY + nGab, nWidth, 5, " ");
			nGab += nLineGab;
		}
		TxtOut.PutTextTailBGImg(nX,nY+nGab,nWidth);
		
	}
}

int CSymbolMgr_Client::GetRareNum(int nSymbolNumber, int AttrCount)
{
	if(m_vtSymbolList.size() < nSymbolNumber || nSymbolNumber <= 0)
	{
		return 0;
	}
	if(AttrCount > SYMBOL_ATTR_COUNT || AttrCount <= 0)
	{
		return 0;
	}

	return m_vtSymbolList[nSymbolNumber-1].nRare[AttrCount-1];
}

int CSymbolMgr_Client::RareValue(int nItemNumber, int AttrCount, int nLevel)
{
	if(m_vtSymbolList.size() < nItemNumber || nItemNumber <= 0) return 0;
	if(AttrCount > SYMBOL_ATTR_COUNT || AttrCount <= 0) return 0;
	if(m_vtRareValueList.size() <= 0) return 0;
	if(m_vtRareValueList[0].size() < nLevel || nLevel <= 0) return 0;

	
	return m_vtRareValueList[AttrCount-1][nLevel - 1].anRare[nItemNumber-1];
}

void CSymbolMgr_Client::SetDualItem(int n1StClass, int n2StClass, int nLevel)
{
 		
 	for(int i = 0;i < SYMBOL_ITEM_MAX;i++)
 	{
		int _1st = m_vtSymbolList[i].n1StClass;
 		int _2st = m_vtSymbolList[i].n2ndClass;
 		if(_1st == n1StClass+1 && _2st == n2StClass+1)
 		{
 			SetDualItem(i+1,nLevel);
 		}
 	}
}

void CSymbolMgr_Client::Load(int lang)
{
	this->lang = lang;
	LoadItemImage();
	LoadTables();
}

int CSymbolMgr_Client::GetItemNumber()
{
	return QuickItemAttr[5].item_no;
}

bool CSymbolMgr_Client::IsHave()
{
	return (QuickItemAttr[5].item_no > 0 && QuickItemAttr[5].item_no <= 23);
}

//DEL void CSymbolMgr_Client::Send_SymbolMake(int n1StClass, int n2StClass)
//DEL {
//DEL 		
//DEL }

//�ɺ��� �����.
void CSymbolMgr_Client::Recv_SymbolMake(t_SYMBOL_ITEM_MSG* pSymbol)
{
	SetDualItem(pSymbol->m_nItemNumber,pSymbol->m_nGrade);
	::AddCurrentStatusMessage(255,255,255,lan->OutputMessage(4,141));
}


//�ɺ� ���׷��̵�.
void CSymbolMgr_Client::Recv_SymbolUpGrade(t_SYMBOL_ITEM_MSG *pSymbol)
{
	SetDualItem(pSymbol->m_nItemNumber,pSymbol->m_nGrade);
	::AddCurrentStatusMessage(255,255,255,lan->OutputMessage(4,142));
}

void CSymbolMgr_Client::SaveBinaryFiles()
{
	/*
	FILE*	fp = NULL;
	
	int SymbolCount = m_vtSymbolList.size();
	
	if(m_bBeta)
	{	//< CSD-031030
		fp = fopen("./Itemtable_b5/Item_Symbol.bin","w+b");
	}	//> CSD-031030
	else
	{
		fp = fopen("./Itemtable/Item_Symbol.bin","w+b");
	}	
		
	if(fp)
	{
		for(int i = 0;i < SymbolCount;i++)
		{
			int nItemNumber = m_vtSymbolList[i].nItemNumber;
			char strName[65] = {NULL,};strcpy(strName,m_vtSymbolList[i].strName);
			char strHanName[65] = {NULL,};strcpy(strHanName,m_vtSymbolList[i].strHanName);
			int n1StClass = m_vtSymbolList[i].n1StClass;
			int	n2ndClass = m_vtSymbolList[i].n2ndClass;
			int nRare[5] = {0,};memcpy(nRare,m_vtSymbolList[i].nRare,sizeof(int)*5);

			int size = fwrite(&nItemNumber,sizeof(int),1,fp);
			size = fwrite(strName,sizeof(char),65,fp);
			size = fwrite(strHanName,sizeof(char),65,fp);
			size = fwrite(&n1StClass,sizeof(int),1,fp);
			size = fwrite(&n2ndClass,sizeof(int),1,fp);
			size = fwrite(nRare,sizeof(int),5,fp);
		}

		fclose(fp);
	}

	fp = NULL;
	int RareListSize = m_vtRareValueList.size();

	for(int FileCount = 0;FileCount < RareListSize;FileCount++)
	{
		char szFileName[MAX_PATH] = {NULL,};
		if(m_bBeta)
		{	//< CSD-031030
			sprintf(szFileName,"./Itemtable_b5/Item_Symbol_Ability%d.bin",FileCount+1);
		}	//> CSD-031030
		else
		{
			sprintf(szFileName,"./Itemtable/Item_Symbol_Ability%d.bin",FileCount+1);			
		}

		fp = fopen(szFileName,"w+b");
		
		if(fp)
		{
			int nGradeCount = m_vtRareValueList[FileCount].size();
			for(int i = 0;i < nGradeCount;i++)
			{
				for(int j = 0;j < SYMBOL_ITEM_MAX;j++)
				{
					int nRare = m_vtRareValueList[FileCount][i].anRare[j];
					int size = fwrite(&nRare,sizeof(int),1,fp);
				}
			}
			fclose(fp);
		}	
	}
	*/
}

// 建議修正方案
/*
void CSymbolMgr_Client::SaveBinFile_Item_Symbol()
{
	if (!m_bTextItemMode) return;

	FILE* fp = NULL;
	int SymbolCount = m_vtSymbolList.size();

	{
		// 建議：使用 "wb" 模式。如果檔案不存在，"w+b" 會創建它；
		// 如果存在，"w+b" 會清空它並允許讀寫。對於單純寫入，"wb" 更清晰。
		fp = fopen("./Itemtable/Item_Symbol.bin", "wb");
	}

	if (fp)
	{
		for (int i = 0; i < SymbolCount; i++)
		{
			// ... (其他變數宣告保持不變) ...
			int nItemNumber = m_vtSymbolList[i].nItemNumber;

			// 修正：使用 strncpy_s 確保不會溢出 strName 緩衝區
			char strName[65];
			strncpy_s(strName, m_vtSymbolList[i].strName, 64); // 注意：複製最多 64 個字元，保留 1 個給 '\0'
			strName[64] = '\0'; // 確保目標字串總是 '\0' 結尾

			// 修正：使用 strncpy_s 確保不會溢出 strHanName 緩衝區
			char strHanName[65];
			strncpy_s(strHanName, m_vtSymbolList[i].strHanName, 64);
			strHanName[64] = '\0';

			int n1StClass = m_vtSymbolList[i].n1StClass;
			int n2ndClass = m_vtSymbolList[i].n2ndClass;
			int nRare[5];
			memcpy(nRare, m_vtSymbolList[i].nRare, sizeof(int) * 5); // 可以保留 memcpy

			// ... (fwrite 保持不變) ...
			fwrite(&nItemNumber, sizeof(int), 1, fp);
			fwrite(strName, sizeof(char), 64, fp);
			fwrite(strHanName, sizeof(char), 64, fp);
			fwrite(&n1StClass, sizeof(int), 1, fp);
			fwrite(&n2ndClass, sizeof(int), 1, fp);
			fwrite(nRare, sizeof(int), 5, fp);
		}

		fclose(fp);
	}
}
*/


void CSymbolMgr_Client::SaveBinFile_Item_Symbol()
{
	if(!m_bTextItemMode)return;
	FILE*	fp = NULL;
	
	int SymbolCount = m_vtSymbolList.size();
	
	{
		fp = fopen("./Itemtable/Item_Symbol.bin","w+b");
	}	
		
	if(fp)
	{
		for(int i = 0;i < SymbolCount;i++)
		{
			int nItemNumber = m_vtSymbolList[i].nItemNumber;
			char strName[65] = {0};strncpy(strName,m_vtSymbolList[i].strName, 64);
			char strHanName[65] = {0};strncpy(strHanName,m_vtSymbolList[i].strHanName, 64);
			int n1StClass = m_vtSymbolList[i].n1StClass;
			int	n2ndClass = m_vtSymbolList[i].n2ndClass;
			int nRare[5] = {0,};memcpy(nRare,m_vtSymbolList[i].nRare,sizeof(int)*5);

			int size = fwrite(&nItemNumber,sizeof(int),1,fp);
			size = fwrite(strName,sizeof(char),64,fp);
			size = fwrite(strHanName,sizeof(char),64,fp);
			size = fwrite(&n1StClass,sizeof(int),1,fp);
			size = fwrite(&n2ndClass,sizeof(int),1,fp);
			size = fwrite(nRare,sizeof(int),5,fp);
		}

		fclose(fp);
	}		
}


void CSymbolMgr_Client::SaveBinFile_Item_Symbol_Abilliy()
{
	
	FILE*	fp = NULL;

	int RareListSize = m_vtRareValueList.size();

	for(int FileCount = 0;FileCount < RareListSize;FileCount++)
	{
		char szFileName[MAX_PATH] = {NULL,};

		if (!m_bTextAbilliyMode[FileCount]) continue;

		sprintf(szFileName, "./Itemtable/Item_Symbol_Ability%d.bin", FileCount + 1);

		fp = fopen(szFileName,"w+b");
		
		if(fp)
		{
			int nGradeCount = m_vtRareValueList[FileCount].size();
			for(int i = 0;i < nGradeCount;i++)
			{
				for(int j = 0;j < SYMBOL_ITEM_MAX;j++)
				{
					int nRare = m_vtRareValueList[FileCount][i].anRare[j];
					int size = fwrite(&nRare,sizeof(int),1,fp);
				}
			}
			fclose(fp);
		}	
	}
}
