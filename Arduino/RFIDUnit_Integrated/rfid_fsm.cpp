//latest 4 PM dec 3

#include <SPI.h>
#include <MFRC522.h>
#include "rfid_fsm.h"
#include "PIR_sense.h" // add when made

// Define pin connections for Raspberry Pi Pico
#define RST_PIN         9   // GP9
#define SS_PIN          17  // GP17 (CS pin for SPI)

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

//define states global variable 
RFID_STATES RFID_STATE; 

//global variables 
//bool occupied = 1; // TEMPORARY remove later

uint16_t booking_userID[4]; // = uid from RFID //rn hard set size to 4 but might wanna check with different RFID fobs - might wanna set it = mfrc522.uid.size
//occupied (access from PIR_sense.h) //make extern and add to .h

bool booking_tmr_complete = false; //flag for when booking timer is up
bool welcome_tmr_complete = false; //flag for when welcome screen timer is up, after which it will switch to "booked" screen

bool alert = false; //when alert = 1; the screen will simply highlight (with a different Bg color) the person who is currenbtly booked
//make extern and add to .h

unsigned long booking_tmr = 0; 
unsigned long welcome_tmr = 0; 

const uint32_t BOOKING_TMR_PERIOD = 40000; //in ms set as needed, constant - total time one booking can be 
const uint32_t WELCOME_TMR_PERIOD = 5000; //in ms set as needed, constant

uint32_t booking_tmr_remaining = 0; //make extern and add to .h

//functions to use: 
//mfrc522.PICC_IsNewCardPresent()
//mfrc522.PICC_DumpToSerial(&(mfrc522.uid));



// helper functions 

void update_booking_userID(){ //copy booking_userID = the mfrc522.uid (copy the struct to the integer array)
            for (byte i = 0; i < mfrc522.uid.size; i++) {
              booking_userID[i] = mfrc522.uid.uidByte[i];
              Serial.print(F("mfrc522.uid.uidByte[i]: "));
              Serial.println(mfrc522.uid.uidByte[i]); 
            }
}

void reset_booking_userID(){ //reset booking_userID
            for (byte i = 0; i < mfrc522.uid.size; i++) {
              booking_userID[i] = 0;
            }
}


void rfid_init(){
    mfrc522.PCD_Init();             // Initialize the MFRC522 module
    delay(4);                       // Optional delay for some boards

    // Print MFRC522 firmware details
    mfrc522.PCD_DumpVersionToSerial();
    Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

//FSM 
void rfid_fsm(){

    //FSM state actions 

    switch(RFID_STATE){
        case initial:
            //do nothing
            Serial.println(F("Entered inital state"));
            break; 

        case available:
            Serial.println(F("Entered available state"));
            //reset booking_userID = ""
            reset_booking_userID(); 

            break;

        case welcome:
            Serial.println(F("Entered welcome state"));
            booking_tmr_complete = false; //reset booking tmr complete
            //reset booking timer 
            booking_tmr = millis(); 

            welcome_tmr_complete = false; //timer that counts once the welcome screen has shown for 5 seconds 
            
            Serial.print(F("welcome timer count:"));
            Serial.println(millis() - welcome_tmr);

            Serial.print(F("welcome timer start time:"));
            Serial.println(welcome_tmr);

            Serial.print(F("welcome_tmr_complete:"));
            Serial.println(welcome_tmr_complete);

            //check if welcome timer period has elapsed: 
            if(millis() - welcome_tmr > WELCOME_TMR_PERIOD){
                Serial.println(F("welcome_tmr_complete set to true"));
                welcome_tmr_complete = true; 
                //return; //CHECKME
            }

            ////from demo code for serial output/debugging: 
                if (!mfrc522.PICC_ReadCardSerial()) { // Check if a card can be read
                Serial.println(F("Card read failed"));
                delay(500);
                }

                Serial.println(F("Card detected!"));
                // Dump debug info about the card
                mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
            ////

            //save uid of person who booked in booking_userID 
            update_booking_userID(); 
            Serial.println(F("Set booking_userID = mfcr522.uid"));

            break;

        case booked: //screen will display ID of user who booked and booking timer time left
             Serial.println(F("Entered booked state"));

             
            // if someone else tries to RFID in when it is booked, it will set "alert" high - which will then update the screen to highlight the current name of person who booked
            if(mfrc522.PICC_IsNewCardPresent()){
                alert = true; 
            } else{
                alert = false; 
            }

            Serial.print(F("booked state bool alert:"));
            Serial.println(alert);

            //printing booking_userID
            Serial.print(F("booked state booking_userID :"));
            for(int i = 0; i<mfrc522.uid.size; i++){
                
                if(i<mfrc522.uid.size-1){
                    Serial.print(booking_userID[i]);
                    Serial.print(" ");
                } else{
                    Serial.println(booking_userID[i]);
                }               
            }
            

            

            booking_tmr_remaining = millis() - booking_tmr; 

            //check if booking timer has elapsed: 
            if(booking_tmr_remaining > BOOKING_TMR_PERIOD){
                booking_tmr_complete = true; 
            }



            break;

    }

    // FSM state transitions 

    switch(RFID_STATE){
        case initial:
            RFID_STATE = available; //next state
            break; 

        case available:
            if(mfrc522.PICC_IsNewCardPresent()){ //If RFID is detected, go to welcome state else stay in avaiable 
                RFID_STATE = welcome; 
                // start welcome timer 
                welcome_tmr = millis(); 
            } else{
                RFID_STATE = available; 
            }
            break;

        case welcome:
            if(welcome_tmr_complete){
                RFID_STATE = booked; 
            } else {
                RFID_STATE = welcome; 
            }

            break;

        case booked:
            if(!occupied || booking_tmr_complete){ //bool occupied is a global variable set by PIR.c module on motion detector and receioved over LoRa on lora_rec.c 
                RFID_STATE = available; 
            } else {
                RFID_STATE = booked; 
            }
            break;

        default:
            RFID_STATE = initial; 
            break;

    }


}
