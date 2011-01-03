/* 
 * Producer Example -- Producer.pde
 * Created by: Ryan M Sutton
 *  
 * Summary:  Uses TinyGPS and Wire Lib to collect GPS data and send it to
             the main processor on the I2C bus.
 *
 * Hardware: I2C connection to Consumer on pins 4 & 5
 *           
 *           
 *
 * Copyright (c) 2011, Ryan M Sutton
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Ryan M Sutton nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Ryan M Sutton BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
*/


#include <Producer.h>
#include <Wire.h>
//#define TWI_FREQ 400000L
//#define BUFMAX    100

Producer myProducer(2);


void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  Serial.println("hello");
  
  Wire.begin(2);
  Wire.onRequest(myRequest);
  
}

void myRequest(){
  myProducer.i2cRequest();
}

void loop()
{
  word data=5;
  unsigned int myCount=0;
  
  while(myCount<65000){
    byte ret=0; 
    ret = myProducer.bufferWrite(myCount,0x1);
    Serial.print("Writing: Meta:0x1 Data:0x");
    Serial.println(myCount,HEX);
    ret = myProducer.bufferWrite(myCount/2,0x2);
    Serial.print("Writing: Meta:0x2 Data:0x");
    Serial.println(myCount/2,HEX);
    ret = myProducer.bufferWrite(myCount/3,0x3);
    Serial.print("Writing: Meta:0x3 Data:0x");
    Serial.println(myCount/3,HEX);
 
    delay(1000);
    myCount++;
  }
}


