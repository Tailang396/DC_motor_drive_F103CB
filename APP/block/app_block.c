//
// Created by asus on 2025/4/14.
//

#include "app_block.h"
#include "stdlib.h"
#include "app_motor.h"
#include "PID.h"

uint32_t block_counter = 0;
uint8_t block_flag = 0x00;
static motor_mode_e motor_mode = disable_loop;
void app_block(void *argument)
{
    for (;;)
    {
        if(abs(motor.current) > 2000 && abs(motor.speed) < 5)
        {
            if(block_counter > 100)
            {
                block_flag = 0xFF;
                bsp_led_set(255,255,255);
                motor_mode = motor_aim.mode;
                motor_aim.mode = disable_loop;
                block_counter = 0;
                osDelay(3000);
                block_flag = 0X00;
                motor_aim.mode = motor_mode;
            }
            block_counter ++;
        }
        else
            block_counter = 0;
        osDelay(10);
    }
}
