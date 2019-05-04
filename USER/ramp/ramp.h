#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "sys.h"
#include "tim.h"

typedef enum
{
	CHASSIS_RAMP_FB = 0,
	CHASSIS_RAMP_RL = 1,
	FRICTION_RAMP = 2,
	PITCH_RAMP = 3,
	YAW_RAMP = 4,
	ROTATE = 5,
}ramp_t;	 

typedef struct{
	u8 chassis_fb;
	u8 friction;
	u8 pitch;
	u8 yaw;
	u8 chassis_rl;
	u8 rotate;
}Flag_t;

typedef struct{
	u32 chassis_fb;
	u32 friction;
	u32 pitch;
	u32 yaw;
	u32 chassis_rl;
	u32 rotate;
}Value_t;

typedef struct{
	u32 chassis_fb;
	u32 friction;
	u32 pitch;
	u32 yaw_get;
	u32 chassis_rl;
	u32 rotate;
}Get_t;

extern ramp_t ramp;
extern Flag_t flag ;
extern Value_t value ;
extern Get_t get ;
extern ramp_t down_ramp;

float Slope(u32 maximum_value, ramp_t ramp);
void ResetSlope(ramp_t ramp);

#ifdef __cplusplus
}
#endif



