
/*************************************************************************
**  Device: 74HC595 bit shifter                                        	**
**  File:   AH_74HC595.h   				          	**
**									**
**  Created/Modified by A. Hinkel 2012-05-28                            **
**  Download from "http://www.alhin.de/arduino"  			**
**									**
**  Based on Code from:							**
**  http://bildr.org/2011/08/74hc595-breakout-arduino/ 			**
**									**
**  

The code is provided under the MIT license please use, 		**
**  edit, change, and share.  		                    		**
**                                                                  	**
**************************************************************************/

#include <Arduino.h>
#include "AH_74HC595.h"

//************************************************************************
// Constructor
AH_74HC595::AH_74HC595(int SER_Pin, int RCLK_Pin, int SRCLK_Pin){	
	_SERIAL_DATA_Pin          = SER_Pin;
	_REGISTER_CLOCK_Pin       = RCLK_Pin;
	_SHIFT_REGISTER_CLOCK_Pin = SRCLK_Pin;
	
	pinMode(_SERIAL_DATA_Pin, OUTPUT);
	pinMode(_REGISTER_CLOCK_Pin, OUTPUT);
	pinMode(_SHIFT_REGISTER_CLOCK_Pin, OUTPUT);
	
	clear(); 
}



//************************************************************************

void AH_74HC595::shift(boolean value){

    digitalWrite(_SERIAL_DATA_Pin, value);
    clock_register();
    clock_shift_register();

    _SHIFT_REGISTER = _SHIFT_REGISTER<<1 & value;
} 
//************************************************************************

void AH_74HC595::rotate(){
        
    byte LAST_BIT = _SHIFT_REGISTER & B10000000;
    _SHIFT_REGISTER = (_SHIFT_REGISTER << 1) | (LAST_BIT>>7);
    write();
}

//************************************************************************

void AH_74HC595::setValue(byte value){
    _SHIFT_REGISTER = value;
    write();
} 
 	
//************************************************************************
	
void AH_74HC595::setPin(int index, boolean value){

  int bitIndex = index % 8;
	
  byte CURRENT = _SHIFT_REGISTER;
	
  CURRENT &= ~(1 << bitIndex); //clear the bit
  CURRENT |= value << bitIndex;  //set the bit
	
  _SHIFT_REGISTER = CURRENT;   //set the value

  write();
}

//************************************************************************

void AH_74HC595::setAll(boolean value){
//set all register pins to LOW  
  for(int i=7;i>=0;i--){
     setPin(i, value);
  }
  write();  
}

//************************************************************************

void AH_74HC595::clear(){
//set all register pins to LOW  
  for(int i=7;i>=0;i--){
     setPin(i, LOW);
  }
  _SHIFT_REGISTER = 0;
  write();
}

//************************************************************************

void AH_74HC595::showNumber(byte number){

  switch (number) {
    case 0:
      setValue(63);
      break;
    case 1:
      setValue(6);
      break;
    case 2:
      setValue(91);
      break;
    case 3:
      setValue(79);
      break;
    case 4:
      setValue(102);
      break;
    case 5:
      setValue(109);
      break;
    case 6:
      setValue(125);
      break;
    case 7:
      setValue(7);
      break;
    case 8:
      setValue(127);
      break;
    case 9:
      setValue(111);
      break;
    case 10:
      setValue(119);
      break;
    case 11:
      setValue(124);
      break;
    case 12:
      setValue(57);
      break;
    case 13:
      setValue(94);
      break;
    case 14:
      setValue(121);
      break;
    case 15:
      setValue(113);
      break;
    default: 
      setValue(0);	
  }  
}

//**************************************************************************

void AH_74HC595::showSign(char sign){

  switch (sign) {
    case '0':
      setValue(63);
      break;
    case '1':
      setValue(6);
      break;
    case '2':
      setValue(91);
      break;
    case '3':
      setValue(79);
      break;
    case '4':
      setValue(102);
      break;
    case '5':
      setValue(109);
      break;
    case '6':
      setValue(125);
      break;
    case '7':
      setValue(7);
      break;
    case '8':
      setValue(127);
      break;
    case '9':
      setValue(111);
      break;
    case '-':
      setValue(128);
      break;
    case 'A':
      setValue(119);
      break;
    case 'B':
      setValue(124);
      break;
    case 'C':
      setValue(57);
      break;
    case 'D':
      setValue(94);
      break;
    case 'E':
      setValue(121);
      break;
    case 'F':
      setValue(113);
      break;
    default: 
      setValue(0);	
  }  
}

//************************************************************************

void AH_74HC595::setPoint(boolean value){

  setPin(7,value);
 
}

//************************************************************************

void AH_74HC595::clock_register(){

    digitalWrite(_REGISTER_CLOCK_Pin, LOW);      
    digitalWrite(_REGISTER_CLOCK_Pin, HIGH);

} 

//************************************************************************

void AH_74HC595::clock_shift_register(){
   
    digitalWrite(_SHIFT_REGISTER_CLOCK_Pin, LOW);         
    digitalWrite(_SHIFT_REGISTER_CLOCK_Pin, HIGH);

} 

//************************************************************************

void AH_74HC595::write(){

    //iterate through the bits in each registers
    for(int j=7;j>=0;j--){     
      int val = _SHIFT_REGISTER & (1 << j);      
      digitalWrite(_SERIAL_DATA_Pin, val);
      clock_shift_register();
    }
    clock_register();
}

