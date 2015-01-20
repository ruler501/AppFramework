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
* \file XF_Billing.h
* 	No description yet
* 	
* 	
*  Fri Sep 13 16:39:42 IST 2013
*  \author Pallav Nawani
* Copyright IronCode Gaming Pvt. Ltd. (c) 2013
* 
*/



#ifndef _IC_XF_BILLING_H_
#define _IC_XF_BILLING_H_

#include <string>
using std::string;

#include <map>
using std::map;

#include <vector>
using std::vector;

namespace FrameworkX {
   
   class XAppBase;
   
   /// Purchase handler interface
   class XFPurchaseHandler {
   public:
      
      /// Handles Purchase
      virtual void OnPurchaseSucceeded(const string &pId, const string &pNum) = 0;
      virtual void OnPurchaseFailure(const string &pId) = 0;
   };
   
   /// In App Purchase/Billing Manager
   class XFBillingManager {
      
   protected:

      /// Product Information
      struct XFBMProductInfo
      {
	 std::string mProductID;
	 std::string mProductTitle;
	 std::string mProductDescription;
	 std::string mPrice;
	 bool mVerified;			///< Whether product data is verified
	 
	 /// Constructor
	 XFBMProductInfo(): mVerified(false) {}
      };
      
      typedef map<std::string, XFBMProductInfo> XFBMProductInfoMap;
      
   protected:
      XFBMProductInfoMap mProducts;
      
   private:
      
      XAppBase *mApp;	///< Pointer to the app
      XFPurchaseHandler *mPurchaseHandler;
      
      
   public:
      
      /// Constructor
      XFBillingManager();

      /// Destructor
      virtual ~XFBillingManager();
      
      /// Initialize Billing Manager
      virtual void Init(XAppBase *pApp, XFPurchaseHandler *pPurchaseHandler);

      /// Called after product description is obtained
      virtual void GotProductDescription(const string &pId, const string &pTitle, const string &pDescription, const string &pPrice);

      /// Called when purchase is completed on a product
      virtual void PurchaseCompleted(const string &pId, const string &pNum, bool pSuccess);

      /// Is it possible to purchase?
      virtual bool CanPurchase() = 0;
      
      /// Can this product be purchased?
      bool CanPurchaseProduct(const std::string &pProductID);
      
      /// Load Product Ids for the producs on sale. Always call the base class version
      /// if you override this function
      virtual void LoadProducts(const vector<string> &pProductIDs);
      
      /// Init purchase for a product
      virtual void InitPurchase(const string &pProductID) = 0;
      
      /// Restore previous purchases
      virtual void RestorePurchases() = 0;

      /// Restore previous purchases
      virtual void RestorePurchase(const string &pProductID);
      
      /// Fetches product information
      bool GetProductInfo(const std::string &pProductID, string &pTitle, string &pDescription, string &pPrice);

      
   };
   
   /// Creates a billing manager
   XFBillingManager* XFCreateBillingManager(XAppBase *pApp, XFPurchaseHandler *pPurchaseHandler);
   
}


#endif




/*  End of file XF_Billing.h  */

