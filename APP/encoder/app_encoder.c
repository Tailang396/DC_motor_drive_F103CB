//
// Created by asus on 2025/4/12.
//

#include "app_encoder.h"
#include "motorCtrl.h"

void app_encoder(void *argument)
{
    for (;;)
    {

        motor_encoder_get(&motor);
//        bsp_uart_printf(E_UART_DEBUG, "%d,%d,%d\r\n", motor.current, motor.speed, motor.position);
        osDelay(10);
    }
}