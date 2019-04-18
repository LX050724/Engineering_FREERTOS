#ifndef _FREERTOSTASK_H_
#define _FREERTOSTASK_H_
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"

#include "can.h"
#include "usart.h"
#include "PID.h"
#include "remote.h"
#include "set.h"
#include "gpio.h"
#include "test_can.h"
#include "dma.h"
#include "tim.h"


void StartTask(void);

extern TaskHandle_t PID_task_Handler;
void PID_task(void *pvParameters);

extern SemaphoreHandle_t Remote_Semaphore;
extern TaskHandle_t Remote_task_Handler;
void Remote_task(void *pvParameters);

extern TaskHandle_t Auto_task_Handler;
void Auto_task(void *pvParameters);

#endif
