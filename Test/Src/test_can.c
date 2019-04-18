/*以下代码是can的滤波器*/
#include "test_can.h"

#define CAN2_ENABLE 0


void CanFilter_Init(CAN_HandleTypeDef *hcan)
{
  CAN_FilterTypeDef canfilter;

  canfilter.FilterMode = CAN_FILTERMODE_IDMASK;
  canfilter.FilterScale = CAN_FILTERSCALE_32BIT;

  canfilter.FilterIdHigh = 0x0000;
  canfilter.FilterIdLow = 0x0000;
  canfilter.FilterMaskIdHigh = 0x0000;
  canfilter.FilterMaskIdLow = 0x0000;

  canfilter.FilterFIFOAssignment = CAN_FILTER_FIFO0; // CAN_FilterFIFO0;
  canfilter.FilterActivation = ENABLE;
  canfilter.FilterBank = 14;
  /*can1和CAN2使用不同的滤波器*/
  if (hcan == &hcan1)
  {
    canfilter.SlaveStartFilterBank = 0;
  }
#if CAN2_ENABLE == 1
  if (hcan == &hcan2)
  {
    canfilter.FilterNumber = 14;
  }
#endif
  HAL_CAN_ConfigFilter(hcan, &canfilter);
}
