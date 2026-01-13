// FileMgr.h: interface for the CFileMgr class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEMGR_H__8410F295_2C38_46C6_ACE6_EE7F1880660D__INCLUDED_)
#define AFX_FILEMGR_H__8410F295_2C38_46C6_ACE6_EE7F1880660D__INCLUDED_

#include "SDL.h"          // [Android] 引入 SDL
#include "network.h"
#include <map>            // [Android] 標準容器
#include <string>         // [Android] 字串處理
#include <cstring>        // [Android] memcpy, memset
#include <cmath>          // [Android] abs
#include <cstdlib>        // [Android] rand
#include <cstdint>        // [Android] int16_t

// [Android] 定義 Windows 型別，如果 stdafx.h 已定義可忽略
#ifndef DWORD
typedef unsigned int DWORD;
#endif
#ifndef BYTE
typedef unsigned char BYTE;
#endif

// 使用 std namespace 方便 map 使用
using namespace std;

///////////////////////////////////////////////////////////////////////////////
// 畫面管理 類別
template <typename T, int N> class TFileMgr
{
    typedef map<int, T*>           hash;
    typedef map<int, int>          check;
    typedef typename map<int, T*>::iterator index;

    union
    {
        struct
        {
            int16_t m_nIndex; // [Android] __int16 -> int16_t
            int16_t m_nMagic; // [Android] __int16 -> int16_t
        };

        DWORD m_dwHandle;
    };

    //020808 YGI  ---------------
    int m_nClassSize; // sizeof( T )
    int m_nMarkSize;
    T *m_pCommon;
    int m_nCommonIndex;
    char *m_pMark;
    //------------------------------

public:
    TFileMgr() { Create(); }
    ~TFileMgr() { Destroy(); }

public:
    bool Load(const char* pFile);
    bool Load(int nIndex, const char* pFile);
    bool LoadFromClass(int nIndex, T *pData, short int check_sum );
    bool Save(const char* pFile);

public:
    void Create()
    {
        //020808 YGI  ------------------
        m_pCommon = new T;
        m_nClassSize = sizeof( T );
        m_nMarkSize = (m_nClassSize >100)?100:m_nClassSize;
        m_pMark = new char[m_nMarkSize];
        m_nCommonIndex = -1;
        for( int i=0; i<m_nMarkSize; i++ )
        {
            m_pMark[i] = (BYTE)( (rand() % 69) + 1 );
        }
        // ------------------------------

        m_dwHandle = 0;
        m_nError = 0;
        m_pBuffer = new T[N];
        memset(m_pBuffer, 0, m_nClassSize*N);      //020808 YGI
    }

    void Destroy()
    {
        if (m_pBuffer != NULL)
        {
            delete [] m_pBuffer;
            m_pBuffer = NULL;
        }

        m_mpBank.clear();
        //020808 YGI
        if (m_pCommon) delete m_pCommon; // [Android] 安全檢查
        if (m_pMark) delete [] m_pMark;
    }

    void AddNew(int nKey)
    {
        T* pT = GetBuffer(nKey);

        if (pT != NULL)
        {
            m_mpCheck.insert(std::make_pair(nKey, pT->Check()));
            m_mpBank.insert(std::make_pair(nKey, pT));
        }
    }
    //020808 YGI
    void AddNew(int nKey, int check_num)
    {
        T* pT = GetBuffer(nKey);

        if (pT != NULL)
        {
            m_mpCheck.insert(std::make_pair(nKey, check_num));
            m_mpBank.insert(std::make_pair(nKey, pT));
        }
    }

    bool IsError() const
    {
        if (m_nError )
        {
            // 注意：這裡假設 connections 是全域變數，如果不在此 header 定義可能會報錯
            // 建議確認 connections 的定義位置
            if (connections.socket) {
                Release(&connections);
            }
            return true;
        }

        return false;
    }

public:
    T* GetBuffer(int nIndex)
    {
        return &m_pBuffer[nIndex];
    }

    //020808 YGI
    T& operator[](int nIndex)
    {
        if( m_nCommonIndex != nIndex )
        {
            memcpy( m_pCommon, m_mpBank[nIndex], m_nClassSize );
            char *pSource = (char * )m_pCommon;
            for( int i=0; i<m_nMarkSize; i++ )
            {
                pSource[i] ^= m_pMark[i];
            }
            m_nCommonIndex = nIndex;
        }

        m_nError += abs(m_mpCheck[nIndex] - m_pCommon->Check());
        m_nError <<= 15;
        m_nError += m_nError?4:0;

        if( m_nError )
        {
            return *(m_mpBank[nIndex + m_nError]);
        }
        return *m_pCommon;
    }

protected:
    void Encode(char* pData, size_t nSize);
    void Decode(char* pData, size_t nSize);

private:
    T* m_pBuffer;
    check        m_mpCheck;
    hash         m_mpBank;
    int          m_nError;
    CHSEL_STREAM m_hsel;
};

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

