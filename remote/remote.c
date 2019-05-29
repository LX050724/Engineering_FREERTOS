#include "remote.h"

short can_stop[4]= {0,0,0,0};
unsigned char Auto_flag;

uint8_t GetMod=0;//ȡ��չ����־λ 0:��  1:չ��һ��λ��  2:չ������λ��
uint8_t View_mode1 = 0,View_mode2 = 1;

static InputMode_e inputmode = REMOTE_INPUT;   //����ģʽ�趨
RC_Ctl_t RC_CtrlData;   //remote control data

u8 down_play_flag = 1, down_FB_flag = 1,down_RL_flag = 1;
u8 play_flag = 1,FB_flag=1,RL_flag=1,rr_flag=1;
int16_t FB_SD,RL_SD;


unsigned char keyQ_flag=0,keyE_flag=0,keymr_flag=0,Shiftflag=0,ML_Auto_flag=0;

//����ģʽ����

void SetInputMode(Remote *rc)
{
	if(rc->s2 == 1)
	{
		inputmode = REMOTE_INPUT;     //ң��������
	}
	else if(rc->s2 == 3)
	{
		inputmode = KEY_MOUSE_INPUT;  //����������
	}
	else if(rc->s2 == 2)
	{
		HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET); //ֹͣ
		Auto_flag=0;
		inputmode = STOP;
	}
}

InputMode_e GetInputMode()
{
	return inputmode;
}

  /********��갴�����ƹ��̳�*********/
