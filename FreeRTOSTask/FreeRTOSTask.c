#include "FreeRTOSTask.h"
unsigned char i;

void StartTask(void)
{
	/*创建二值信号量*/
	Remote_Semaphore= xSemaphoreCreateBinary();
	
	/*创建队列*/
	
	/*外�?�初始化*/
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
	
	/*创建任务*/
	//PID进程
	xTaskCreate((TaskFunction_t)PID_task,
		(const char *)"PID_task",
		(uint16_t)150,
		(void *)NULL,
		(UBaseType_t)1,
		(TaskHandle_t *)&PID_task_Handler);

	//遥控器数�?处理%USARTinterrupt
	xTaskCreate((TaskFunction_t)Remote_task,
		(const char *)"Remote_task",
		(uint16_t)150,
		(void *)NULL,
		(UBaseType_t)1,
		(TaskHandle_t *)&Remote_task_Handler);
		
	//�?动流�?
	xTaskCreate((TaskFunction_t)Auto_task,
		(const char *)"Auto_task",
		(uint16_t)100,
		(void *)NULL,
		(UBaseType_t)1,
		(TaskHandle_t *)&Auto_task_Handler);
}

/*PID进程*/
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
		if (Auto_flag == 0)PID_Expect();//如果处于�?动模式，不从遥控器获取期�?

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

/*遥控器数�?处理%USARTinterrupt*/
SemaphoreHandle_t Remote_Semaphore;
TaskHandle_t Remote_task_Handler;
void Remote_task(void *pvParameters)
{
	BaseType_t err = pdFALSE;
	RC_Ctl_t *pRC_CtrlData;
	pRC_CtrlData=&RC_CtrlData;
	
	while (1)
	{
		err = xSemaphoreTake(Remote_Semaphore, portMAX_DELAY);	//获取信号�?
		if (err == pdTRUE)										//获取信号量成�?
		{
			Remote_Rx(usart1_dma_buff,pRC_CtrlData);
		}
		else if (err == pdFALSE)
		{
			vTaskDelay(10);      //延时10ms，也就是10�?时钟节拍	
		}
	}
}

/*�?动流�?*/
TaskHandle_t Auto_task_Handler;
void Auto_task(void *pvParameters)
{
	while (1)
	{
		switch(Auto_flag)
		{

			case 1:
				HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
				/*上岛流程*/
				HAL_GPIO_WritePin(Q1_GPIO_Port,Q1_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Q2_GPIO_Port,Q2_Pin,GPIO_PIN_RESET);//全升
			
				osDelay(450);  /*此�?�减小了100ms*/
			  
				HAL_GPIO_WritePin(Q_GPIO_Port,Q_Pin,GPIO_PIN_RESET);//气缸弹出
			    Get_PID_Expect(2000);  /*初�??1500，加�?500*///全升之后，车向前靠近
				osDelay(450);
				
				HAL_GPIO_WritePin(Q1_GPIO_Port,Q1_Pin,GPIO_PIN_SET);		//前腿�?
				osDelay(700);  /*�?以改�?？先开始run起来*/
				
//				Get_PID_Expect_qian(3500);
//				Get_PID_Expect_hou(3000);
                Get_PID_Expect(3500);  
				while(RF4==1) vTaskDelay(1);	//等待传感�? 检测辅助轮上台�?
				
				Get_PID_Expect(500);  //后轮撞上�? 反弹 车往前开 轻触台阶
				osDelay(500);
				
				HAL_GPIO_WritePin(Q2_GPIO_Port,Q2_Pin,GPIO_PIN_SET); //后腿�?
				osDelay(700);          //等待后腿�?
				
				Get_PID_Expect(6000);  /*初�??4000，加1000*///前后�?全速前�? �?台阶 
				
				osDelay(1000);         /*初�??1000，缩�?200*///�?5000速度行进1000�?�?
				
				Get_PID_Expect(0);  //�?
				
				Auto_flag=0;  //标志位清�?
				HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET); //指示�?�?�? 表示程序正常运�??
				
			    HAL_GPIO_WritePin(Q_GPIO_Port,Q_Pin,GPIO_PIN_SET);//气缸�?
			    break;
                
			case 2:
				HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
				/*下岛流程*/
				HAL_GPIO_WritePin(Q_GPIO_Port,Q_Pin,GPIO_PIN_RESET);//气缸弹出
				
				Get_PID_Expect(-2000);
				
				while((RF2==0)||(RF3==0))osDelay(1);	//等待传感�?
//				while((RF2==0))osDelay(1);	//等待传感�?					
				Get_PID_Expect(0);
								
				HAL_GPIO_WritePin(Q2_GPIO_Port,Q2_Pin,GPIO_PIN_RESET);//后退�?
				osDelay(700);/*等待后退�?*/
				
				Get_PID_Expect(-2800);
				
			//	while((RF0==0)||(RF1==0))osDelay(1);	//等待传感�?
				while(RF0==0)vTaskDelay(1);	//等待传感�?
				Get_PID_Expect(0);
				
				HAL_GPIO_WritePin(Q1_GPIO_Port,Q1_Pin,GPIO_PIN_RESET);//前伸
				osDelay(700);
				
				HAL_GPIO_WritePin(Q_GPIO_Port,Q_Pin,GPIO_PIN_SET);//气缸�?
				vTaskDelay(100);
				HAL_GPIO_WritePin(Q1_GPIO_Port,Q1_Pin,GPIO_PIN_SET);	//全收
				HAL_GPIO_WritePin(Q2_GPIO_Port,Q2_Pin,GPIO_PIN_SET);			
				
				Auto_flag=0;
				HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
			  break;
			  
			  default:
				vTaskDelay(5);
				break;
		}
	}
}
