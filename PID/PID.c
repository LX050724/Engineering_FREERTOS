#include "PID.h"

PID RM3510_1_PID;
PID RM3510_2_PID;
PID RM3510_3_PID;
PID RM3510_4_PID;

void PID_Init()
{
	RM3510_1_PID.error_inter = 0;
	RM3510_1_PID.error_last = 0;
	RM3510_1_PID.error_now = 0;
	RM3510_1_PID.Kd = KD;
	RM3510_1_PID.Ki = KI;
	RM3510_1_PID.Kp = KP;
	
	RM3510_2_PID.error_inter = 0;
	RM3510_2_PID.error_last = 0;
	RM3510_2_PID.error_now = 0;
	RM3510_2_PID.Kd = KD;
	RM3510_2_PID.Ki = KI;
	RM3510_2_PID.Kp = KP;

	RM3510_3_PID.error_inter = 0;
	RM3510_3_PID.error_last = 0;
	RM3510_3_PID.error_now = 0;
	RM3510_3_PID.Kd = KD;
	RM3510_3_PID.Ki = KI;
	RM3510_3_PID.Kp = KP;
	
	RM3510_4_PID.error_inter = 0;
	RM3510_4_PID.error_last = 0;
	RM3510_4_PID.error_now = 0;
	RM3510_4_PID.Kd = KD;
	RM3510_4_PID.Ki = KI;
	RM3510_4_PID.Kp = KP;
}

void PID_Control(float current/*实际值*/, float expected/*期望值*/, PID* motor_type/*参数*/)
{

	motor_type->error_last=motor_type->error_now;
	motor_type->error_now = expected - current;
	motor_type->error_inter += motor_type->error_now;
	
	if(motor_type->error_inter>10000)
		  motor_type->error_inter = 10000;
	if(motor_type->error_inter<-10000)
		  motor_type->error_inter = -10000;
	motor_type->pid_out = motor_type->Kp * motor_type->error_now + motor_type->Ki * motor_type->error_inter +	
													motor_type->Kd * (motor_type->error_now-motor_type->error_last);
}
