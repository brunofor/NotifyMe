package com.example.bruno.hci;

import android.content.Context;
import android.os.AsyncTask;
import android.util.Log;
import android.widget.Toast;
import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.URL;


/**
 * Created by Bruno on 03/02/2017.
 */

/**
 * An AsyncTask is needed to execute HTTP requests in the background so that they do not
 * block the user interface.
 */
public class HttpRequestAsyncTask extends AsyncTask<Void, Void, Void> {

    private static final String TAG = "CallsHandler";
    private String ipAddress, portNumber;
    private int requestReply;
    private Context context;
    private String typeNotify;
    private String payloadNotify;

    HttpURLConnection httpURLConnection = null;

    /**
     * Description: The asyncTask class constructor. Assigns the values used in its other methods.
     * @param context the application context, needed to create the dialog
     * @param typeNotify the type ofn notification
     * @param ipAddress the ip address to send the request to
     * @param portNumber the port number of the ip address
     */
    public HttpRequestAsyncTask(Context context, String ipAddress, String portNumber, String typeNotify, String payloadNotify)
    {
        this.context = context;

        this.ipAddress = ipAddress;
        this.portNumber = portNumber;
        this.typeNotify = typeNotify;
        this.payloadNotify = payloadNotify;
    }

    /**
     * Name: doInBackground
     * Description: Sends the request to the ip address
     * @param voids
     * @return
     */
    @Override
    protected Void doInBackground(Void... voids) {

        try {
            requestReply = -1;

            URL url = new URL("http://"+ipAddress+":"+portNumber+"/?typeNotify="+typeNotify+",payloadNotify="+payloadNotify+"]");
            Log.i(TAG, String.valueOf(url));
            httpURLConnection = (HttpURLConnection) url.openConnection();

            httpURLConnection.setConnectTimeout(10000);
            httpURLConnection.setReadTimeout(10000);
            httpURLConnection.setRequestMethod("GET");
            httpURLConnection.connect();

            //if the server doesn't respond the requestReply is always -1.
            requestReply = httpURLConnection.getResponseCode();

            httpURLConnection.disconnect();

            if(requestReply != -1)
                Log.i(TAG, "Response from Arduino "+requestReply);

        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;

    }

    /**
     * Name: onPostExecute
     * Description: This function is executed after the HTTP request returns from the ip address.
     * The function sets the dialog's message with the reply text from the server and display the dialog
     * if it's not displayed already (in case it was closed by accident);
     * @param aVoid void parameter
     */
    @Override
    protected void onPostExecute(Void aVoid) {

        MainActivity.pd.dismiss();
        if(requestReply == -1){
            Toast.makeText(context,"Error on server. Please, restert the Arduino.", Toast.LENGTH_LONG).show();
            MainActivity.editTextIPAddress.setEnabled(true);
            MainActivity.editTextPortNumber.setEnabled(true);
            MainActivity.buttonCheck.setEnabled(true);
            MainActivity.buttonReset.setEnabled(false);}
        else
            if((requestReply == HttpURLConnection.HTTP_OK) && (typeNotify == "0"))
                Toast.makeText(context,"Connection ready.", Toast.LENGTH_LONG).show();

    }

    /**
     * Name: onPreExecute
     * Description: This function is executed before the HTTP request is sent to ip address.
     * The function will set the dialog's message and display the dialog.
     */
    @Override
    protected void onPreExecute() {
    }
}
