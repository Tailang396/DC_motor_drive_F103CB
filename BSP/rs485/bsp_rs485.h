//
// Created by asus on 2025/4/12.
//

#ifndef CBT6_BSP_RS485_H
#define CBT6_BSP_RS485_H
#include "bsp_def.h"


void bsp_rs485_init(void);
void bsp_rs485_send(uint8_t *data, uint16_t len);

#endif //CBT6_BSP_RS485_H
