# Application Android

The Android app is composed by five classes java:

1.	Mainactivity: it is the main class and built the layout of app as shown in the figure. Here are managed the IP and PORT of connection that the user inserts in order to communicate with the Arduino device. Clicking on the "Check connection" button the user can check if the device opened the receiving channel.

2.	CallsHandler: this class provides access to information about the telephony services on the smartphone. Using the libraries android.telephony.TelephonyManager, android.telephony.PhoneStateListener and android.content.BroadcastReceiver it is possible to notify if there is a new calling in coming and if the user takes the call.

3.	SMSHandler: Using the libraries android.content.BroadcastReceiver and android.telephony.SmsMessage this class notifies if there are new SMSs.

4.	NotificationsHandler: this class uses the libraries android.service.notification.NotificationListenerService and android.service.notification.StatusBarNotification that allow to "intercept" the push notifications. Thus, it is possible to handle all type of notifications that arrive on the smartphone. In our work, we have been taken into consideration only the Gmail e-mail, Twitter and Messenger messages.

5.	HttpRequestAsyncTask: it is the class that allows to communicate with the Arduino device through  HTTP messages of kind GET. It sends the information in the following way:

    url = URL("http://"+ipAddress+":"\\+portNumber+"/?typeNotify="+typeNotify+",\\payloadNotify="+payloadNotify+"]");

    where ipAddress and port Number are the IP and PORT of server that runs on Arduino. The typeNotify is the type of notifications and       the payloadNotify is a string that contains the sender of notification and the body of the message.     
    An example for the new SMS notifications:
    
    http://192.180.9.6:80/?typeNotify=2,payloadNotify=\\+393271234567@hi,+how+are+you?]

    All the spaces are replaced by the + because their use causes errors during the send phase. The symbol @ concatenates sender of           notifications with the body of the message to make the request's payload; the symbol ] indicates the end of  URL string. 
