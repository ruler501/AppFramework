
/*!
*
* \file ph_os_android_callbacks.cpp
* 	No description yet
*
*
*  26 May 2014 06:49:43
* \author Pallav Nawani
* Copyright IronCode Gaming Pvt. Ltd. (c) 2014
*
*/

#include "SDL.h"

#include "ph_os_android_callbacks.h"
using namespace FrameworkX;

extern "C" {

//==///////////////////////////////////////////////////////////////////
//
///
/// Called by JAVA code to pass parameters back to c++
///
/// \param
///
/// \return
///
//==///////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_com_fillmyblank_app_FillMyBlank_IcUpdateProductData(JNIEnv *pEnv, jobject pObj, int pBManPtr, jstring pProdID, jstring pProdTitle, jstring pProdDes, jstring pProdPrice)
{
   XFBillingManager *aBMan = (XFBillingManager*)pBManPtr;
   const char *aProdID = pEnv->GetStringUTFChars(pProdID, NULL);
   const char *aProdTitle = pEnv->GetStringUTFChars(pProdTitle, NULL);
   const char *aProdDes = pEnv->GetStringUTFChars(pProdDes, NULL);
   const char *aProdPrice = pEnv->GetStringUTFChars(pProdPrice, NULL);

   aBMan->GotProductDescription(aProdID, aProdTitle, aProdDes, aProdPrice);

   pEnv->ReleaseStringUTFChars(pProdID, aProdID);
   pEnv->ReleaseStringUTFChars(pProdTitle, aProdTitle);
   pEnv->ReleaseStringUTFChars(pProdDes, aProdDes);
   pEnv->ReleaseStringUTFChars(pProdPrice, aProdPrice);

}


//==///////////////////////////////////////////////////////////////////
//
///
/// Called by JAVA code to pass parameters back to c++
///
/// \param
///
/// \return
///
//==///////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_com_fillmyblank_app_FillMyBlank_IcProductPurchased(JNIEnv *pEnv, jobject pObj, int pBManPtr, jstring pProdID, jboolean pSuccess)
{

   SDL_Log("FILLMYBLANK Product Purchased");
   XFBillingManager *aBMan = (XFBillingManager*)pBManPtr;
   const char *aProdID = pEnv->GetStringUTFChars(pProdID, NULL);

   aBMan->PurchaseCompleted(aProdID, "1", pSuccess);

   pEnv->ReleaseStringUTFChars(pProdID, aProdID);


}

}

//==///////////////////////////////////////////////////////////////////
//
///
///
/// \param
///
/// \return
///
//==///////////////////////////////////////////////////////////////////


/*  End of file ph_os_android_callbacks.cpp  */

