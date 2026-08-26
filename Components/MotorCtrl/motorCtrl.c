//
// Created by asus on 2025/3/31.
//

#include "motorCtrl.h"
#include "PID.h"

int16_t enc_speed = 0;
int16_t enc_position = 0;
motor_Typedef motor = {0, 0, 0, 0, 0};
void motor_init(void)
{
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 0);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, 0);
}

void motor_load(int16_t pwm)
{
    if (pwm > 0)
    {
        __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, pwm);
        __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, 0);
    }
    else if (pwm < 0)
    {
        __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 0);
        __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, -pwm);
    }
    else if (pwm == 0)
    {
        __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 0);
        __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, 0);
    }
}

void motor_encoder_init(void)
{
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_2);
    __HAL_TIM_SET_COUNTER(&htim3, 30000);
}

void motor_encoder_get(motor_Typedef *motor)
{
    enc_speed = 30000 - __HAL_TIM_GET_COUNTER(&htim3);
    __HAL_TIM_SET_COUNTER(&htim3, 30000);
    enc_position += enc_speed;
    if(enc_position > 32767) enc_position = -32768;
    else if(enc_position < -32768) enc_position = 32767;
    motor->speed = enc_speed;
    motor->position = enc_position;
}
