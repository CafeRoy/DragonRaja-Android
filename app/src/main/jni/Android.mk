LOCAL_PATH := $(call my-dir)

# 1. 載入 SDL (注意：這裡絕對不能有 /src/)
include $(LOCAL_PATH)/SDL/Android.mk
include $(LOCAL_PATH)/../SDL2_image/Android.mk
include $(LOCAL_PATH)/../SDL2_ttf/Android.mk
include $(LOCAL_PATH)/../SDL2_net/Android.mk
include $(LOCAL_PATH)/../SDL2_mixer/Android.mk

# 2. 載入您的遊戲代碼
include $(LOCAL_PATH)/../src/Android.mk