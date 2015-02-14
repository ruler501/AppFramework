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
* \file XF_Billing_Mac.h
* 	No description yet
* 	
* 	
*  Sat Sep 14 12:27:31 IST 2013
*  \author Pallav Nawani
* Copyright IronCode Gaming Pvt. Ltd. (c) 2013
* 
*/



#ifndef _IC_XF_BILLING_MAC_H_
#define _IC_XF_BILLING_MAC_H_

#ifdef __APPLE__
#include <TargetConditionals.h>


#include "XF_Billing.h"

namespace FrameworkX {
   
   
   /// Mac/iOS Billing (In App Purchase) Handler
   class XFMacBillingManager: public XFBillingManager {

      /// Forward declaration
      struct XFPrivateImpl;
      
      XFPrivateImpl *mPrivateImpl;	 ///< Private Billing Manager Implementation
      
   public:
      
      /// Constructor
      XFMacBillingManager();
      
      /// Destructor
      virtual ~XFMacBillingManager();
      
      /// Initialize Billing Manager
      virtual void Init(XAppBase *pApp, XFPurchaseHandler *pPurchaseHandler);
      
      /// Is it possible to purchase?
      virtual bool CanPurchase();
      
      /// Load Product Ids for the producs on sale. Always call the base class version
      /// if you override this function
      virtual void LoadProducts(const vector<string> &pProductIDs);
      
      /// Init purchase for a product
      virtual void InitPurchase(const string &pProductID);
      
      /// Restore previous purchases
      virtual void RestorePurchases();
      
   };
   
   
}

//com.ironcode.ios.pahelikarevelations.unlockfull
#endif


#endif




/*  End of file XF_Billing_Mac.h  */

