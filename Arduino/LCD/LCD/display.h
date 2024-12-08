#ifndef DISPLAY_H
#define DISPLAY_H

typedef enum{
  INIT, AVAIL, WELC, BOOKED
} DISP_STATES;

extern DISP_STATES STATE_D;

void uid_read();
uint32_t time_left();
void lcd_init();
void tickFnct_disp();


#endif