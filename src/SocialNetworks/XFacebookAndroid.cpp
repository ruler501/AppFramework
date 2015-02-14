#include "XFacebook.h"
#include <jni.h>

XFacebook::XFacebook(){
    goodCall* tCall = new goodCall;

    tCall->waiting = SDL_CreateCond();
    SDL_mutex* tMutex = SDL_CreateMutex();

    JNIEnv *aEnv = (JNIEnv *)SDL_AndroidGetJNIEnv();
    std::string mClassPath = "com/myapp/game/MyGame";

    jclass aActivityClass = aEnv->FindClass(mClassPath.c_str());

    std::string pFuncType = "()Lcom/myapp/game/MyGame;";
    jmethodID aStaticMid = aEnv->GetStaticMethodID(aActivityClass, "GetActivity", "()Lcom/myapp/game/MyGame;");
    jobject aActivity =  aEnv->CallStaticObjectMethod(aActivityClass, aStaticMid);

    jmethodID aJavaMethodID = aEnv->GetMethodID(aActivityClass, "checkFbLogin", "(I)V");

    aEnv->CallVoidMethod(aActivity, aJavaMethodID, tCall);

    if(!tCall->completed) SDL_CondWait(tCall->waiting, tMutex);

    loggedIn = tCall->success;
}

bool XFacebook::login(){
    goodCall* tCall = new goodCall;

    tCall->waiting = SDL_CreateCond();
    SDL_mutex* tMutex = SDL_CreateMutex();

    JNIEnv *aEnv = (JNIEnv *)SDL_AndroidGetJNIEnv();
    std::string mClassPath = "com/myapp/game/MyGame";

    jclass aActivityClass = aEnv->FindClass(mClassPath.c_str());

    std::string pFuncType = "()Lcom/myapp/game/MyGame;";
    jmethodID aStaticMid = aEnv->GetStaticMethodID(aActivityClass, "GetActivity", "()Lcom/myapp/game/MyGame;");
    jobject aActivity =  aEnv->CallStaticObjectMethod(aActivityClass, aStaticMid);

    jmethodID aJavaMethodID = aEnv->GetMethodID(aActivityClass, "findFriendIDs", "(I)V");

    aEnv->CallVoidMethod(aActivity, aJavaMethodID, tCall);

    if(!tCall->completed) SDL_CondWait(tCall->waiting, tMutex);

    return (loggedIn = tCall->success);
}

std::vector<std::string> XFacebook::getFriendIDs(){
    if(!loggedIn) login();

    JNIEnv *aEnv = (JNIEnv *)SDL_AndroidGetJNIEnv();

    std::string mClassPath = "com/myapp/game/MyGame";

    jclass aActivityClass = aEnv->FindClass(mClassPath.c_str());

    jmethodID aStaticMid = aEnv->GetStaticMethodID(aActivityClass, "GetActivity", "()Lcom/myapp/game/MyGame;");
    jobject aActivity =  aEnv->CallStaticObjectMethod(aActivityClass, aStaticMid);

    friends* fp = new friends;
    fp->waiting = SDL_CreateCond();
    SDL_mutex* tMutex = SDL_CreateMutex();

    jmethodID aJavaMethodID = aEnv->GetMethodID(aActivityClass, "returnFriendIDs", "(I)V");

    aEnv->CallVoidMethod(aActivity, aJavaMethodID, fp);

    if(!fp->completed) SDL_CondWait(fp->waiting, tMutex);

    return fp->friendIDs;
}

std::string XFacebook::getProfileID(){
    if(!loggedIn) login();
    sReturn* tRet = new sReturn;
    tRet->waiting = SDL_CreateCond();

    JNIEnv *aEnv = (JNIEnv *)SDL_AndroidGetJNIEnv();

    std::string mClassPath = "com/myapp/game/MyGame";

    jclass aActivityClass = aEnv->FindClass(mClassPath.c_str());

    jmethodID aStaticMid = aEnv->GetStaticMethodID(aActivityClass, "GetActivity", "()Lcom/myapp/game/MyGame;");
    jobject aActivity =  aEnv->CallStaticObjectMethod(aActivityClass, aStaticMid);

    SDL_mutex* tMutex = SDL_CreateMutex();

    jmethodID aJavaMethodID = aEnv->GetMethodID(aActivityClass, "returnFbID", "(I)V");

    aEnv->CallVoidMethod(aActivity, aJavaMethodID, tRet);

    if(!tRet->completed) SDL_CondWait(tRet->waiting, tMutex);

    return tRet->result;
}

std::string XFacebook::getProfileImage(){
    return std::string("https://graph.facebook.com/") + getProfileID() + "/picture?type=large";
}

std::string XFacebook::getProfileName(){
    if(!loggedIn) login();
    sReturn* tRet = new sReturn;
    tRet->waiting = SDL_CreateCond();

    JNIEnv *aEnv = (JNIEnv *)SDL_AndroidGetJNIEnv();

    std::string mClassPath = "com/myapp/game/MyGame";

    jclass aActivityClass = aEnv->FindClass(mClassPath.c_str());

    jmethodID aStaticMid = aEnv->GetStaticMethodID(aActivityClass, "GetActivity", "()Lcom/myapp/game/MyGame;");
    jobject aActivity =  aEnv->CallStaticObjectMethod(aActivityClass, aStaticMid);

    SDL_mutex* tMutex = SDL_CreateMutex();

    jmethodID aJavaMethodID = aEnv->GetMethodID(aActivityClass, "returnName", "(I)V");

    aEnv->CallVoidMethod(aActivity, aJavaMethodID, tRet);

    if(!tRet->completed) SDL_CondWait(tRet->waiting, tMutex);

    return tRet->result;
}
