LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

# 1. 設定標頭檔路徑 (Includes)
# -----------------------------------------------------------
# 因為我們現在在 src 資料夾內，所以找 SDL 要往上一層 (../) 找 jni
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../jni/SDL/include \
                    $(LOCAL_PATH)/../SDL2_image \
                    $(LOCAL_PATH)/../SDL2_ttf \
                    $(LOCAL_PATH)/../SDL2_net \
                    $(LOCAL_PATH)/../SDL2_mixer

# ★★★ 關鍵修改：ImGui 就在當前目錄下的 imgui 資料夾 ★★★
# 不要在這裡寫 /src/imgui，只要寫 /imgui
LOCAL_C_INCLUDES += $(LOCAL_PATH)/imgui

# 2. 載入原始碼檔案 (Source Files)
# -----------------------------------------------------------

LOCAL_SRC_FILES := \
    $(wildcard $(LOCAL_PATH)/*.cpp) \
    $(LOCAL_PATH)/imgui/imgui.cpp \
    $(LOCAL_PATH)/imgui/imgui_demo.cpp \
    $(LOCAL_PATH)/imgui/imgui_draw.cpp \
    $(LOCAL_PATH)/imgui/imgui_tables.cpp \
    $(LOCAL_PATH)/imgui/imgui_widgets.cpp \
    $(LOCAL_PATH)/imgui/imgui_impl_sdl2.cpp \
    $(LOCAL_PATH)/imgui/imgui_impl_sdlrenderer2.cpp

# 3. 連結設定
LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image SDL2_ttf SDL2_net SDL2_mixer
LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog -landroid

LOCAL_CFLAGS += -fsigned-char
LOCAL_CPPFLAGS += -fsigned-char

include $(BUILD_SHARED_LIBRARY)