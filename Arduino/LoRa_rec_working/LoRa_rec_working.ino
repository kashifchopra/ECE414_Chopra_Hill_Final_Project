/*
  Author: Kashif Chopra
  Date: December 6, 2024
  LoRa Receiver
  Receives and displays contents of test packet
  Requires LoRa Library by Sandeep Mistry - https://github.com/sandeepmistry/arduino-LoRa

*/
 
// Include required libraries
#include <SPI.h>
#include <LoRa.h>
#include "LoRa_rec.h"

 
void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
 
  lora_rec_init();

}
 
void loop() {
 
  lora_rec_operate();

}










// extra code for buzzer:

 
//uint8_t buzzerPin = 8; //buzzer connected to GP8 // testing buzzer
  
  //buzzer
  //pinMode(buzzerPin, OUTPUT);


  //buzzer testing
  // tone(buzzerPin,440); //A4
  // delay(500);
  // tone(buzzerPin,494); //B4
  // delay(500);
  // tone(buzzerPin,523); //C4
  // delay(500);