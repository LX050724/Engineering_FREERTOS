#include "FreeRTOSTask.h"
unsigned char i;

void StartTask(void)
{
	/*������ֵ�ź���*/
	Remote_Semaphore= xSemaphoreCreateBinary();
	
	/*��������*/
	
	/*�����ʼ��*/
	CanFilter_Init(&hcan1);
	HAL_CAN_Start(&hcan1);
	HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING);
	
	HAL_NVIC_DisableIRQ(DMA2_Stream2_IRQn);
	__HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart1,usart1_dma_buff,BUFLEN);

	HAL_TIM_Base_Start(&htim5);
	HAL_TIM_Base_Start(&htim7);
	
	
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);

	HAL_TIM_PWM_Start(&htim9,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim9,TIM_CHANNEL_2);

	PID_Init();
	Chassis_Speed_Ref_Init();
	
	/*��������*/
	//PID����
	xTaskCreate((TaskFunction_t)PID_task,
		(const char *)"PID_task",
		(uint16_t)150,
		(void *)NULL,
		(UBaseType_t)1,
		(TaskHandle_t *)&PID_task_Handler);

	//ң�������ݴ���%USARTinterrupt
	xTaskCreate((TaskFunction_t)Remote_task,
		(const char *)"Remote_task",
		(uint16_t)150,
		(void *)NULL,
		(UBaseType_t)1,
		(TaskHandle_t *)&Remote_task_Handler);
		
	//�Զ�����
	xTaskCreate((TaskFunction_t)Auto_task,
		(const char *)"Auto_task",
		(uint16_t)100,
		(void *)NULL,
		(UBaseType_t)1,
		(TaskHandle_t *)&Auto_task_Handler);
}

/*PID����*/
TaskHandle_t PID_task_Handler;
void PID_task(void *pvParameters)
{
	portTickType xLastWakeTime;
	
	Chassis_Motor_PID_Expect 	 *pRM3510_PID_Expect;
	Chassis_Motor_Actual_Speed *pRM3510_Actual_Speedt;
	pRM3510_PID_Expect=&RM3510_PID_Expect;
	pRM3510_Actual_Speedt=&RM3510_Actual_Speedt;
	
	short RM3510_Current[4];
	
	xLastWakeTime = xTaskGetTickCount();
	
	while (1)
	{
		if (Auto_flag == 0)PID_Expect();//��������Զ�ģʽ������ң������ȡ����

		PID_Control(pRM3510_PID_Expect->Chassis_Motor_PID_Expect_1,
			pRM3510_Actual_Speedt->Chassis_Motor_Actual_Speed_1,
			&RM3510_1_PID);

		PID_Control(pRM3510_PID_Expect->Chassis_Motor_PID_Expect_2,
			pRM3510_Actual_Speedt->Chassis_Motor_Actual_Speed_2,
			&RM3510_2_PID);

		PID_Control(pRM3510_PID_Expect->Chassis_Motor_PID_Expect_3,
			pRM3510_Actual_Speedt->Chassis_Motor_Actual_Speed_3,
			&RM3510_3_PID);

		PID_Control(pRM3510_PID_Expect->Chassis_Motor_PID_Expect_4,
			pRM3510_Actual_Speedt->Chassis_Motor_Actual_Speed_4,
			&RM3510_4_PID);

		RM3510_Current[0] = RM3510_1_PID.pid_out;
		RM3510_Current[1] = RM3510_2_PID.pid_out;
		RM3510_Current[2] = RM3510_3_PID.pid_out;
		RM3510_Current[3] = RM3510_4_PID.pid_out;
		
#if TEST==OFF
		
		RM3510_CAN_Send(RM3510_Current);
		
#endif

		vTaskDelayUntil(&xLastWakeTime, 2);
	}
}

