//MAIN file to be ran

#include <SPI.h>
#include "PIR_sense.h"
#include <LoRa.h>
#include "LoRa_trans.h"

void setup(){
  Serial.begin(9600);
  while (!Serial);
  SPI.begin();
  pir_init();
  delay(5000); //allow PIR to settle

  //LoRa transmitter init
  lora_trans_init();
  delay(1000);
}

void loop(){ //MAIN but arduino
  pir_read();
  //tickFnct_pir(); // Not needed here - Now this function is being used in the RFID main unit 
  pir_debounce();
  Serial.print("motion_debounced: ");
  Serial.println(motion_debounced);

  //send PIR motion_debounced over LoRa:
  lora_trans_operate();
}