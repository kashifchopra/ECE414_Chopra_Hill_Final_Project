/* NOTE - Latest files and changes have been integrated within the MotionSensor.ino module. This sketch is only the 
// basic LoRa test version for the transmitter used upto 11:30PM on December 6, 2024
Author: Kashif Chopra 
  Date: December 6, 2024
  LoRa Transmitter
  Sends test packet with packet count
  Requires LoRa Library by Sandeep Mistry - https://github.com/sandeepmistry/arduino-LoRa
*/
#include <SPI.h>
#include <LoRa.h>
#include "LoRa_trans.h"
//#include "PIR.h" //INCLUDE THIS for final implementation to set the value for the bool occupied variable

// Define the pins used by the LoRa module
const int csPin = 13;     // LoRa radio chip select
const int resetPin = 10;  // LoRa radio reset
const int irqPin = 7;    // Must be a hardware interrupt pin
bool occupied = 1; //CONNEC WITH PIR UNIT

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
  //TEMPORARY testing alternate between 1 and 0. occupied value must be read from PIR unit
  if(msgCount%2 == 1){
    occupied = 1;
  } else {
    occupied = 0;
  }
  // testing unit end// DELTE THIS LATER
 
  Serial.print("Sending packet: ");
  Serial.println(msgCount);
 
  // Send packet
  LoRa.beginPacket();
  //LoRa.print("Packet ");
  //LoRa.print(msgCount);
  LoRa.print(occupied); 
  LoRa.endPacket();
 
  Serial.print("Sent: ");
  Serial.println(occupied);


  msgCount++;

  //2 second delay
  delay(2000);

}