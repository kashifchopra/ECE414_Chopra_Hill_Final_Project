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
  tickFnct_pir();

  lora_trans_operate();
}