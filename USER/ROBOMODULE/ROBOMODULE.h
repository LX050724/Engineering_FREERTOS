#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <sys.h>
#include "cmsis_os.h"
#include <usart.h>

void ROBOMODULE_Reset(void);
void Location_mode_Sent(int32_t Location);
void ALL_Reset(void);

#ifdef __cplusplus
}
#endif
