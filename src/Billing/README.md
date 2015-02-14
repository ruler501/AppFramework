#In-App Billing

Implemented by Pallav Nawani

Here is his Readme he attached all credit goes to him, I just included it and made minor changes

The referenced docs are available under src/Billing/docs I have made some changes so that documentation is out of date on some things.

##How to use the code:


        std::string aProdID1 = mDataManager.GetString("PH_BUY_DROID_PURCHASEID");
        std::string aProdID2 = mDataManager.GetString("PH_BUY_DROID_ONETIME_PURCHASEID");
    #ifdef __APPLE__
        aProdID1 = mDataManager.GetString("PH_BUY_MAC_PURCHASEID");
    #if TARGET_OS_IPHONE
        aProdID1 = mDataManager.GetString("PH_BUY_IPHONE_PURCHASEID");
        aProdID2 = mDataManager.GetString("PH_BUY_IPHONE_ONETIME_PURCHASEID");
    #endif
    #endif

         mBman = XFCreateBillingManager(this, this);
         if(mBman) {
             if(mBman->CanPurchase()) {
	          vector<string> aProducts;
	          aProducts.push_back(aProdID1);
	          if(!aProdID2.empty())
	              aProducts.push_back(aProdID2);
	          mBman->LoadProducts(aProducts);
             }
         }
   

##Explanation:
(Lines 5-6) First we load the ProductID from the game engine string database.

(Lines 7-14) Since the product IDs differ for Android, iOS and Mac, there is some further code for loading the correct ID.

(Line 16) Create a Billing Manager

(Line 18) Check if a purchase is possible. (If the Customer has disabled purchases from the settings, this might return false)

(Line 23) Load product data from Apple or Google servers. Note that a call with any productID on Apple platform returns data for ALL the products, while on Android, we need to push all the product IDs into the vector and then call LoadProducts()

Now, after this initialization, you are ready to Init Purchase by calling

    mBman->InitPurchase(ProductID);

If the purchase is successful, then OnPurchaseSucceeded() will be called for the XFPurchaseHandler you passed in, with pID set to the purchase ID.
On failure, OnPurchaseFailure() is called. pID field will be set on iOS & Mac, but on Android, it might be empty.


##How all this works:
XFBillingManager is the interface class. For iOS/Mac/Android there are separate derived classes that implement the actual functionality. The function XFCreateBillingManager() creates & returns the appropriate class.

###iOS/Mac
The iOS & Mac versions include a objective class that does the actual interfacing with the OS.

###Android Version
For the android version, the game activity contains the JAVA code that calls into the Google API. Helper code is included. It is slightly modified version of the goole example code. You need to add the included JAVA files into your Project & resolve the 
dependencies. You also need to include your license key in the given code.

Please read the google docs to better understand what is going on. Unlike the iOS/Mac code, which is pretty much plug and play (once you set things up in itunes connect), android code would require some work from you.  Additionally, you need to add some code into 
the main activity to receive data from google. Again, see docs.

The C++ code then calls the Java code in the main activity through JNI. The code in main activity, in turn, uses the helper code to provide the desired functionality.
