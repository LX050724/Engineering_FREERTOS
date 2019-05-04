                                  /*斜坡函数，目的是为了让速度缓慢的增加，使功率更小*/
#include "ramp.h"
#include "sys.h"
#include "tim.h"

Flag_t flag_t = {0, 0, 0, 0, 0, 0};
Value_t value = {0, 0, 0, 0, 0, 0};
Get_t get     = {0, 0, 0, 0, 0, 0};

ramp_t ramp;
ramp_t down_ramp;

float Slope(u32 maximum_value, ramp_t ramp)//斜坡函数，让缓慢增速
{
	switch(ramp)
	{
		case CHASSIS_RAMP_FB:
		{
			value.chassis_fb = maximum_value;
			
			if(!flag_t.chassis_fb)
			{
				get.chassis_fb = Get_Time5_Micros();
				flag_t.chassis_fb = 1;
			}
				
			if(Get_Time5_Micros() > (value.chassis_fb + get.chassis_fb))
				return 1;
			else
				return ((Get_Time5_Micros() - get.chassis_fb) / (value.chassis_fb * 1.0f));
		} 
		
		case CHASSIS_RAMP_RL:
		{
			value.chassis_rl = maximum_value;
			
			if(!flag_t.chassis_rl)
			{
				get.chassis_rl = Get_Time5_Micros();
				flag_t.chassis_rl = 1;
			}
				
			if(Get_Time5_Micros() > (value.chassis_rl + get.chassis_rl))
				return 1;
			else
				return ((Get_Time5_Micros() - get.chassis_rl) / (value.chassis_rl * 1.0f));
		}
		
		case FRICTION_RAMP:
		{
			value.friction = maximum_value;
			
			if(!flag_t.friction)
			{
				get.friction = Get_Time5_Micros();
				flag_t.friction = 1;
			}
				
			if(Get_Time5_Micros() > (value.friction + get.friction))
				return 1;
			else
				return ((Get_Time5_Micros() - get.friction) / (value.friction * 1.0f));
		} 
		
		case PITCH_RAMP:
		{
			value.pitch = maximum_value;
			
			if(!flag_t.pitch)
			{
				get.pitch = Get_Time5_Micros();
				flag_t.pitch = 1;
			}
				
			if(Get_Time5_Micros() > (value.pitch + get.pitch))
				return 1;
			else
				return ((Get_Time5_Micros() - get.pitch) / (value.pitch * 1.0f));
		} 
		
		case YAW_RAMP:
		{
			value.yaw = maximum_value;
			
			if(!flag_t.yaw)
			{
				get.yaw_get = Get_Time5_Micros();
				flag_t.yaw = 1;
			}
				
			if(Get_Time5_Micros() > (value.yaw + get.yaw_get))
				return 1;
			else
				return ((Get_Time5_Micros() - get.yaw_get) / (value.yaw * 1.0f));
		} 
		
		case ROTATE:
		{
			value.rotate = maximum_value;
			
			if(!flag_t.rotate)
			{
				get.rotate = Get_Time5_Micros();
				flag_t.rotate = 1;
			}
				
			if(Get_Time5_Micros() > (value.rotate + get.rotate))
				return 1;
			else
				return ((Get_Time5_Micros() - get.rotate) / (value.rotate * 1.0f));
		} 

		default:
			return 0;
	}
}

void ResetSlope(ramp_t ramp)//释放只是为了让下次在进斜坡
{
	switch(ramp)
	{
		
		case CHASSIS_RAMP_FB:
		{
			flag_t.chassis_fb = 0;
		}break;
		
		case CHASSIS_RAMP_RL:
		{
			flag_t.chassis_rl = 0;
		}break;
		
		case FRICTION_RAMP:
		{
			flag_t.friction = 0;
		}break;
		
		case PITCH_RAMP:
		{
			flag_t.pitch = 0;
		}break;
		
		case YAW_RAMP:
		{
			flag_t.yaw = 0;
		}break;
		
		case ROTATE:
		{
			flag_t.rotate = 0;
		}break;
		
	}
}
