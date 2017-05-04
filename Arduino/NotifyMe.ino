 /*
  Created by Bruno Formato  bruno.formato@stud.unifi.it
             Luca Cultrera  luca.cultrera@stud.unifi.it  

Table Leds:

B white blu red yellow grenn 0 0 0

B00000000   0
B00001000   8
B00010000   16
B00011000   24
B00100000   32
B00101000   40
B00110000   48
B00111000   56
B01000000   64
B01001000   72
B01010000   80
B01011000   88
B01100000   96
B01101000   104
B01110000   112
B01111000   120
B10000000   128
B10001000   136
B10010000   144
B10011000   152
B10100000   160
B10101000   168
B10110000   176
B10111000   184
B11000000   192
B11001000   200
B11010000   208
B11011000   216
B11100000   224
B11101000   232
B11110000   240
B11111000   248

To manage the shift registers of seven-segment display and the set of leds we used the the AH\_74HC595.h library. 
In particular this library allows to select its output using a number as input. 
E.g.: 248 248 to select the first five outputs. 
Taking into consideration this feature, we store the types of notifications adding the value of 
output (led) that correspond to notification's type to a counting variable. 
E.g.: if we have a SMS and a e-mail we add 16 and 32 to countByteLeds variable.

*/


// include the library code:
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include "AH_74HC595.h"

#define DEBUG true

// This means that you need to connect the TX line (white) from the esp to the Arduino's pin 9
// and the RX line (yellow) from the esp to the Arduino's pin 12
SoftwareSerial esp8266(12, 9);

// To initialize the library with the number of the sspin 
//(or the latch pin of the 74HC595)
LiquidCrystal lcd(10);

//7 segments - Shif register 
#define SER_Pin_SS 6   //Serial data input
#define RCLK_Pin_SS 5  //Register clock
#define SRCLK_Pin_SS 7 //Shift register clock

//Leds - Shif register 
#define SER_Pin_Leds 4   //Serial data input
#define RCLK_Pin_Leds 3  //Register clock
#define SRCLK_Pin_Leds 2 //Shift register clock

//To initialize the shift register
AH_74HC595 shifter_SS(SER_Pin_SS, RCLK_Pin_SS, SRCLK_Pin_SS);  //The seven-segments display
AH_74HC595 shifter_Leds(SER_Pin_Leds, RCLK_Pin_Leds, SRCLK_Pin_Leds); //The leds

//The next variables are useful to memorize the type of the new notifications and to turn on their leds. 
//E.g.: for gmail and twitter notifications you have to add 32 and 128 to countByteLeds in order to turn on the corresponding leds. 
int countByteLeds = 0; 
int leds[5] = {8, 16, 32, 64, 128};

//piezo
#define buzzer 8
int frequencies [3] = {660,480,520/*660,660,660,510,660,770,380,510,380,320,440,480,450,430,380,668,760,860,700,760,660,520,580,480,510,380,320,440,480,450,430,380,660,760,860,700,760,660,520,580,480,500*/};
int durations[3] = {100,50,80/*100,100,100,100,100,100,100,100,100,100,100, 80,100,100,100, 80, 50,100, 80, 50, 80, 80, 80, 80,100,100,100,100, 80,100,100,100, 80, 50,100, 80, 50, 80, 80, 80, 80,100*/};
int delays[3] = {150,300,100/*150,300,300,100,300,500,575,450,400,500,300,330,150,300,200,200,150,300,150,350,300,150,150,500,450,400,500,300,330,150,300,200,200,150,300,150,350,300,150,150,500,300*/};

//variable to control if the device is connected with a network
int controlConn = 0;

//variables for the notifications
int count = 0;
int countOffset = 1;

String messages[5]; //To store the messages
String senders[5]; //To store the senders
int types[5]; //To store the type of notifications

String numCall;
int index = 0;

//The use of these variable it's a trick to handle more notificatins of the same type
int controlMessage = 0;
int controlMail = 0;
int controlLostCall = 0;
int controlMessenger = 0;
int controlTwitter = 0;
int controlCall = 0;

//proximity sensor
#define proxSensor A4
int proxCount = 0;


