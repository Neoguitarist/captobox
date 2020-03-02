/*
    ATTiny85 slave, sample a sensor and send a float value to Arduino UNO master
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

                  +-\/-+
        (RESET)  1|    |8  VCC (2.7-5.5V)
      Temp Read  2|    |7  I2C   SCK -> Uno A5
          DEBUG  3|    |6  (PB1) NA
            GND  4|    |5  I2C   SDA -> Uno A4
                  +----+
*/

#include "TinyWireS.h"      // ATTiny wire lib
#include "Statistic.h"      // Arduino Statistic lib

#define I2C_SLAVE_ADDR 0x27 // I2C slave address


#define DELAY_ADC       10  // time before re-read ADC 10 us

Statistic dust;
float     result;

byte*     floatPtr;
byte      byteRcvd;
byte      i;
int measurePin = A3;
int ledPower = 4;
int ledInd = 1;
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

void setup() {

  pinMode(measurePin, OUTPUT);
  pinMode(ledPower, OUTPUT);
  pinMode(ledInd, OUTPUT);
  TinyWireS.begin(I2C_SLAVE_ADDR);

  dust.clear();
  result  = 0;
  byteRcvd = 0;
  i       = 0;
}

void loop() {
  if (TinyWireS.available()) {         // if we get an I2C message
    byteRcvd = TinyWireS.receive();      // do nothing with the message
    digitalWrite(ledInd, HIGH);

    for (i = 0; i < 5; i++) {
      digitalWrite(ledPower, LOW); // power on the LED
      delayMicroseconds(samplingTime);// make 50 temperature sample
      dust.add(analogRead(measurePin));   // read and store temperature sensor output
      delayMicroseconds(DELAY_ADC);      // wait 10 us to stabilise ADC
      delayMicroseconds(deltaTime);
      digitalWrite(ledPower, HIGH); // turn the LED off
      delayMicroseconds(sleepTime);
    }



    // 0 - 5.0V mapped to 0 - 1023 integer values
    calcVoltage = dust.average() * (5.0 / 1024);

    // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
    // Chris Nafis (c) 2012
    dustDensity = (0.17 * calcVoltage - 0.1) * 1000;






    result = dustDensity ; // convert the result into a humain readable output

    if (result < 0) result = 0;          // avoid eratic datas
    dust.clear();                        // clear statistics to avoid leack and data stacking

    floatPtr = (byte*) &result;
    TinyWireS.send( *floatPtr );  // send first byte
    ++floatPtr; TinyWireS.send( *floatPtr );  // the second
    ++floatPtr; TinyWireS.send( *floatPtr );  // third
    ++floatPtr; TinyWireS.send( *floatPtr );  // fourth and final byte
    digitalWrite(ledInd, LOW); // turn the LED off
  }
}
