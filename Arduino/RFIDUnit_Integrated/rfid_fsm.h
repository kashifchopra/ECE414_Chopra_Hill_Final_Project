#ifndef RFID_FSM_H
#define RFID_FSM_H


typedef enum {
  initial, available, welcome, booked
} RFID_STATES; 

extern RFID_STATES RFID_STATE;

void update_booking_userID();
void reset_booking_userID();
void rfid_init();

void rfid_fsm(); 


#endif