
# Uncomment this if you're using STL in your project
# See CPLUSPLUS-SUPPORT.html in the NDK documentation for more information
# APP_STL := stlport_static

APP_ABI := armeabi armeabi-v7a
APP_STL := gnustl_static
APP_PLATFORM := android-12
APP_OPTIM := release
APP_CPPFLAGS += -std=gnu++11 -fexceptions -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -Wall -Wextra
NDK_TOOLCHAIN_VERSION := 4.8
