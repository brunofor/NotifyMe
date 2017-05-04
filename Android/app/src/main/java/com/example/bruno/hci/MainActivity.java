package com.example.bruno.hci;

import android.Manifest;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.os.Build;
import android.provider.Settings;
import android.support.annotation.RequiresApi;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.Toast;

import static com.example.bruno.hci.R.color.color_process_dialog;


public class MainActivity extends AppCompatActivity{

    public final static String PREF_IP = "PREF_IP_ADDRESS";
    public final static String PREF_PORT = "PREF_PORT_NUMBER";
    private static final String ACTION_NOTIFICATION_LISTENER = "enabled_notification_listeners";

    public static boolean controlFirstUse = false; //to check if the app has been used at least one time

    private static final String typeNotify = "0";

    public static ProgressDialog pd;

    public static Button buttonCheck,buttonReset;
    public static EditText editTextIPAddress, editTextPortNumber;

    // shared preferences objects used to save the IP address and port so that the user doesn't have to
    // type them next time he/she opens the app.
    SharedPreferences.Editor editor;
    public static SharedPreferences sharedPreferences;

    private ProgressBar bar;

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //the progress dialog
        pd = new ProgressDialog(MainActivity.this);
        pd.setTitle("Please Wait...");
        pd.setMessage("Connecting to server...");
        pd.setProgressStyle(ProgressDialog.STYLE_SPINNER);
        pd.getWindow().setBackgroundDrawableResource(color_process_dialog);
        pd.setCancelable(false);

        //To save the IP and PORT in memory
        sharedPreferences = getSharedPreferences("HTTP_HELPER_PREFS", Context.MODE_PRIVATE);
        editor = sharedPreferences.edit();

        editTextIPAddress = (EditText)findViewById(R.id.editTextIPAddress);
        editTextIPAddress.setText(sharedPreferences.getString(PREF_IP,""));
        editTextPortNumber = (EditText)findViewById(R.id.editTextPortNumber);
        editTextPortNumber.setText(sharedPreferences.getString(PREF_PORT,""));

        buttonCheck = (Button)findViewById(R.id.buttonCheck); //Check connection
        buttonCheck.setOnClickListener(new View.OnClickListener(){
            @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
            @Override
            public void onClick(View view) {
                final String ipAddress = editTextIPAddress.getText().toString().trim();
                final String portNumber = editTextPortNumber.getText().toString().trim();
                if(!ipAddress.isEmpty() && !portNumber.isEmpty()){

                    // save the IP address and port for the next time the app is used
                    editor.putString(PREF_IP,ipAddress);
                    editor.putString(PREF_PORT,portNumber);
                    editor.commit(); // save the IP and PORT
                    controlFirstUse = true;

                    editTextIPAddress.setEnabled(false);
                    editTextPortNumber.setEnabled(false);
                    buttonCheck.setEnabled(false);
                    buttonReset.setEnabled(true);

                    pd.show();

                    Thread mThread = new Thread() {
                        @Override
                        public void run() {

                            String payloadNotify = Build.MODEL+"@";

                            new HttpRequestAsyncTask(MainActivity.this, ipAddress, portNumber, typeNotify, payloadNotify).execute();
                        }
                    };
                    mThread.start();
                }
                else{
                    Toast.makeText(getApplicationContext(),"IP and PORT are necessary.", Toast.LENGTH_LONG).show();
                }
            }
        });

        buttonReset = (Button)findViewById(R.id.buttonReset);
        buttonReset.setEnabled(false);
        buttonReset.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View view) {
                editTextIPAddress.setEnabled(true);
                editTextPortNumber.setEnabled(true);
                buttonCheck.setEnabled(true);
                buttonReset.setEnabled(false);
            }
        });

        //To request the permission
        int PERMISSION_ALL = 1;
        String[] PERMISSIONS = {Manifest.permission.READ_SMS, Manifest.permission.READ_PHONE_STATE, Manifest.permission.RECEIVE_SMS };
        if(!hasPermissions(this, PERMISSIONS)){
            ActivityCompat.requestPermissions(this, PERMISSIONS, PERMISSION_ALL);
        }

        if (!checkNotificationListenerPermission()) {
            // Permission is not granted
            Toast.makeText(getApplicationContext(),"Grant notification permission",Toast.LENGTH_SHORT).show();
            startActivity(new Intent(Settings.ACTION_NOTIFICATION_LISTENER_SETTINGS));
        }
    }

    /**
     * Name: checkNotificationListenerPermission
     * Description: Check if the app can intercept the push notifications
     * @return boolean
     */
    private boolean checkNotificationListenerPermission() {
        return Settings.Secure.getString(getContentResolver(),ACTION_NOTIFICATION_LISTENER).contains(getApplicationContext().getPackageName());
    }

    /**
     * Name: hasPermissions
     * Description: Check if the app can intercept the push notifications
     * @param context the application context
     * @param permissions the list of permissions to check
     * @return boolean
     */
    private static boolean hasPermissions(Context context, String... permissions) {
        if (android.os.Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && context != null && permissions != null) {
            for (String permission : permissions) {
                if (ActivityCompat.checkSelfPermission(context, permission) != PackageManager.PERMISSION_GRANTED) {
                    return false;
                }
            }
        }
        return true;
    }

    @Override
    protected void onStop() {
        super.onStop();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (!checkNotificationListenerPermission()) {
            // Permission is not granted
            Toast.makeText(getApplicationContext(),"Grant notification permission",Toast.LENGTH_SHORT).show();
            startActivity(new Intent(Settings.ACTION_NOTIFICATION_LISTENER_SETTINGS));
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
    }
}