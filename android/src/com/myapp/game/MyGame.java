package com.myapp.game;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.List;

import org.json.JSONException;
import org.json.JSONObject;
import org.libsdl.app.SDLActivity;

import com.ironcode.droid.util.IabHelper;
import com.ironcode.droid.util.IabHelper.OnIabSetupFinishedListener;
import com.ironcode.droid.util.IabResult;
import com.ironcode.droid.util.Inventory;
import com.ironcode.droid.util.Purchase;

import android.os.*;
import android.util.Base64;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.app.Dialog;
import android.app.PendingIntent;
import android.content.*;
import android.content.IntentSender.SendIntentException;
import android.content.pm.*;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.Configuration;
import android.net.*;

/*
 * A sample wrapper class that just calls SDLActivity
 */

public class MyGame extends SDLActivity {

	// In-App Billing
	private static MyGame mGameActivity;
	private IabHelper mBillingHelper = null;
	private int mObjPtr = 0;
	private boolean mIAPIsWorking = false;
	Inventory mPurchaseInventory = null;
	private static final int REQUEST_INAPP_PURCHASE = 1006;
	private static final String ICPACKAGENAME = "com.mygame.game";
	private static final String LOGPREFIX = ICPACKAGENAME;

	// Override getContext so we can return this activity context
    public static MyGame GetActivity() {
        return mGameActivity;
    }

	protected void onCreate(Bundle savedInstanceState)
	{
		mGameActivity = this;

        super.onCreate(savedInstanceState);

        // Setup In-App Purchase
        String base64EncodedPublicKey = "<INSERT YOUR PUBLIC KEY HERE>";
        mBillingHelper = new IabHelper(this, base64EncodedPublicKey);
        mBillingHelper.enableDebugLogging(true, LOGPREFIX);
        mBillingHelper.startSetup(mIabFinishedHelper);

	}

	// IN APP PURCHASE CODE FOLLOWS

