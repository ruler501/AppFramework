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
import java.util.Set;
import java.util.UUID;

import org.json.JSONException;
import org.json.JSONObject;
import org.libsdl.app.SDLActivity;

import com.ironcode.droid.util.IabHelper;
import com.ironcode.droid.util.IabHelper.OnIabSetupFinishedListener;
import com.ironcode.droid.util.IabResult;
import com.ironcode.droid.util.Inventory;
import com.ironcode.droid.util.Purchase;

import com.sromku.simple.fb.Permission;
import com.sromku.simple.fb.SimpleFacebook;
import com.sromku.simple.fb.SimpleFacebookConfiguration;
import com.sromku.simple.fb.utils.Logger;
import com.sromku.simple.fb.SessionManager;
import com.sromku.simple.fb.listeners.OnLoginListener;
import com.sromku.simple.fb.listeners.OnFriendsListener;
import com.sromku.simple.fb.entities.Profile;

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
import android.bluetooth.*;
import android.location.*;
import android.support.v4.app.NotificationCompat;
import android.app.NotificationManager;

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
	private SimpleFacebook mSimpleFacebook = null;
	private BluetoothDevice ourBTDevice = null;
	private BluetoothSocket ourBTSocket = null;

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

		Permission[] permissions = new Permission[] {
			Permission.USER_FRIENDS,
			Permission.EMAIL
		};

		SimpleFacebookConfiguration configuration = new SimpleFacebookConfiguration.Builder()
			.setAppId("770450676382785")
			.setNamespace("testingapptwoasd")
			.setPermissions(permissions)
			.build();

		SimpleFacebook.setConfiguration(configuration);

		mSimpleFacebook = SimpleFacebook.getInstance(this);
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

	public native void signalDone(int pointer, boolean result);

	public void findFriendIDs(final int pointer)
	{
		Log.d(LOGPREFIX, Integer.toString(pointer));
		OnLoginListener onLoginListener = new OnLoginListener() {
			@Override
			public void onLogin() {
				// change the state of the button or do whatever you want
				Log.i(LOGPREFIX, "Logged in");
				signalDone(pointer, true);
			}

			@Override
			public void onNotAcceptingPermissions(Permission.Type type) {
				// user didn't accept READ or WRITE permission
				Log.w(LOGPREFIX, String.format("You didn't accept %s permissions", type.name()));
				Log.d(LOGPREFIX, Integer.toString(pointer));
				signalDone(pointer, false);
			}

			@Override
			public void onThinking(){
				return;
			}

			public void onFail(String reason){
				Log.d(LOGPREFIX, reason);
				signalDone(pointer, false);
			}

			public void onException(Throwable throwable){
				signalDone(pointer, false);
			}
		};

		mSimpleFacebook.login(onLoginListener);
	}

	public Boolean enableBluetooth()
	{
	    BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
	    if(bluetoothAdapter == null) return false;
        boolean isEnabled = bluetoothAdapter.isEnabled();
        if (!isEnabled) {
            return bluetoothAdapter.enable();
        }
        return false;
	}

    //! \todo Implement discovery and the likes
	public Boolean findBluetoothDevice(String name)
	{
	    BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
	    if(bluetoothAdapter == null) return false;
        Set<BluetoothDevice> pairedDevices = bluetoothAdapter.getBondedDevices();
        if(pairedDevices.size() > 0){
            for (BluetoothDevice device : pairedDevices){
                if(device.getName() == name){
                    ourBTDevice = device;
                    return true;
                }
            }
        }
        return false;
	}

    //! \todo Have this done in a seperate thread
	public Boolean connectBluetoothDevice(String uuid){
        if(ourBTDevice == null) return false;
        try{
            ourBTSocket = ourBTDevice.createRfcommSocketToServiceRecord(UUID.fromString(uuid));
        } catch(IOException e){
            return false;
        }
        if(ourBTSocket == null) return false;
        BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if(bluetoothAdapter == null) return false;
        if(bluetoothAdapter.isDiscovering()) bluetoothAdapter.cancelDiscovery();
        try{
            ourBTSocket.connect();
        }
        catch(Throwable t){
            return false;
        }
        return true;
	}

	native public void receiveBytes(byte[] bytes, int length, int pointer);

    //! \todo Thread out the work
	public Boolean receiveBluetooth(int readsize, final int pointer){
        if(ourBTSocket == null) return false;
        byte[] bytes = new byte[readsize];
        try{
            ourBTSocket.getInputStream().read(bytes);
        } catch(IOException e){
            return false;
        }
        receiveBytes(bytes, bytes.length, pointer);
        return true;
	}

	public Boolean sendBluetooth(byte[] bytes, final int pointer){
        if(ourBTSocket == null) return false;
        try{
            ourBTSocket.getOutputStream().write(bytes);
        } catch(IOException e){
            return false;
        }
        return true;
	}

	native public void retCoords(double lat, double lon);

//Credits to http://stackoverflow.com/a/16498506/3300171
    public Boolean getLocation()
    {
        //Get the location manager
        LocationManager locationManager = (LocationManager) getSystemService(LOCATION_SERVICE);
        Criteria criteria = new Criteria();
        String bestProvider = locationManager.getBestProvider(criteria, false);
        Location location = locationManager.getLastKnownLocation(bestProvider);
        Double lat,lon;
        try {
            lat = location.getLatitude ();
            lon = location.getLongitude ();
            retCoords(lat,lon);
            return true;
        }
        catch (NullPointerException e){
            e.printStackTrace();
            return false;
        }
    }

    public boolean notify(String iconName, String title, String text, int id){
        int resID = getResources().getIdentifier(iconName , "drawable", getPackageName());
        if(resID == 0) return false;
        NotificationCompat.Builder mBuilder = new NotificationCompat.Builder(this)
            .setSmallIcon(resID)
            .setContentTitle(title)
            .setContentText(text);

        Intent resultIntent = new Intent(this, MyGame.class);
        // Because clicking the notification opens a new ("special") activity, there's
        // no need to create an artificial back stack.
        PendingIntent resultPendingIntent =
            PendingIntent.getActivity(
            this,
            0,
            resultIntent,
            PendingIntent.FLAG_UPDATE_CURRENT
        );

        mBuilder.setContentIntent(resultPendingIntent);

        // Gets an instance of the NotificationManager service
        NotificationManager mNotifyMgr =
                (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
        // Builds the notification and issues it.
        mNotifyMgr.notify(id, mBuilder.build());

        return true;
    }

    public void cancelNotify(int id){
        NotificationManager mNotifyMgr = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
        mNotifyMgr.cancel(id);
    }
}
