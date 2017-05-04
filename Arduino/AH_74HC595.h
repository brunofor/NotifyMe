
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
**  The code is provided under the MIT license please use, 		**
**  edit, change, and share.  		                    		**
**                                                                  	**
**************************************************************************/


#ifndef AH_74HC595_h
#define AH_74HC595_h

#include <Arduino.h>   //Arduino IDE >= V1.0


class AH_74HC595
{
  public:
    // Constructor
    AH_74HC595(int SER_Pin, int RCLK_Pin, int SRCLK_Pin);   	

    //Register functions
        void clear();		
   	void setPin(int index, boolean value);
   	void setAll(boolean value);
   	void setValue(byte value);
	void shift(boolean value); 
	void rotate(); 

    // 7-Segment display functions
	void showNumber(byte number);  //0-F only
	void showSign(char sign);
        void setPoint(boolean value);

  private:
	int  _SERIAL_DATA_Pin;
  	int  _REGISTER_CLOCK_Pin;
  	int  _SHIFT_REGISTER_CLOCK_Pin;
  	byte _SHIFT_REGISTER;

        void clock_shift_register();
        void clock_register();
   	void write();
};

#endif 