	@Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data)
    {
    	Log.w(LOGPREFIX, "onActivityResult() called.");

		// Handle InApp Purchase call results
		if(REQUEST_INAPP_PURCHASE == requestCode)
        {
        	 if (!mBillingHelper.handleActivityResult(requestCode, resultCode, data)) {
        		 super.onActivityResult(requestCode, resultCode, data);
             }
        }
    }

	// Called to update native code with product data
	public native void IcUpdateProductData(int pObjPtr, String pProdID, String pProdTitle, String pProdDes, String pProdPrice);

	// Called to update native code with product purchase status
	public native void IcProductPurchased(int pObjPtr, String pProdID, boolean pSuccess);

	// Called after In App Billing Setup Finishes
	IabHelper.OnIabSetupFinishedListener mIabFinishedHelper = new IabHelper.OnIabSetupFinishedListener() {
		@Override
		public void onIabSetupFinished(IabResult result)
		{
			if (!result.isSuccess()) {
				Log.d(LOGPREFIX, "In-app Billing setup failed: " + result);
				mIAPIsWorking = false;
			} else {
				mIAPIsWorking = true;
				Log.d(LOGPREFIX, "In-app Billing is set up OK");
				mBillingHelper.queryInventoryAsync(false, null, new IabHelper.QueryInventoryFinishedListener()
				{
					@Override
					public void onQueryInventoryFinished(IabResult result, Inventory inv)
					{
						if(result.isSuccess()) {
							mPurchaseInventory = inv;
							Log.d(LOGPREFIX, "Got Inventory Data: " + inv.toString() );
						}
					}
				});
			}
		}

	};

	// Listens asynchronously for item consumption completion
	IabHelper.OnConsumeFinishedListener mConsumeFinishedListener = new IabHelper.OnConsumeFinishedListener() {
		public void onConsumeFinished(Purchase purchase, IabResult result) {

			if (result.isSuccess()) {
				// Item consumed, send message to C++ code for consumable items
			} else {
				// handle error
			}
		}
	};


	// Handles Purchase Result
	IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener()
	{
		@Override
		public void onIabPurchaseFinished(IabResult pResult, Purchase pPurchase)
		{
			if (pResult.isFailure()) {
				IcProductPurchased(mObjPtr, pResult.getMessage(), false);
				//XFBillingConsumeItem(pPurchase.getSku());
				return;
			}
			else if (pPurchase.getSku().equals(pPurchase.getDeveloperPayload())) {
				IcProductPurchased(mObjPtr, pPurchase.getSku(), true);
				// Consume consumable items by calling this.
				/*try{
					mBillingHelper.consume(pPurchase);
				}
				catch(Exception e){
					return;
				}*/
			}

		}
	};

	// Query Purchases
	public boolean XFBillingCanPurchase()
	{
		return mIAPIsWorking;
	}

	// Query Purchases
	public void XFBillingQueryPurchases(String[] pProductIdVec, int pObjPtr)
	{
		Log.d(LOGPREFIX, "XFBillingQueryPurchases(): Called.");
		mObjPtr = pObjPtr;
		if(mObjPtr == 0)
			return;

		// Copy from Array to List
		ArrayList<String> aList = new ArrayList<String>();
		for(int aI = 0; aI < pProductIdVec.length; aI++)
			aList.add(pProductIdVec[aI]);

		try {
			Bundle aIAPData = mBillingHelper.querySkuDetailsEx("inapp", aList);
			XFBillingUpdatePurchaseData(aIAPData);
		} catch (RemoteException e) {
			e.printStackTrace();
		} catch (JSONException e) {
			e.printStackTrace();
		}

	}

	// Query Purchases Data
	public void XFBillingUpdatePurchaseData(Bundle pIAPData)
	{

		Log.d(LOGPREFIX,  String.format("XFBillingUpdatePurchaseData() Called: Product Bundle is %s, mObjPtr is %d", pIAPData,  mObjPtr));
		if(null == pIAPData || mObjPtr == 0) {
			Log.d(LOGPREFIX, "XFBillingUpdatePurchaseData(): Failed!");
			return;
		}

		ArrayList<String> responseList = pIAPData.getStringArrayList("DETAILS_LIST");

		for (String thisResponse : responseList)
		{
			JSONObject object;
			try {
				object = new JSONObject(thisResponse);
				String aPID = object.getString("productId");
				String aTitle = object.getString("title");
				String aDescription = object.getString("description");
				String aPrice = object.getString("price");
				IcUpdateProductData(mObjPtr, aPID, aTitle, aDescription, aPrice);

				String aInfo = "Java: " + aPID + " " + aTitle + " " + aDescription + " " + aPrice;
				Log.d(LOGPREFIX, aInfo);


			} catch (JSONException e) {
				e.printStackTrace();
			}
		}

	}


	// Restores purchase
	public boolean XFBillingRestorePurchaseHelper(String pPurchaseID, int pObjPtr)
	{
		mObjPtr = pObjPtr;
		if(null != mPurchaseInventory)
		{
			Purchase aPurchase = mPurchaseInventory.getPurchase(pPurchaseID);
			if(null == aPurchase || !pPurchaseID.equals(aPurchase.getSku())){
				return false;
			}
			IcProductPurchased(mObjPtr, aPurchase.getSku(), true);
			return true;
		}

		return false;
	}

	// Restores purchase
	public boolean XFBillingRestorePurchase(String pPurchaseID, int pObjPtr)
	{
		boolean aResult = XFBillingRestorePurchaseHelper(pPurchaseID, pObjPtr);
		if(!aResult)
			IcProductPurchased(mObjPtr, "Purchase Restore Failed", false);
		return aResult;
	}

	// Initializes Purchase
	public void XFBillingInitPurchase(String pPurchaseID, int pObjPtr)
	{
		if(XFBillingRestorePurchaseHelper(pPurchaseID, pObjPtr) == false) {
			mObjPtr = pObjPtr;
			mBillingHelper.launchPurchaseFlow(this, pPurchaseID, REQUEST_INAPP_PURCHASE,  mPurchaseFinishedListener, pPurchaseID);
		}
		else{
			Log.d(LOGPREFIX, "Removing old purchases");
			try{
				Inventory inv = mBillingHelper.queryInventory(true,null);
				mBillingHelper.consume(inv.getPurchase(pPurchaseID));
				mObjPtr = pObjPtr;
				mBillingHelper.launchPurchaseFlow(this, pPurchaseID, REQUEST_INAPP_PURCHASE,  mPurchaseFinishedListener, pPurchaseID);
            }
            catch(Exception e){
				return;
            }
		}
	}

	// Call this function to consume an item & remove it from Google DB.
	// Caution: only use for Consumable Items!
	// Caution: Untested!
	public void XFBillingConsumeItem(final String pPurchaseID)
	{
		mBillingHelper.queryInventoryAsync(new IabHelper.QueryInventoryFinishedListener() {
			@Override
		   public void onQueryInventoryFinished(IabResult result, Inventory inventory) {
			  if (result.isFailure()) {
			  // Handle failure
			  } else {
					  mBillingHelper.consumeAsync(inventory.getPurchase(pPurchaseID), mConsumeFinishedListener);
			  }
		 }
	 });

	}
}
