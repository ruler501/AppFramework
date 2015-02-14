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
* \file XF_Billing_ios.cpp
* 	No description yet
* 	
* 	
*  Fri Nov 15 16:05:02 IST 2013
*  \author Pallav Nawani
* Copyright IronCode Gaming Pvt. Ltd. (c) 2013
* 
*/


#include "XF_Billing_ios.h"


#ifdef __APPLE__
#include <TargetConditionals.h>

#if TARGET_OS_IPHONE

#import <CoreFoundation/CoreFoundation.h>
#import <StoreKit/StoreKit.h>


/// Billing class for Mac/iOS
@interface XFBillingiOS : NSObject <SKProductsRequestDelegate, SKPaymentTransactionObserver>
{
   NSArray *mProducts;
   SKProductsRequest *mProductRequest;

   FrameworkX::XFPurchaseHandler *mPurchaseHandler;	 ///< Manages Purchases
   FrameworkX::XFBillingManager *mBillingManager;	 ///< Handles Billing
}

/// Can we make a purchase?
- (BOOL) CanPurchase;

/// Opens a new facebook session
- (void) Init: (FrameworkX::XFPurchaseHandler *) pPurchaseHandler BillingManager:  (FrameworkX::XFBillingManager *) pBillingManager;

/// Fetches Product Description
- (void) GetProductDescription: (NSSet *)pProductIDs;

/// Given a product ID, initiates a purchase
- (void) InitPurchase: (NSString *) pProductID;

/// Restores Old Purchases
- (void) RestorePurchases;

/// Called when a transaction is successful
- (void)FinishTransaction: (SKPaymentTransaction *) aTransaction;

/// Called when restore for a user is successful
- (void)RestoreTransaction: (SKPaymentTransaction *) aTransaction;

/// Called when a transact ion failed
- (void)TransactionFailed: (SKPaymentTransaction *) aTransaction;


@end



/// Implementation of the class above.
@implementation XFBillingiOS


/// Initialize stuff
- (id) init
{
   self = [super init];
   mProducts = nil;
   mProductRequest = nil;
   mPurchaseHandler = NULL;
   mBillingManager = NULL;

   // Add Transaction observer
   [[SKPaymentQueue defaultQueue] addTransactionObserver:self];

   // Remove failed transactions

   NSArray *aAllTransactions = [[SKPaymentQueue defaultQueue] transactions];
   for (SKPaymentTransaction *aTransaction in aAllTransactions)
   {
      if (aTransaction.transactionState == SKPaymentTransactionStateFailed) {
	 [[SKPaymentQueue defaultQueue] finishTransaction:aTransaction];
      }
   }
   return self;
}

/// Remove stuff
- (void) dealloc
{

   if(mProductRequest) {
      [mProductRequest release];
      mProductRequest = nil;
   }
   [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];

   [super dealloc];
}

/// Can we make a purchase?
- (BOOL) CanPurchase
{
   return [SKPaymentQueue canMakePayments];
}



/// Fetches Product Description
- (void) GetProductDescription: (NSSet *)pProductIDs
{
   mProductRequest    = [[SKProductsRequest alloc] initWithProductIdentifiers:pProductIDs];
   mProductRequest.delegate = self;
   [mProductRequest start];
}


/// Initializes the class
- (void) Init: (FrameworkX::XFPurchaseHandler *)pPurchaseHandler BillingManager:  (FrameworkX::XFBillingManager *) pBillingManager
{
   mPurchaseHandler = pPurchaseHandler;
   mBillingManager = pBillingManager;
}


/// Given a product ID, initiates a purchase
- (void) InitPurchase: (NSString *) pProductID
{
   if(mProducts != nil)
   {
      for (SKProduct *aProduct in mProducts)
      {
	 if ([aProduct.productIdentifier isEqualToString:pProductID])
	 {
	    NSLog(@"InitPurchase(): Initiating Purchase: %@...\n", aProduct.productIdentifier);

	    SKPayment * aPayment = [SKPayment paymentWithProduct:aProduct];
	    [[SKPaymentQueue defaultQueue] addPayment:aPayment];
	    break;
	 }
      }
   }
   else {
      NSLog(@"InitPurchase(): Failure: mProduct not initialized.\n");
   }

}

