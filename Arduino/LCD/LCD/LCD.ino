#include <SPI.h>
#include <Arduino.h>
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include <stdio.h>
#include <stdbool.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
//#include "display.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(2, 0, 3, 1, -1, -1);


void setup() {
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
  tft.fillRect(0, 180, 320, 60, ILI9341_MAROON);
}

void loop() {
  //AVAILABLE
        // tft.setCursor(40, 60);
        // tft.setTextSize(4);
        // tft.setTextColor(ILI9341_WHITE);
        // tft.println("Available");
        // tft.setCursor(45, 95);
        // tft.setTextSize(2);
        // tft.println("Scan RFID to Book");
        // tft.setCursor(100, 200);
        // tft.setTextSize(2);
        // tft.println("Occupied");

  //WELCOME
        // tft.fillRect(10, 10, 300, 220, ILI9341_CYAN);
        // tft.setCursor(20, 60);
        // tft.setTextSize(6);
        // tft.setTextColor(ILI9341_NAVY);
        // tft.println("WELCOME!");
        // tft.setCursor(90, 130);
        // tft.setTextSize(3);
        // tft.println("Kasheef");

  //BOOKED 
        tft.setCursor(75, 50);
        tft.setTextSize(4);
        tft.setTextColor(ILI9341_WHITE);
        tft.println("BOOKED");
        tft.setCursor(45, 95);
        tft.setTextSize(2);
        tft.println("Booked By:");
        tft.setCursor(165, 95);
        tft.println("Kasheef"); //replace with uid_read()
        tft.setCursor(75, 115);
        tft.setTextSize(2);
        tft.println("Time Left:");
        tft.setCursor(195, 115);
        tft.println("60"); //replace with actual time left

        tft.setCursor(100, 200);
        tft.setTextSize(2);
        tft.println("Occupied");


}
