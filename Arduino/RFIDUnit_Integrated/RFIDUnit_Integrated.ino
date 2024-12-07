/*
 * --------------------------------------------------------------------------------------------------------------------
 * 
 * --------------------------------------------------------------------------------------------------------------------
 * Uses MFRC522 library for RFID; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * Example sketch/program showing how to read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
 * Reader on the Arduino SPI interface.
 * 
 */

#include <SPI.h>
#include <MFRC522.h>
#include "rfid_fsm.h"
#include <LoRa.h>
#include "LoRa_rec.h"
#include "PIR_sense.h"
//#include SCREEN MODULE ADD HERE

#define RFID_CS 17
#define LoRa_rec_CS 13

void setup() {
    Serial.begin(9600);             // Initialize serial communications
    while (!Serial);                // Wait for serial port to connect
    SPI.begin();                    // Start SPI communication

    //Turn on RFID via chip select 
    digitalWrite(RFID_CS, LOW);
    digitalWrite(LoRa_rec_CS, HIGH);
    rfid_init();

    //Turn on LoRa rec via chip select (logic low = on)
    digitalWrite(RFID_CS, HIGH);
    digitalWrite(LoRa_rec_CS, LOW);
    lora_rec_init();


    //screen_init add here
}

void loop() {

//Two differnet SPI peripherals - turn one on that you are using

//Turn on LoRa rec via chip select (logic low = on)
digitalWrite(RFID_CS, HIGH);
digitalWrite(LoRa_rec_CS, LOW);
lora_rec_operate();

//After receving LoRa data, process it using PIR FSM: 
tickFnct_pir();

//Turn on RFID via chip select and run top level RFID FSM
digitalWrite(RFID_CS, LOW);
digitalWrite(LoRa_rec_CS, HIGH);
rfid_fsm();

}

