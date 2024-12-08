/*Author: Kashif Chopra
  Date: December 6, 2024
  LoRa Transmitter
  Sends test packet with packet count
  Requires LoRa Library by Sandeep Mistry - https://github.com/sandeepmistry/arduino-LoRa
*/

#include <SPI.h>
#include <LoRa.h>
#include "LoRa_trans.h"
#include "PIR_sense.h" //INCLUDE THIS for final implementation to set the value for the bool motion variable

// Define the pins used by the LoRa module
const int csPin = 13;     // LoRa radio chip select
const int resetPin = 10;  // LoRa radio reset
const int irqPin = 7;    // Must be a hardware interrupt pin
//bool motion_debounced; //Now received from PIR UNIT

// Message counter
byte msgCount = 0;

//initialise LoRa transmitter (same as initialising receiver)
void lora_trans_init(){
  Serial.println("Initialising LoRa Transmitter");
  LoRa.setPins(csPin, resetPin, irqPin); // set the pins for LoRa 

  //Our RFM9x chip uses 915MHz frequency, so initialsed below using that:
  if(!LoRa.begin(915E6)){
    Serial.println("LoRa init failed");
    while(1);
  }
}

void lora_trans_operate(){
  
  Serial.print("Sending packet: ");
  Serial.println(msgCount);
 
  // Send packet
  LoRa.beginPacket();
  //LoRa.print("Packet ");
  //LoRa.print(msgCount);
  LoRa.print(motion_debounced); // send debounced PIR motion data 
  LoRa.endPacket();
 
  Serial.print("Sent: ");
  Serial.println(motion_debounced);


  msgCount++;

  //0.5 second delay
  delay(500);

}