/// Restores Old Purchases
- (void) RestorePurchases
{
   [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
}



/// GetProductDescription() succeeded
/// Implements productsRequest:didReceiveResponse for SKProductsRequestDelegate
- (void) productsRequest: (SKProductsRequest *)pRequest didReceiveResponse: (SKProductsResponse *)pResponse
{
   mProducts = [[NSArray alloc] initWithArray: pResponse.products];
   if([mProducts count] > 0) {

      for (SKProduct *aProduct in mProducts)
      {
	 NSLog(@"GetProductDescription(): Success!\n");
	 NSLog(@"GetProductDescription(): Product Title: %@, Description: %@, Price: %@, ID: %@\n",
	       aProduct.localizedTitle, aProduct.localizedDescription, aProduct.price, aProduct.productIdentifier);

	 NSNumberFormatter *fmtr = [[[NSNumberFormatter alloc] init] autorelease];
	 [fmtr setNumberStyle:NSNumberFormatterCurrencyStyle];
	 [fmtr setLocale:aProduct.priceLocale];
	 NSString *aFullPrice = [NSString stringWithFormat:@"%@ %@", [fmtr currencyCode], [ aProduct.price stringValue]];

	 const char *aId = [aProduct.productIdentifier UTF8String];
	 const char *aTitle = [aProduct.localizedTitle UTF8String];
	 const char *aDescription = [aProduct.localizedDescription UTF8String];
	 const char *aPrice = [ aFullPrice UTF8String];

	 if(mBillingManager)
	    mBillingManager->GotProductDescription(aId, aTitle, aDescription, aPrice);
      }

   }

   for (NSString *aInvalidProductId in pResponse.invalidProductIdentifiers)
   {
      NSLog(@"GetProductDescription(): ERROR: Invalid product ID: %@\n" , aInvalidProductId);
   }

   [pRequest autorelease];
   mProductRequest = nil;

}

/// GetProductDescription() failed
- (void)request: (SKRequest *)request didFailWithError: (NSError *)pError
{

   [mProductRequest release];
   mProductRequest = nil;

   if (nil == pError) {
      NSLog(@"GetProductDescription(): Failed to fetch list of products: Nil Error!");
      return;
   }

   NSLog(@"GetProductDescription(): Failed to fetch list of products: %@", [pError localizedDescription]);
   mPurchaseHandler->OnPurchaseFailure("");

}




/// Called when a Transaction is updated
- (void)paymentQueue: (SKPaymentQueue *) pQueue updatedTransactions: (NSArray *)pTransactions
{
   for (SKPaymentTransaction *aTransaction in pTransactions)
   {
      switch (aTransaction.transactionState)
      {
	 case SKPaymentTransactionStatePurchasing:
	    NSLog(@"Purchase Underway\n");
	    break;

	 case SKPaymentTransactionStatePurchased:
	    [self FinishTransaction:aTransaction];
	    break;

	 case SKPaymentTransactionStateFailed:
	    [self TransactionFailed:aTransaction];
	    break;

	 case SKPaymentTransactionStateRestored:
	    [self RestoreTransaction:aTransaction];

	 default:
	    NSLog(@"Warning: Unknown Transaction State!\n");
	    break;
      }
   }
}


/// Called when a transaction is successful
- (void)FinishTransaction: (SKPaymentTransaction *) aTransaction
{
   NSLog(@"InitPurchase(): Transaction Successful!");

   const char *aPID = [ aTransaction.payment.productIdentifier UTF8String ];
   const char aNo[] = "1";
   mPurchaseHandler->OnPurchaseSucceeded(aPID,  aNo);
   [[SKPaymentQueue defaultQueue] finishTransaction:aTransaction];
}


/// Called when restore for a user is successful
- (void)RestoreTransaction: (SKPaymentTransaction *) aTransaction
{
   NSLog(@"InitPurchase(): Restore Transaction Complete!");

   const char *aPID = [ aTransaction.payment.productIdentifier UTF8String ];
   const char aNo[] = "1";
   mPurchaseHandler->OnPurchaseSucceeded(aPID,  aNo);
   [[SKPaymentQueue defaultQueue] finishTransaction:aTransaction];

}


/// Called when a transaction failed
- (void)TransactionFailed: (SKPaymentTransaction *) aTransaction
{

   if (aTransaction.error.code != SKErrorPaymentCancelled)
   {
      NSString *aStr = [[NSString alloc] initWithFormat:@"%@", aTransaction.error.localizedDescription];
      NSLog(@"InitPurchase(): Transaction error: %@", aStr);

      const char *aPID = [ aStr UTF8String ];
      mPurchaseHandler->OnPurchaseFailure(aPID);
      [aStr release];

   }
   else {
      NSLog(@"InitPurchase(): User Cancelled!");
   }

   [[SKPaymentQueue defaultQueue] finishTransaction: aTransaction];

}

@end


namespace FrameworkX {


   struct XFiOSBillingManager::XFPrivateImpl {
      XFBillingiOS *mBilling;

   };


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
   XFiOSBillingManager :: XFiOSBillingManager()
   {
      mPrivateImpl = new XFPrivateImpl;
      XFBillingiOS *aBilling = [[XFBillingiOS alloc] init];

      mPrivateImpl->mBilling = aBilling;
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
   XFiOSBillingManager :: ~XFiOSBillingManager()
   {
      if (mPrivateImpl)
      {
	 [mPrivateImpl->mBilling dealloc];
	 delete mPrivateImpl;
	 mPrivateImpl = NULL;
      }

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
   void XFiOSBillingManager :: Init(XAppBase *pApp, XFPurchaseHandler *pPurchaseHandler)
   {
      XFBillingManager::Init(pApp, pPurchaseHandler);
      [mPrivateImpl->mBilling Init: pPurchaseHandler BillingManager:this];

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
   bool XFiOSBillingManager :: CanPurchase()
   {
      return [mPrivateImpl->mBilling CanPurchase];
   }


   //==///////////////////////////////////////////////////////////////////
   //
   ///
   /// Load Product Ids for the producs on sale
   ///
   /// \param
   ///
   /// \return
   ///
   //==///////////////////////////////////////////////////////////////////
   void XFiOSBillingManager :: LoadProducts(const vector<string> &pProductIDs)
   {

      XFBillingManager::LoadProducts(pProductIDs);
      NSMutableSet *aSet = [[NSMutableSet alloc] init];

      for (size_t aI = 0; aI < pProductIDs.size(); aI++)
      {
	 NSString *aStr = [[NSString alloc] initWithCString:pProductIDs[aI].c_str() encoding:NSUTF8StringEncoding];
	 [ aSet addObject:aStr ];
	 [ aStr release ];
      }

      [mPrivateImpl->mBilling GetProductDescription: aSet];
      [aSet release];
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
   void XFiOSBillingManager :: InitPurchase(const string &pProductID)
   {
      if(!CanPurchaseProduct(pProductID))
	 return;

      NSString *aStr = [[NSString alloc] initWithCString:pProductID.c_str() encoding:NSUTF8StringEncoding];
      [mPrivateImpl->mBilling InitPurchase: aStr];
      [aStr release];
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
   void XFiOSBillingManager :: RestorePurchases()
   {
      [mPrivateImpl->mBilling RestorePurchases];

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
      XFiOSBillingManager *aMacBillMan = new XFiOSBillingManager;
      if(aMacBillMan) {
	 aMacBillMan->Init(pApp, pPurchaseHandler);
	 return aMacBillMan;
      }
      
      return NULL;
   }
   
}



#endif


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


/*  End of file XF_Billing_ios.cpp  */

