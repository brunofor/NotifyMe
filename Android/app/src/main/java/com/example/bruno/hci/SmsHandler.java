package com.example.bruno.hci;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.telephony.SmsMessage;
import android.util.Log;

/**
 * Created by Bruno on 04/02/2017.
 */

/**
 * The BroadcastReceiver abstract class is needed to receive and handle broadcast intents from smartphone
 */
public class SmsHandler extends BroadcastReceiver {

    private static final String TAG = "SmsHandler";
    private static final String typeNotify = "2";

    public void onReceive(Context context, Intent intent) {

        Log.i(TAG, "Receiver start");
        // Retrieves a map of extended data from the intent.
        final Bundle bundle = intent.getExtras();

        try {
            if (bundle != null) {
                final Object[] pdusObj = (Object[]) bundle.get("pdus");

                for (int i = 0; i < pdusObj.length; i++) {
                    SmsMessage currentMessage = SmsMessage.createFromPdu((byte[]) pdusObj[i]);
                    String phoneNumber = currentMessage.getDisplayOriginatingAddress();
                    String messageOrigin = currentMessage.getDisplayMessageBody();
                    String message;

                    //if the message's size is bigger than 16 characters
                    if(messageOrigin.length() > 16)
                        message = messageOrigin.substring(0,13)+"...";
                    else
                        message = messageOrigin;

                    //to avoid errors during the submit of notification
                    message = message.replace(' ','+');
                    String payloadNotify = phoneNumber+"@"+message;

                    Log.i(TAG, "senderNum: "+ phoneNumber + "; message: " + message);
                    if(MainActivity.controlFirstUse)
                        new HttpRequestAsyncTask(context, MainActivity.sharedPreferences.getString(MainActivity.PREF_IP,""),  MainActivity.sharedPreferences.getString(MainActivity.PREF_PORT,""), typeNotify, payloadNotify).execute();
                }
            }

        } catch (Exception e) {
            Log.e(TAG, "Exception smsReceiver " +e);

        }
    }
}