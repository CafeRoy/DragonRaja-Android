#include "stdafx.h"
#include "language.h"
#include "hong_sub.h"
#include "CMessage.h"
#include <cstdio>
#include "utf8.h"
/*////////////////////////////////////////////////////////////////////////////////////////////
// ���� : CLan.cpp,CLan.h ȭ��.                                                             //
// ���� : txtȭ�Ϸ� ���� ��Ʈ���� �о �޸𸮿� ����. �ܺο� �����ϴ� OutputMessage�Լ��� //
//        ���ؼ� �ش� ��Ʈ���� ����.                                                        //
// ���� : txtȭ���� �����쿡 �°� �ش� ����� txt�� ã�Ƽ� �ε�. txtȭ���� �ѹ� �м��Ͽ�    //
//        �ʿ��� �޸𸮸� ���, new�� �ѹ��� ȣ���Ͽ� �ε����� ���̺�� ��Ʈ���� �Բ� ����. //
// Ư¡ : �������� ��Ʈ������ new�� ȣ���ϰ� �ε����� �������� new�� ����Ͽ� �޸��� ���� //
//        ȭ �� memory leak�� �����Ͽ�����, 3�� �������� �������, ���������� ����� �߻�.  //
//        ������ new�� �ѹ��� ȣ�������μ� memory leak�� ���� �� �޸� ����ȭ�� �����Ѵ�.  //
//        All code is made by zoung. zoung@esofnet.com ICQ : 66924157  Written at 2002/4/12 //
*/////////////////////////////////////////////////////////////////////////////////////////////

LanguagePack* lan = NULL;

char    LanguageInt[6][3]={ "en", "kr", "ch", "en", "jp","th"};
char	header[10][20]={ "[melee]","[magic]","[skill]","[notice]","[item]","[etc]","[justmsg]","[sprintf]","[yesorno]","[temp]"};	

int		CLanguage::CheckArea()			//� �� ������ üũ�ϴ� �Լ�
{
	AreaCode=0;	// EN
	return(1);
}

int CLanguage::LoadingByAreaCode()
{
    char path[MAX_FILE_LENGTH];

    const char* filenameOnly;
    if (AreaCode == 0) filenameOnly = "en_lan.txt";
    else filenameOnly = "ch_lan.txt";

    // Android Asset 路徑修正
    snprintf(path, MAX_FILE_LENGTH, "data/%s", filenameOnly);
    snprintf(this->LoadedFileName, MAX_FILE_LENGTH, "%s", path);

    // 使用 SDL 開啟
    SDL_RWops* fp = SDL_RWFromFile(path, "rb");

    if(fp == NULL)
    {
        SDL_Log("Error: Language file not found: %s", path);
        return -1;
    }

    // 1. 預讀：計算大小
    Preloading(fp);

    // 2. 重置指標到開頭 (關鍵!)
    SDL_RWseek(fp, 0, RW_SEEK_SET);

    // 3. 讀取：建立索引與資料
    Loading(fp);

    // 4. 關閉
    SDL_RWclose(fp);
    return 1;
}

char*	CLanguage::OutputMessage(int NumOfSentence)	//�ش��ϴ� �޽����� Sentence���� �ְ� ������ Sentence���� �ƹ����� ����.
{
	int kind=0,number=0;
	kind=NumOfSentence/100000;
	number=NumOfSentence%100000;
	return this->OutputMessage(kind,number);
}

char* CLanguage::OutputMessage(int Kind, int Num)
{
    // 基本檢查
    if (Kind < 0 || Kind >= MAX_KIND) return this->szNullString;
    if (Num <= 0 || Num > (int)this->m_dwMaxLineNumberOfKind[Kind]) return this->szNullString;

    // 計算 Index 偏移
    DWORD MaxNumber = 0;
    if (Kind > 0)
    {
        for(int i=0; i<Kind; i++)
            MaxNumber += this->m_dwMaxLineNumberOfKind[i];
    }

    // 計算在 IndexAddress 中的位置
    DWORD IndexOffset = (MaxNumber + Num) * 4;

    // 讀取儲存的 Data Offset
    DWORD StoredDataOffset = 0;
    memcpy(&StoredDataOffset, (this->IndexAddress + IndexOffset), 4);

    // ★★★ 64位元修正重點 ★★★
    // 如果 Offset 是 0 (且不是第0句)，代表該處沒資料
    // (注意：如果第0個字串剛好在 Offset 0 是合法的，但這裡 Num 從 1 開始判斷，通常沒問題)
    if (StoredDataOffset == 0 && Num != 0)
    {
        // 這裡可以視情況檢查是否真的無資料，或僅是初始化為0
        // 但依據原本邏輯，這裡我們假設 0 代表空
        // 如果你的第一個字串確實存在 Offset 0，這個判斷可能要微調
        // 比較保險的做法是檢查字串內容
        if ((this->DataAddress + 0)[0] == 0) return this->szNullString;
    }

    // 舊做法：直接轉型回指標 -> 崩潰
    // return (char*)ReturnAddress;

    // 新做法：基底位址 + 偏移量 = 真實記憶體位址 (64-bit safe)
    return (this->DataAddress + StoredDataOffset);
}


