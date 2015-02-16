#include "jni.h"
#include "SDL.h"

#include "XNotification.h"

bool dispatchNotification(std::string iconName, std::string title, std::string text){
    JNIEnv *aEnv = (JNIEnv *)SDL_AndroidGetJNIEnv();
    std::string mClassPath = "com/myapp/game/MyGame";

    jclass aActivityClass = aEnv->FindClass(mClassPath.c_str());

    std::string pFuncType = "()Lcom/myapp/game/MyGame;";
    jmethodID aStaticMid = aEnv->GetStaticMethodID(aActivityClass, "GetActivity", "()Lcom/myapp/game/MyGame;");
    jobject aActivity =  aEnv->CallStaticObjectMethod(aActivityClass, aStaticMid);

    int pos = iconName.find(".");
    iconName = iconName.substr(0, pos);

    jmethodID aJavaMethodID = aEnv->GetMethodID(aActivityClass, "notify", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z");

    jboolean result = aEnv->CallBooleanMethod(aActivity, aJavaMethodID, aEnv->NewStringUTF(iconName.c_str()),
                                              aEnv->NewStringUTF(title.c_str()), aEnv->NewStringUTF(text.c_str()));

    return (result == JNI_TRUE);
}
