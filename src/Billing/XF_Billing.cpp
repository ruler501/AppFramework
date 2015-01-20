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
* \file XF_Billing.cpp
* 	No description yet
*
*
*  Fri Sep 13 16:38:57 IST 2013
*  \author Pallav Nawani
* Copyright IronCode Gaming Pvt. Ltd. (c) 2013
*
*/


#include "XF_Billing.h"
#include <assert.h>
#include "SDL.h"

namespace FrameworkX {


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
   XFBillingManager :: XFBillingManager(): mApp(NULL), mPurchaseHandler(NULL)
   {


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
   XFBillingManager :: ~XFBillingManager()
   {


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
   void XFBillingManager :: Init(XAppBase *pApp, XFPurchaseHandler *pPurchaseHandler)
   {
      mApp = pApp;
      mPurchaseHandler = pPurchaseHandler;

   }

   /// Can this product be purchased?
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
   bool XFBillingManager :: CanPurchaseProduct(const std::string &pProductID)
   {
      XFBMProductInfoMap::iterator aIProductIter(mProducts.find(pProductID));
      if (aIProductIter != mProducts.end())
	 return aIProductIter->second.mVerified;

      return false;
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
   void XFBillingManager :: GotProductDescription(const string &pId, const string &pTitle, const string &pDescription, const string &pPrice)
   {
      assert(!pId.empty());
      assert(!pPrice.empty());

      XFBMProductInfoMap::iterator aIProductIter(mProducts.find(pId));
      if (aIProductIter != mProducts.end()) {
	 aIProductIter->second.mProductID = pId;
	 aIProductIter->second.mProductTitle = pTitle;
	 aIProductIter->second.mProductDescription = pDescription;
	 aIProductIter->second.mPrice = pPrice;
	 aIProductIter->second.mVerified = true;
      }

   }


   //==///////////////////////////////////////////////////////////////////
   //
   /// Callback method.
   /// Called when purchase is completed on a product
   ///
   /// \param
   ///
   /// \return
   ///
   //==///////////////////////////////////////////////////////////////////
   void XFBillingManager :: PurchaseCompleted(const string &pId, const string &pNum, bool pSuccess)
   {
      SDL_Log("FILLMYBLANK Purchase Completed");
      if(!mPurchaseHandler)
	 return;

      if(pSuccess)
	 mPurchaseHandler->OnPurchaseSucceeded(pId, pNum);
      else
	 mPurchaseHandler->OnPurchaseFailure(pId);

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
   void XFBillingManager :: LoadProducts(const vector<string> &pProductIDs)
   {
      XFBMProductInfo aInfo;
      for (unsigned aI = 0; aI < pProductIDs.size(); aI++) {
	 if (!pProductIDs[aI].empty())
	    mProducts[pProductIDs[aI]] = aInfo;
      }
   }


   //==///////////////////////////////////////////////////////////////////
   //
   ///
   /// Fetches product information
   ///
   /// \param
   ///
   /// \return
   ///
   //==///////////////////////////////////////////////////////////////////
   bool XFBillingManager :: GetProductInfo(const std::string &pProductID, string &pTitle, string &pDescription, string &pPrice)
   {
      XFBMProductInfoMap::iterator aIProductIter(mProducts.find(pProductID));
      if (aIProductIter != mProducts.end())
      {
	 pTitle		= aIProductIter->second.mProductTitle;
	 pDescription	= aIProductIter->second.mProductDescription;
	 pPrice		= aIProductIter->second.mPrice;
	 return true;
      }

      return false;
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
   void XFBillingManager :: RestorePurchase(const string &pProductID)
   {
      RestorePurchases();
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


/*  End of file XF_Billing.cpp  */

