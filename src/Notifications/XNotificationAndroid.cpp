#include "jni.h"
#include "SDL.h"

#include "XNotification.h"
#include "main.h"

int XNotification::lastID = 0;

XNotification::XNotification(std::string _iconName, std::string _title, std::string _text, bool _dispatch = true)
: iconName(_iconName), title(_title), text(_text), id(++lastID)
{ dispatched = _dispatch ? dispatch() : false; }

bool XNotification::dispatch(){
    JNIEnv *aEnv = (JNIEnv *)SDL_AndroidGetJNIEnv();

    jclass aActivityClass = aEnv->FindClass(xClassPath.c_str());

    jmethodID aStaticMid = aEnv->GetStaticMethodID(aActivityClass, "GetActivity", (std::string("()L")+xClassPath+";").c_str());
    jobject aActivity =  aEnv->CallStaticObjectMethod(aActivityClass, aStaticMid);

    int pos = iconName.find(".");
    iconName = iconName.substr(0, pos);

    jmethodID aJavaMethodID = aEnv->GetMethodID(aActivityClass, "notify", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)Z");

    jboolean result = aEnv->CallBooleanMethod(aActivity, aJavaMethodID, aEnv->NewStringUTF(iconName.c_str()),
                                              aEnv->NewStringUTF(title.c_str()), aEnv->NewStringUTF(text.c_str()), id);

    return (result == JNI_TRUE);
}

bool XNotification::cancel(){
    JNIEnv *aEnv = (JNIEnv *)SDL_AndroidGetJNIEnv();

    jclass aActivityClass = aEnv->FindClass(xClassPath.c_str());

    jmethodID aStaticMid = aEnv->GetStaticMethodID(aActivityClass, "GetActivity", (std::string("()L")+xClassPath+";").c_str());
    jobject aActivity =  aEnv->CallStaticObjectMethod(aActivityClass, aStaticMid);

    jmethodID aJavaMethodID = aEnv->GetMethodID(aActivityClass, "cancelNotify", "(I)V");

    aEnv->CallVoidMethod(aActivity, aJavaMethodID, id);

    dispatched = false;

    return true;
}

XNotification::~XNotification(){
    if(dispatched) cancel();
}
