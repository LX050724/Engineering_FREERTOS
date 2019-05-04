#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"                  // Device header
#include "usart.h"


/*字体颜色*/
#define HMI_Color_Red 63488
#define HMI_Color_Blue 31
#define HMI_Color_Green 2016
#define HMI_Color_Black 0
#define HMI_Color_White 65535

/*HIM屏相关外设初始化，参数：串口号*/
void HMI_Init(UART_HandleTypeDef *huart);

/*打印函数1，参数x，y坐标，一字一个单位，color：字体颜色*/
void HMI_print1(unsigned short x,unsigned short y,unsigned short color);
/*中间使用printf格式输出*/
/*打印函数2*/
void HMI_print2(void);

/*清屏*/
void HMI_cls(void);

/*中断函数*/
void HMI_IT(UART_HandleTypeDef huartx);


extern unsigned char HMI_buff[6];		//串口缓存
extern short HMI_Receive_Data[3];		//接收数据

/*需要printf支持
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((UART4->SR&0X40)==0);		//循环发送,直到发送完毕   
    UART4->DR = (unsigned char) ch;      
	return ch;
}
*/

#ifdef __cplusplus
}
#endif
