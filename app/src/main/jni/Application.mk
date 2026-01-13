# 使用動態連結的 C++ 標準庫
APP_STL := c++_shared
APP_CPPFLAGS := -std=c++17 -fexceptions -frtti

# 16KB 對齊 (必備)
APP_LDFLAGS += -Wl,-z,max-page-size=16384
APP_ABI := armeabi-v7a arm64-v8a x86 x86_64
APP_PLATFORM := android-21

# ==========================================================
#  請在這裡切換模式 (二選一)
# ==========================================================

# --- 模式 A：正式發布 / 追求極速 (無法除錯) ---
APP_OPTIM := release

# --- 模式 B：高速除錯 (推薦你現在用這個！) ---
# 說明：告訴 NDK 這是 debug 模式 (讓 APK 可被除錯)，
# 但手動強制開啟 -O2 優化 (讓遊戲不會卡頓)。
#APP_OPTIM := debug
#APP_CPPFLAGS += -O2 -g -ggdb
#APP_CFLAGS   += -O2 -g -ggdb

# --- 模式 C：傳統除錯 (變數最清楚，但會卡頓) ---
# APP_OPTIM := debug
# APP_CPPFLAGS += -O0 -g -ggdb
# APP_CFLAGS   += -O0 -g -ggdb