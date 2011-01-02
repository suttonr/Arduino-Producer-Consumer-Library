/*
 * Producer/Producer Library -- Producer.cpp
 * Created by: Ryan M Sutton
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

#include "Producer.h"
void Producer::onRequest( void (Producer::*function)() )
{
  onRequest(function);
}

Producer::Producer(int addr){
  _addr = addr;
  _buffRead = 0;
  _buffWrite=0;
  _buffInvert=false;
  _lastUpdate = 0;

  begin(_addr);
  onRequest(&Producer::i2cRequest);
}

byte Producer::bufferWrite(word data,byte meta){
  _buffer[_buffWrite] = data;
  _buffMeta[_buffWrite] = meta;
  _buffWrite++;

  if (_buffWrite == BUFMAX){
    _buffWrite = 0;
    _buffInvert=true;
  }
  return 1;
}

void Producer::i2cRequest(){
  byte answer[6];

  // responce: 0xDDDDDDDDMMCC ( D:data M:meta C:checksum )
  if (((_buffWrite >_buffRead)&&!_buffInvert)||((_buffWrite <=_buffRead)&&_buffInvert)){
    //build responce
    answer[4] = _buffMeta[_buffRead];
    answer[3] = _buffer[_buffRead] & 0xFF;
    answer[2] = (_buffer[_buffRead]>>8)&0xFF; 
    answer[1] = (_buffer[_buffRead]>>16)&0xFF;
    answer[0] = (_buffer[_buffRead]>>24)&0xFF;
   
    //compute checksum
    answer[5] = answer[0] ^ answer[1] ^ answer[2] ^ answer[3] ^ answer[4];

    //send responce
    send(answer,6);

  } else {
    //build error message
    answer[4] = 0xFF;
    answer[3] = _buffRead & 0xFF;
    answer[2] = (_buffRead>>8)&0xFF;
    answer[1] = _buffWrite & 0xFF;
    answer[0] = (_buffWrite>>8)&0xFF;

    //compute checksum
    answer[5] = answer[0] ^ answer[1] ^ answer[2] ^ answer[3] ^ answer[4];

    //send responce
    send(answer,6);
  }
}
