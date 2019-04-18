#include <ROBOMODULE.h>


const uint8_t Reset[10]		 		 ={0x48,0x00,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55};
const uint8_t Locationmodel[10]={0x48,0x01,0x05,0x55,0x55,0x55,0x55,0x55,0x55,0x55};

void ROBOMODULE_Reset(void)
{
	HAL_UART_Transmit_DMA(&huart6,(uint8_t*)Reset,10);
	HAL_Delay(500);
	HAL_UART_Transmit_DMA(&huart6,(uint8_t*)Locationmodel,10);
	HAL_Delay(1000);
}

void Location_mode_Sent(int32_t Location)
{
	static unsigned char RO_Data[10]={0x48,0x06,0x13,0x88,0x07,0xd0,0x00,0x00,0x00,0x00};
	RO_Data[6]=((Location>>24)&0xff);
	RO_Data[7]=((Location>>16)&0xff);
	RO_Data[8]=((Location>>8)&0xff);
	RO_Data[9]=(Location&0xff);
	HAL_UART_Transmit_DMA(&huart6,RO_Data,10);
}

void ALL_Reset(void)
{
	/*À¶×©³õÊ¼»¯*/
	ROBOMODULE_Reset();
	Location_mode_Sent(-2000);
	HAL_Delay(1000);
	HAL_Delay(1000);
	HAL_Delay(1000);
	ROBOMODULE_Reset();
}
