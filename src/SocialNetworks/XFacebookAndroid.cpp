#include "main.h"
#include "XFacebook.h"
#include <jni.h>

XFacebook::XFacebook(){
    icon = loadImage("fb.png");

    callReturn<bool>* tCall = new callReturn<bool>;

    tCall->result = false;
    SDL_mutex* tMutex = SDL_CreateMutex();

    JNIEnv *aEnv = (JNIEnv *)SDL_AndroidGetJNIEnv();

    jclass aActivityClass = aEnv->FindClass(xClassPath.c_str());

    jmethodID aStaticMid = aEnv->GetStaticMethodID(aActivityClass, "GetActivity", (std::string("()L")+xClassPath+";").c_str());
    jobject aActivity =  aEnv->CallStaticObjectMethod(aActivityClass, aStaticMid);

    jmethodID aJavaMethodID = aEnv->GetMethodID(aActivityClass, "checkFbLogin", "(I)V");

    aEnv->CallVoidMethod(aActivity, aJavaMethodID, tCall);

    if(!tCall->completed) SDL_CondWait(tCall->waiting, tMutex);

    loggedIn = tCall->result;
}

bool XFacebook::login(){
    callReturn<bool>* tCall = new callReturn<bool>;

    tCall->result = false;
    SDL_mutex* tMutex = SDL_CreateMutex();

    JNIEnv *aEnv = (JNIEnv *)SDL_AndroidGetJNIEnv();

    jclass aActivityClass = aEnv->FindClass(xClassPath.c_str());

    jmethodID aStaticMid = aEnv->GetStaticMethodID(aActivityClass, "GetActivity", (std::string("()L")+xClassPath+";").c_str());
    jobject aActivity =  aEnv->CallStaticObjectMethod(aActivityClass, aStaticMid);

    jmethodID aJavaMethodID = aEnv->GetMethodID(aActivityClass, "findFriendIDs", "(I)V");

    aEnv->CallVoidMethod(aActivity, aJavaMethodID, tCall);

    if(!tCall->completed) SDL_CondWait(tCall->waiting, tMutex);

    return (loggedIn = tCall->result);
}

std::vector<std::string> XFacebook::getFriendIDs(){
    if(!loggedIn) login();

    JNIEnv *aEnv = (JNIEnv *)SDL_AndroidGetJNIEnv();

    jclass aActivityClass = aEnv->FindClass(xClassPath.c_str());

    jmethodID aStaticMid = aEnv->GetStaticMethodID(aActivityClass, "GetActivity", (std::string("()L")+xClassPath+";").c_str());
    jobject aActivity =  aEnv->CallStaticObjectMethod(aActivityClass, aStaticMid);

    callReturn<std::vector<std::string> >* tCall = new callReturn<std::vector<std::string> >;
    SDL_mutex* tMutex = SDL_CreateMutex();

    jmethodID aJavaMethodID = aEnv->GetMethodID(aActivityClass, "returnFriendIDs", "(I)V");

    aEnv->CallVoidMethod(aActivity, aJavaMethodID, tCall);

    if(!tCall->completed) SDL_CondWait(tCall->waiting, tMutex);

    return tCall->result;
}

std::string XFacebook::getProfileID(){
    if(!loggedIn) login();
    callReturn<std::string>* tCall = new callReturn<std::string>;

    JNIEnv *aEnv = (JNIEnv *)SDL_AndroidGetJNIEnv();

    jclass aActivityClass = aEnv->FindClass(xClassPath.c_str());

    jmethodID aStaticMid = aEnv->GetStaticMethodID(aActivityClass, "GetActivity", (std::string("()L")+xClassPath+";").c_str());
    jobject aActivity =  aEnv->CallStaticObjectMethod(aActivityClass, aStaticMid);

    SDL_mutex* tMutex = SDL_CreateMutex();

    jmethodID aJavaMethodID = aEnv->GetMethodID(aActivityClass, "returnFbID", "(I)V");

    aEnv->CallVoidMethod(aActivity, aJavaMethodID, tCall);

    if(!tCall->completed) SDL_CondWait(tCall->waiting, tMutex);

    return tCall->result;
}

std::string XFacebook::getProfileImage(){
    return std::string("https://graph.facebook.com/") + getProfileID() + "/picture?type=large";
}

std::string XFacebook::getProfileName(){
    if(!loggedIn) login();
    callReturn<std::string>* tCall = new callReturn<std::string>;

    JNIEnv *aEnv = (JNIEnv *)SDL_AndroidGetJNIEnv();

    jclass aActivityClass = aEnv->FindClass(xClassPath.c_str());

    jmethodID aStaticMid = aEnv->GetStaticMethodID(aActivityClass, "GetActivity", (std::string("()L")+xClassPath+";").c_str());
    jobject aActivity =  aEnv->CallStaticObjectMethod(aActivityClass, aStaticMid);

    SDL_mutex* tMutex = SDL_CreateMutex();

    jmethodID aJavaMethodID = aEnv->GetMethodID(aActivityClass, "returnName", "(I)V");

    aEnv->CallVoidMethod(aActivity, aJavaMethodID, tCall);

    if(!tCall->completed) SDL_CondWait(tCall->waiting, tMutex);

    return tCall->result;
}
