/*
 * Muscial Lights Controller
 * Just Barran 2020
 * project video At
 * www.youtube.com/c/justbarran
 * Like Share Subscribe
 */

#include "LedControl.h"


#define msg7RESET 8
#define msg7Strobe 7
#define msg7DCout A0

#define maxDataIn 10
#define maxCLK 12
#define maxLoad 11

#define ledHIGHs 5
#define ledMIDs 4
#define ledLOWs 3

#define ledLow HIGH  //Switch the output depening on Common cathode or anode
#define ledHigh LOW  


#define ledPeakLOWs   5 //Sensing peaks 0-7
#define ledPeakMIDs   5
#define ledPeakHIGHs  5

#define leddelay 100 // Needed for relay to prevent constant switching
/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
LedControl lc=LedControl(maxDataIn,maxCLK,maxLoad,1);

/* we always wait a bit between updates of the display */
unsigned long delaytime=100;
byte level[8]={B10000000,B11000000,B11100000,B11110000,B11111000,B11111100,B11111110,B11111111};
byte value[7]={0,0,0,0,0,0,0};

unsigned long LOWstime=0;
unsigned long MIDstime=0;
unsigned long HIGHstime=0;

byte LOWsFlag = 0;
byte MIDsFlag = 0;
byte HIGHsFlag = 0;

void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  pinMode(msg7RESET, OUTPUT);
  pinMode(msg7Strobe, OUTPUT);
  pinMode(ledHIGHs, OUTPUT);
  pinMode(ledMIDs, OUTPUT);
  pinMode(ledLOWs, OUTPUT);

  
  digitalWrite(msg7Strobe, HIGH);
  digitalWrite(msg7RESET, LOW);
  digitalWrite(ledHIGHs, ledLow);
  digitalWrite(ledMIDs, ledLow);
  digitalWrite(ledLOWs, ledLow);
  
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,5);
  /* and clear the display */
  lc.clearDisplay(0);
}

void loop()
{ 
  digitalWrite(msg7RESET, HIGH);
  delayMicroseconds(1);
  digitalWrite(msg7RESET, LOW);
  delayMicroseconds(100);
  for (int x = 0 ; x < 7 ; x++)
    {
      digitalWrite(msg7Strobe, LOW);      
      delayMicroseconds(40);
      int spectrumRead = analogRead(msg7DCout);
      value[x] = map(spectrumRead, 0, 1024, 0, 7);
      lc.setRow(0,x,level[value[x]]);
      digitalWrite(msg7Strobe, HIGH);
    }
    //================SET Flags=====================================
    if(value[0]>= ledPeakLOWs || value[1]>= ledPeakLOWs || value[2]>= ledPeakMIDs)
    {
      LOWsFlag = 1;
      digitalWrite(ledLOWs, ledHigh);
      LOWstime = millis();
    }
    if(  value[3]>= ledPeakMIDs )
    {
      MIDsFlag = 1;
      digitalWrite(ledMIDs, ledHigh);
      MIDstime = millis();
    }
    if(value[4]>= ledPeakMIDs || value[5]>= ledPeakHIGHs || value[6]>= ledPeakHIGHs)
    {
      HIGHsFlag = 1;
      digitalWrite(ledHIGHs, ledHigh);
      HIGHstime = millis();
    }

    //================Clear Flags=====================================

    if(HIGHsFlag == 1)
    {
      if((millis() - HIGHstime)>leddelay)
      {
        digitalWrite(ledHIGHs, ledLow);
        HIGHsFlag =0;
      }
    }
    if(MIDsFlag == 1)
    {
      if((millis() - MIDstime)>leddelay)
      {
        digitalWrite(ledMIDs, ledLow);
        MIDsFlag =0;
      }
    }
    if(LOWsFlag == 1)
    {
      if((millis() - LOWstime)>leddelay)
      {
        digitalWrite(ledLOWs, ledLow);
        LOWsFlag = 0;
      }
    }
}

void test()
{
  lc.setRow(0,0,level[0]);
  lc.setRow(0,1,level[1]);
  lc.setRow(0,2,level[2]);
  lc.setRow(0,3,level[3]);
  lc.setRow(0,4,level[4]);
  lc.setRow(0,5,level[5]);
  lc.setRow(0,6,level[6]);
  lc.setRow(0,7,level[7]);
  delay(1000);
  lc.clearDisplay(0);
  for(int i = 0; i<8;i++)
  {
    lc.setRow(0,0,level[i]);
    delay(500);
  }

}
