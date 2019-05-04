#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "sys.h"
#include "can.h"

void GM3510_CAN_Send(short * data);

short * GM3510_Feedback(unsigned char * data);

#ifdef __cplusplus
}
#endif
