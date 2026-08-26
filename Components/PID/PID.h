#ifndef __PID_H
#define __PID_H
#include "main.h"

typedef struct
{
	float kp,ki,kd;
	float Aim,Real;
	float err;
	float last1_err;
	float last2_err;
	float sum_err;
	float max_sum;
	float Pout,Iout,Dout;
	float PIDout;	
	float max_out;
	uint8_t active;
}PID_TypeDef;

void PID_Init(PID_TypeDef *pid,float Kp, float Ki, float Kd, float maxsum, float maxout);
void PID_Calculate(PID_TypeDef *pid, float aim, float real);

#endif
