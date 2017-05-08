# NotifyMe
The NotifyMe project is born in the field of Human-Computer Interaction and it shows how a simple device based on Arduino can display the notifications that arrive on your smartphone.

The proposed system allows users to check and to read the notifications that arrive on their smartphone. For example, NotifyMe can be used when the user is working on his pc and his smartphone is in another room; thus, through this system he can monitor at any time the new notifications, e.g. SMS, calls, e-mail, etc.

The system consists of two components: the Android application mobile that is installed on the Android smartphone of the user and the  device based on an Arduino microcontroller.
When the user wants to connect the smartphone to the device in order to see on this last the new notifications, he has to choose the Wi-Fi local network to which he wants to connect with the Arduino device and he has to install the app on his smartphone. This network must be the same where it is connected the smartphone and it is necessary because the device opens a listening server to a specific IP accepting the new notifications, in the form of HTTP requests, on a specific PORT. The IP and PORT are showed to user through a display of device; in this way, the user can check if it is ready to receive the notifications. 

Once this is done, when the smartphone receives a new SMS or a new e-mail, the app builds the request specifying the type of notification, the sender (number or name) and the body of the message. Thus, this request can be sent to device: it reads the notification's info showing its type and the number of notifications that there are into device. At this way the user can monitor how many and which notifications there are.

A funny feature that we have implemented is the "calling in coming" function: when the user receives a new call in coming, the Arduino device plays a configurated ringtone (in our case the immortal SuperMario song) and the various leds turn on with a random pattern.
Finally, the user through a simple and natural hand gesture can: see the new notifications (sender and message), switch between them and reset the device deleting all the notifications.
