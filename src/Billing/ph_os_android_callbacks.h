
/*! 
*
* \file ph_os_android_callbacks.h
* 	No description yet
* 	
* 	
*  26 May 2014 06:49:40
* \author Pallav Nawani
* Copyright IronCode Gaming Pvt. Ltd. (c) 2014
* 
*/



#ifndef _IC_PH_OS_ANDROID_CALLBACKS_H_
#define _IC_PH_OS_ANDROID_CALLBACKS_H_

#include <jni.h>
#include "XF_Billing.h"

#ifdef __cplusplus
extern "C" {
#endif

// Called by JAVA code to pass parameters back to c++
JNIEXPORT void JNICALL Java_com_fillmyblank_app_IcUpdateProductData(JNIEnv *pEnv, jobject pObj, jint pBManPtr, jstring pProdID, jstring pProdTitle, jstring pProdDes, jstring pProdPrice);


// Called by JAVA code to pass parameters back to c++
JNIEXPORT void JNICALL Java_com_fillmyblank_app_IcProductPurchased(JNIEnv *pEnv, jobject pObj, jint pBManPtr, jstring pProdID, jboolean pSuccess);

#ifdef __cplusplus
}
#endif

#endif




/*  End of file ph_os_android_callbacks.h  */

