//
// Created by asus on 2025/4/12.
//

#include "bsp_rs485.h"

#define RS485_DIR_TX  HAL_GPIO_WritePin(USART2_DIR_GPIO_Port,USART2_DIR_Pin, GPIO_PIN_SET)
#define RS485_DIR_RX  HAL_GPIO_WritePin(USART2_DIR_GPIO_Port,USART2_DIR_Pin, GPIO_PIN_RESET)

extern void bsp_rs485_callback(bsp_uart_e e, uint8_t *s, uint16_t l);

void bsp_rs485_init(void)
{
    bsp_uart_init(E_RS485, &huart2);
    bsp_uart_set_callback(E_RS485, bsp_rs485_callback);
}

void bsp_rs485_send(uint8_t *data, uint16_t len)
{
    RS485_DIR_TX;
    bsp_uart_send(E_RS485, data, len);
    RS485_DIR_RX;
}
