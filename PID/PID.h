#ifndef _PID_H_
#define _PID_H_
#include "sys.h"
#include "set.h"

typedef struct PID_PARAMETER
{
	float Kp;
	float Ki;
	float Kd;
	float error_now;
	float error_last;
	float error_inter;
	short pid_out;
}PID;

typedef struct PID_INCREASE
{
	float error_now;
	float error_next;
	float error_last;
	float increament;
}PID_ADD;



void PID_Control(float current, float expected,PID* motor_type);
void PID_Init(void);

extern PID RM3510_1_PID;
extern PID RM3510_2_PID;
extern PID RM3510_3_PID;
extern PID RM3510_4_PID;

#endif
