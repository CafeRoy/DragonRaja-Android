#ifndef HWID_MANAGER_H
#define HWID_MANAGER_H

#include <string>

// ==========================================
// Windows 平台標頭檔
// ==========================================
#ifdef _WIN32
#include <windows.h>
    #include <sstream>
    #include <iomanip>

// ==========================================
// Android (SDL2) 平台標頭檔
// ==========================================
#elif defined(__ANDROID__)
#include <jni.h>
#include <SDL.h> // 引入 SDL 標頭檔
#endif

class HWIDManager
{
public:
    // =============================================================
    // 統一介面: 取得機器碼 (跨平台)
    // =============================================================
    static std::string GetHWID()
    {
#ifdef _WIN32
        // ---------------------------------------------------------
        // Windows 實作: 讀取 C 槽序號
        // ---------------------------------------------------------
        DWORD dwVolSerial = 0;
        BOOL bIsRetrieved = GetVolumeInformationA("C:\\", NULL, 0, &dwVolSerial, NULL, NULL, NULL, 0);

        if (bIsRetrieved)
        {
            std::stringstream ss;
            ss << std::uppercase << std::hex << std::setw(8) << std::setfill('0') << dwVolSerial;
            return ss.str();
        }
        return "WIN-ERR-0000";

#elif defined(__ANDROID__)
        // ---------------------------------------------------------
        // Android (SDL2) 實作: 透過 JNI 讀取 ANDROID_ID
        // ---------------------------------------------------------

        // 1. 透過 SDL 取得 JNIEnv 和 Activity
        JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
        jobject activity = (jobject)SDL_AndroidGetActivity();

        if (!env || !activity) return "SDL-JNI-ERR";

        std::string result = "ANDROID-ERR-UNKNOWN";

        // 2. 取得 Settings.Secure 類別
        jclass settingsSecureClass = env->FindClass("android/provider/Settings$Secure");
        if (settingsSecureClass)
        {
            // 3. 取得 ANDROID_ID 常數
            jfieldID androidIdField = env->GetStaticFieldID(settingsSecureClass, "ANDROID_ID", "Ljava/lang/String;");
            if (androidIdField)
            {
                jstring androidIdKey = (jstring)env->GetStaticObjectField(settingsSecureClass, androidIdField);

                // 4. 取得 getString 方法
                jmethodID getStringMethod = env->GetStaticMethodID(settingsSecureClass, "getString",
                                                                   "(Landroid/content/ContentResolver;Ljava/lang/String;)Ljava/lang/String;");

                if (getStringMethod && androidIdKey)
                {
                    // 5. 取得 ContentResolver (從 SDL Activity 取得)
                    jclass activityClass = env->GetObjectClass(activity);
                    jmethodID getContentResolverMethod = env->GetMethodID(activityClass, "getContentResolver",
                                                                          "()Landroid/content/ContentResolver;");

                    if (activityClass && getContentResolverMethod)
                    {
                        jobject contentResolver = env->CallObjectMethod(activity, getContentResolverMethod);
                        if (contentResolver)
                        {
                            // 6. 執行: Settings.Secure.getString(resolver, ANDROID_ID)
                            jstring androidIdValue = (jstring)env->CallStaticObjectMethod(settingsSecureClass, getStringMethod, contentResolver, androidIdKey);

                            if (androidIdValue)
                            {
                                const char* idStr = env->GetStringUTFChars(androidIdValue, NULL);
                                if (idStr)
                                {
                                    result = idStr;
                                    env->ReleaseStringUTFChars(androidIdValue, idStr);
                                }
                                env->DeleteLocalRef(androidIdValue);
                            }
                            env->DeleteLocalRef(contentResolver);
                        }
                    }
                    if (activityClass) env->DeleteLocalRef(activityClass);
                }
                if (androidIdKey) env->DeleteLocalRef(androidIdKey);
            }
            env->DeleteLocalRef(settingsSecureClass);
        }

        // 注意：SDL_AndroidGetActivity 回傳的是全域參考，不需要 DeleteLocalRef
        return result;
#else
        return "UNKNOWN-PLATFORM";
#endif
    }
};

#endif // HWID_MANAGER_H