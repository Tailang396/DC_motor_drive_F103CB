//
// Created by asus on 2025/3/31.
//

#include "app_motor.h"
#include "bsp_adc.h"
#include "app_sys.h"
#include "app_message.h"
#include "app_command.h"


PID_TypeDef pid_current, pid_speed, pid_position;

/**
 * @brief PID参数
 * current_cfg:0.0015f,0.600f,0.00f,10000,4500
 * position_cfg:0.125f,0.0f,0.18f,0,55
 * speed_cfg:45.125f,15.25f,0.00f,500,4500
 */

pid_cfg_Typedef current_cfg, position_cfg, speed_cfg;
//pid_cfg_Typedef current_cfg = {0.0015f, 0.600f, 0.00f, 10000, 4500};
//pid_cfg_Typedef position_cfg = {0.125f, 0.0f, 0.18f, 0, 55};
//pid_cfg_Typedef speed_cfg = {45.125f, 15.25f, 0.00f, 500, 4500};

motor_rec_Typedef motor_aim = {disable_loop, 0, 0, 0, 0};

float filter_current = 0.0f;
float filter_output = 0.0f;

void app_readflash_pidcfg(void)
{
    bsp_flash_read("cur_pidcfg", &current_cfg,sizeof(current_cfg));
    bsp_flash_read("pos_pidcfg", &position_cfg,sizeof(position_cfg));
    bsp_flash_read("spd_pidcfg", &speed_cfg,sizeof(speed_cfg));
}
void app_pid_init(void)
{
    PID_Init(&pid_speed, speed_cfg.kp, speed_cfg.ki, speed_cfg.kd, speed_cfg.maxsum, speed_cfg.maxout);
    PID_Init(&pid_position, position_cfg.kp,  position_cfg.ki, position_cfg.kd, position_cfg.maxsum, position_cfg.maxout);
    PID_Init(&pid_current, current_cfg.kp, current_cfg.ki, current_cfg.kd, current_cfg.maxsum, current_cfg.maxout);
}

void app_motorCtrl(void *argument)
{
    for (;;)
    {
        if (sys_state == sys_ok)
        {
            if (motor_aim.rec_id == motor.id)
            {
                motor.mode = motor_aim.mode;
                if (motor.mode == speed_loop)
                {
                    PID_Calculate(&pid_speed, motor_aim.speed, motor.speed);
                    motor_load((int16_t) pid_speed.PIDout);
                    bsp_led_set(0, 50, 0);
                    if(debug_mode)
                        bsp_uart_printf(E_UART_DEBUG, "speed:%d,%d,%.2f\r\n", motor.speed, motor_aim.speed,pid_speed.PIDout);
                } else if (motor.mode == position_loop)
                {
                    PID_Calculate(&pid_position, (float) motor_aim.position, (float) motor.position);
                    PID_Calculate(&pid_speed, pid_position.PIDout, motor.speed);
                    motor_load((int16_t) pid_speed.PIDout);
                    bsp_led_set(0, 0, 50);
                    if(debug_mode)
                        bsp_uart_printf(E_UART_DEBUG, "position:%d,%d,%.2f\r\n", motor.position, motor_aim.position,pid_position.PIDout);
                } else if (motor.mode == no_loop)
                {
                    motor_load(motor_aim.pwm);
                    bsp_led_set(0, 50, 50);
                } else if (motor.mode == disable_loop)
                {
                    motor_load(0);
                    enc_position = 0;
                    enc_speed = 0;
                    motor.position = 0;
                    pid_speed.sum_err = 0;
                    bsp_led_set(50, 0, 0);
                } else if(motor.mode == current_loop)
                {
                    bsp_led_set(50, 0, 50);
                    if(debug_mode)
                        bsp_uart_printf(E_UART_DEBUG, "current:%d,%d,%.2f\r\n", motor.current, motor_aim.current,pid_current.PIDout);
                }
                if(rs485_rx_flag && tx_flag)
                {
                    message_rs485_send(&motor);
                    rs485_rx_flag = 0x00;
                }
                if(can_rx_flag && tx_flag)
                {
                    message_can_send(&motor);
                    can_rx_flag = 0x00;
                }
            }
        }
        osDelay(10);
    }
}

//TIM2 中断回调函数 频率2kHZ 500us
void app_tim_current(void)
{
    if (sys_state == sys_ok)
    {
        filter_current = filter_current * 0.98f + (float) bsp_adc_dma_read() * 0.02f;
        motor.current = (int16_t) filter_current;
        if(motor.mode == current_loop)
        {
            PID_Calculate(&pid_current, motor_aim.current, motor.current);
            filter_output = filter_output * 0.98f + pid_current.PIDout * 0.02f;
            motor_load((int16_t) pid_current.PIDout);
        }
    }
}

void app_Step(void *argument)
{
    for (;;)
    {
        if(sys_state == sys_ok)
        {
            if (step_mode)
            {
                motor_aim.position = -motor_aim.position;
                motor_aim.speed = -motor_aim.speed;
                motor_aim.current = -motor_aim.current;
            }
        }
        osDelay(2000);
    }
}