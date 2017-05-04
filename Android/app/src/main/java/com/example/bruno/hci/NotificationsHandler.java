package com.example.bruno.hci;

import android.annotation.TargetApi;
import android.content.Context;
import android.os.Build;
import android.os.Bundle;
import android.service.notification.NotificationListenerService;
import android.service.notification.StatusBarNotification;
import android.support.annotation.RequiresApi;
import android.util.Log;

/**
 * Created by Bruno on 06/02/2017.
 */

@TargetApi(Build.VERSION_CODES.JELLY_BEAN_MR2)
@RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN_MR2)

/**
 * The NotificationListenerService abstract class is needed to receive and handle push notifications
 */
public class NotificationsHandler extends NotificationListenerService {
    Context context;

    private static final String TAG = "NotificationsHandler";
    private static final String typeNotifyEmail = "3";
    private static final String typeNotifyMessenger = "5";
    private static final String typeNotifyTwitter = "6";

    @Override

    public void onCreate() {
        super.onCreate();
        context = getApplicationContext();
    }

    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    @Override
    public void onNotificationPosted(StatusBarNotification sbn) {

        String pack = sbn.getPackageName();
        String ticker = "";
        try{
            if(sbn.getNotification().tickerText != null)
                ticker = sbn.getNotification().tickerText.toString();
            Bundle extras = sbn.getNotification().extras;
            String sender = extras.getString("android.title");
            String object = null;

            if(extras.getCharSequence("android.text") != null)
                object = extras.getCharSequence("android.text").toString();

            Log.i(TAG, "sender: "+ sender + "; object: " + object + " ticker: " + ticker + " pack: " + pack);
            String payloadNotify = sender+"@"+object;
            String typeNotify = "-";

            switch (pack){
                case "com.google.android.gm":  //gmail
                    typeNotify = typeNotifyEmail;
                    Log.i(TAG, "sender email: "+ sender + "; object: " + object);
                    break;

                case "com.facebook.orca":  //messenger
                    if(sender.equals("Anteprime delle chat attive"))
                        typeNotify = "-";
                    else{
                        typeNotify = typeNotifyMessenger;
                        Log.i(TAG, "sender message messenger: "+ sender + "; object: " + object);
                    }
                    break;

                case "com.twitter.android":  //twitter
                    typeNotify = typeNotifyTwitter;
                    Log.i(TAG, "sender message twitter: "+ sender + "; object: " + object);
                    break;

                //TODO: other cases
                default:
                    break;
            }
            if(!typeNotify.equals("-"))
                if(MainActivity.controlFirstUse)
                    new HttpRequestAsyncTask(context, MainActivity.sharedPreferences.getString(MainActivity.PREF_IP,""),  MainActivity.sharedPreferences.getString(MainActivity.PREF_PORT,""), typeNotify, payloadNotify).execute();

        }catch (NullPointerException e) {
            e.printStackTrace();
        }
    }

    @Override

    public void onNotificationRemoved(StatusBarNotification sbn) {
        Log.i(TAG,"Notification Removed");
    }
}