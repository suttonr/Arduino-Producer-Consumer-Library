#include <Wire.h>
#include <Consumer.h>



Consumer myConsumer(2);

void setup(){
 Serial.begin(9600);
 Wire.begin();
 
}

void loop(){
 byte ret = myConsumer.readBuffer();

 Serial.print("Meta:0x");
 Serial.print(ret,HEX);
 Serial.print(" Value:0x");
 Serial.println(myConsumer.getValue(ret),HEX);
  
  
  delay(500);
}
