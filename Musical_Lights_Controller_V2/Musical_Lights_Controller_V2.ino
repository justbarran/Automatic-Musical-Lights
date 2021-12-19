/*
 * Muscial Lights Controller V2
 * Just Barran 2021
 * project video At
 * www.youtube.com/c/justbarran
 * Like Share Subscribe
 */

#include "LedControl.h"

#define msg7RESET 13
#define msg7Strobe 9
#define msg7DCout A0


#define maxDataIn 10
#define maxLoad 11
#define maxCLK 12

#define led1 2
#define led2 3
#define led3 4
#define led4 5

#define led5 6
#define led6 7
#define led7 8

#define TRIM_POT A1

#define LED_LOW LOW  //Switch the output depening on Relays
#define LED_HIGH HIGH  

#define CYCLE_DELAY 50 // Time lights remain on for  
 
LedControl lc=LedControl(maxDataIn,maxCLK,maxLoad,1);

/* we always wait a bit between updates of the display */
unsigned long delaytime=100;
byte ledPins[7]= {led1,led2,led3,led4,led5,led6,led7};
byte level[8]=   {B10000000,B11000000,B11100000,B11110000,B11111000,B11111100,B11111110,B11111111};
byte setlevel[8]={B10000000,B01000000,B00100000,B00010000,B00001000,B00000100,B00000010,B00000001};
unsigned int valueMSG[7];
unsigned int potValue=0;
byte potSet = 0;

void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  pinMode(msg7RESET, OUTPUT);
  pinMode(msg7Strobe, OUTPUT);
  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  
  digitalWrite(msg7Strobe, HIGH);
  digitalWrite(msg7RESET, LOW);
  digitalWrite(led1, LED_LOW);
  digitalWrite(led2, LED_LOW);
  digitalWrite(led3, LED_LOW);
  digitalWrite(led4, LED_LOW);
  digitalWrite(led5, LED_LOW);
  digitalWrite(led6, LED_LOW);
  digitalWrite(led7, LED_LOW);

  
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,5);
  /* and clear the display */
  lc.clearDisplay(0); 
}

void loop()
{ 
  digitalWrite(msg7Strobe, HIGH);  
  digitalWrite(msg7RESET, HIGH);
  delayMicroseconds(1); //Reset Pluse Width 100nS Min
  potValue = analogRead(TRIM_POT);
  potSet = map(potValue, 0, 1023, 0, 8);  
  lc.setRow(0,7,setlevel[potSet]);  
  
  digitalWrite(msg7RESET, LOW);
  delayMicroseconds(100); // Reset to Stobe Delay 72uS min
  
  for (int x = 0 ; x < 7 ; x++)
    {
      digitalWrite(msg7Strobe, LOW);      
      delayMicroseconds(40); //Output settling time 36us Min
      //Read MSG Value
      int spectrumRead = analogRead(msg7DCout);
      valueMSG[x] = map(spectrumRead, 0, 1023, 0, 8);
      lc.setRow(0,x,level[valueMSG[x]]);
      digitalWrite(msg7Strobe, HIGH);
      delayMicroseconds(40); //Strobe to Strobe 72uS Min
      //Set Lights 
      if(valueMSG[x]>=potSet)
      {
        digitalWrite(ledPins[x], LED_HIGH);
      }
      else
      {
        digitalWrite(ledPins[x], LED_LOW);
      }
    }
    delay(CYCLE_DELAY); //Allow them to be on or off for a bit of time 
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
