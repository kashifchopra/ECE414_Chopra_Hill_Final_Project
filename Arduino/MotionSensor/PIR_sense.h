#ifndef PIR_H
#define PIR_H


typedef enum{
  INIT, UNOCC, OCC
} PIR_STATES;

extern PIR_STATES STATE;
extern bool motion; //not sent via LoRa
extern bool motion_debounced; //sent via LoRa

void pir_init();
bool pir_read();
void tickFnct_pir();
void pir_debounce();

#endif

