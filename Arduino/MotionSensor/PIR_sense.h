#ifndef PIR_H
#define PIR_H


typedef enum{
  INIT, UNOCC, OCC
} PIR_STATES;

extern PIR_STATES STATE;

void pir_init();
bool pir_read();
void tickFnct_pir();

#endif

