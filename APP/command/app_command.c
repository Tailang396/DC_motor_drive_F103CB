//
// Created by asus on 2025/4/29.
//

#include "app_command.h"
#include <string.h>
#include "stdio.h"
#include "app_message.h"
#include "app_motor.h"

uint8_t debug_mode = 0x00;
uint8_t step_mode = 0x00;
uint8_t debug_data[128];
static int cur_data, spd_data, pos_data;
void uart_callback(bsp_uart_e e, uint8_t *s, uint16_t l)
{
    if (e == E_UART_DEBUG)
    {
        if(s[0] == 0xBB && s[1] == 0xAA)
        {
            memcpy(ctrl_data, s, 6);
            message_rec(&motor_aim);
            rs485_rx_flag = 0xff;
        } else
        {
            memset(debug_data,  0x00, 128);
            memcpy(debug_data, s, l);
            if(memcmp(debug_data, "debug", 5) == 0)
            {
                debug_mode = 0xff;
                motor_aim.rec_id = motor.id;
            }
            else if(memcmp(debug_data, "step", 4) == 0)
            {
                if (debug_mode)
                    step_mode = 0xff;
            }
            else if(memcmp(debug_data, "work", 4) == 0)
            {
                motor_aim.mode = disable_loop;
                debug_mode = 0x00;
                step_mode = 0x00;
            }
            else if(memcmp(debug_data, "disable", 7) == 0)
            {
                motor_aim.mode = disable_loop;
                step_mode = 0x00;
            }
            else if(memcmp(debug_data, "pidwrite", 8) == 0)
            {
                if(debug_mode)
                {
                    motor_aim.mode = disable_loop;
                    while (!bsp_flash_write("cur_pidcfg", &current_cfg, sizeof(current_cfg)));
                    while (!bsp_flash_write("pos_pidcfg", &position_cfg, sizeof(position_cfg)));
                    while (!bsp_flash_write("spd_pidcfg", &speed_cfg, sizeof(speed_cfg)));
                    bsp_uart_printf(E_UART_DEBUG, "write success\r\n");
                }
            }
            else if(memcmp(debug_data, "curcfg:", 7) == 0)
            {
                if(debug_mode)
                {
                    sscanf(debug_data, "curcfg:%f,%f,%f,%f,%f", &current_cfg.kp, &current_cfg.ki, &current_cfg.kd, &current_cfg.maxsum, &current_cfg.maxout);
                    app_pid_init();
                }
            }
            else if(memcmp(debug_data, "spdcfg:", 7) == 0)
            {
                if(debug_mode)
                {
                    sscanf(debug_data, "spdcfg:%f,%f,%f,%f,%f", &speed_cfg.kp, &speed_cfg.ki, &speed_cfg.kd, &speed_cfg.maxsum, &speed_cfg.maxout);
                    app_pid_init();
                }
            }
            else if(memcmp(debug_data, "poscfg:", 7) == 0)
            {
                if(debug_mode)
                {
                    sscanf(debug_data, "poscfg:%f,%f,%f,%f,%f", &position_cfg.kp, &position_cfg.ki, &position_cfg.kd, &position_cfg.maxsum, &position_cfg.maxout);
                    app_pid_init();
                }
            }
            else if(memcmp(debug_data, "readpidcfg", 10) == 0)
            {
                if(debug_mode)
                {
                    app_readflash_pidcfg();
                    bsp_uart_printf(E_UART_DEBUG, "cur_pidcfg:P:%f,I:%f,D:%f,IMax:%.0f,OMax:%.0f\r\n", current_cfg.kp, current_cfg.ki, current_cfg.kd, current_cfg.maxsum, current_cfg.maxout);
                    bsp_uart_printf(E_UART_DEBUG, "spd_pidcfg:P:%f,I:%f,D:%f,IMax:%.0f,OMax:%.0f\r\n", speed_cfg.kp, speed_cfg.ki, speed_cfg.kd, speed_cfg.maxsum, speed_cfg.maxout);
                    bsp_uart_printf(E_UART_DEBUG, "pos_pidcfg:P:%f,I:%f,D:%f,IMax:%.0f,OMax:%.0f\r\n", position_cfg.kp, position_cfg.ki, position_cfg.kd, position_cfg.maxsum, position_cfg.maxout);
                }
            }
            else if(memcmp(debug_data, "spd:", 4) == 0)
            {
                if(debug_mode)
                {
                    sscanf(debug_data, "spd:%d",&spd_data);
                    motor_aim.speed = (int16_t)spd_data;
                    motor_aim.mode = speed_loop;
                }
            }
            else if(memcmp(debug_data, "pos:", 4) == 0)
            {
                if(debug_mode)
                {
                    sscanf(debug_data, "pos:%d",&pos_data);
                    motor_aim.position = (int16_t)pos_data;
                    motor_aim.mode = position_loop;
                }
            }
            else if(memcmp(debug_data, "cur:", 4) == 0)
            {
                if(debug_mode)
                {
                    sscanf(debug_data, "cur:%d",&cur_data);
                    motor_aim.current = (int16_t)cur_data;
                    motor_aim.mode = current_loop;
                }
            }
        }
    }
}
