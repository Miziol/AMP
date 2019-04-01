#include "i2c.h"

/*
 Connection list
 Arduino <--> RTC DS3231 (sign on my board)
 SDA     <--> SDA (D)
 SCL     <--> SCL (C)
 3.3V    <--> Vin (+) //2.3V-5.5V
 GND     <--> GND (-)
*/

void setup_RTC()
{
  Wire.begin();
  Wire.beginTransmission(0x68);
  Wire.write(0x0E);
  Wire.write(0b00011100);
  Wire.endTransmission();
}


//Data
uint8_t hours, minutes, seconds; //data RTC (DS3231)

//Setup of elements
void setup()
{
  Serial.begin(9600);
  
  //RTC
  setup_RTC();
}

//Operating system
void loop()
{
  
//reading data form RTC
  Wire.beginTransmission(0x68); //It's an address of RTC DS3231
  Wire.write(0);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 3);

  while(Wire.available())
  {
    seconds = Wire.read();
    minutes = Wire.read();
    hours = Wire.read();

    //conversion of data (for more look to spec od RTC)
    seconds = (((seconds & 0b11110000) >> 4) * 10 + (seconds & 0b00001111));
    minutes = (((minutes & 0b11110000) >> 4) * 10 + (minutes & 0b00001111));
    hours = (((hours & 0b00100000) >> 5) * 20 + ((hours & 0b00010000) >> 4) * 10 + (hours & 0b00001111));
  }
  
  Serial.print(hours); Serial.print(":"); Serial.print(minutes); Serial.print(":"); Serial.println(seconds);
}
