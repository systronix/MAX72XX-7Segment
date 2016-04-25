//We always have to include the library
#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
//LedControl lc=LedControl(12,11,10,1);
/**
 * Teensy 3.1 or 3.2 pins
 * Controller needs to tolerate 5V on I/O
 */
LedControl lc=LedControl(11,13,10,1);

/* we always wait a bit between updates of the display */
unsigned long delaytime=250;

void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /** Set the brightness
   *  setIntensity(chip, intensity)
   *  chip is the chip #, 0 if only one
   *  intensity is 0..15
  *
  **/
  lc.setIntensity(0,7);
  /* and clear the display */
  lc.clearDisplay(0);
}


/*
 This method will display the characters for the
 word "Arduino" one after the other on digit 0. 
 */
void writeArduinoOn7Segment() {
  lc.setChar(0,7,'a',false);
  delay(delaytime);
  lc.setRow(0,6,0x05);
  delay(delaytime);
  lc.setChar(0,5,'d',false);
  delay(delaytime);
  lc.setRow(0,4,0x1c);
  delay(delaytime);
  lc.setRow(0,3,B00010000);
  delay(delaytime);
  lc.setRow(0,2,0x15);
  delay(delaytime);
  lc.setRow(0,1,0x1D);
  delay(delaytime);
  lc.setRow(0,0,0x80);
  
  delay(delaytime*4);
  lc.clearDisplay(0);
} 

/*
  This method will scroll all the hexa-decimal
 numbers and letters on the display. You will need at least
 four 7-Segment digits. otherwise it won't really look that good.

 Digit 0 is the right-most. 7 is left-most.
 */
void scrollDigits() {
  for(int i=0;i<9;i++) {
    lc.setDigit(0,7,i,false);
    lc.setDigit(0,6,i+1,false);
    lc.setDigit(0,5,i+2,false);
    lc.setDigit(0,4,i+3,false);
    lc.setDigit(0,3,i+4,false);
    lc.setDigit(0,2,i+5,false);
    lc.setDigit(0,1,i+6,false);
    lc.setDigit(0,0,i+7,false);
    delay(delaytime*2);
  }
  lc.clearDisplay(0);
  delay(delaytime);
}

/**
 * Write all hexadecimal values 0..F to a digit
 */
void testAllHexValues() {
  for(uint16_t val=0; val <=0x0f; val++)
    {
      for(int i=0;i<=7;i++) 
      {
        // set all digits to same val
        lc.setDigit(0,i,val,false);
        //delay(delaytime/8);
      }
      delay(delaytime);
    }
    lc.clearDisplay(0);
    delay(delaytime);
}

/**
 * Turn on segments, only one segment lit at a time
 * There are 7 digit segments plus the decimal point so 8 LEDs per digit
 * 0x01 middle '-'
 * 0x02 upper left
 * 0x04 lwer left
 * 0x08 bottom
 * 0x10 lower right
 * 0x20 upper right
 * 0x40 top
 * 0x80 dec point
 * 
 * So values 0x80 and above are the same as 0x00 and above
 * but with decimal point lit.
 */
void testSingleSegments() {
  for(uint16_t val=1; val <=0xFF; val=val<<1)
    {
      Serial.println(val, HEX);
      for(int i=0;i<=7;i++) 
      {
        // set all digits to same val
        lc.setDigit(0,i,val,false);
        lc.setRow(0,i,val);
        //delay(delaytime/8);
      }
      delay(delaytime);
      
    }
    lc.clearDisplay(0);
    delay(delaytime);
}

/**
 * Write all possible values 0x00..0xFF to each digit, all the same.
 * 
 * 0x80 and above are the same as 0x00 and above
 * but with decimal point lit.
 */
void testAllSegments() {
  for(uint16_t val=0; val <=0xFF; val++)
    {
      Serial.println(val, HEX);
      for(int i=0;i<=7;i++) 
      {
        // set all digits to same val
        lc.setDigit(0,i,val,false);
        lc.setRow(0,i,val);
        //delay(delaytime/8);
      }
      delay(delaytime/2);
      
    }
    lc.clearDisplay(0);
    delay(delaytime);
}

/**
 * try all brightness levels 0..0xF as single hex digit X
 * and display "brt X" on the LEDs
 */
void testBrightness()
{
  lc.setRow(0,7,0x1F);  // 'b'
  lc.setRow(0,6,0x05);  // 'r'
  lc.setRow(0,5,0x07);  // 't'
  
  for (uint8_t brt=0; brt<=15; brt++)
  {
   lc.setDigit(0,3,brt,false);
   lc.setIntensity(0,brt);
   delay(delaytime*4);
  }
  lc.clearDisplay(0);
  lc.setIntensity(0,7);   // mid
  delay(delaytime);
}


void loop() { 
  writeArduinoOn7Segment();
  testAllHexValues();
  testBrightness();
  testSingleSegments();
  testAllSegments();
  scrollDigits();
}