CLanguage::CLanguage(int lang)
{
	//memset(this,0,sizeof( *this ) );
	m_dwTotalLength=0;
	m_dwSumOfTotalLine = 0;
	m_dwSumOfMaxLineNumber=0;
	emoji_ranges.clear();
	AreaCode = lang;
	this->Init();
}

CLanguage::~CLanguage()
{
	SAFE_DELETE(this->IndexAddress);
}

int CLanguage::Init()
{	
	//if( this->CheckArea()	< 0 )			return -1;
	if( this->LoadingByAreaCode() < 0 )		return -2;	
	//if( this->Preloading()	< 0 )			return -3;
	//if( this->Loading()		< 0	)			return -4;
	if (!this->load_emoji_data("Event/emoji.ent")) return -5;
	return 1;
}

int CLanguage::Preloading(SDL_RWops* fp)
{
    DWORD Header=0, Number=0;
    char Original_Sentence[MAX_ONE_LINE_CHAR_NUM];
    char Temp_Sentence[MAX_ONE_LINE_CHAR_NUM];
    char Sentence[MAX_ONE_LINE_CHAR_NUM];

    while(true)
    {
        int ret = GetOneLine(Original_Sentence, fp);
        if (ret == 0) break; // EOF
        if (ret == -1) continue; // 註解行
        if (strlen(Original_Sentence) < 2) continue; // 空行

        strcpy(Temp_Sentence, Original_Sentence);

        if( CheckHeader(Header,Temp_Sentence) < 0 ) continue;
        if( CheckNumber(Number,Temp_Sentence) < 0 ) continue;
        if( CheckSentence(Sentence,Temp_Sentence) < 0 ) continue;

        this->m_dwMaxLineNumberOfKind[Header] =
                (this->m_dwMaxLineNumberOfKind[Header] > Number) ? this->m_dwMaxLineNumberOfKind[Header] : Number;

        this->m_dwTotalLineOfKind[Header]++;
        this->m_dwSumOfTotalLine++;
        this->m_dwTotalLength = this->m_dwTotalLength + strlen(Sentence) + 1;
    }

    for( int i=0; i<MAX_KIND; i++)
        this->m_dwSumOfMaxLineNumber += this->m_dwMaxLineNumberOfKind[i];

    return 1;
}

// 020620 YGI
void ConvertSpecialWord(char *txt,const char *delTarget,const char AfterConvert)
{
	char *pdest = txt;
	while( pdest = strstr(pdest,delTarget) )
	{
		*pdest = ' '; pdest++;
		*pdest = AfterConvert;
	} 
}


int CLanguage::Loading(SDL_RWops* fp)
{
    DWORD Header=0, Number=0;
    DWORD DataPos=0; // 這是相對於 DataAddress 的偏移量 (Offset)

    // 分配記憶體 (索引區 + 資料區)
    // 索引區大小：總行數 * 4 bytes (用來存 offset)
    // 資料區大小：總文字長度
    this->IndexAddress = new char [this->m_dwSumOfMaxLineNumber*4 + this->m_dwTotalLength + 4];

    if( IndexAddress == NULL) return -1;

    // DataAddress 指向資料區的開頭
    this->DataAddress = this->IndexAddress + this->m_dwSumOfMaxLineNumber*4 + 4;

    // 初始化
    memset(this->IndexAddress, 0, this->m_dwSumOfMaxLineNumber*4 + this->m_dwTotalLength + 4);

    char Original_Sentence[MAX_ONE_LINE_CHAR_NUM];
    char Temp_Sentence[MAX_ONE_LINE_CHAR_NUM];
    char Sentence[MAX_ONE_LINE_CHAR_NUM];

    while(true)
    {
        int ret = GetOneLine(Original_Sentence, fp);
        if (ret == 0) break; // EOF
        if (ret == -1) continue; // 註解
        if (strlen(Original_Sentence) < 2) continue; // 空行

        strcpy(Temp_Sentence, Original_Sentence);

        if( CheckHeader(Header, Temp_Sentence) < 0 ) continue;
        if( CheckNumber(Number, Temp_Sentence) < 0 ) continue;
        if( CheckSentence(Sentence, Temp_Sentence) < 0 ) continue;

        // 特殊字元轉換
        ConvertSpecialWord(Sentence, "\\n", '\n');
        ConvertSpecialWord(Sentence, "\\r", '\r');
        ConvertSpecialWord(Sentence, "\\t", '\t');

        DWORD Sentence_Length = strlen(Sentence);
        DWORD BaseAddress = 0; // 這是計算索引陣列的 Index

        if( Header > 0 )
        {
            for( int i=0; i<Header; i++)
                BaseAddress += this->m_dwMaxLineNumberOfKind[i];
        }
        else
        {
            BaseAddress = 0;
        }

        // 1. 將文字拷貝到資料區
        strcpy(this->DataAddress + DataPos, Sentence);

        // 2. ★★★ 64位元修正重點 ★★★
        // 舊做法：存絕對位址 (指標) -> 轉成 DWORD 會在 64-bit 系統上截斷 -> 崩潰
        // DWORD TempDataAddress = (DWORD)((this->DataAddress + DataPos));

        // 新做法：只存「偏移量 (Offset)」
        // DataPos 本身就是 DWORD (unsigned int)，不會有截斷問題
        DWORD offsetToStore = DataPos;

        // 將偏移量寫入索引表
        // 索引位置 = (BaseAddress + Number) * 4
        memcpy(this->IndexAddress + (BaseAddress + Number)*4, &offsetToStore, 4);

        // 移動資料區游標
        DataPos += Sentence_Length + 1;
    }

    return 0;
}

