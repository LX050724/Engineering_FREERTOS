#ifndef _CHASSIS_H_
#define _CHASSIS_H_
#include "sys.h"
#include "PID.h"
#include "remote.h"

typedef __packed struct
{
     short forward_back_ref;
     short left_right_ref;
     short rotate_ref;
}ChassisSpeed_Ref_t;

typedef  __packed struct
{
     short Chassis_Motor_PID_Expect_1;
     short Chassis_Motor_PID_Expect_2;
     short Chassis_Motor_PID_Expect_3;
		 short Chassis_Motor_PID_Expect_4;
}Chassis_Motor_PID_Expect;

typedef __packed struct
{
     short Chassis_Motor_Actual_Speed_1;
     short Chassis_Motor_Actual_Speed_2;
     short Chassis_Motor_Actual_Speed_3;
		 short Chassis_Motor_Actual_Speed_4;
}Chassis_Motor_Actual_Speed;

void PID_Expect(void);
void Chassis_Speed_Ref_Init(void);
void Get_PID_Expect(short speed);
void Get_PID_Expect_qian(short speed);
void Get_PID_Expect_hou(short speed);

extern Chassis_Motor_PID_Expect    RM3510_PID_Expect;
extern Chassis_Motor_Actual_Speed  RM3510_Actual_Speedt;
extern ChassisSpeed_Ref_t          Chassis_Speed_Ref;

#endif
