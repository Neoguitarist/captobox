/*
    UNO Master I2C, read a float value from ATTiny85 slave
    Copyright (C) <2012>  <Lucas Fernandez>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Wire.h>

#define I2C_SLAVE_ADDR_TEMP   0x27         // I2C Temperature slave address

#define DELAY_TEMP_READ   140              // wait 140 ms before ask temp value
#define DELAY_NEW_SAMPLE 5000              // time before new sample (1s)

float dustValue;
byte* floatPtr;

void setup(){
  Wire.begin();                            // start I2C Bus as Master
  Serial.begin(38400);                     // start serial to send dust output
  dustValue = 0;
}

void loop(){
  dustValue = 0;

  Wire.beginTransmission(I2C_SLAVE_ADDR_TEMP);  // transmit to slave device
  Wire.write(0xFF);                             // sends usuless data 
  Wire.endTransmission();                       // stop transmitting

  delay(DELAY_TEMP_READ);

  Wire.requestFrom(I2C_SLAVE_ADDR_TEMP, 4);     // request 4 byte from slave (float = 4 bytes)

  if (Wire.available()) {
    floatPtr = (byte*) &dustValue;
    (*floatPtr) = Wire.read(); ++floatPtr;
    (*floatPtr) = Wire.read(); ++floatPtr;
    (*floatPtr) = Wire.read(); ++floatPtr;
    (*floatPtr) = Wire.read();
    Serial.print("dust = ");      // print out slave byte to Serial monitor
    Serial.print(dustValue,DEC);  // print out slave byte to Serial monitor
    Serial.println(" ug/m3");         // print out slave byte to Serial monitor
  } else {
    Serial.println("NaN");  
  }

  delay(DELAY_NEW_SAMPLE);
}
