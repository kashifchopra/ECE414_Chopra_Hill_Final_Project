#ifndef RFID_FSM_H
#define RFID_FSM_H


typedef enum {
  initial, available, welcome, booked
} RFID_STATES; 

extern RFID_STATES RFID_STATE;
extern bool alert;//make extern and add to .h
extern uint16_t booking_userID[4];//make extern and add to .h/
extern uint32_t booking_tmr_remaining;//make extern and add to .h
extern const uint32_t BOOKING_TMR_PERIOD;

void update_booking_userID();
void reset_booking_userID();
void rfid_init();

void rfid_fsm(); 


#endif