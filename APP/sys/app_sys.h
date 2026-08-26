//
// Created by ge147 on 2025/3/28.
//

#ifndef C8T6_APP_SYS_H
#define C8T6_APP_SYS_H
#include "bsp_def.h"

typedef enum {
    sys_ok,
    sys_err,
} sys_state_e;

extern sys_state_e sys_state;
extern uint8_t motor_id;

void sysInit(void);
void uart_callback(bsp_uart_e e, uint8_t *s, uint16_t l);
uint8_t motor_id_get(void);

#endif //C8T6_APP_SYS_H