// 讀取一行文字，支援 Android/Windows 換行
// 回傳值: 1=成功, 0=EOF, -1=註解行(;)
int CLanguage::GetOneLine(void * data, SDL_RWops* fp)
{
    int i = 0;
    char chardata;
    char* charBuffer = (char*)data;

    // 清空緩衝區
    memset(data, 0, MAX_ONE_LINE_CHAR_NUM);

    // SDL_RWread 回傳讀取的 byte 數，0 代表 EOF 或錯誤
    while (SDL_RWread(fp, &chardata, 1, 1) > 0)
    {
        // 遇到 CR (Windows 換行前半段) 忽略
        if (chardata == 13) continue;

        // 遇到 LF (換行) 結束
        if (chardata == 10) return 1;

        // 遇到分號 (;)，標記為註解行，但我們仍需把這行讀完直到換行
        if (chardata == 59) {
            // 繼續讀到換行符號為止
            while (SDL_RWread(fp, &chardata, 1, 1) > 0) {
                if (chardata == 10) break;
            }
            return -1; // 回傳 -1 代表這是註解行
        }

        // 存入 Buffer (防止溢位)
        if (i < MAX_ONE_LINE_CHAR_NUM - 1) {
            charBuffer[i++] = chardata;
        }
    }

    // 如果讀到檔尾 (EOF) 但 Buffer 有資料，算讀取成功
    if (i > 0) return 1;

    return 0; // 真正的 EOF
}


int	CLanguage::CheckHeader(uint32_t &Header,char* data)				//�����ΰ��� �Ǵ�	.���° ��ȣ���� ��ȯ
{
	for(int i=0;i<MAX_KIND;i++)
	{
		if( strlen(header[i]) > strlen(data) )
			continue;
		if( android_memicmp(header[i],data,strlen(header[i])) )			//���� ��ġ�ϴ� header�� ������
			continue;
		Header = i;
		return -1;		//����� ã���� -�� ��ȯ
	}
	return 1;
}
int	CLanguage::CheckNumber(uint32_t &Number,char* data)				//��ȣ�� ������� Ȯ���Ѵ�.��ȣ ��ȯ
{
	char *semi = NULL;
	semi = strchr(data,':');
	if( semi == NULL)		//:�� ������
		return -1;
	char temp[MAX_ONE_LINE_CHAR_NUM]={0,};
	memcpy(temp,data,( (DWORD)(semi-data) ) );
	Number = atoi(temp);
	return 1;
}
int	CLanguage::CheckSentence(void * sentence,char* data)			//������ �󸶳� �䰡�� Ȯ���Ѵ�.������ ���� ��ȯ
{
	DWORD length = strlen(data);
	char *start = NULL,*end=NULL;
	start = strchr(data,'"');
	if( start == NULL)		//"�� ������
		return -1;

	//�ּ��� ������ 2��° �̾�� �Ѵ�.
	if( start-data+1 >= length )
		return -2;

	end = strchr(start+1,'"');
	if( end == NULL)
		return -3;

	//�ּ��� ������ ù��° �̾�� �Ѵ�
	if( end-data >= length )
		return -4;

	if( start+1 > end -1 )
		return -5;

	//������� �Դٴ� ���� "���� ��ȣ�Ǵ� ������ �ִٴ� �̾߱Ⱑ �ȴ�. ������ start, ���� end�̴�
	memset(sentence,0,end-start+2);
	memcpy(sentence,start+1,end-start-1);
	return 1;
}

