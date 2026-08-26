//
// Created by asus on 2025/4/13.
//

#ifndef CBT6_APP_MESSAGE_H
#define CBT6_APP_MESSAGE_H
#include "bsp_def.h"
#include "motorCtrl.h"

extern uint8_t rs485_rx_flag;
extern uint8_t can_rx_flag;
extern uint8_t tx_flag;
extern uint8_t can_tx_id;
extern uint8_t ctrl_data[6];

void message_rec(motor_rec_Typedef *rec_motor);
void message_rs485_send(motor_Typedef *send_motor);
void message_can_send(motor_Typedef *send_motor);


#endif //CBT6_APP_MESSAGE_H
