/*
  Author: Kashif Chopra
  Date: December 6, 2024
  LoRa Receiver
  Receives and displays contents of test packet
  Requires LoRa Library by Sandeep Mistry - https://github.com/sandeepmistry/arduino-LoRa

*/

#include <SPI.h>
#include <LoRa.h>
#include "LoRa_rec.h"

//define pins used by LoRa module. Change/configurable according to your board pinout. 
const int csPin = 13; // LoRa radio chip select
const int resetPin = 10; // LoRa radio reset
const int irqPin = 7; // Must be a hardware interrupt pin
bool motion = 0; //motion variable 
int receivedValue;

//initialise LoRa receiver (same as initialising transmitter)
void lora_rec_init(){
  Serial.println("Initialising LoRa Receiver");
  LoRa.setPins(csPin, resetPin, irqPin); // set the pins for LoRa 

  //Our RFM9x chip uses 915MHz frequency, so initialsed below using that:
  if(!LoRa.begin(915E6)){
    Serial.println("LoRa init failed.");
    //added extra tries
      // if(!LoRa.begin(915E6)){
      //     Serial.println("LoRa init failed, trying again...");
      //     if(!LoRa.begin(915E6)){
      //     Serial.println("LoRa init failed.");
      //     }
      // }
     // end extra tries

    while(1);
    //while(!LoRa.begin(915E6));
  }
}


//receiver function to run within loop 
void lora_rec_operate(){
   
  Serial.println("running lora_rec_operate()");
  

  // Try to parse packet ie check if its been received. Returns packet size in bytes if received, else retruns 0
  int packetSize = LoRa.parsePacket();

  if(packetSize){

    Serial.println("Received a packet");
    
    //print to serial out
    while(LoRa.available()){
      receivedValue = LoRa.read(); //CHECKME should be fine though 

      if(receivedValue == 49){ //"1" as a character is ASCII 49, since LoRa.read(0) reads it as a character
        motion = 1;
      } else {
        motion = 0;
      }

      Serial.print("Motion: ");
      Serial.print(motion);

      // Serial.print((char)LoRa.read()); //Use for printing if packet has multiple characters 
    }
    Serial.println("");
    
    //Print RSSI and SNR. 
    //RSSI = signal strength = Less negative dB values are stronger signals (ie closer to 0)
    Serial.print("RSSI: ");
    Serial.println(LoRa.packetRssi());

    //SNR = signal to noise ratio in dB. Higher = better quality
    Serial.print("SNR: ");
    Serial.println(LoRa.packetSnr());

  }


}