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

void pir_init() {  //low level code to initialize port
  adc_init();
  _gpio_init(pir_gpio);
  gpio_set_dir(pir_gpio, GPIO_IN);
  Serial.println("PIR init");
}

bool pir_read() {  //boolean output of pir
  motion = gpio_get(pir_gpio);
  Serial.print("Motion Detected: ");
  Serial.println(motion);
  return motion;
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