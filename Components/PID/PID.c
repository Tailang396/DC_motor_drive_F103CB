#include "main.h"
#include "PID.h"


void PID_Init(PID_TypeDef *pid,float Kp, float Ki, float Kd, float maxsum, float maxout)
{	
	pid->kp=Kp;
	pid->ki=Ki;
	pid->kd=Kd;
	pid->max_sum=maxsum;
	pid->max_out=maxout;
  pid->PIDout = 0;
  pid->sum_err = 0;
  pid->err = 0;
	pid->last1_err=0;
	pid->last2_err=0;
	pid->active =1;
}

void PID_Calculate(PID_TypeDef *pid, float aim, float real)
{
	if(pid->active)
	{
		pid->Aim = aim;
		pid->Real = real;
		pid->err = pid->Aim - pid->Real;
		pid->sum_err = pid->sum_err + pid->err;
		
		if(pid->sum_err >= pid->max_sum) pid->sum_err = pid->max_sum;
		else if(pid->sum_err <= -pid->max_sum) pid->sum_err = -pid->max_sum;
		
		pid->last2_err = pid->last1_err;
		pid->last1_err = pid->err;
		
		pid->Pout = pid->kp * pid->err;
		pid->Iout = pid->ki * pid->sum_err;
		pid->Dout = pid->kd * (pid->err - 2*pid->last1_err + pid->last2_err);
		pid->PIDout = pid->Pout + pid->Iout + pid->Dout;
		
		if(pid->PIDout >= pid->max_out) pid->PIDout = pid->max_out;	
		else if(pid->PIDout <= -pid->max_out) pid->PIDout = -pid->max_out;
	}
	else
  {
      pid->sum_err=0;pid->last1_err=0;pid->last2_err=0;pid->err=0;
  }
}
