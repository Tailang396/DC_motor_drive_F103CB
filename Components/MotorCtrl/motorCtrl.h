//
// Created by asus on 2025/3/31.
//

#ifndef C8T6_MOTORCTRL_H
#define C8T6_MOTORCTRL_H
#include "bsp_def.h"

extern int16_t enc_position;
extern int16_t enc_speed;

typedef enum
{
    disable_loop = 8, //disable loop
    no_loop, //-4499~4499
    current_loop, //-2048~2047
    speed_loop, //-60~60
    position_loop, //-32768~32767
}motor_mode_e;

typedef struct
{
    uint8_t id;
    int16_t speed;
    int16_t position;
    int16_t current;
    uint8_t Temperature;
    motor_mode_e mode;
}motor_Typedef;

typedef struct
{
    motor_mode_e mode;
    int16_t speed;
    int16_t position;
    int16_t current;
    int16_t pwm;
    uint8_t rec_id;
}motor_rec_Typedef;

typedef struct
{
    float kp, ki, kd;
    float maxsum, maxout;
}pid_cfg_Typedef;

extern motor_Typedef motor;

void motor_init(void);
void motor_load(int16_t);
void motor_encoder_init(void);
void motor_encoder_get(motor_Typedef *motor);


#endif //C8T6_MOTORCTRL_H
