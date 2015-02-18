// The MIT License (MIT)
//
// Copyright (c) 2014 Ironcode Gaming
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

/*!
*
* \file XF_Billing_Android.cpp
* 	No description yet
*
*
*  18 May 2014 14:59:24
* \author Pallav Nawani
* Copyright IronCode Gaming Pvt. Ltd. (c) 2014
*
*/


#ifdef __ANDROID__


#include "SDL.h"
#include "XF_Billing_Android.h"
#include "main.h"


namespace FrameworkX
{


   //==///////////////////////////////////////////////////////////////////
   //
   ///
   /// Constructor
   ///
   /// \param
   ///
   /// \return
   ///
   //==///////////////////////////////////////////////////////////////////
   XFBillingManagerAndroid :: XFBillingManagerAndroid(): mJVM(NULL), mActivityClass(NULL)
   {
      JNIEnv *aEnv = (JNIEnv *)SDL_AndroidGetJNIEnv();
      aEnv->GetJavaVM(&mJVM);

   }

   //==///////////////////////////////////////////////////////////////////
   //
   ///
   /// Destructor
   ///
   /// \param
   ///
   /// \return
   ///
   //==///////////////////////////////////////////////////////////////////
   XFBillingManagerAndroid :: ~XFBillingManagerAndroid()
   {
      JNIEnv *aEnv = (JNIEnv *)SDL_AndroidGetJNIEnv();
      if(mActivityClass)
	 aEnv->DeleteLocalRef(mActivityClass);
	 mActivityClass = NULL;

   }


   //==///////////////////////////////////////////////////////////////////
   //
   ///
   /// Initialize Billing Manager
   ///
   /// \param
   ///
   /// \return
   ///
   //==///////////////////////////////////////////////////////////////////
   void XFBillingManagerAndroid :: Init(XAppBase *pApp, XFPurchaseHandler *pPurchaseHandler)
   {
      XFBillingManager::Init(pApp, pPurchaseHandler);
      mClassPath = xClassPath;
      JNIEnv *aEnv = (JNIEnv *)SDL_AndroidGetJNIEnv();
      mActivityClass = (jclass)aEnv->NewGlobalRef(aEnv->FindClass(mClassPath.c_str()));
   }


   //==///////////////////////////////////////////////////////////////////
   //
   ///
   /// Is it possible to purchase?
   ///
   /// \param
   ///
   /// \return
   ///
   //==///////////////////////////////////////////////////////////////////
   bool XFBillingManagerAndroid :: CanPurchase( )
   {
      JNIEnv *aEnv = (JNIEnv *)SDL_AndroidGetJNIEnv();
      //XFOsAndroidManageLocalRef aLocalRef(aEnv);

      jclass aActivityClass = aEnv->FindClass(mClassPath.c_str());

      string pFuncType = "()Lcom/myapp/game/MyGame;";
      jmethodID aStaticMid = aEnv->GetStaticMethodID(aActivityClass, "GetActivity", (std::string("()L")+xClassPath+";").c_str());
      jobject aActivity =  aEnv->CallStaticObjectMethod(aActivityClass, aStaticMid);

      jmethodID aJavaMethodID = aEnv->GetMethodID(aActivityClass, "XFBillingCanPurchase", "()Z");

      return aEnv->CallBooleanMethod(aActivity, aJavaMethodID);
   }

   //==///////////////////////////////////////////////////////////////////
   //
   /// Callback method.
   /// Called after product description is obtained
   ///
   /// \param
   ///
   /// \return
   ///
   //==///////////////////////////////////////////////////////////////////
   void XFBillingManagerAndroid :: GotProductDescription(const string &pId, const string &pTitle, const string &pDescription, const string &pPrice)
   {
      //XFLog(XFLog_Warning, "Product ID: %s, Title: %s, Description %s, Price %s! Pretty Cool, Huh!", pId.c_str(), pTitle.c_str(), pDescription.c_str(), pPrice.c_str());
      XFBillingManager :: GotProductDescription(pId, pTitle, pDescription, pPrice);
   }

   //==///////////////////////////////////////////////////////////////////
   //
   ///
   /// Load Product Ids for the producs on sale. Always call the base class version
   /// if you override this function
   ///
   /// \param
   ///
   /// \return
   ///
   //==///////////////////////////////////////////////////////////////////
   void XFBillingManagerAndroid :: LoadProducts(const vector<string> &pProductIDs)
   {
      XFBillingManager::LoadProducts(pProductIDs);

      SDL_Thread *aThread = SDL_CreateThread(FrameworkX::XFFetchProductDataJNI, "FetchProdData", this);
      if(!aThread) ;
	 //XFLog(XFLog_Warning, "XFBillingManagerAndroid :: LoadProducts(): Creating thread to fetch product data failed.");
   }


   //==///////////////////////////////////////////////////////////////////
   //
   ///
   /// Init purchase for a product
   ///
   /// \param
   ///
   /// \return
   ///
   //==///////////////////////////////////////////////////////////////////
   void XFBillingManagerAndroid :: InitPurchase(const string &pProductID)
   {
      SDL_Log("FILLMYBLANK Initiating purchase");
      JNIEnv *aEnv = (JNIEnv *)SDL_AndroidGetJNIEnv();
      //XFOsAndroidManageLocalRef aLocalRef(aEnv);

      jclass aActivityClass = aEnv->FindClass(mClassPath.c_str());

      string pFuncType = string("()L") + mClassPath + ";";
      jmethodID aStaticMid = aEnv->GetStaticMethodID(aActivityClass, "GetActivity", pFuncType.c_str());
      jobject aActivity =  aEnv->CallStaticObjectMethod(aActivityClass, aStaticMid);

      SDL_Log("FILLMYBLANK Got Activity");

      jmethodID aJavaMethodID = aEnv->GetMethodID(aActivityClass, "XFBillingInitPurchase", "(Ljava/lang/String;I)V");

      jstring aJStr = aEnv->NewStringUTF(pProductID.c_str());

      SDL_Log("Calling XFBillingInitPurchase");
      aEnv->CallVoidMethod(aActivity, aJavaMethodID, aJStr, this);
   }