/*ң�������ݴ���%USARTinterrupt*/
SemaphoreHandle_t Remote_Semaphore;
TaskHandle_t Remote_task_Handler;
void Remote_task(void *pvParameters)
{
	BaseType_t err = pdFALSE;
	RC_Ctl_t *pRC_CtrlData;
	pRC_CtrlData=&RC_CtrlData;
	
	while (1)
	{
		err = xSemaphoreTake(Remote_Semaphore, portMAX_DELAY);	//��ȡ�ź���
		if (err == pdTRUE)										//��ȡ�ź����ɹ�
		{
			Remote_Rx(usart1_dma_buff,pRC_CtrlData);
		}
		else if (err == pdFALSE)
		{
			vTaskDelay(10);      //��ʱ10ms��Ҳ����10��ʱ�ӽ���	
		}
	}
}

/*�Զ�����*/
TaskHandle_t Auto_task_Handler;
void Auto_task(void *pvParameters)
{
	while (1)
	{
		switch(Auto_flag)
		{
			case 1:
				HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
				/*�ϵ�����*/
				HAL_GPIO_WritePin(Q1_GPIO_Port,Q1_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Q2_GPIO_Port,Q2_Pin,GPIO_PIN_RESET);//ȫ��
			
				osDelay(700);
			  
				HAL_GPIO_WritePin(Q_GPIO_Port,Q_Pin,GPIO_PIN_RESET);//���׵���
			 // Get_PID_Expect(700);  //ȫ��֮�󣬳���ǰ����
				osDelay(700);
				
				HAL_GPIO_WritePin(Q1_GPIO_Port,Q1_Pin,GPIO_PIN_SET);		//ǰ����
				osDelay(700);
				
			//	Get_PID_Expect(2500);   //ǰ����̨�� ���ֿ���̨��
			Get_PID_Expect_qian(2800);
			Get_PID_Expect_hou(2500);
				while(RF4==1) vTaskDelay(1);	//�ȴ������� ��⸨������̨��
				
				Get_PID_Expect(500);  //����ײ�Ϻ� ���� ����ǰ�� �ᴥ̨��
				osDelay(500);
				
				HAL_GPIO_WritePin(Q2_GPIO_Port,Q2_Pin,GPIO_PIN_SET); //������
				osDelay(700);          //�ȴ�������
				
				Get_PID_Expect(4000);  //ǰ����ȫ��ǰ�� ��̨��
				
				osDelay(700);          //��4000�ٶ��н�700����
				
				Get_PID_Expect(0);  //ͣ
				
				Auto_flag=0;  //��־λ����
				HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET); //ָʾ����˸ ��ʾ������������
				
			//osDelay(1000);
			  HAL_GPIO_WritePin(Q_GPIO_Port,Q_Pin,GPIO_PIN_SET);//������
			break;
			
			case 2:
				HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
				/*�µ�����*/
				HAL_GPIO_WritePin(Q_GPIO_Port,Q_Pin,GPIO_PIN_RESET);//���׵���
				
				Get_PID_Expect(-1000);
				
				while((RF2==0)||(RF3==0))osDelay(1);	//�ȴ�������
					
				Get_PID_Expect(0);
								
				HAL_GPIO_WritePin(Q2_GPIO_Port,Q2_Pin,GPIO_PIN_RESET);//������
				osDelay(700);/*�ȴ�������*/
				
				Get_PID_Expect(-1800);
				
			//	while((RF0==0)||(RF1==0))osDelay(1);	//�ȴ�������
				while((RF0==0))osDelay(1);	//�ȴ�������
				Get_PID_Expect(0);
				
				HAL_GPIO_WritePin(Q1_GPIO_Port,Q1_Pin,GPIO_PIN_RESET);//ǰ��
				osDelay(700);
				
				HAL_GPIO_WritePin(Q_GPIO_Port,Q_Pin,GPIO_PIN_SET);//������
//				osDelay(1000);
				
				HAL_GPIO_WritePin(Q1_GPIO_Port,Q1_Pin,GPIO_PIN_SET);	//ȫ��
				HAL_GPIO_WritePin(Q2_GPIO_Port,Q2_Pin,GPIO_PIN_SET);			
//				osDelay(1000);
				
				Auto_flag=0;
				HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
			break;
			
			default:
				vTaskDelay(5);
				break;
		}
	}
}
