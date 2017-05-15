# Sketch Arduino
The device based on Arduino that receives and shows the new notifications is composed on several hardware components. All of this elements can be bought in every shop (except for Arduino/Genuino Uno that we suggest to buy on https://www.arduino.cc/ ).

## Shopping List

•	1 x Arduino/Genuino Uno

•	1 x WiFi sensor ESP8266

•	1 x Infrared Obstacle Sensor

•	1 x LCD Display

•	1 x Seven-Segment Display

•	3 x Shift Register 74HC595

•	1 x Piezo

•	2 x Breadboards

•	5 x Leds

•	1 x Potentiometer

•	1 x USB Cable

•	7 x Resistors of 220 Ω

•	Jumper Wires

## Assembly

![alt text](https://github.com/brunofor/NotifyMe/blob/master/Arduino/NotifyMe%20-%20Circuit%20Diagram.JPG)

Each main component has been positioned to satisfy the user when he use the system. The main aspect that we have taken into consideration is  to make the system usable and intuitive. 
According to these preliminary concepts, we have assembled the system using two breadboard with different size in order to divide the various component. On the smaller breadboard there are the five coloured leds that identify the types of new notifications:

•	green led for "missed call" notification;

• yellow led for new SMS;

• red led for new e-mail on Gmail;

• white led for new message on Messenger;

• blue led for new message on Twitter.


The leds are handled by one shift register (74HC595) with eight outputs to reduce the Arduino's outputs to use. 
Furthermore, on this breadboard there are the infrared obstacle sensor and the Wi-Fi sensor. The first sensor is connected with an analogic input of Arduino: when the user want to change or reset the notifications, the sensor sends a specific value to the Arduino. The Wi-Fi sensor is connected with two output pin of Arduino to communicate with it.

On the bigger breadboard there are: the LCD display, that is handled by another shift register with eight outputs; the seven-segments display, also it handled by a shift register; the piezo sensor, that is connected with a output pin of Arduino to receive the signal to play. 

All almost components are powered by 5V provided by the Arduino; only the Wi-Fi sensor is powered by the 3.3V.