   //==///////////////////////////////////////////////////////////////////
   //
   ///
   /// Restore previous purchases
   ///
   /// \param
   ///
   /// \return
   ///
   //==///////////////////////////////////////////////////////////////////
   void XFBillingManagerAndroid :: RestorePurchases()
   {

   }


   //==///////////////////////////////////////////////////////////////////
   //
   ///
   /// Restore previous purchases
   ///
   /// \param
   ///
   /// \return
   ///
   //==///////////////////////////////////////////////////////////////////
   void XFBillingManagerAndroid :: RestorePurchase(const string &pProductID)
   {
      JNIEnv *aEnv = (JNIEnv *)SDL_AndroidGetJNIEnv();
      //XFOsAndroidManageLocalRef aLocalRef(aEnv);

      jclass aActivityClass = aEnv->FindClass(mClassPath.c_str());

      string pFuncType = string("()L") + mClassPath + ";";
      jmethodID aStaticMid = aEnv->GetStaticMethodID(aActivityClass, "GetActivity", pFuncType.c_str());
      jobject aActivity =  aEnv->CallStaticObjectMethod(aActivityClass, aStaticMid);

      jmethodID aJavaMethodID = aEnv->GetMethodID(aActivityClass, "XFBillingRestorePurchase", "(Ljava/lang/String;I)Z");

      jstring aJStr = aEnv->NewStringUTF(pProductID.c_str());
      aEnv->CallBooleanMethod(aActivity, aJavaMethodID, aJStr, this);

   }



   //==///////////////////////////////////////////////////////////////////
   //
   ///
   /// Fetches product IDS
   ///
   /// \param
   ///
   /// \return
   ///
   //==///////////////////////////////////////////////////////////////////
   void XFBillingManagerAndroid :: GetProductIDS(vector<string> &pProductIDs)
   {
      XFBMProductInfoMap::iterator aIter;
      for(aIter = mProducts.begin(); aIter != mProducts.end(); aIter++)
	 pProductIDs.push_back(aIter->first);

   }


   //==///////////////////////////////////////////////////////////////////
   //
   ///
   /// Creates a billing manager
   ///
   /// \param
   ///
   /// \return
   ///
   //==///////////////////////////////////////////////////////////////////
   XFBillingManager* XFCreateBillingManager(XAppBase *pApp, XFPurchaseHandler *pPurchaseHandler)
   {
      XFBillingManagerAndroid *aBManager = new XFBillingManagerAndroid;
      aBManager->Init(pApp, pPurchaseHandler);
      return aBManager;
   }


   //==///////////////////////////////////////////////////////////////////
   //
   ///
   // Call through JNI to fetch product data
   ///
   /// \param
   ///
   /// \return
   ///	s
   //==///////////////////////////////////////////////////////////////////
   int XFFetchProductDataJNI(void *pClassPtr)
   {
      XFBillingManagerAndroid *aBman = static_cast<XFBillingManagerAndroid*>(pClassPtr);

      JNIEnv *aEnv = NULL;
      aBman->mJVM->AttachCurrentThread(&aEnv, NULL);

      vector<string> aIdVec;
      aBman->GetProductIDS(aIdVec);
      //XFOsAndroidManageLocalRef aLocalRef(aEnv, 16 + aIdVec.size());

      string pFuncType = string("()L") + aBman->mClassPath + ";";
      jmethodID aStaticMid = aEnv->GetStaticMethodID(aBman->mActivityClass, "GetActivity", pFuncType.c_str());
      jobject aActivity =  aEnv->CallStaticObjectMethod(aBman->mActivityClass, aStaticMid);
      aEnv->DeleteLocalRef((jobject)aStaticMid);


      jclass aStringClass = aEnv->FindClass("java/lang/String");

      jobjectArray aJavaIDVec = aEnv->NewObjectArray(aIdVec.size(), aStringClass, NULL);
      for (size_t aI = 0; aI < aIdVec.size(); aI++) {
	 jstring aJStr = aEnv->NewStringUTF(aIdVec[aI].c_str());
	 aEnv->SetObjectArrayElement(aJavaIDVec, aI, aJStr);
      }

      aEnv->DeleteLocalRef((jobject)aStringClass);
      jmethodID aJavaMethodID1 = aEnv->GetMethodID(aBman->mActivityClass, "XFBillingQueryPurchases", "([Ljava/lang/String;I)V");
      aEnv->CallVoidMethod(aActivity, aJavaMethodID1, aJavaIDVec, pClassPtr);

      aEnv->DeleteLocalRef((jobject)aJavaIDVec);
      aEnv->DeleteLocalRef((jobject)aJavaMethodID1);
      aEnv->DeleteLocalRef((jobject)aActivity);

      aBman->mJVM->DetachCurrentThread();

   }

}


#endif

//==///////////////////////////////////////////////////////////////////
//
///
///
/// \param
///
/// \return
///
//==///////////////////////////////////////////////////////////////////


/*  End of file XF_Billing_Android.cpp  */

