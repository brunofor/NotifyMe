package com.example.bruno.hci;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.telephony.PhoneStateListener;
import android.telephony.TelephonyManager;
import android.util.Log;
/**
 * Created by Bruno on 24/01/2017.
 */

/**
 * The BroadcastReceiver abstract class is needed to receive and handle broadcast intents from smartphone
 */
public class CallsHandler extends BroadcastReceiver {

    private static final String TAG = "CallsHandler";
    private static final String typeNotifyCallInComing = "1";
    private static final String typeNotifyLostCall = "4";
    private static final String typeNotifyTakeCall = "7";

    private static boolean controlCallInComing = true;

    private static int lastCallState = TelephonyManager.CALL_STATE_IDLE;

    @Override
    public void onReceive(Context context, Intent intent) {

        Log.i(TAG, "Receiver start");
        TelephonyManager telephonyManager = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
        telephonyManager.listen(new CustomPhoneStateListener(context), PhoneStateListener.LISTEN_CALL_STATE);
    }

    /**
     * The BroadcastReceiver abstract class allows to handle the calls
     */
    class CustomPhoneStateListener extends PhoneStateListener {

        private Context context;
        public CustomPhoneStateListener(Context context) {
            super();
            this.context = context;
        }

        /**
         * Name: onCallStateChanged
         * Description: This function is executed to control when the smartphone receives a call.
         * When the smartphone receives a call his state is CALL_STATE_RINGING. Now, if
         * controlFirstUse and controlCallInComing are true then the smartphone can send the
         * notification to Arduino device and sets controlCallInComing to false in order to avoid
         * sends of the same notification. Finally the lastCallState is set to CALL_STATE_RINGING.
         * If the user take the call (the lastCallState is CALL_STATE_RINGING and the current state
         * is CALL_STATE_OFFHOOK, i.e. in call) the controlCallInComing is set true and
         * a new notifications is sent to smartphone.
         * For the case when the user loses the call (the lastCallState is CALL_STATE_RINGING and
         * the current state is CALL_STATE_IDLE) the approach is the same of the previous case.
         * @param state the state of smartphone
         * @param incomingNumber the sender's number
         */

        @Override
        public void onCallStateChanged(int state, String incomingNumber) {
            super.onCallStateChanged(state, incomingNumber);

            switch (state) {
                case TelephonyManager.CALL_STATE_IDLE:
                    // when Idle i.e no call
                    Log.i(TAG, "Phone state: idle - online");
                    if (lastCallState == TelephonyManager.CALL_STATE_RINGING) {
                        Log.i(TAG, "Missed call from : " + incomingNumber);
                        String payloadNotify = "@";
                        controlCallInComing = true;
                        if(MainActivity.controlFirstUse)
                            new HttpRequestAsyncTask(context, MainActivity.sharedPreferences.getString(MainActivity.PREF_IP, ""), MainActivity.sharedPreferences.getString(MainActivity.PREF_PORT, ""), typeNotifyLostCall, payloadNotify).execute();
                    }
                    break;

                case TelephonyManager.CALL_STATE_OFFHOOK:
                    // when Off hook i.e in call
                    Log.i(TAG, "Phone state: off hook - in call");
                    if (lastCallState == TelephonyManager.CALL_STATE_RINGING) {
                        Log.i(TAG, "Take the call of : " + incomingNumber);
                        String payloadNotify = "@";
                        controlCallInComing = true;
                        if(MainActivity.controlFirstUse)
                            new HttpRequestAsyncTask(context, MainActivity.sharedPreferences.getString(MainActivity.PREF_IP, ""), MainActivity.sharedPreferences.getString(MainActivity.PREF_PORT, ""), typeNotifyTakeCall, payloadNotify).execute();
                    }
                    break;

                case TelephonyManager.CALL_STATE_RINGING:
                    // when Ringing
                    Log.i(TAG, "Phone state: ringing from "+incomingNumber);

                    // execute HTTP request
                    if(MainActivity.controlFirstUse && controlCallInComing) {
                        String payloadNotify = incomingNumber+"@";
                        new HttpRequestAsyncTask(context, MainActivity.sharedPreferences.getString(MainActivity.PREF_IP, ""), MainActivity.sharedPreferences.getString(MainActivity.PREF_PORT, ""), typeNotifyCallInComing, payloadNotify).execute();
                        controlCallInComing = false;
                    }
                    break;

                default:
                    break;
            }

            lastCallState = state;
        }
    }
}