void MouseKeyControlProcess(Mouse *mouse, Key *key)  
{
	static uint16_t forward_back_speed = 0;
	static uint16_t left_right_speed = 0;

	forward_back_speed =  5300;
	left_right_speed = 5300;
	
	//Shift����
	if(key->v & Key_Shift)
	{
		forward_back_speed =  7000;
		left_right_speed = 7000;
	}
	
	if(key->v & Key_W)  // key: w   ǰ��
	{
		ramp = CHASSIS_RAMP_FB;

		down_FB_flag = 0;
		if(FB_flag==0)
		{
			ResetSlope(ramp);
			FB_flag=1;
		}
		Chassis_Speed_Ref.forward_back_ref = forward_back_speed * Slope(70000,ramp);

		FB_SD = Chassis_Speed_Ref.forward_back_ref ;
		play_flag = 0;

	}
	else if(key->v & Key_S) //key: s  ����
	{
		ramp = CHASSIS_RAMP_FB;

		down_FB_flag = 0;
		if(play_flag==0)
		{
			ResetSlope(ramp);
			play_flag=1;
		}
		FB_flag=0;
		Chassis_Speed_Ref.forward_back_ref = -forward_back_speed * Slope(70000,ramp);
	}
	else
	{
		ramp = CHASSIS_RAMP_FB;
		ResetSlope(ramp);
		Chassis_Speed_Ref.forward_back_ref = 0;
	}
	
	if(key->v & Key_D)  // key: d ����
	{
		ramp = CHASSIS_RAMP_RL;

		down_RL_flag = 0;
		if(RL_flag==0)
		{
			ResetSlope(ramp);
			RL_flag=1;
		}

		Chassis_Speed_Ref.left_right_ref = left_right_speed * Slope(70000,ramp);
		rr_flag = 0;
	}
	else if(key->v & Key_A) //key: a ����
	{
		ramp = CHASSIS_RAMP_RL;
		down_RL_flag = 0;

		if(rr_flag==0)
		{
			ResetSlope(ramp);
			rr_flag=1;
		}
		Chassis_Speed_Ref.left_right_ref = -left_right_speed * Slope(70000,ramp);
		RL_SD = Chassis_Speed_Ref.left_right_ref ;
		RL_flag=0;
	}
	else
	{
		ramp = CHASSIS_RAMP_RL;
		down_ramp = CHASSIS_RAMP_RL;
		ResetSlope(ramp);
		Chassis_Speed_Ref.left_right_ref=0;
	}
	
	
	/*************************************************************************************************/
	/*******************************************����ٿ�**********************************************/
	/*************************************************************************************************/

	/***********************************************SHIFT����,ȡ��λ���л�**************************************************/
	if (key->v & Key_Shift)
	{
		if (Shiftflag==0)
		{
			if (GetMod == 1)
			{
				Location_mode_Sent(Long1);
				GetMod = 2;//��Ƕ���λ��
			}
			else if (GetMod == 2)
			{
				Location_mode_Sent(Long2);
				GetMod = 1;//���һ��λ��
			}
		}
		Shiftflag = 1;
	}
	else Shiftflag = 0;
	/********************************************Shift+Ctrl��Ctrl���µ�*****************************************************/
	static uint16_t SCtime = 0,Ctime = 0;
	if(key->v & Key_Ctrl)
	{
		if (Shiftflag == 0)
		{
			Ctime++;
			if(Ctime==140)
			{
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
				Auto_flag = 2;
			}
		}
		else if(Shiftflag==1)
		{
			SCtime++;
			if(SCtime==140)
			{
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
				Auto_flag = 1;
			}
		}
	}
	else
	{
		Ctime = 0;
		SCtime = 0;
	}

	
	/***************************************�Ҽ�����**********************************************************/

	//static uint8_t mrflag  = 0;
	//static uint8_t DC_flag = 0;
	if (mouse->press_r)
	{
//		if(mrflag == 0)
//		{
//			mrflag =1;
//			if (DC_flag == 0)
//			{
//				HAL_GPIO_WritePin(dancang_GPIO_Port,dancang_Pin,GPIO_PIN_RESET);
//				DC_flag = 1;
//			}
//			else
//			{
//				HAL_GPIO_WritePin(dancang_GPIO_Port,dancang_Pin,GPIO_PIN_SET);
//				DC_flag = 0;
//			}
//		}
		
		HAL_GPIO_WritePin(dancang_GPIO_Port,dancang_Pin,GPIO_PIN_RESET);
	}
	else //mrflag = 0;
	{
		HAL_GPIO_WritePin(dancang_GPIO_Port,dancang_Pin,GPIO_PIN_SET);
	}
	
	
	/******************************************E����*******************************************************/

	static uint8_t Eflag = 0;
	static uint8_t rescueflag = 0;
	if (key->v & Key_E)
	{
		if (Eflag == 0)
		{
			Eflag = 1;
			if (GetMod == 0)//����ģʽ����Ԯ
			{
				//Rescue;
				if(rescueflag == 0)
				{
					HAL_GPIO_WritePin(rescue_GPIO_Port,rescue_Pin,GPIO_PIN_SET);
					rescueflag = 1;
				}
				else
				{
					HAL_GPIO_WritePin(rescue_GPIO_Port,rescue_Pin,GPIO_PIN_RESET);
					rescueflag = 0;
				}
			}
			else
			{
				Location_mode_Sent(Long0);
				HAL_GPIO_WritePin(Q3_GPIO_Port, Q3_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Q1_GPIO_Port, Q1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Q2_GPIO_Port, Q2_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Q4_GPIO_Port, Q4_Pin, GPIO_PIN_SET);
				//��̨ת��󷽣�ͼ��ת��ǰ��
				Cradle_back;           //ҡ�����
				perspective_forward;   //͸����ǰ
		   	View_mode2 = 1;
				View_mode1 = 0;
				GetMod = 0;//�����־
			}
		}
	}
	else Eflag = 0;
	
	/********************************************Q���ӽǣ�SQ����*****************************************************/
	static uint8_t Qflag = 0;
	if (key->v & Key_Q)
	{
		if (Qflag==0)
		{
			if (Shiftflag==1&&GetMod==0)
			{
				GetMod = 1;//���չ��
				HAL_GPIO_WritePin(Q1_GPIO_Port, Q1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Q2_GPIO_Port, Q2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Q4_GPIO_Port, Q4_Pin, GPIO_PIN_RESET);
				Location_mode_Sent(Long2);
				//ͼ��ת��󷽣���̨��ȡ��
				perspective_back;
				Cradle_forward;
				View_mode1 = 1;
				View_mode2 = 0;
				//�վ�Ԯ
				HAL_GPIO_WritePin(rescue_GPIO_Port,rescue_Pin,GPIO_PIN_SET);
				rescueflag = 0;
			}
			else if(Shiftflag==0)
			{
				if (GetMod==0)
				{
					//����ģʽ��תͼ��
					if (View_mode1 == 0)
					{
						//ͼ����ǰ��
						perspective_back;
						View_mode1 = 1;
					}
					else
					{
						//ͼ������
						perspective_forward;
						View_mode1 = 0;
					}
				}
				else
				{
					//ȡ��ģʽת��̨
					if (View_mode2 ==0)
					{
						//��̨����
						Cradle_back;
						View_mode2 =1;
					}
					else
					{
						//��̨��ǰ��
						Cradle_forward;
						View_mode2 = 0;
					}
				}
			}
			Qflag = 1;
		}
	} 
	else Qflag = 0;
	
	/****************************************���  ��ȡ����**********************************************************/

	if ((mouse->press_l) || (ML_Auto_flag == 1))
	{
		if(GetMod!=0)
		{
			if (ML_Auto_flag == 0)
			{
				ML_Auto_flag = 1;
				HAL_GPIO_WritePin(Q3_GPIO_Port, Q3_Pin, GPIO_PIN_RESET);//��ȥ
				TIM7->CNT = 0;
			}
			if (ML_Auto_flag == 1)
			{
				if (GetMod == 2)
				{
					if ((TIM7->CNT) > 4000)HAL_GPIO_WritePin(Q4_GPIO_Port, Q4_Pin, GPIO_PIN_SET);//��
					if ((TIM7->CNT) > 6000)HAL_GPIO_WritePin(Q3_GPIO_Port, Q3_Pin, GPIO_PIN_SET);//̧
					if ((TIM7->CNT) > 16000)HAL_GPIO_WritePin(Q3_GPIO_Port, Q3_Pin, GPIO_PIN_RESET); //��
					if ((TIM7->CNT) > 17500)HAL_GPIO_WritePin(Q4_GPIO_Port, Q4_Pin, GPIO_PIN_RESET); //�ɼ���
					if ((TIM7->CNT) > 20000)
					{
						HAL_GPIO_WritePin(Q3_GPIO_Port, Q3_Pin, GPIO_PIN_SET);//̧
						ML_Auto_flag = 0;
					}
				}
				else if (GetMod == 1)
				{
					if ((TIM7->CNT) > 4000)HAL_GPIO_WritePin(Q4_GPIO_Port, Q4_Pin, GPIO_PIN_SET);//��
					if ((TIM7->CNT) > 6000)HAL_GPIO_WritePin(Q3_GPIO_Port, Q3_Pin, GPIO_PIN_SET);//̧
					if ((TIM7->CNT) > 16000)Location_mode_Sent(630);
					if ((TIM7->CNT) > 25000)HAL_GPIO_WritePin(Q3_GPIO_Port, Q3_Pin, GPIO_PIN_RESET); //��
					if ((TIM7->CNT) > 26000)HAL_GPIO_WritePin(Q4_GPIO_Port, Q4_Pin, GPIO_PIN_RESET); //�ɼ���
					if ((TIM7->CNT) > 29000)
					{
						HAL_GPIO_WritePin(Q3_GPIO_Port, Q3_Pin, GPIO_PIN_SET);//̧
						Location_mode_Sent(Long2);
						ML_Auto_flag = 0;
					}
				}
			}
		}
	}
	
	
	VAL_LIMIT(mouse->x, -120, 120); 
	VAL_LIMIT(mouse->y, -120, 120); 
	Chassis_Speed_Ref.rotate_ref=mouse->x*-80;
	/****************************************���������������ٶ�����**********************************/
	
//	if((View_mode1 == 1)&&(View_mode2 == 0))//ͼ��������̨��ǰ
//	{
//		Chassis_Speed_Ref.left_right_ref = -Chassis_Speed_Ref.left_right_ref;
//		Chassis_Speed_Ref.rotate_ref     = -Chassis_Speed_Ref.rotate_ref;
//	}
	
	if((View_mode1 == 1)&&(View_mode2 == 1))//ͼ��������̨����
	{
		Chassis_Speed_Ref.forward_back_ref = -Chassis_Speed_Ref.forward_back_ref;
		Chassis_Speed_Ref.rotate_ref = -Chassis_Speed_Ref.rotate_ref;
	}
	
	if(GetMod != 0)//�������ȡ���������ٶ�
	{
		VAL_LIMIT(Chassis_Speed_Ref.forward_back_ref,-1500,1500);
		VAL_LIMIT(Chassis_Speed_Ref.left_right_ref,-2500,2500);
	}
	
	if(key->v & Key_R)
	{
		if(Shiftflag)Chassis_Speed_Ref.rotate_ref += 10000;
		else Chassis_Speed_Ref.rotate_ref += 6000;
	}
	if(key->v & Key_F)
	{
		if(Shiftflag)Chassis_Speed_Ref.rotate_ref += -10000;
		else Chassis_Speed_Ref.rotate_ref += -6000;
	}
	
	
	static uint16_t Rest_time = 0;
	if(key->v &(Key_Shift|Key_Z))
	{
		Rest_time++;
		if(Rest_time >= 350)
			HAL_NVIC_SystemReset();
	}else Rest_time = 0;
}

