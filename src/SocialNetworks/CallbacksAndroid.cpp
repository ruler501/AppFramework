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
    callReturn<std::vector<std::string> >* tCall = (callReturn<std::vector<std::string> >*)pointer;
    tCall->completed = true;
    std::vector<std::string> ids;
    const char *rStr = pEnv->GetStringUTFChars(resultStr, JNI_FALSE);
    split(rStr, ',', ids);
    /*for(auto& s : ids){
        int t;
        std::istringstream(s) >> t;
        tFriends->friendIDs.push_back(t);
    }*/
    tCall->result = ids;
    SDL_CondSignal(tCall->waiting);
}

JNIEXPORT void JNICALL Java_com_myapp_game_MyGame_signalDone(JNIEnv *pEnv, jobject pObj, int pointer, jboolean result){
    callReturn<bool>* tCall = (callReturn<bool>*)pointer;
    tCall->completed = true;
    tCall->result = result == JNI_TRUE;
    SDL_CondSignal(tCall->waiting);
}

JNIEXPORT void JNICALL Java_com_myapp_game_MyGame_returnString(JNIEnv *pEnv, jobject pObj, int pointer, jstring resultStr){
    callReturn<std::string>* tCall = (callReturn<std::string>*)pointer;
    tCall->completed = true;
    const char *rStr = pEnv->GetStringUTFChars(resultStr, JNI_FALSE);
    tCall->result = rStr;
    SDL_CondSignal(tCall->waiting);
}
}
