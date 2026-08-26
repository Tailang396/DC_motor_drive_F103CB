//
// Created by asus on 2025/3/31.
//

#ifndef C8T6_APP_MOTOR_H
#define C8T6_APP_MOTOR_H
#include "motorCtrl.h"
#include "PID.h"

extern motor_rec_Typedef motor_aim;
extern PID_TypeDef pid_current, pid_speed, pid_position;
extern pid_cfg_Typedef current_cfg,position_cfg,speed_cfg;

void app_pid_init(void);
void app_readflash_pidcfg(void);
void app_tim_current(void);

#endif //C8T6_APP_MOTOR_H
