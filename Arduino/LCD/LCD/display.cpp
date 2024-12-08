#include <SPI.h>
#include <iostream>
#include <string>
#include <sstream>
#include <array>
#include <Arduino.h>
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include <stdio.h>
#include <stdbool.h>
#include <string>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <cstdint>



#define TFT_DC 0
#define TFT_CS 2
#define TFT_MOSI 3
#define TFT_SCLK 1

void lcd_init(){
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
  tft.fillRect(0, 180, 320, 60, ILI9341_MAROON);
}

uint16_t time_left(uint32_t time_rem){
  return time_rem / 1000;
}

void uid_read(uint16_t uid[4]){
    if(uid[0] == 147 && uid[1] == 52 && uid[2] == 251 && uid[3] == 225){
      tft.println("K. Chopra");
    }
    if(uid[0] == 243 && uid[1] == 116 && uid[2] == 80 && uid[3] == 226){
      tft.println("B. Hill");
    }
    if(uid[0] == 185 && uid[1] == 22 && uid[2] == 43 && uid[3] == 2){
      tft.println("Dr. Ross");
    }
    if(uid[0] == 181 && uid[1] == 90 && uid[2] == 51 && uid[3] == 2){
      tft.println("Mr. Git");
    }
}


void tickFnct_disp(){

  switch (STATE_D){

      case(INIT):
        tft.setTextColor(ILI9341_WHITE);
        tft.fillScreen(ILI9341_BLACK);
          if(RFID_STATE == available){
            STATE_D = AVAIL;
          }
          if(RFID_STATE == welcome){
            STATE_D = WELC;
          }
          if(RFID_STATE == booked){
            STATE_D = BOOKED;
          }


      break;

      case(AVAIL):
        tft.setCursor(40, 60);
        tft.setTextSize(4);
        tft.setTextColor(ILI9341_WHITE);
        tft.println("Available");
     
        tft.setCursor(50, 90);
        tft.setTextSize(2);
        tft.println("Scan RFID to Book");
        if(occupied){
          tft.setCursor(100, 200);
          tft.setTextSize(2);
          tft.println("OCCUPIED");
        }
        if(!occupied){
          tft.setCursor(100, 200);
          tft.setTextSize(2);
          tft.println("UNOCCUPIED");
        }
        if(RFID_STATE == welcome){
            STATE_D = WELC;
        }
      break;

      case(WELC):
        tft.fillRect(10, 10, 300, 220, ILI9341_CYAN);
        tft.setCursor(20, 60);
        tft.setTextSize(6);
        tft.setTextColor(ILI9341_NAVY);
        tft.println("WELCOME!");
        tft.setCursor(90, 130);
        tft.setTextSize(3);
        uid_read(booking_userID); // replace with return from uid_read()
        if(RFID_STATE == booked){
            STATE_D = BOOKED;
        }

      break;

      case(BOOKED):
        tft.setCursor(75, 50);
        tft.setTextSize(4);
        tft.setTextColor(ILI9341_WHITE);
        tft.println("BOOKED");
        tft.setCursor(45, 95);
        tft.setTextSize(2);
        tft.println("Booked By:");
        tft.setCursor(165, 95);
        uid_read(booking_userID); //replace with uid_read()
        tft.setCursor(75, 115);
        tft.setTextSize(2);
        tft.println("Time Left:");
        tft.setCursor(195, 115);
        tft.println(time_left(booking_tmr_remaining)); //replace with actual time left
        if(occupied){
          tft.setCursor(100, 200);
          tft.setTextSize(2);
          tft.println("OCCUPIED");
        }
        if(!occupied){
          tft.setCursor(100, 200);
          tft.setTextSize(2);
          tft.println("UNOCCUPIED");
        }

      break;







  }


}