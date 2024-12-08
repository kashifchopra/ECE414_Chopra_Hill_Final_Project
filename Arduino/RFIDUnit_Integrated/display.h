#ifndef DISPLAY_H
#define DISPLAY_H

typedef enum {
  INIT_DISPLAY, AVAIL, WELC, BOOKED_DISPLAY
} DISP_STATES;

extern DISP_STATES DISP_STATE;

void uid_read();
uint32_t time_left();
void lcd_init();
void tickFnct_disp();


#endif