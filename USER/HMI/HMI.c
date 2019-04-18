#include "HMI.h"

unsigned char HMI_buff[6];
short HMI_Receive_Data[3];
unsigned char HMI_yinhao[1]={0x22};
unsigned char HMI_END[3]={0xff,0xff,0xff};
unsigned short temp;

UART_HandleTypeDef * HMI_huart;

void HMI_Init(UART_HandleTypeDef *huart)
{
	__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);
	HMI_cls();
	HMI_huart=huart;
}

void HMI_print1(unsigned short x,unsigned short y,unsigned short color)
{
	printf("xstr %d,%d,240,320,0,%d,65535,0,0,3,",x*16,y*16,color);
	HAL_UART_Transmit(HMI_huart,HMI_yinhao,1,10);
}

void HMI_print2(void)
{
	HAL_UART_Transmit(HMI_huart,HMI_yinhao,1,10);
	HAL_UART_Transmit(HMI_huart,HMI_END,3,10);
}

void HMI_cls(void)
{
	printf("cls %d",HMI_Color_White);
	HAL_UART_Transmit(HMI_huart,HMI_END,3,10);
}


void HMI_IT(UART_HandleTypeDef huartx)
{
	HMI_buff[temp]=(uint8_t)(huartx.Instance->DR & (uint8_t)0x00FF);
	temp++;
	(void)huartx.Instance->SR;

	if(temp==6)
	{
		HMI_Receive_Data[0]=HMI_buff[0]|(HMI_buff[1]<<8);
		HMI_Receive_Data[1]=HMI_buff[2]|(HMI_buff[3]<<8);
		HMI_Receive_Data[2]=HMI_buff[4]|(HMI_buff[5]<<8);
		temp=0;
		
		/*在此添加代码*/
		
		
		/*************/
	}
}

