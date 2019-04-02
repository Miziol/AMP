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

void get_sec( uint8_t* out )
{
  Wire.beginTransmission(0x68);
  Wire.write(0x00);
  Wire.endTransmission();

  Wire.requestFrom( 0x68, 1);

  while(Wire.available())
  {
    *out = Wire.read();
    
    *out = (((*out & 0b01110000) >> 4) * 10 + (*out & 0b00001111));
  }
}

void get_min( uint8_t* out )
{
  Wire.beginTransmission(0x68);
  Wire.write(0x01);
  Wire.endTransmission();

  Wire.requestFrom( 0x68, 1);

  while(Wire.available())
  {
    *out = Wire.read();
    
    *out = (((*out & 0b01110000) >> 4) * 10 + (*out & 0b00001111));
  }
}

void get_h( uint8_t* out )
{
  Wire.beginTransmission(0x68);
  Wire.write(0x02);
  Wire.endTransmission();

  Wire.requestFrom( 0x68, 1);

  while(Wire.available())
  {
    *out = Wire.read();
    
    *out = (((*out & 0b00100000 ) >> 5) * 20 + ((*out & 0b00010000) >> 4) * 10 + (*out & 0b00001111));
  }
}

void get_day( uint8_t* out_day, uint8_t* out_month, uint8_t* out_year )
{
  Wire.beginTransmission(0x68);
  Wire.write(0x04);
  Wire.endTransmission();

  Wire.requestFrom(0x68, 3);

  while(Wire.available())
  {
    *out_day = Wire.read();
    *out_month = Wire.read();
    *out_year = Wire.read();

    *out_day = (((*out_day & 0b00110000) >> 4 ) *10 + (*out_day & 0b00001111));
    *out_month = (((*out_month & 0b00010000) >> 4 ) *10 + (*out_month & 0b00001111));
    *out_year = (((*out_year & 0b11110000) >> 4 ) *10 + (*out_year & 0b00001111));
  }
}

//Data
uint8_t hours, minutes, seconds; //time RTC (DS3231)
uint8_t day, month, year;

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
  get_sec( &seconds );
  get_min( &minutes );
  get_h( &hours );
  get_day( &day, &month, &year );
  Serial.print(hours); Serial.print(":"); Serial.print(minutes); Serial.print(":"); Serial.println(seconds);
  Serial.print(day); Serial.print("/"); Serial.print(month); Serial.print("/"); Serial.println(year);
  Serial.println();
}
