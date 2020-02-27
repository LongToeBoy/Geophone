#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include "Waveform.h"
#include <PCD8544.h>



 Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
//Adafruit_ADS1015 ads;     /* Use thi for the 12-bit version */
//int16_t buff[100];
const int btn=12;
int btnstate=0;
int del=1;
static PCD8544 lcd;
const int arrsize=200;
Waveform wform(arrsize);
unsigned long begtime=0;
int count=0;
long cycletime=1;
float freq=0;
void setup(void) 
{
    lcd.begin(84, 48);

  Serial.begin(9600);
 // Serial.println("Hello!");
  
  //Serial.println("Getting single-ended readings from AIN0..3");
  //Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV/ADS1015, 0.1875mV/ADS1115)");
  
  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
   ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
   //ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
   //ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  ads.begin();
  //wform.resize(30);
  begtime=micros();
  pinMode(btn,INPUT_PULLUP);
}
void loop(void) 
{
  btnstate=digitalRead(btn);
  if(btnstate==LOW){
    wform.Cal(freq);
  }
  //Serial.println(samplingFrequency);
  int16_t adc0;//;

  adc0 = ads.readADC_SingleEnded(0);
  //adc0+=0+wform.avg();
  wform.add(adc0);
  //Serial.println("WTF");
  //wform.smooth(0,1);
  //if (cycletime>2){freq=double(wform.zerohz(cycletime));}
  float avg=wform.avg();
  Serial.println(avg);
 if (count==100) {
  cycletime=(micros()-begtime)/100;
    count=0;
    //double freq=wform.hz(micros());
    freq=wform.threshz(cycletime,100,-100,198);
  updateLCD(freq,wform.ppv(freq),wform.peaky);
      begtime=micros();

 // count=0;
 }
 else{count++;}
    //Serial.print("  ");
  //Serial.println(wform.clipping());
  //Serial.print("%");

  delay(del);
}





void updateLCD(float a,float b,int16_t c){
   // Just to show the program is alive...
  lcd.clear();

  // Write a piece of text on the first line...
  lcd.setCursor(0, 0);
  lcd.print("Seismograph");

  // Write the counter on the second line...
  lcd.setCursor(0, 1);
  lcd.print(a);
lcd.print(" Hz");
lcd.setCursor(0, 2);

  lcd.print(b,5);

lcd.print('"');
lcd.print("/sec");
lcd.setCursor(0, 3);
  lcd.print(c);
  // Use a potentiometer to set the LCD contrast...
  // short level = map(analogRead(A0), 0, 1023, 0, 127);
  // lcd.setContrast(level);

}
