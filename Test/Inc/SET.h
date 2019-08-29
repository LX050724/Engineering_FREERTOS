#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#define ON 1
#define OFF 0

#define USE_HAL_UART_IRQHandler  OFF
#define TEST OFF


#define RC_FRAME_LENGTH                     18u
	 
#define REMOTE_CONTROLLER_STICK_OFFSET      1024u   

#define STICK_TO_CHASSIS_SPEED_REF_FACT     16.0f
#define STICK_TO_ROTATE_SPEED_REF_FACT     	12.0f


//#define Cradle_P_forward TIM1->CCR3=700
//#define Cradle_P_back TIM1->CCR3=1350
//#define Cradle_Y_forward TIM1->CCR4=1150
//#define Cradle_Y_back TIM1->CCR4=2300
#define perspective_forward TIM1->CCR2=750
#define perspective_back TIM1->CCR2=1950

#define Cradle_forward HAL_GPIO_WritePin(IMG_GPIO_Port,IMG_Pin,GPIO_PIN_SET);
											 
#define Cradle_back HAL_GPIO_WritePin(IMG_GPIO_Port,IMG_Pin,GPIO_PIN_RESET);
										




#define KP 2
#define KI 0
#define KD 3

#define Long2 250  //第一排电推杆位置   
#define Long1 850  //第 二 排
#define Long0 0 

#ifdef __cplusplus
}
#endif