template <typename T, int N>
bool TFileMgr<T, N>::Load(const char* pFile)
{
    // [Android] 路徑修正：移除開頭的 ./
    std::string path = pFile;
    if (path.find("./") == 0) path = path.substr(2);

    // 如果這個數字跟你在 Windows 上看到的不一樣，那就是問題所在！
    // [Android] 使用 SDL_RWFromFile 取代 ifstream
    SDL_RWops* fin = SDL_RWFromFile(path.c_str(), "rb");

    SDL_Log("Loading %s, sizeof(T) = %zu", path.c_str(), sizeof(T));

    if( !fin ) {
        SDL_Log("TFileMgr Load Error: %s not found. SDL Error: %s", path.c_str(), SDL_GetError());
        return false;
    }

    // 讀取版本
    int nVersion = 0;
    SDL_RWread(fin, &nVersion, sizeof(nVersion), 1);

    if (m_hsel.GetVersion() != nVersion) {
        SDL_RWclose(fin);
        return false;
    }

    HselInit deinit;
    SDL_RWread(fin, &deinit, sizeof(HselInit), 1);

    if (!m_hsel.Initial(deinit)) {
        SDL_RWclose(fin);
        return false;
    }

    // 資料讀取
    T* pT = NULL;

    for (int i = 0; i < N; ++i)
    {
        pT = GetBuffer(i);
        if (pT == NULL) {
            SDL_RWclose(fin);
            return false;
        }

        // 讀取資料與 Handle
        SDL_RWread(fin, pT, sizeof(T), 1);
        SDL_RWread(fin, &m_dwHandle, sizeof(m_dwHandle), 1);

        Decode((char*)(pT), sizeof(T));

        // Checksum 檢驗
        if (m_hsel.GetCRCConvertShort() != m_nIndex) {
            SDL_RWclose(fin);
            return false;
        }
        if (pT->Check() != m_nMagic) {
            SDL_RWclose(fin);
            return false;
        }

        //020808 YGI  --------------------
        char *pSource = (char *)pT;
        int check = pT->Check();
        for( int j=0; j< m_nMarkSize; j++ )
        {
            pSource[j] ^= m_pMark[j];
        }
        // --------------------------------
        AddNew(i, check);
    }

    SDL_RWclose(fin);
    return true;
}

template <typename T, int N>
bool TFileMgr<T, N>::Load(int nIndex, const char* pFile)
{
    // [Android] 路徑修正
    std::string path = pFile;
    if (path.find("./") == 0) path = path.substr(2);

    SDL_RWops* fin = SDL_RWFromFile(path.c_str(), "rb");

    T* pT = GetBuffer(nIndex);
    if (pT == NULL) {
        if(fin) SDL_RWclose(fin);
        return false;
    }

    if( fin )
    {
        int nVersion = 0;
        SDL_RWread(fin, &nVersion, sizeof(nVersion), 1);

        if (m_hsel.GetVersion() != nVersion) {
            SDL_RWclose(fin);
            return false;
        }

        HselInit deinit;
        SDL_RWread(fin, &deinit, sizeof(HselInit), 1);

        if (!m_hsel.Initial(deinit)) {
            SDL_RWclose(fin);
            return false;
        }

        // 讀取資料
        SDL_RWread(fin, pT, sizeof(T), 1);
        SDL_RWread(fin, &m_dwHandle, sizeof(m_dwHandle), 1);

        Decode((char*)(pT), sizeof(T));

        if (m_hsel.GetCRCConvertShort() != m_nIndex) {
            SDL_RWclose(fin);
            return false;
        }
        if (pT->Check() != m_nMagic) {
            SDL_RWclose(fin);
            return false;
        }
        SDL_RWclose(fin);
    }
    else
    {
        // 檔案不存在時的處理 (初始化為空)
        T temp;
        memset(&temp, 0, sizeof(T)); // 確保 temp 初始化
        memcpy( pT, &temp, sizeof( T ) );
        m_nMagic = pT->Check();
    }

    //020808 YGI  --------------------
    char *pSource = (char *)pT;
    int check = m_nMagic;
    for( int j=0; j< m_nMarkSize; j++ )
    {
        pSource[j] ^= m_pMark[j];
    }
    // --------------------------------
    AddNew(nIndex, check);

    return true;
}

