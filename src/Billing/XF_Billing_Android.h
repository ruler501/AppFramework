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
* \file XF_Billing_Android.h
* 	No description yet
* 	
* 	
*  18 May 2014 14:59:20
* \author Pallav Nawani
* Copyright IronCode Gaming Pvt. Ltd. (c) 2014
* 
*/



#ifndef _IC_XF_BILLING_ANDROID_H_
#define _IC_XF_BILLING_ANDROID_H_

#ifdef __ANDROID__

#include <string>
using std::string;

#include "XF_Billing.h"

#include <jni.h>

namespace FrameworkX {
   
   
   /// In App Purchase/Billing Manager
   class XFBillingManagerAndroid : public XFBillingManager
   {
   public:
      string mClassPath;
      JavaVM* mJVM;
      jclass mActivityClass;


   public:
      
      /// Constructor
      XFBillingManagerAndroid();

      /// Destructor
      virtual ~XFBillingManagerAndroid();
      
      /// Initialize Billing Manager
      virtual void Init(XAppBase *pApp, XFPurchaseHandler *pPurchaseHandler);

      /// Is it possible to purchase?
      virtual bool CanPurchase( );

      /// Called after product description is obtained
      virtual void GotProductDescription(const string &pId, const string &pTitle, const string &pDescription, const string &pPrice);

      /// Load Product Ids for the producs on sale. Always call the base class version
      /// if you override this function
      virtual void LoadProducts(const vector<string> &pProductIDs);
                
      /// Init purchase for a product
      virtual void InitPurchase(const string &pProductID);
      
      /// Restore previous purchases
      virtual void RestorePurchases();

      /// Restore previous purchase
      virtual void RestorePurchase(const string &pProductID);

      /// Fetches product IDS
      void GetProductIDS(vector<string> &pProductIDs);

     
   };
  
   /// Called in a separate thread
   int XFFetchProductDataJNI(void *pClassPtr);

}


#endif


#endif




/*  End of file XF_Billing_Android.h  */

