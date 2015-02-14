#include <jni.h>
#include <sstream>

#include "../main.h"
#include "XFacebook.h"

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

extern "C" {
JNIEXPORT void JNICALL Java_com_myapp_game_MyGame_returnFriends(JNIEnv *pEnv, jobject pObj, int pointer, jstring resultStr){
    friends* tFriends = (friends*)pointer;
    tFriends->completed = true;
    std::vector<std::string> ids;
    const char *rStr = pEnv->GetStringUTFChars(resultStr, JNI_FALSE);
    split(rStr, ',', ids);
    /*for(auto& s : ids){
        int t;
        std::istringstream(s) >> t;
        tFriends->friendIDs.push_back(t);
    }*/
    tFriends->friendIDs = ids;
    SDL_CondSignal(tFriends->waiting);
}

JNIEXPORT void JNICALL Java_com_myapp_game_MyGame_signalDone(JNIEnv *pEnv, jobject pObj, int pointer, jboolean result){
    goodCall* tCall = (goodCall*)pointer;
    tCall->completed = true;
    tCall->success = result == JNI_TRUE;
    SDL_CondSignal(tCall->waiting);
}

JNIEXPORT void JNICALL Java_com_myapp_game_MyGame_returnString(JNIEnv *pEnv, jobject pObj, int pointer, jstring resultStr){
    sReturn* tRet = (sReturn*)pointer;
    tRet->completed = true;
    const char *rStr = pEnv->GetStringUTFChars(resultStr, JNI_FALSE);
    tRet->result = rStr;
    SDL_CondSignal(tRet->waiting);
}
}
