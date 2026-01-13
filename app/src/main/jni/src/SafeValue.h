#pragma once

#include <iostream>
#include <random>
#include <chrono>
#include <type_traits>
#include <cstring> // 用於 std::memcmp
#include <SDL.h> // 保持你的 SDL 依賴

// 使用模板，讓它可以保護 int, float, double, bool 等
template <typename T>
class SafeValue {
    // 確保 T 是支持的類型：必須是數字或 bool
    /*static_assert(std::is_arithmetic_v<T> || std::is_same_v<T, bool>,
        "SafeValue 只能用於基本算術類型 (int, float, double, etc.) 或 bool。");*/

public:
    SafeValue() {
        Init();
        Set(T{}); // 預設值初始化為 0 或 false
    }

    SafeValue(T val) {
        Init();
        Set(val);
    }

    // --- 運算符重載 (讓使用更像普通變數) ---

    // 賦值運算： hp = 100;
    SafeValue<T>& operator=(T val) {
        Set(val);
        return *this;
    }

    // 取值運算： int currentHp = hp;
    operator T() {
        return Get();
    }

    // 算術運算： hp -= 10;
    SafeValue<T>& operator+=(T val) { Set(Get() + val); return *this; }
    SafeValue<T>& operator-=(T val) { Set(Get() - val); return *this; }
    // 可以根據需要添加 *=, /= 等

private:
    // --- 隨機垃圾填充 (Junk Padding) ---
    // volatile 防止編譯器優化掉這些未使用的變數
    volatile std::uint32_t _pad1;

    T _encryptedValue; // 存儲 XOR 後的值

    volatile std::uint32_t _pad2;

    T _key;            // 動態金鑰

    volatile std::uint32_t _pad3;

    T _checksum;       // 校驗和

    // 動態混淆因子 (基於物件地址，每個實例都不同)
    std::uintptr_t _addressSalt;

    // --- 初始化與輔助類型 ---

    void Init() {
        _addressSalt = reinterpret_cast<std::uintptr_t>(this);
        // 初始化垃圾數據
        _pad1 = GenerateRandomInt();
        _pad2 = GenerateRandomInt();
        _pad3 = GenerateRandomInt();
    }

    // 取得與 T 類型大小相匹配的地址鹽值（位元級安全）
    // 這解決了將 uintptr_t 直接轉為 T 可能導致的截斷或浮點數問題。
    T GetAddressSaltAsT() const {
        // 使用一個與 T 大小相同的整數類型 (例如 T 是 float，IntType 是 uint32_t)
        using IntType = typename std::conditional<sizeof(T) == 8, std::uint64_t, std::uint32_t>::type;

        // 僅取出 _addressSalt 的低位元部分
        IntType lowBits = static_cast<IntType>(_addressSalt);

        // 將這些位元視為 T 類型的值
        return *reinterpret_cast<const T*>(&lowBits);
    }

    // --- 核心邏輯 ---

    void Set(T plainValue) {
        // 1. 生成新金鑰 (每次寫入都換鎖)
        _key = GenerateNewKey();

        // 2. 加密： Cipher = Plain ^ Key
        _encryptedValue = XorOp(plainValue, _key);

        // 3. 計算校驗和： Checksum = Encrypted ^ Key ^ AddressSalt
        // 校驗和等於 Plain ^ AddressSalt (確保所有 XOR 都在位元層面進行)
        T plain = XorOp(_encryptedValue, _key);
        _checksum = XorOp(plain, GetAddressSaltAsT());
    }

    T Get() {
        // 1. 完整性檢查
        T plain = XorOp(_encryptedValue, _key); // 解密得到 Plain Value
        T calculatedChecksum = XorOp(plain, GetAddressSaltAsT()); // Plain ^ AddressSalt

        // 比較校驗和（使用 memcmp 確保對浮點數和填充數據的位元級比較）
        if (std::memcmp(&calculatedChecksum, &_checksum, sizeof(T)) != 0) {
            OnHackDetected();
            return T{}; // 返回安全預設值 (0 或 false)
        }

        // 2. 解密
        return plain;
    }

    // --- 輔助函式：位元級 XOR 操作 ---

    // 統一處理 XOR 操作，支援整數和浮點數
    T XorOp(T val, T key) const {
        /*if constexpr (std::is_floating_point_v<T> || std::is_same_v<T, bool>) {
            // 浮點數或布爾值需要轉成整數位元處理
            if constexpr (sizeof(T) == 8) {
                using IntType = std::uint64_t;
                IntType iVal = *reinterpret_cast<const IntType*>(&val);
                IntType iKey = *reinterpret_cast<const IntType*>(&key);
                IntType res = iVal ^ iKey;
                return *reinterpret_cast<T*>(&res);
            }
            else { // 適用於 float (4 bytes) 或 bool (1 byte，通常按 4 byte 處理)
                using IntType = std::uint32_t;
                // 注意：這裡假設 T (如 bool) 即使小於 4 bytes，我們也用 32 位元的操作數來進行位元級 XOR。
                // 這是為了確保浮點數轉換的一致性。
                IntType iVal, iKey;
                std::memcpy(&iVal, &val, sizeof(T));
                std::memcpy(&iKey, &key, sizeof(T));

                // 清除 T 類型以外的高位 (如果 T < sizeof(IntType))
                IntType mask = (1ULL << (sizeof(T) * 8)) - 1;
                iVal &= mask;
                iKey &= mask;

                IntType res = iVal ^ iKey;
                // 將結果的低位寫回 T
                T result;
                std::memcpy(&result, &res, sizeof(T));
                return result;
            }
        }
        else*/ {
            // 對於整數類型可以直接進行 XOR
            return val ^ key;
        }
    }


    // 效能優化的隨機數生成
    // 使用 thread_local 避免重複初始化開銷
    static std::uint32_t GenerateRandomInt() {
        // 使用 Xor-Shift 或其他輕量級 PRNG 可以獲得更好的性能，但 std::mt19937 在此處是安全且可接受的。
        static thread_local std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution<std::uint32_t> distribution;
        return distribution(generator);
    }

    static std::uint64_t GenerateRandomInt64() {
        static thread_local std::mt19937_64 generator(std::random_device{}());
        std::uniform_int_distribution<std::uint64_t> distribution;
        return distribution(generator);
    }

    static T GenerateNewKey() {
        /*if constexpr (sizeof(T) == 8) {
            std::uint64_t randomValue = GenerateRandomInt64(); // 儲存為 l-value
            return *reinterpret_cast<T*>(&randomValue);
        }
        else*/ {
            // 對於所有其他大小的 T (包括 int, float, bool)
            std::uint32_t randomValue = GenerateRandomInt(); // 儲存為 l-value
            return *reinterpret_cast<T*>(&randomValue);
        }
    }

    void OnHackDetected() {
        // 使用 SDL_LogCritical 保持與您原始程式碼的連貫性
        //SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SECURITY ALERT: Memory tampering detected at address %p. Resetting value.", this);

        // 策略：重置並更新垃圾數據，避免檢測器掃描相同模式
        _pad1 = GenerateRandomInt();
        Set(T{}); // 強制歸零/歸 false
    }
};