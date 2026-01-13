// Ability.h: interface for the CAbility class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABILITY_H__893DF137_6634_4AE7_BC35_F2B34294522C__INCLUDED_)
#define AFX_ABILITY_H__893DF137_6634_4AE7_BC35_F2B34294522C__INCLUDED_
#include <fstream>  // 👈 加入這一行，解決 ifstream/ofstream 錯誤
using namespace std; // 👈 加入這一行，解決 ios::in, ios::binary 錯誤
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define WORD short int
#include "SDL.h"
///////////////////////////////////////////////////////////////////////////////
//
class CAbility
{
public:
	CAbility();

public:
  WORD GetStr() const { return m_wStr; }
  WORD GetDex() const { return m_wDex; }
  WORD GetCon() const { return m_wCon; }
  WORD GetWis() const { return m_wWis; }
  WORD GetInt() const { return m_wInt; }
  WORD GetCha() const { return m_wCha; }
  WORD GetEndu() const { return m_wEndu; }
  WORD GetMovp() const { return m_wMovp; }
  WORD GetMor() const { return m_wMor; }
  WORD GetLuck() const { return m_wLuck; }
  WORD GetWs() const { return m_wWs; }
  WORD GetPs() const { return m_wPs; }
  
  void SetStr(WORD wValue) { m_wStr = wValue; }
  void SetDex(WORD wValue) { m_wDex = wValue; }
  void SetCon(WORD wValue) { m_wCon = wValue; }
  void SetWis(WORD wValue) { m_wWis = wValue; }
  void SetInt(WORD wValue) { m_wInt = wValue; }
  void SetCha(WORD wValue) { m_wCha = wValue; }
  void SetEndu(WORD wValue) { m_wEndu = wValue; }
  void SetMovp(WORD wValue) { m_wMovp = wValue; }
  void SetMor(WORD wValue) { m_wMor = wValue; }
  void SetLuck(WORD wValue) { m_wLuck = wValue; }
  void SetWs(WORD wValue) { m_wWs = wValue; }
  void SetPs(WORD wValue) { m_wPs = wValue; }

private:
  WORD m_wStr;
  WORD m_wDex;
  WORD m_wCon;
  WORD m_wWis;
  WORD m_wInt;
  WORD m_wCha;
  WORD m_wEndu;
  WORD m_wMovp;
  WORD m_wMor;
  WORD m_wLuck;
  WORD m_wWs;
  WORD m_wPs;
};

class CAbilityDivide : public CAbility
{
public:
  CAbilityDivide();

public:
  WORD GetBeforeClass() const { return m_wBefore; }
  WORD GetAfterClass() const { return m_wAfter; }
  
  void SetBeforeClass(WORD wValue) { m_wBefore = wValue; }
  void SetAfterClass(WORD wValue) { m_wAfter = wValue; }
  
private:
  WORD m_wBefore;
  WORD m_wAfter;
};

class CAbilityLimit : public CAbility
{
public:
  CAbilityLimit();

public:
  WORD GetClass() const { return m_wClass; }
  WORD GetStep() const { return m_wStep; }
  WORD GetMaxTactics() const { return m_wMaxTactic; }
  
  void SetClass(WORD wValue) { m_wClass = wValue; }
  void SetStep(WORD wValue) { m_wStep = wValue; }
  void SetMaxTactics(WORD wValue) { m_wMaxTactic = wValue; }

private:
  WORD m_wClass;
  WORD m_wStep;
  WORD m_wMaxTactic;
};

template <class T, int N1, int N2> class TAbilityMgr
{
public:
  TAbilityMgr() { m_cMagic = 1; }

public:
  T* GetCell(WORD wOne, WORD wTwo)
  {
    return &m_aBank[wOne][wTwo];
  }

  bool Load(const char* pFile);
  bool Save(const char* pFile);

protected:
  int GetCheckSum(char* pDst, int nSize);
  void Encode(char* pData, int nSize);
  void Decode(char* pData, int nSize);

private:
  char m_cMagic;
  T    m_aBank[N1][N2];
};
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

