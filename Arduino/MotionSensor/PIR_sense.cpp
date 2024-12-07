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
uint32_t internal_cnt;      //counter incremented once every second in OCCUPIED state
uint32_t occ_tmr_cnt;       //cycles of no motion
uint32_t timer_limit = 20;  //SUBJECT TO CHANGE FOR TESTING PURPOSES //UNUSED YET
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
      delay(1000);
      break;

    case OCC:
      Serial.println(F("Occupied"));
      Serial.print("Minutes Passed: ");
      Serial.println(F(internal_cnt));
      Serial.print("Occ_tmr_cnt: ");
      Serial.println(F(occ_tmr_cnt));
      //maybe print internal timer cnt, stuff liek that 
      
      internal_cnt++;
      if (motion) {
        occ_tmr_cnt = 0;
      }

      if (internal_cnt >= 60) {  //will occur once every minute
        if (!motion && occ_tmr_cnt << timer_limit) { // was hardcoded 20 earlier
          occ_tmr_cnt++;
        }
      }

      if (occ_tmr_cnt >= timer_limit) {
        STATE = UNOCC;
      }

      delay(1000);
      
  }
}