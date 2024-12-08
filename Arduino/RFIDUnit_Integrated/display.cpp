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
#include "rfid_fsm.h"
#include "PIR_sense.h"
#include "display.h"

#define TFT_DC 0
#define TFT_CS 2
#define TFT_MOSI 3
#define TFT_SCLK 1

Adafruit_ILI9341 tft = Adafruit_ILI9341(2, 0, 3, 1, -1, -1); // create instance of screen

//define states global variable 
DISP_STATES DISP_STATE;

void lcd_init(){
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
  tft.fillRect(0, 180, 320, 60, ILI9341_MAROON);
}

uint16_t time_left(uint32_t time_rem){
  
  return (BOOKING_TMR_PERIOD / 1000) - (time_rem / 1000);
}

void uid_read(uint16_t uid[4]){
    if(uid[0] == 147 && uid[1] == 52 && uid[2] == 251 && uid[3] == 225){
      tft.println("Otieno");
    }
    if(uid[0] == 243 && uid[1] == 116 && uid[2] == 80 && uid[3] == 226){ //NOT REGISTERING, FAULTY CARD
      tft.println("Hilius");
    }
    if(uid[0] == 185 && uid[1] == 22 && uid[2] == 43 && uid[3] == 2){
      tft.println("Dr. Ross");
    }
    if(uid[0] == 181 && uid[1] == 90 && uid[2] == 51 && uid[3] == 2){
      tft.println("Mr. Git");
    }
    if(uid[0] == 179 && uid[1] == 27 && uid[2] == 221 && uid[3] == 228){ // FAULY
      tft.println("Prof. Kal");
    }
    if(uid[0] == 41 && uid[1] == 113 && uid[2] == 51 && uid[3] == 2){
      tft.println("Kasheef");
    }
}


void tickFnct_disp(){

  switch (DISP_STATE){

      case(INIT_DISPLAY):
        tft.setTextColor(ILI9341_WHITE);
        //tft.fillScreen(ILI9341_BLACK);
          if(RFID_STATE == available){
            DISP_STATE = AVAIL;
          }
          if(RFID_STATE == welcome){
            DISP_STATE = WELC;
          }
          if(RFID_STATE == booked){
            DISP_STATE =  BOOKED_DISPLAY;
          }


      break;

      case(AVAIL):
        tft.setCursor(40, 60);
        tft.setTextSize(4);
        tft.setTextColor(ILI9341_WHITE);
        tft.println("Available");
     
        tft.setCursor(50, 100);
        tft.setTextSize(2);
        tft.println("Scan RFID to Book");
        if(occupied == true){
          tft.setCursor(0, 180);
          tft.fillRect(0, 180, 320, 60, ILI9341_MAROON);
          tft.setCursor(100, 200);
          tft.setTextSize(2);
          tft.setTextColor(ILI9341_WHITE);
          tft.println("OCCUPIED");
        }
        if(occupied == false){
          tft.setCursor(0, 180);
          tft.fillRect(0, 180, 320, 60, ILI9341_DARKGREEN);
          tft.setCursor(100, 200);
          tft.setTextSize(2);
          tft.setTextColor(ILI9341_WHITE);
          tft.println("UNOCCUPIED");
        }
        if(RFID_STATE == welcome){
            DISP_STATE = WELC;
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
            tft.fillRect(0, 0, 320, 240, ILI9341_BLACK);
            DISP_STATE =  BOOKED_DISPLAY;
        }

      break;

      case( BOOKED_DISPLAY):

        if(alert == 0){
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
          tft.fillRect(195, 115, 80, 50, ILI9341_BLACK);
          tft.println(time_left(booking_tmr_remaining)); //replace with actual time left
        } else {
          tft.setCursor(75, 50);
          tft.setTextSize(4);
          tft.setTextColor(ILI9341_RED);
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
          tft.fillRect(195, 115, 80, 50, ILI9341_BLACK);
          tft.println(time_left(booking_tmr_remaining)); //replace with actual time left
        }

        if(occupied == true){
          tft.setCursor(0, 180);
          tft.fillRect(0, 180, 320, 60, ILI9341_MAROON);
          tft.setCursor(100, 200);
          tft.setTextSize(2);
          tft.setTextColor(ILI9341_WHITE);
          tft.println("OCCUPIED");
        }
        if(occupied == false){
          tft.setCursor(0, 180);
          tft.fillRect(0, 180, 320, 60, ILI9341_DARKGREEN);
          tft.setCursor(100, 200);
          tft.setTextSize(2);
          tft.setTextColor(ILI9341_WHITE);
          tft.println("UNOCCUPIED");
        }

        if(RFID_STATE == available){
            tft.fillRect(0, 0, 320, 180, ILI9341_BLACK);
            DISP_STATE = AVAIL;
          }



      break;







  }


}