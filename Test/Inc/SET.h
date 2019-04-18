#ifndef _SET_H_
#define _SET_H_

#define ON 1
#define OFF 0

#define USE_HAL_UART_IRQHandler  OFF
#define TEST OFF


#define RC_FRAME_LENGTH                     18u
	 
#define REMOTE_CONTROLLER_STICK_OFFSET      1024u   

#define STICK_TO_CHASSIS_SPEED_REF_FACT     16.0f
#define STICK_TO_ROTATE_SPEED_REF_FACT     	12.0f

//×ó¾ÈÔ®
#define steering_gear_1_0 2600
#define steering_gear_1_1 1500

//ÓÒ¾ÈÔ®
#define steering_gear_2_0 1350
#define steering_gear_2_1 500

//µ¯²Õ
#define Cartridge if(TIM1->CCR1==2500)TIM1->CCR1=900;\
									else TIM1->CCR1=2500;

#define Cradle_P_forward TIM1->CCR3=800
#define Cradle_P_back TIM1->CCR3=1500
#define Cradle_Y_forward TIM1->CCR4=2500
#define Cradle_Y_back TIM1->CCR4=500
#define perspective_forward TIM1->CCR2=650
#define perspective_back TIM1->CCR2=2440

#define Cradle_forward Cradle_P_forward;\
											 Cradle_Y_forward;
											 
#define Cradle_back Cradle_P_back;\
										Cradle_Y_back;
										
#define Rescue TIM9->CCR1=(TIM9->CCR1==2400 ? 500:2400);\
							 TIM9->CCR2=(TIM9->CCR2==500  ? 2400:500);


#define KP 2
#define KI 0
#define KD 3

#define Long2 200

#define Long1 850
#define Long0 0

#endif
