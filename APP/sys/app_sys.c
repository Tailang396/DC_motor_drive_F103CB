//
// Created by ge147 on 2025/3/28.
//

#include "app_sys.h"
#include "bsp_def.h"
#include "motorCtrl.h"
#include "app_motor.h"
#include "app_message.h"
#include "app_command.h"

extern void uart_callback(bsp_uart_e e, uint8_t *s, uint16_t l);
extern void bsp_rs485_callback(bsp_uart_e e, uint8_t *s, uint16_t l);
extern void bsp_can_mode_callback(bsp_can_msg_t *msg);
extern void bsp_can_ctrl_callback(bsp_can_msg_t *msg);

static uint8_t id_bit0,id_bit1,id_bit2,motor_get_id;
uint8_t motor_id;
uint8_t id_led = 0;

sys_state_e sys_state = sys_err;
void sysInit(void)
{
    bsp_led_set(0, 0, 50);
    motor.id = motor_id_get();
    motor_id = motor.id;
    bsp_uart_init(E_UART_DEBUG, &huart1);
    bsp_uart_set_callback(E_UART_DEBUG, uart_callback);
    bsp_uart_init(E_UART3, &huart3);
    bsp_uart_set_callback(E_UART3, bsp_rs485_callback);
    HAL_TIM_Base_Start_IT(&htim2);
    bsp_rs485_init();
    bsp_adc_dma_init();
    bsp_flash_init();
    motor_init();
    motor_encoder_init();
    app_readflash_pidcfg();
    app_pid_init();
    bsp_can_init();
    can_tx_id = can_tx_id + motor.id;
    bsp_can_set_callback(0x0A, bsp_can_mode_callback);
    if(motor.id<4)
        bsp_can_set_callback(0xAF, bsp_can_ctrl_callback);
    else
        bsp_can_set_callback(0xAE, bsp_can_ctrl_callback);

    osDelay(10);
}

void app_sysTask(void *argument)
{
    sysInit();
    bsp_led_set(50, 0, 0);
    sys_state = sys_ok;
    for (;;)
    {
        if (!debug_mode)
        {
            for (id_led = 0; id_led < motor.id + 1; id_led++)
            {
                HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
                osDelay(150);
                HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
                osDelay(150);
            }
        } else
            HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
        osDelay(500);
    }
}


uint8_t motor_id_get(void)
{
    motor_get_id = 0;
    id_bit0 = HAL_GPIO_ReadPin(BIT0_GPIO_Port, BIT0_Pin);
    id_bit1 = HAL_GPIO_ReadPin(BIT1_GPIO_Port, BIT1_Pin);
    id_bit2 = HAL_GPIO_ReadPin(BIT2_GPIO_Port, BIT2_Pin);
    motor_get_id = ((id_bit0 << 0) & 0X01) | ((id_bit1 << 1) & 0X02) | ((id_bit2 << 2) & 0X04);
    return motor_get_id;
}

