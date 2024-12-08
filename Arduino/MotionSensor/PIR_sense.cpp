/*
Authors: Ben Hill and Kashif Chopra
Date: December 6th 2024 
Latest: Added debouncer to get stable PIR readings 
*/

#include <SPI.h>
#include <Arduino.h>
#include "PIR_sense.h"
//#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include <stdio.h>
#include <stdbool.h>
//#include "lora_trans.h" -- when made

//Define PICO pin connections

#define SS_PIN 4  // GPIO 4 for PIR //No use for SS here since its not SPI correct?

//Define states global var
PIR_STATES STATE = INIT; //initialize to init

bool motion;                //PIR data, high or low
bool occupied;              //boolean flag
uint32_t internal_cnt = 0;      //counter incremented once every second in OCCUPIED state
uint32_t occ_tmr_cnt;       //cycles of no motion
uint32_t TIMER_LIMIT = 20;  //SUBJECT TO CHANGE FOR TESTING PURPOSES 
const uint32_t pir_gpio = 4; //gpio 4

//variables used for debouncer
uint32_t current_time = 0; 
uint32_t elapsed_time_debounce; 
const uint32_t DEBOUNCE_TIMER = 1000; //sample motion every 1s
const uint32_t DEBOUNCE_PERIOD = 5; // total time (s) over which it is debounced. 
const uint32_t MOTION_CNT_THRESHOLD = 4; // ie, if 4 or more out of every 5 debounced readings (ie readings taken once per second) of the PIR must be high for us to say motion_debounced = 1. Adjusted as needed according to sensitivity desired. 
bool motion_debounced = 0; //transmitted over LoRa
uint32_t motion_cnt = 0;
uint32_t debounce_cnt = 0;


void pir_init() {  //low level code to initialize port
  adc_init();
  _gpio_init(pir_gpio);
  gpio_set_dir(pir_gpio, GPIO_IN);
  Serial.println("PIR init");
}

bool pir_read() {  //boolean output of pir
  motion = gpio_get(pir_gpio);
  // Serial.print("Motion Detected: "); //only for testing uncomment if needed (better to Serial.print motion_debounced instead)
  // Serial.println(motion);
  return motion;
}


// Motion detection debouncer to get stable results from PIR
void pir_debounce(){

  elapsed_time_debounce = millis() - current_time; //

  // Serial.print("elapsed_time_debounce: ");  //for testing only
  // Serial.println(elapsed_time_debounce);
  // Serial.print("motion_cnt: ");
  // Serial.println(motion_cnt);
  // Serial.print("debounce_cnt: ");
  // Serial.println(debounce_cnt);

  if(elapsed_time_debounce >= DEBOUNCE_TIMER){ //every 1s sample motion
    
    if(motion){
      motion_cnt++;
    } 
    current_time = millis();
    debounce_cnt++;
  }

  if(debounce_cnt >= DEBOUNCE_PERIOD){ //rn 5 seconds debounce period

    if(motion_cnt>=MOTION_CNT_THRESHOLD){ // if MOTION_CNT_THRESHOLD  = 4 that means 4 or more times out of 5 if motion is detected, then motion is high else it is debounced to low. Adjust this number as needed via testing. 
      motion_debounced = 1;
    } else {
      motion_debounced = 0;
    }
    debounce_cnt = 0; //reset debounce_cnt
    motion_cnt = 0; //reset this
  }
}
  



void tickFnct_pir() { // UNUSED HERE used in the RFID unit

  switch (STATE) {

    case INIT:
      Serial.println(F("Entering Intial State"));
      if (motion) {
        STATE = OCC;
      } else {
        STATE = UNOCC;
      }
      break;

    case UNOCC:
      Serial.println(F("Unoccupied"));
      occupied = false;
      if (motion) {
        Serial.println("Motion Detected, going to OCC");
        occupied = true;
        STATE = OCC;
      }
      delay(1000);
      break;

    case OCC:
      Serial.println(F("Occupied"));
      Serial.print("Seconds Passed: ");
      Serial.println(internal_cnt);
      Serial.print("Occ_tmr_cnt: ");
      Serial.println(occ_tmr_cnt);
      //maybe print internal timer cnt, stuff liek that 
      
      internal_cnt++;

      //Serial.print("internal_cnt ");
      //Serial.println(internal_cnt);

      if (motion) {
        occ_tmr_cnt = 0;
      }

      if (internal_cnt >= 5) {  //will occur once every minute (put 60) // TEMPORARILY 5 for testing
        if (occ_tmr_cnt <= TIMER_LIMIT) { // was hardcoded 20 earlier // !motion &&
          occ_tmr_cnt++;
        }
      }

      if (occ_tmr_cnt >= TIMER_LIMIT) {
        internal_cnt = 0; //reset this back to 0
        STATE = UNOCC;
      }

      if (internal_cnt == 5){ //same as above, only 5 for testing purposes, use 60
        internal_cnt = 0;
      }

      delay(1000);
      
  }
}