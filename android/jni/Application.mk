
# Uncomment this if you're using STL in your project
# See CPLUSPLUS-SUPPORT.html in the NDK documentation for more information
# APP_STL := stlport_static

APP_ABI := armeabi armeabi-v7a
APP_STL := gnustl_static
APP_PLATFORM := android-12
APP_CPPFLAGS += -std=c++11 -fexceptions -fpermissive -Wall -Wextra
