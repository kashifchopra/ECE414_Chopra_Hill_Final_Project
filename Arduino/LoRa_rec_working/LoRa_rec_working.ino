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