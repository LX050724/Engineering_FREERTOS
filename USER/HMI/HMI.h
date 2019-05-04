#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"                  // Device header
#include "usart.h"


/*������ɫ*/
#define HMI_Color_Red 63488
#define HMI_Color_Blue 31
#define HMI_Color_Green 2016
#define HMI_Color_Black 0
#define HMI_Color_White 65535

/*HIM����������ʼ�������������ں�*/
void HMI_Init(UART_HandleTypeDef *huart);

/*��ӡ����1������x��y���꣬һ��һ����λ��color��������ɫ*/
void HMI_print1(unsigned short x,unsigned short y,unsigned short color);
/*�м�ʹ��printf��ʽ���*/
/*��ӡ����2*/
void HMI_print2(void);

/*����*/
void HMI_cls(void);

/*�жϺ���*/
void HMI_IT(UART_HandleTypeDef huartx);


extern unsigned char HMI_buff[6];		//���ڻ���
extern short HMI_Receive_Data[3];		//��������

/*��Ҫprintf֧��
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((UART4->SR&0X40)==0);		//ѭ������,ֱ���������   
    UART4->DR = (unsigned char) ch;      
	return ch;
}
*/

#ifdef __cplusplus
}
#endif