void setup(){
    
  Serial.begin(9600);  
  esp8266.begin(9600); // your esp's baud rate might be different

  //To check if the components are ready
  shifter_Leds.setAll(255);
  shifter_SS.setAll(255);
  tone(buzzer, 660 ,100);
  shifter_Leds.setAll(countByteLeds);
  shifter_SS.setAll(0);
  
  pinMode(buzzer,OUTPUT); 
  pinMode(proxSensor, INPUT);

  // set up the number of columns and rows on the LCD
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Configuration");
  // set the cursor to column 0, line 1
  // line 1 is the second row, since counting begins with 0
  lcd.setCursor(0, 1);
  // print to the second line
  lcd.print("server...") ;

  //To set the connection
  sendCommand("AT+RST\r\n", 2000, DEBUG); //to reset module
  sendCommand("AT+CWMODE=1\r\n", 1000, DEBUG); //to configure as access point
  //sendCommand("AT+CWJAP=\"D-LINKHOMECELSO\",\"viacels0dieci+\"\r\n", 3000, DEBUG);//to configure the parameters
  sendCommand("AT+CWJAP=\"iPhone di luca\",\"timori44\"\r\n", 3000, DEBUG); //to configure params
  delay(10000);
  String resultIP = sendCommand("AT+CIFSR\r\n", 1000, DEBUG); //to get ip address 
  String ip = "";
  int control = 0;
  for(int i=0; i<resultIP.length(); i++){
    if((resultIP[i] == '"')&&(control == 0)){
      i++;
      while(resultIP[i] != '"'){
        ip += resultIP[i];
        i++;
      }
      control = 1;
    }  
  }
  
  if(ip == ""){
    
    Serial.println("Connection failed");
    controlConn = 0;
    lcd.clear();
    lcd.print("Connection Fail");
    lcd.setCursor(0, 1);
    lcd.print("Check Network");  
  }
  else{  
    
    sendCommand("AT+CIPMUX=1\r\n", 1000, DEBUG); //to configure for multiple connections
    sendCommand("AT+CIPSERVER=1,80\r\n", 1000, DEBUG); //to turn on server on port 80
    
    Serial.println("Server Ready");
    controlConn = 1;
    lcd.clear();
    lcd.print("Ready! Port:80");
    lcd.setCursor(0, 1);
    lcd.print(ip); 
    
    tone(buzzer, 700 ,100);
    shifter_SS.showNumber(count);
  }
}


void loop(){
    
  if(controlConn == 1){
    
    if(analogRead(proxSensor) < 500){  //light from 0 (very bright) to 1024 (very dark)
      
      proxCount++;
      delay(500);

      //to reset the device if the proxSensor is LOW for 6*500 = 3000ms = 3s
      if(proxCount == 6){
        
        lcd.clear();
        lcd.print("Reset complete.");
        proxCount = 0;

        for( int i = 0; i < count;  ++i ){
          senders[i] = "";
          messages[i] = "";
        }
       
        count = 0;
        countByteLeds = 0;
        controlMessage = 0;
        controlMail = 0;
        controlLostCall = 0;
        controlMessenger = 0;
        controlTwitter = 0;
        shifter_SS.showNumber(count);
        shifter_Leds.setValue(countByteLeds);
      }
    }else{
      //to switch to next notification
      if((proxCount > 0) && (count > 0)){ 
        
        lcd.clear();
        lcd.print(senders[count-countOffset]);
        lcd.setCursor(0, 1);
        lcd.print(messages[count-countOffset]);
        shifter_Leds.setValue(types[count-countOffset]);
        proxCount = 0;
        countOffset++;
        if(countOffset > count)
          countOffset = 1;
      }
    }
    
    requestFromAndroid(); //to wait the request from Android app

    //if the previous notification is a "call in coming" so the device plays the ringtone until a "end call" notification (typeNotify = 7) doesn't arrive
    if(controlCall == 1){ 
      index++;
      if(index > 2)
        index = 0;
      superMario(0,5,index);
    }
  }
}

