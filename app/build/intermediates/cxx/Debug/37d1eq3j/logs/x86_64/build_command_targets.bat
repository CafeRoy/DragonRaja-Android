@echo off
"C:\\Users\\lamyu\\AppData\\Local\\Android\\Sdk\\ndk\\27.0.12077973\\ndk-build.cmd" ^
  "NDK_PROJECT_PATH=null" ^
  "APP_BUILD_SCRIPT=C:\\MyAndroidGame\\android-project\\app\\src\\main\\jni\\Android.mk" ^
  "NDK_APPLICATION_MK=C:\\MyAndroidGame\\android-project\\app\\src\\main\\jni\\Application.mk" ^
  "APP_ABI=x86_64" ^
  "NDK_ALL_ABIS=x86_64" ^
  "NDK_DEBUG=1" ^
  "NDK_OUT=C:\\MyAndroidGame\\android-project\\app\\build\\intermediates\\cxx\\Debug\\37d1eq3j/obj" ^
  "NDK_LIBS_OUT=C:\\MyAndroidGame\\android-project\\app\\build\\intermediates\\cxx\\Debug\\37d1eq3j/lib" ^
  "APP_PLATFORM=android-21" ^
  SDL2 ^
  SDL2_image ^
  SDL2_mixer ^
  SDL2_net ^
  SDL2_ttf ^
  main
