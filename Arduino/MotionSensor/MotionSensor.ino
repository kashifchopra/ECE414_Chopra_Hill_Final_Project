//MAIN file to be ran

#include <SPI.h>
#include "PIR_sense.h"


void setup(){
  Serial.begin(9600);
  while (!Serial);
  SPI.begin();
  pir_init();
  delay(5000); //allow PIR to settle

}

void loop(){ //MAIN but arduino
  pir_read();
  tickFnct_pir();

}