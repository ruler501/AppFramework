#include <jni.h>
#include <sstream>

#include "../main.h"
#include "Facebook.h"

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

extern "C" {

JNIEXPORT void JNICALL Java_com_fillmyblank_app_FillMyBlank_returnFriends(JNIEnv *pEnv, jobject pObj, int pointer, jstring resultStr){
    friends* tFriends = (friends*)pointer;
    std::vector<std::string> ids;
    const char *rStr = env->GetStringUTFChars(resultStr, JNI_FALSE);
    split(rStr, ',', ids);
    for(auto& s : ids){
        int t;
        std::istringstream(s) >> t;
        tFriends->friendIDs.push_back(t);
    }
    SDL_CondSignal(tFriends->waiting);
}

JNIEXPORT void JNICALL Java_com_fillmyblank_app_FillMyBlank_signalDone(JNIEnv *pEnv, jobject pObj, int pointer, jboolean result){
    goodCall* tCall = (goodCall*)pointer;
    tCall->success = result == JNI_TRUE;
    SDL_CondSignal(tCall->waiting);
}

}
