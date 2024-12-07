/*Author: Kashif Chopra
  Date: December 6, 2024
  LoRa Transmitter
  Sends test packet with packet count
  Requires LoRa Library by Sandeep Mistry - https://github.com/sandeepmistry/arduino-LoRa
*/
 
// Include required libraries
#include <SPI.h>
#include <LoRa.h>
#include "LoRa_trans.h"
 

void setup() {
 
  Serial.begin(9600);
  while (!Serial)
    ;
  lora_trans_init();
}
 
void loop() {
  lora_trans_operate();
}