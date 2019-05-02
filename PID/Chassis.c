#include "Chassis.h"

Chassis_Motor_PID_Expect    RM3510_PID_Expect;
Chassis_Motor_Actual_Speed  RM3510_Actual_Speedt;
ChassisSpeed_Ref_t          Chassis_Speed_Ref;

void Get_PID_Expect(short speed)
{
	RM3510_PID_Expect.Chassis_Motor_PID_Expect_1 = -speed;  //前轮
	RM3510_PID_Expect.Chassis_Motor_PID_Expect_2 =  speed;  //前轮
	RM3510_PID_Expect.Chassis_Motor_PID_Expect_3 = -speed;  //后轮
	RM3510_PID_Expect.Chassis_Motor_PID_Expect_4 =  speed;  //后轮
}

void Get_PID_Expect_qian(short speed)
{
	RM3510_PID_Expect.Chassis_Motor_PID_Expect_1 = -speed;  //前轮
	RM3510_PID_Expect.Chassis_Motor_PID_Expect_2 =  speed;  //前轮
	
}

void Get_PID_Expect_hou(short speed)
{
	
	RM3510_PID_Expect.Chassis_Motor_PID_Expect_3 = -speed;  //后轮
	RM3510_PID_Expect.Chassis_Motor_PID_Expect_4 =  speed;  //后轮
}

void Chassis_Speed_Ref_Init(void)
{
	Chassis_Speed_Ref.forward_back_ref=0;
	Chassis_Speed_Ref.left_right_ref=0;
	Chassis_Speed_Ref.rotate_ref=0;
}

void PID_Expect(void)                            //得到底盘电机所期望的速度
{
	
	RM3510_PID_Expect.Chassis_Motor_PID_Expect_1 =- Chassis_Speed_Ref.forward_back_ref -
																										 Chassis_Speed_Ref.left_right_ref + Chassis_Speed_Ref.rotate_ref;
	
	RM3510_PID_Expect.Chassis_Motor_PID_Expect_2 = Chassis_Speed_Ref.forward_back_ref -
																										 Chassis_Speed_Ref.left_right_ref + Chassis_Speed_Ref.rotate_ref;
	
	RM3510_PID_Expect.Chassis_Motor_PID_Expect_4 = Chassis_Speed_Ref.forward_back_ref +
	                                                   Chassis_Speed_Ref.left_right_ref + Chassis_Speed_Ref.rotate_ref;
	
	RM3510_PID_Expect.Chassis_Motor_PID_Expect_3 = -Chassis_Speed_Ref.forward_back_ref +
																										 Chassis_Speed_Ref.left_right_ref + Chassis_Speed_Ref.rotate_ref;
																										 							
}
