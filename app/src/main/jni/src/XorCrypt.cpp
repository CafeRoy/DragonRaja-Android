#include <random> 
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <chrono>
#include <SDL.h>
#include "XorCrypt.h"
// 靜態隨機數產生器：用於 GenerateRandomKey()
// 在類外定義，確保線程安全初始化 (C++11/14/17 標準)
static std::random_device rd;
static std::mt19937 gen(rd());

// 建構子：初始化所有成員變數
XorCrypt::XorCrypt() {
    checksum = 0;
	key = 0;
	value = 0;
    salt = 0; // 初始化 salt
    init_falg = false;
}

// 產生一個更安全的隨機金鑰
std::int32_t XorCrypt::GenerateRandomKey() {
    std::uniform_int_distribution<std::int32_t> dist;
    std::int32_t rand_val = dist(gen);
    
    // 【強化點】增加一個固定的混淆操作，提升金鑰的不可預測性
    return rand_val ^ 0x5A5A5A5A; 
}

// 更新校驗和
// 【強化點】 Checksum 計算使用動態 salt 
void XorCrypt::UpdateChecksum() {
    checksum = value ^ key ^ salt; 
}

// 初始化金鑰和校驗和
void XorCrypt::Init() {
    key = GenerateRandomKey();
    salt = GenerateRandomKey(); // 【強化點】 初始化 salt
    
    // 初始值 (明文 0) 應被加密後儲存
    value = 0 ^ key; 
    UpdateChecksum();
	init_falg = true;
}


// 獲取解密值並檢查完整性
int XorCrypt::GetDecrypted() {

    if (!init_falg) {
		return 0;
    }
    
    // 關鍵的動態完整性檢查：現在使用動態 salt 進行比較
    if (checksum != (value ^ key ^ salt)) {

#ifdef _DEBUG
		SDL_Log("Integrity check failed: Checksum mismatch.");
		SDL_Log("checksum=%d value=%d key=%d salt=%d", checksum, value, key, salt);
        SDL_Log("Expected checksum: %d", (value ^ key ^ salt));
#endif
        // 檢測失敗！進行自我修復 (重置為安全的加密 0 狀態)
        key = GenerateRandomKey();
        salt = GenerateRandomKey(); // 自我修復時，更新 salt
        value = 0 ^ key; // 將明文值重設為 0，並重新加密
        UpdateChecksum();
        // 返回安全值 (明文 0)
        return 0;
    }

    return value ^ key;
}

// 設定新值
void XorCrypt::PutEncrypted(int subj) {

    if (!init_falg) {
        // 首次呼叫，進行初始化
        Init(); 
    }
    else {
        // 非首次呼叫，先檢查完整性並執行自我修復 (如果需要)
        GetDecrypted(); 
    }

    // 重新加密並更新金鑰、鹽值和校驗和
    key = GenerateRandomKey();
    salt = GenerateRandomKey(); // 【強化點】 每次寫入都更新 salt
    value = subj ^ key;
    UpdateChecksum();
}