void	CLanguage::DisplayAllData()
{
	int count=0;
	int i;
	for( i=0;i<MAX_KIND;i++)
	{
		for( int j=0;j<this->m_dwMaxLineNumberOfKind[i]+1;j++)
		{
			char* data = (char*)this->OutputMessage(i,j);
			if( data[0] != 0 )	printf("[%2d:%6d] %s\n",i,j,data );
		//	if( count++%100 == 0 )
		//		getchar();
		}
	}
	printf("\nTotal Summary\n");
	for( i=0;i<MAX_KIND;i++)
	{		
		printf("Line 1 : Max Line Number : %d , Total Line Number : %d\n",this->m_dwMaxLineNumberOfKind[i],this->m_dwTotalLineOfKind[i]);				
	}
	printf("\nSum of Max Line Number : %d Sum of Total Line : %d , Total Length : %d" , this->m_dwSumOfMaxLineNumber,
		this->m_dwSumOfTotalLine,
		this->m_dwTotalLength); 

}

bool CLanguage::is_emoji(uint32_t code_point) {
	auto it = emoji_ranges.upper_bound(code_point);
	if (it == emoji_ranges.begin()) {
		return false;
	}
	--it;
	return code_point >= it->first && code_point <= it->second;
}

bool CLanguage::contains_emoji_in_name(const std::string& name) {
	std::string::const_iterator end_it = name.end();
	for (std::string::const_iterator it = name.begin(); it != end_it; ) {
		uint32_t code_point = utf8::next(it, end_it);
		if (is_emoji(code_point)) {
			return true;
		}
	}
	return false;
}

bool CLanguage::load_emoji_data(const std::string& filename) {
    // 1. 改用 SDL 開啟檔案 (讀取 APK 內的 assets)
    // 確保 filename 是相對路徑，例如 "data/emoji.txt"，不要有 "./"
    SDL_RWops* file = SDL_RWFromFile(filename.c_str(), "rb");

    if (file == NULL) {
        // 使用 SDL_Log 或原本的 g_DBGLog 都可以
        //g_DBGLog.Log(LOG_LV1, "Error: Could not open file %s. SDL Error: %s", filename.c_str(), SDL_GetError());
        return false;
    }

    // 2. 讀取檔案內容到記憶體
    Sint64 fileSize = SDL_RWsize(file);
    std::string fileContent;

    if (fileSize > 0) {
        fileContent.resize(fileSize);
        SDL_RWread(file, &fileContent[0], fileSize, 1);
    }

    // 3. 關閉檔案指標
    SDL_RWclose(file);

    // 4. 將記憶體內容轉換為 stringstream
    // 命名為 'ssFile'，用來取代原本的 'file'
    std::stringstream ssFile(fileContent);

    // =========================================================
    // 以下邏輯幾乎保持不變，只針對 ssFile 操作
    // =========================================================

    std::string line;
    while (std::getline(ssFile, line)) { // 從 stringstream 讀取

        // Android/Linux 換行符號修正：移除 Windows 可能殘留的 \r
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        // 移除行首的空白
        size_t first_char = line.find_first_not_of(" \t\r\n");
        if (std::string::npos == first_char || line[first_char] == '#') {
            continue;
        }
        line = line.substr(first_char);

        // 尋找碼點和屬性分隔符號 ';'
        size_t semicolon_pos = line.find(';');
        if (semicolon_pos == std::string::npos) {
            continue; // 忽略格式不正確的行
        }

        std::string code_part = line.substr(0, semicolon_pos);

        try {
            // 尋找範圍 ".."
            size_t range_pos = code_part.find("..");
            if (range_pos != std::string::npos) {
                // 處理範圍，例如: 1F600..1F64F
                std::string start_hex = code_part.substr(0, range_pos);
                std::string end_hex = code_part.substr(range_pos + 2);
                // 這裡建議加上錯誤捕捉，以防 hex 格式不對
                uint32_t start_code = std::stoul(start_hex, nullptr, 16);
                uint32_t end_code = std::stoul(end_hex, nullptr, 16);
                emoji_ranges[start_code] = end_code;
            }
            else {
                // 處理單一碼點，例如: 260E
                uint32_t code_point = std::stoul(code_part, nullptr, 16);
                emoji_ranges[code_point] = code_point;
            }
        }
        catch (const std::exception& e) {
            g_DBGLog.Log(LOG_LV1, "Error parsing line: %s - %s", line.c_str(), e.what());
            // 這裡通常 return false 比較好，或者是 continue 略過這行繼續讀
            continue;
        }
    }
    return true;
}

// 020428 YGI acer
extern int CheckSumError;

bool LoadLanguageText(int lang)
{
	g_cMsg = new CMessage(static_cast<CMessage::Language>(lang));
	lan = new LanguagePack(lang);
	//g_cMsg->displayAllMessage();
	return (lan != nullptr && g_cMsg != nullptr);
}