template <class T, int N1, int N2>
bool TAbilityMgr<T, N1, N2>::Load(const char* pFile)
{
    // [Android] 路徑修正：移除開頭的 ./
    std::string path = pFile;
    if (path.find("./") == 0) path = path.substr(2);

    const int nSize = sizeof(T);

    // [Android] 使用 SDL 開啟檔案
    SDL_RWops* fin = SDL_RWFromFile(path.c_str(), "rb");

    if (!fin) {
        SDL_Log("TAbilityMgr::Load Error: Failed to open %s", path.c_str());
        return false;
    }

    for (int i = 0; i < N1; ++i)
    {
        for  (int j = 0; j < N2; ++j)
        {
            int nSum = 0;
            // [Android] 讀取 CheckSum
            SDL_RWread(fin, &nSum, sizeof(int), 1);

            // [Android] 讀取 Data
            SDL_RWread(fin, &m_aBank[i][j], nSize, 1);

            // 解密 (維持原樣)
            Decode((char*)(&m_aBank[i][j]), nSize);

            // 驗證 CheckSum (維持原樣)
            if (GetCheckSum((char*)(&m_aBank[i][j]), nSize) != nSum)
            {
                SDL_Log("TAbilityMgr::Load Error: CheckSum mismatch at [%d][%d]", i, j);
                SDL_RWclose(fin);
                return false;
            }
        }
    }

    SDL_RWclose(fin);
    return true;
}

template <class T, int N1, int N2>
bool TAbilityMgr<T, N1, N2>::Save(const char* pFile)
{
#ifdef __ANDROID__
    // [Android] 手機版禁止寫入資源檔 (APK 是唯讀的)
    // 這裡直接返回 true 避免報錯，或是您可以選擇 log warning
    // SDL_Log("Warning: TAbilityMgr::Save ignored on Android (Read-only)");
    return true;
#else
    // [Windows/PC] 保留存檔功能，但改用 SDL 介面以保持一致性
  // 當然您也可以保留 ofstream，但建議統一用 SDL

  std::string path = pFile;
  if (path.find("./") == 0) path = path.substr(2);

  const int nSize = sizeof(T);
  SDL_RWops* fout = SDL_RWFromFile(path.c_str(), "wb");

  if (!fout) return false;

  T t;
  memset(&t, 0, nSize);

  for (int i = 0; i < N1; ++i)
  {
    for  (int j = 0; j < N2; ++j)
    {
      // 複製到暫存變數，避免加密時汙染到記憶體中的原始數據
      memcpy(&t, &m_aBank[i][j], nSize);

      const int nSum = GetCheckSum((char*)(&t), nSize);

      // 寫入 CheckSum
      SDL_RWwrite(fout, &nSum, sizeof(int), 1);

      // 加密暫存變數
      Encode((char*)(&t), nSize);

      // 寫入加密後的 Data
      SDL_RWwrite(fout, &t, nSize, 1);
    }
  }

  SDL_RWclose(fout);
  return true;
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Protected Method (這些是純邏輯運算，完全不需要修改)
///////////////////////////////////////////////////////////////////////////////

template <class T, int N1, int N2>
int TAbilityMgr<T, N1, N2>::GetCheckSum(char* pData, int nSize)
{
    int nSum = 0;

    while (nSize--)
    {
        nSum += *pData++;
    }

    return nSum;
}

template <class T, int N1, int N2>
void TAbilityMgr<T, N1, N2>::Encode(char* pData, int nSize)
{
    while (nSize--)
    {
        *pData++ = (*pData + m_cMagic);
    }
}

template <class T, int N1, int N2>
void TAbilityMgr<T, N1, N2>::Decode(char* pData, int nSize)
{
    while (nSize--)
    {
        *pData++ = (*pData - m_cMagic);
    }
}

#define ABILITYLIMIT  TAbilityMgr<CAbilityLimit, 5, 6>  
#define ABILITYDIVIDE TAbilityMgr<CAbilityDivide, 5, 5>
extern ABILITYLIMIT  g_mgrLimit;
extern ABILITYDIVIDE g_mgrDivide;

#endif // !defined(AFX_ABILITY_H__893DF137_6634_4AE7_BC35_F2B34294522C__INCLUDED_)
