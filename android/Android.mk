LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL2

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../SDL2_ttf

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../SDL2_mixer

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../SDL2_net

LOCAL_C_INCLUDES += $(abspath $(LOCAL_PATH))/../gpg-cpp-sdk/include

# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c

LOCAL_SRC_FILES += main.cpp

LOCAL_SRC_FILES += EventController.cpp

LOCAL_SRC_FILES += example.cpp

LOCAL_SRC_FILES += Billing/ph_os_android_callbacks.cpp

LOCAL_SRC_FILES += Billing/XF_Billing.cpp

LOCAL_SRC_FILES += Billing/XF_Billing_Android.cpp

LOCAL_SRC_FILES += Facebook/CallbacksAndroid.cpp

LOCAL_SRC_FILES += GooglePlus/GooglePlus.cpp

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_mixer SDL2_ttf SDL2_net gpg-1

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)