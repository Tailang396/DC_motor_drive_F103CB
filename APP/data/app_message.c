//
// Created by asus on 2025/4/13.
//

#include <string.h>
#include "app_message.h"
#include "app_motor.h"
#include "app_command.h"
#include "app_block.h"


uint8_t ctrl_data[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t tx_data[11];
uint8_t rec_mode_temp;
uint8_t rs485_rx_flag = 0x00;
uint8_t can_rx_flag = 0x00;
uint8_t tx_flag = 0x00;
uint8_t can_tx_id = 0xF0;
uint8_t can_tx_data[8];

void bsp_rs485_callback(bsp_uart_e e, uint8_t *s, uint16_t l)
{
    if(e == E_RS485 || e == E_UART3)
    {
        if(l==6)
        {
            if(!block_flag)
            {
                memcpy(ctrl_data, s, l);
                message_rec(&motor_aim);
            }
        }
    }
}

void bsp_can_mode_callback(bsp_can_msg_t *msg)
{
    ctrl_data[0] = 0XBB;
    ctrl_data[1] = 0XAA;
    ctrl_data[2] = ((motor.id << 4) & 0xf0) | (msg->data[motor.id] & 0x0f);
    ctrl_data[3] = 0x00;
    ctrl_data[4] = 0x00;
    ctrl_data[5] = 0xCC;
    message_rec(&motor_aim);
    can_rx_flag = 0xff;
}

void bsp_can_ctrl_callback(bsp_can_msg_t *msg)
{
    if(!block_flag)
    {
        ctrl_data[3] = msg->data[2 * motor.id];
        ctrl_data[4] = msg->data[2 * motor.id + 1];
        message_rec(&motor_aim);
        can_rx_flag = 0xff;
    }
}


void message_rec(motor_rec_Typedef *rec_motor)
{
    uint8_t motor_rec_id;
    if(ctrl_data[0]==0xBB && ctrl_data[1]==0xAA && ctrl_data[5]==0xCC)
    {
        tx_flag = 0x00;
        motor_rec_id = (ctrl_data[2] & 0xf0) >> 4;
        if(motor_rec_id != motor.id)
        {
            return;
        }
        rec_motor->rec_id = motor_rec_id;
        rs485_rx_flag = 0xff;
        rec_mode_temp = ctrl_data[2] & 0x0f;
        switch (rec_mode_temp)
        {
            case 0x08:
                tx_flag = 0xff;
            case 0x00:
                rec_motor->mode = disable_loop;
                break;
            case 0x09:
                tx_flag = 0xff;
            case 0x01:
                rec_motor->mode = no_loop;
                rec_motor->pwm = ctrl_data[3] << 8 | ctrl_data[4];
                break;
            case 0x0A:
                tx_flag = 0xff;
            case 0x02:
                rec_motor->mode = current_loop;
                rec_motor->current = ctrl_data[3] << 8 | ctrl_data[4];
                break;
            case 0x0B:
                tx_flag = 0xff;
            case 0x03:
                rec_motor->mode = speed_loop;
                rec_motor->speed = ctrl_data[3] << 8 | ctrl_data[4];
                break;
            case 0x0C:
                tx_flag = 0xff;
            case 0x04:
                rec_motor->mode = position_loop;
                rec_motor->position = ctrl_data[3] << 8 | ctrl_data[4];
                break;
            default:
                rec_motor->mode = disable_loop;
                break;
        }
    }
}


void message_rs485_send(motor_Typedef *send_motor)
{
    tx_data[0] = 0xBB;
    tx_data[1] = 0xAC;
    tx_data[2] = ((send_motor->id << 4) & 0xf0) | (send_motor->mode & 0x0f);
    tx_data[3] = send_motor->current >> 8;
    tx_data[4] = send_motor->current & 0xff;
    tx_data[5] = send_motor->speed >> 8;
    tx_data[6] = send_motor->speed & 0xff;
    tx_data[7] = send_motor->position >> 8;
    tx_data[8] = send_motor->position & 0xff;
    tx_data[9] = send_motor->Temperature;
    tx_data[10] = 0xCC;
    bsp_rs485_send(tx_data, 11);
    bsp_uart_send(E_UART3, tx_data, 11);
    if(!debug_mode)
        bsp_uart_send(E_UART_DEBUG, tx_data, 11);
}

void message_can_send(motor_Typedef *send_motor)
{
    can_tx_data[0] = ((send_motor->id << 4) & 0xf0) | (send_motor->mode & 0x0f);
    can_tx_data[1] = send_motor->current >> 8;
    can_tx_data[2] = send_motor->current & 0xff;
    can_tx_data[3] = send_motor->speed >> 8;
    can_tx_data[4] = send_motor->speed & 0xff;
    can_tx_data[5] = send_motor->position >> 8;
    can_tx_data[6] = send_motor->position & 0xff;
    can_tx_data[7] = send_motor->Temperature;
    bsp_can_send(can_tx_id, can_tx_data);
}