#ifndef _ROBOMODULE_H_
#define _ROBOMODULE_H_

#include <sys.h>
#include "cmsis_os.h"
#include <usart.h>

void ROBOMODULE_Reset(void);
void Location_mode_Sent(int32_t Location);
void ALL_Reset(void);

#endif
