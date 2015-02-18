#include "main.h"
#include "XBluetooth.h"
#include <jni.h>

bool XBluetooth::initiate(){
    JNIEnv *aEnv = (JNIEnv *)SDL_AndroidGetJNIEnv();

    jclass aActivityClass = aEnv->FindClass(xClassPath.c_str());

    jmethodID aStaticMid = aEnv->GetStaticMethodID(aActivityClass, "GetActivity", (std::string("()L")+xClassPath+";").c_str());
    jobject aActivity =  aEnv->CallStaticObjectMethod(aActivityClass, aStaticMid);

    jmethodID aJavaMethodID = aEnv->GetMethodID(aActivityClass, "enableBluetooth", "()Z");

    jboolean result = aEnv->CallBooleanMethod(aActivity, aJavaMethodID);

    return (initiated = (result == JNI_TRUE));
}

bool XBluetooth::findDevice(std::string name){
    if(!initiated) return false;
    JNIEnv *aEnv = (JNIEnv *)SDL_AndroidGetJNIEnv();

    jclass aActivityClass = aEnv->FindClass(xClassPath.c_str());

    jmethodID aStaticMid = aEnv->GetStaticMethodID(aActivityClass, "GetActivity", (std::string("()L")+xClassPath+";").c_str());
    jobject aActivity =  aEnv->CallStaticObjectMethod(aActivityClass, aStaticMid);

    jmethodID aJavaMethodID = aEnv->GetMethodID(aActivityClass, "findBluetoothDevice", "(Ljava/lang/String)Z");

    jboolean result = aEnv->CallBooleanMethod(aActivity, aJavaMethodID, aEnv->NewStringUTF(name.c_str()));

    return result == JNI_TRUE;
}

bool XBluetooth::connectDevice(std::string uuid="00001101-0000-1000-8000-00805F9B34FB"){
    if(!initiated) return false;
    JNIEnv *aEnv = (JNIEnv *)SDL_AndroidGetJNIEnv();

    jclass aActivityClass = aEnv->FindClass(xClassPath.c_str());

    jmethodID aStaticMid = aEnv->GetStaticMethodID(aActivityClass, "GetActivity", (std::string("()L")+xClassPath+";").c_str());
    jobject aActivity =  aEnv->CallStaticObjectMethod(aActivityClass, aStaticMid);

    jmethodID aJavaMethodID = aEnv->GetMethodID(aActivityClass, "connectBluetoothDevice", "(Ljava/lang/String)Z");

    jboolean result = aEnv->CallBooleanMethod(aActivity, aJavaMethodID, aEnv->NewStringUTF(uuid.c_str()));

    return (connected = (result == JNI_TRUE));
}

bool XBluetooth::findAndConnect(std::string name, std::string uuid="00001101-0000-1000-8000-00805F9B34FB"){
    return findDevice(name) && connectDevice(uuid);
}

std::vector<uint8_t> XBluetooth::receiveBytes(size_t amount){
	if(!connected) return {};

	callReturn<std::vector<uint8_t> >* tBuffer = new callReturn<std::vector<uint8_t> >;
	SDL_mutex* tMutex = SDL_CreateMutex();

    JNIEnv *aEnv = (JNIEnv *)SDL_AndroidGetJNIEnv();

    jclass aActivityClass = aEnv->FindClass(xClassPath.c_str());

    std::string pFuncType = (std::string("()L")+xClassPath+";").c_str();
    jmethodID aStaticMid = aEnv->GetStaticMethodID(aActivityClass, "GetActivity", (std::string("()L")+xClassPath+";").c_str());
    jobject aActivity =  aEnv->CallStaticObjectMethod(aActivityClass, aStaticMid);

    jmethodID aJavaMethodID = aEnv->GetMethodID(aActivityClass, "receiveBluetooth", "(II)Z");

    jboolean result = aEnv->CallBooleanMethod(aActivity, aJavaMethodID, amount, tBuffer);

	if(result != JNI_TRUE) return std::vector<uint8_t>{};

	SDL_CondWait(tBuffer->waiting, tMutex);
	return tBuffer->result;
}

bool XBluetooth::sendBytes(std::vector<uint8_t> bytes){
	JNIEnv *aEnv = (JNIEnv *)SDL_AndroidGetJNIEnv();

    jclass aActivityClass = aEnv->FindClass(xClassPath.c_str());

    jmethodID aStaticMid = aEnv->GetStaticMethodID(aActivityClass, "GetActivity", (std::string("()L")+xClassPath+";").c_str());
    jobject aActivity =  aEnv->CallStaticObjectMethod(aActivityClass, aStaticMid);

    jbyteArray tBytes = aEnv->NewByteArray(bytes.size());
    aEnv->SetByteArrayRegion(tBytes, 0, bytes.size(), (jbyte*)bytes.data());

    jmethodID aJavaMethodID = aEnv->GetMethodID(aActivityClass, "receiveBluetooth", "([B)Z");

    jboolean result = aEnv->CallBooleanMethod(aActivity, aJavaMethodID, tBytes);

    return result == JNI_TRUE;
}

extern "C" {

JNIEXPORT void JNICALL Java_com_myapp_game_MyGame_receiveBytes(JNIEnv *pEnv, jobject pObj, jbyteArray bytes, int length, int pointer){
	jboolean isCopy;
	uint8_t* b = (uint8_t*)pEnv->GetByteArrayElements(bytes, &isCopy);
	callReturn<std::vector<uint8_t> >* tBuffer = (callReturn<std::vector<uint8_t> >*)pointer;
	tBuffer->result.assign(b, b+length);
	SDL_CondSignal(tBuffer->waiting);
}
}