template <typename T, int N>
bool TFileMgr<T, N>::LoadFromClass(int nIndex, T *pData, short int check_sum )
{
    T* pT = GetBuffer(nIndex);
    if (pT == NULL)  return false;
    memcpy( pT, pData, sizeof( T ) );
    m_nMagic = check_sum;
    if (pT->Check() != m_nMagic)  return false;
    char *pSource = (char *)pT;
    for( int j=0; j< m_nMarkSize; j++ )
    {
        pSource[j] ^= m_pMark[j];
    }
    AddNew(nIndex, m_nMagic);
    return true;
}

// [Android 注意] Save 函式在手機上通常無法寫入 Assets 資料夾。
// 如果你在手機上呼叫這個，除非路徑是可寫路徑 (如 SDL_GetPrefPath)，否則會失敗。
// 為了編譯通過，我將其改為使用 SDL_RWops。
template <typename T, int N>
bool TFileMgr<T, N>::Save(const char* pFile)
{
#ifdef __ANDROID__
    // Android 上禁止寫入資源檔
    SDL_Log("Warning: TFileMgr::Save called on Android. Skipping write to %s", pFile);
    // 你可以選擇 return false，或者視為成功但不動作
    return true;
#else
    // [Windows/PC] 維持原本邏輯，但改用 SDL 介面以保持一致性
  // 或者是你可以保留原本的 ofstream，但這裡為了統一我用 SDL
  SDL_RWops* fout = SDL_RWFromFile(pFile, "wb");
  if (!fout) return false;

  // CRC 計算
  HselInit eninit;
    eninit.iEncryptType = HSEL_ENCRYPTTYPE_RAND;
    eninit.iDesCount = HSEL_DES_TRIPLE;
    eninit.iCustomize = HSEL_KEY_TYPE_DEFAULT;
    eninit.iSwapFlag = HSEL_SWAP_FLAG_ON;
    if (!m_hsel.Initial(eninit)) {
        SDL_RWclose(fout);
        return false;
    }

  HselInit deinit;
  deinit = m_hsel.GetHSELCustomizeOption();
  const int nVersion = m_hsel.GetVersion();

  SDL_RWwrite(fout, &nVersion, sizeof(nVersion), 1);
  SDL_RWwrite(fout, &deinit, sizeof(HselInit), 1);

  T* pT = NULL;

  for (int i = 0; i < N; ++i)
  {
    pT = (m_mpBank[i] != NULL) ? m_mpBank[i] : GetBuffer(i);
    if (pT == NULL) {
        SDL_RWclose(fout);
        return false;
    }

    m_nMagic = pT->Check();

    // 注意：因為 Encode 會修改 pData 原地加密
    // 如果這裡直接改 pT，可能會影響記憶體中的資料
    // 原本代碼似乎是直接改，這裡維持原樣
    Encode((char*)pT, sizeof(T));

    SDL_RWwrite(fout, pT, sizeof(T), 1);

    m_nIndex = m_hsel.GetCRCConvertShort();
    SDL_RWwrite(fout, &m_dwHandle, sizeof(m_dwHandle), 1);
  }

  SDL_RWclose(fout);
  return true;
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////

template <typename T, int N>
void TFileMgr<T, N>::Encode(char* pData, size_t nSize)
{
    m_hsel.Encrypt(pData, nSize);
}

template <typename T, int N>
void TFileMgr<T, N>::Decode(char* pData, size_t nSize)
{
    m_hsel.Decrypt(pData, nSize);
}

#endif // !defined(AFX_FILEMGR_H__8410F295_2C38_46C6_ACE6_EE7F1880660D__INCLUDED_)