/*
  Name: requestFromAndroid
  Description: Function used to receive the request from app Android.
*/  
void requestFromAndroid(){

  if (esp8266.available()){ // check if the esp is connected
  
    if (esp8266.find("+IPD,")){
         
        delay(1000); // wait for the serial buffer to fill up (read all the serial data)
        // get the connection id so that we can then disconnect
        int connectionId = esp8266.read() - 48; // subtract 48 because the read() function returns
        // the ASCII decimal value and 0 (the first decimal number) starts at 48

        if (count < 5){ //the device can store only 5 notifications
          
            esp8266.find("typeNotify="); // advance cursor to "typeNotify="
            int typeNotify = (esp8266.read() - 48); // get the type of notification
            
            esp8266.find(",payloadNotify="); // advance cursor to "payloadNotify=" 
            String payload = esp8266.readStringUntil(']');
  
            Serial.println(payload);
            int pos = payload.indexOf('@');
      
            //TODO: problemi con i caratteri accentati => tabella ASCII non estesa
            
            String sender = payload.substring(0,pos);
            String message = payload.substring(pos+1,payload.length());
      
            message.replace('+',' ');
            
            switch(typeNotify){
              
              case 0: //Request from device
                lcd.clear();
                lcd.print("Request from:");
                lcd.setCursor(0, 1);
                lcd.print(sender);
                break;
                
              case 1: //Calling in Coming
                lcd.clear();
                lcd.print("Calling coming:");
                lcd.setCursor(0, 1);
                lcd.print(sender);
                numCall = sender;
                controlCall = 1; //To play the ringtone
                break;
                
              case 2: //New SMS
                senders[count] = sender;
                messages[count] = message;
                types[count] = 16;
                count++;
                if(controlMessage == 0){
                  countByteLeds += 16;
                  controlMessage = 1;
                }
                break;
                
              case 3: //New email on Gmail
 
                senders[count] = sender;
                messages[count] = message;
                types[count] = 32;
                count++;
                if(controlMail == 0){
                   countByteLeds += 32;
                   controlMail = 1;
                }
                break;
            
              case 4: //New lost call
                senders[count] = "Missed Call From";
                messages[count] = numCall;
                types[count] = 8;
                count++;
                if(controlLostCall == 0){
                   countByteLeds += 8;
                   controlLostCall = 1;
                }
                index = 0;
                controlCall = 0;
                break;

              case 5: //New nessage on messenger
                senders[count] = sender;
                messages[count] = message;
                types[count] = 64;
                count++;
                if(controlMessenger == 0){
                   countByteLeds += 64;
                   controlMessenger = 1;
                }
                break;

              case 6: //New nessage on twitter 
                senders[count] = sender;
                messages[count] = message;
                types[count] = 128;
                count++;
                if(controlTwitter == 0){
                   countByteLeds += 128;
                   controlTwitter = 1;
                }
                break;

              case 7: //The user takes the call
                index = 0;
                controlCall = 0;
                break;
                          
              default:
                break;
            }              

            if(typeNotify > 1){
              Serial.println("There are "+String(count)+" new");
              lcd.clear();
              lcd.print("There are "+String(count)+" new");
              lcd.setCursor(0, 1);
              lcd.print("notifications");
            }
      
            shifter_SS.showNumber(count);
            shifter_Leds.setValue(countByteLeds);

          }else{
            lcd.clear();
            lcd.print("Full memory...");
            lcd.setCursor(0, 1);
            lcd.print("reset the unit");
        }

        // build string that is send back to device
        String content = "OK";
        sendHTTPResponse(connectionId, content);
  
        // make close command
        String closeCommand = "AT+CIPCLOSE=";
        closeCommand += connectionId; // append connection id
        closeCommand += "\r\n";
        sendCommand(closeCommand, 1000, DEBUG); // close connection
      }

  }
}

/*
  Name: sendData
  Description: Function used to send data to ESP8266.
  Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
  Returns: The response from the esp8266 (if there is a reponse)
*/
String sendData(String command, const int timeout, boolean debug){
  
  String response = "";

  int dataSize = command.length();
  char data[dataSize];
  command.toCharArray(data, dataSize);
  command = "";
  esp8266.write(data, dataSize); // send the read character to the esp8266
  
  if (debug){
    
    Serial.println("\r\n====== HTTP Response From Arduino ======");
    Serial.write(data, dataSize);
    Serial.println("\r\n========================================");
  }

  for( int i = 0; i < sizeof(data);  ++i )
    data[i] = (char)0;

  long int time = millis();
  while ( (time + timeout) > millis()){
    
    while (esp8266.available()){

      // The esp has data so display its output to the serial window
      char c = esp8266.read(); // read the next character.
      response += c;
    }
  }

  if (debug){
    
    Serial.print(response);
  }

  return response;
}

/*
  Name: sendHTTPResponse
  Description: Function that sends HTTP 200, HTML UTF-8 response
*/
void sendHTTPResponse(int connectionId, String content){
  

  // build HTTP response
  String httpResponse;
  String httpHeader;
  
  // HTTP Header
  httpHeader = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n";
  httpHeader += "Content-Length: ";
  httpHeader += content.length();
  httpHeader += "\r\n";
  httpHeader += "Connection: close\r\n\r\n";
  httpResponse = httpHeader + content + " "; // There is a bug in this code: the last character of "content" is not sent, I cheated by adding this extra space

  sendCIPData(connectionId, httpResponse);
}

/*
  Name: sendCIPDATA
  Description: sends a CIPSEND=<connectionId>,<data> command

*/
void sendCIPData(int connectionId, String data){
  
  String cipSend = "AT+CIPSEND=";
  cipSend += connectionId;
  cipSend += ",";
  cipSend += data.length();
  cipSend += "\r\n";
  sendCommand(cipSend, 1000, DEBUG);
  sendData(data, 1000, DEBUG);
}

/*
  Name: sendCommand
  Description: Function used to send data to ESP8266.
  Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
  Returns: The response from the esp8266 (if there is a reponse)
*/
String sendCommand(String command, const int timeout, boolean debug){
  
  String response = "";

  esp8266.print(command); // send the read character to the esp8266

  long int time = millis();

  while ( (time + timeout) > millis()){
    
    while (esp8266.available()){

      // The esp has data so display its output to the serial window
      char c = esp8266.read(); // read the next character.
      response += c;
    }
  }

  if (debug){
    
    Serial.print(response);
  }

  return response;
}

/*
  Name: superMario
  Description: Function used to play the ringtone
  Params: minLed - the min led's value; maxLed - the max led's value; index - vectors' index
*/
void superMario(int minLed, int maxLed, int index){
  
  int pinNumber = random(minLed,maxLed);
  shifter_Leds.setValue(leds[pinNumber]);
  tone(buzzer, frequencies[index], durations[index]);
  delay(delays[index]);
}