void RemoteShootControl(int8_t s1)
{
	switch(s1)
	{
		case 1://�ϵ�ģʽ
		{
			Auto_flag=1;
			break;
		}
		case 2://�µ�ģʽ
		{
			Auto_flag=2;
			break;
		}
		case 3://����
		{
			
			break;
		}
	}
}

//ң��������ģʽ����
void RemoteControlProcess(Remote *rc)
{

	Chassis_Speed_Ref.forward_back_ref = (RC_CtrlData.rc.ch1 - (int16_t)REMOTE_CONTROLLER_STICK_OFFSET) * STICK_TO_CHASSIS_SPEED_REF_FACT;
	Chassis_Speed_Ref.left_right_ref   = (rc->ch0 - (int16_t)REMOTE_CONTROLLER_STICK_OFFSET) * STICK_TO_CHASSIS_SPEED_REF_FACT;

	Chassis_Speed_Ref.rotate_ref=-((rc->ch2 - (int16_t)REMOTE_CONTROLLER_STICK_OFFSET)*STICK_TO_ROTATE_SPEED_REF_FACT);


	/* not used to control, just as a flag */

	RemoteShootControl(rc->s1);
}

void Remote_Rx(unsigned char *RxMsg,RC_Ctl_t *RCData)
{
	RCData->rc.ch0 = ((int16_t)RxMsg[0] | ((int16_t)RxMsg[1] << 8)) & 0x07FF;
	RCData->rc.ch1 = (((int16_t)RxMsg[1] >> 3) | ((int16_t)RxMsg[2] << 5)) & 0x07FF;
	RCData->rc.ch2 = (((int16_t)RxMsg[2] >> 6) | ((int16_t)RxMsg[3] << 2) |
	                      ((int16_t)RxMsg[4] << 10)) & 0x07FF;
	RCData->rc.ch3 = (((int16_t)RxMsg[4] >> 1) | ((int16_t)RxMsg[5]<<7)) & 0x07FF;

	RCData->rc.s1 = ((RxMsg[5] >> 4) & 0x000C) >> 2;
	RCData->rc.s2 = ((RxMsg[5] >> 4) & 0x0003);

	RCData->mouse.x = ((int16_t)RxMsg[6]) | ((int16_t)RxMsg[7] << 8);
	RCData->mouse.y = ((int16_t)RxMsg[8]) | ((int16_t)RxMsg[9] << 8);
	RCData->mouse.z = ((int16_t)RxMsg[10]) | ((int16_t)RxMsg[11] << 8);

	RCData->mouse.press_l = RxMsg[12];
	RCData->mouse.press_r = RxMsg[13];

	RC_CtrlData.key.v = ((int16_t)RxMsg[14]|(int16_t)RxMsg[15]<<8);

	SetInputMode(&(RCData->rc));

	switch(GetInputMode())
	{
		case REMOTE_INPUT:
		{
			//ң��������ģʽ
			RemoteControlProcess(&(RCData->rc));
		}
		break;

		case KEY_MOUSE_INPUT:
		{
			//�������ģʽ
			MouseKeyControlProcess(&(RCData->mouse),&(RCData->key));
		}
		break;
		
		case STOP:
		{
			Chassis_Speed_Ref_Init();
			RM3510_CAN_Send(can_stop);
		}
		break;
	}
}


	
	
