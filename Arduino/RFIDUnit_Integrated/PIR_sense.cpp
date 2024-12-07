#include <SPI.h>
#include <Arduino.h>
#include "PIR_sense.h"
//#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include <stdio.h>
#include <stdbool.h>
#include "LoRa_rec.h"

//Define PICO pin connections

#define SS_PIN 4  // GPIO 4 for PIR //No use for SS here since its not SPI correct?

//Define states global var
PIR_STATES STATE = INIT; //initialize to init

//bool motion;                //PIR data, high or low = gets by importing LoRa_rec,h
bool occupied;              //boolean flag
uint32_t internal_cnt;      //counter incremented once every second in OCCUPIED state
uint32_t occ_tmr_cnt;       //cycles of no motion
uint32_t TIMER_LIMIT = 4;  //SUBJECT TO CHANGE FOR TESTING PURPOSES // 4 cycles of 5 seconds (comes from INTERNAL_CNT_MAX) each = 20 seconds for total time of no motion after which it switches to unoccupied
const uint32_t pir_gpio = 4; //gpio 4
const uint32_t INTERNAL_CNT_MAX = 5; // max number of seconds for internal count before it gets set back to zero. At the end of each 5 second interval with no motion, occupied_tmr_cnt gets incremented once. 

void pir_init() {  //low level code to initialize port
  adc_init();
  _gpio_init(pir_gpio);
  gpio_set_dir(pir_gpio, GPIO_IN);
  Serial.println("PIR init");
}

// bool pir_read() {  //boolean output of pir no use in this module since this is the reciever just need PIR FSM
//   //motion = gpio_get(pir_gpio);
//   Serial.print("Motion Detected: ");
//   Serial.println(motion);
//   return motion;
// }

void tickFnct_pir() {

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
      //delay(1000);
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

      if (internal_cnt >= INTERNAL_CNT_MAX){ //same as above, only 5 for testing purposes, use 60
        internal_cnt = 0;
      }

      delay(1000);
      
  }
}