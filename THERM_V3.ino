//..................................................................................//
//IN CASE OF CHANGE OF THE TEMPERATURE SENSOR DONT FORGET TO DELETE THE DHT LIBRARY //
//FOR MEMORY SAVING AS WELL AS CHANGE THE VARIABLES IN THE LCD CODE AND UNCOMMENT   //             
//ALL PIN AND VAR FOR LM35 AND COMMENT ALL DHT11 PIN AND VAR
//..................................................................................//


#include <Arduino.h>
#include <U8g2lib.h>  //SCREEN LIBRARY
#include <SPI.h>
#include <Wire.h>
#include <dht.h>  //DHT11 LIBRARY
#define dataPin 8  //DHT11 PIN CONFIG
#include <virtuabotixRTC.h> //Library used

int inputPin = A0;  //LM35 PIN CONFIG

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0); //SCREEN CONFIG
virtuabotixRTC myRTC(10, 11, 12);

const int numReadings = 10; // NUMBER OF READINGS
const int relay =7;  //RELAY PIN
const int buttonUp =2;  //UP BUTTON PIN
const int buttonDown =4;  //DOWN BUTTON PIN
int targetTemp=17;  //TARGET TEMPERATURE VAR

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
int tem = 18;

int period = 3000;  // VARIABLES FOR MILLIS
int period2 = 6000;
int periodlm35 = 10000;
int period4 = 60000;
unsigned long time_1 = 0;
unsigned long time_2 = 0;
unsigned long time_3 = 0;
unsigned long time_4 = 0;

dht DHT;  //DHT11 CONFIG

void setup() 
{   
   Serial.begin(9600);
   
   u8g2.begin();
  
   pinMode(relay,OUTPUT);  //RELAY PIN CONFIG
   pinMode(buttonUp, INPUT);  //BUTTON UP CONFIG
   pinMode(buttonDown, INPUT);  //BUTTON DOWN CONFIG

   for (int thisReading = 0; thisReading < numReadings; thisReading++) 
   {
    readings[thisReading] = 0;
   }

   //myRTC.setDS1302Time(0, 39, 21, 7, 03, 02, 2020);
  
}

void loop() 
{
    //TEMP AND HUM READING DHT11

   myRTC.updateTime();
   
   int readData = DHT.read11(dataPin);
//   int t = DHT.temperature;
   int h = DHT.humidity;   

    //SCREEN CYCLE

    if (millis() >= time_1 + period)
    {
     time_1 += period;
     u8g2.clearBuffer();
     u8g2.drawRFrame(0,0,128,32,7);
     u8g2.setFont(u8g2_font_t0_22b_te);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
     
     u8g2.setCursor(30,16);
     u8g2.print(myRTC.hours);
     
     u8g2.drawStr(58,16,":");
     
     u8g2.setCursor(75,16);
     u8g2.print(myRTC.minutes);
    
     u8g2.setCursor(7,30);
     u8g2.print(myRTC.dayofmonth);
     
     u8g2.drawStr(30,30,"/");
     
     u8g2.setCursor(40,30);
     u8g2.print(myRTC.month);
    
     u8g2.drawStr(64,30,"/");
     
     u8g2.setCursor(75,30);
     u8g2.print(myRTC.year);
    
     u8g2.sendBuffer();         // transfer internal memory to the display

     delay(100);
    }
    if (millis() >= time_2 + period2)
    {
     time_2 += period2;    
     u8g2.clearBuffer();
     u8g2.drawRFrame(0,0,128,32,7);
     u8g2.setFont(u8g2_font_t0_22b_te);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
     
     u8g2.drawStr(10,16,"TEM:"); // write something to the internal memory
     u8g2.setCursor(55,16);
     u8g2.print(tem);
     //u8g2.drawStr(65,16,char(223));
     u8g2.setCursor(83,16);
     u8g2.print("C");
    
     u8g2.drawStr(10,30,"HUM:"); // write something to the internal memory
     u8g2.setCursor(55,30);
     u8g2.print(h);
     u8g2.setCursor(83,30);
     u8g2.print("%");
     
     u8g2.sendBuffer();         // transfer internal memory to the display

     delay(100);
    }
  
    //BUTTONS
  
   if (digitalRead(buttonUp) == HIGH) {targetTemp = targetTemp + 1; }

   else{  }
  
   if (digitalRead(buttonDown) == HIGH) {targetTemp = targetTemp - 1; }

   else {  }     
     
     //SCREEN CHANGE ON PRESS
 
   if (digitalRead(buttonUp) == HIGH || digitalRead(buttonDown) == HIGH) 
    {  
      u8g2.clearBuffer();         // clear the internal memory
      u8g2.setFont(u8g2_font_t0_22b_te);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
      u8g2.drawStr(4,30,"SET:");  // write something to the internal memory
      u8g2.setCursor(65,30);
      u8g2.print(targetTemp);
      u8g2.sendBuffer();         // transfer internal memory to the display
      delay(1000);
    }

    delay(300);

  //TEMP READING (AVERAGE)

 if (millis() >= time_3 + periodlm35)
  {
     time_3 += periodlm35;
  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(inputPin);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  tem = (average * 0.0048828125 * 100);
  
  // send it to the computer as ASCII digits
  Serial.println(tem);
  delay(200);        // delay in between reads for stability
  }

   //RELAY 

   if (millis() >= time_4 + period4)
    {
     time_4 += period4;

   if (tem <= targetTemp ) {digitalWrite(7,HIGH);}
  
   else {digitalWrite(7,LOW);}

   delay(100);
    }
}
