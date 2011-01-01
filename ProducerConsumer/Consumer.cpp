/*
 * Producer/Consumer Library -- Consumer.cpp
 * Created by: Ryan M Sutton
 *
 * Copyright (c) 2010, Ryan M Sutton
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

#include <Consumer.h>

Consumer::Consumer(int addr){
  _addr = addr;
  lat=0;
  lon=0;
  alt=0;
  dataValid=false;
}

byte Consumer::getUpdate(){
  byte ret;
  byte count=0;
  ret=readSensorBuffer(false);
  while (ret){
    //Serial.println("READ");
    ret=readSensorBuffer(false);
    count++;
  }
  //Serial.println(count,HEX);
  return count;
}

byte Consumer::readSensorBuffer(boolean debug)
{
  //reset loop variables 
  int byteCount=0;
  long recievedVal=0;
  byte check=0;
  byte valType,c;
  
  //Serial.print("addr:");
  //Serial.println(_addr);
  Wire.begin();
  Wire.requestFrom(_addr, 6);
  //Serial.print("wa:");
  //Serial.println(Wire.available(),HEX); 
  if (6==Wire.available()){
    //Serial.print("!");
    while(Wire.available()) 
    { 
      //Serial.print("!");

      c = Wire.receive();
      if (debug){
        Serial.print(c,HEX);
      }
      if (byteCount <4){
        check ^= c; 
        recievedVal = recievedVal<<8;
        recievedVal |= c;
      }else if (byteCount <5){
        valType = c;
        check ^= c;
      }else{
        if (debug){
          if (check==c){
            Serial.print(":");
            Serial.print(recievedVal);
            Serial.println(":VALID");
          }else{
            Serial.print(":");
            Serial.print(recievedVal);
            Serial.println(":INVALID");
          }
        }
      }
      byteCount++;
    }
  }

  if ((check==c)&&(valType==0x1)){
    lat = recievedVal;
    return 0x1;
  } else if ((check==c)&&(valType==0x2)){
    lon = recievedVal;
    return 0x2;
  } else if ((check==c)&&(valType==0x3)){
    alt = recievedVal;
    return 0x3;
  }
  return 0x0;
}

