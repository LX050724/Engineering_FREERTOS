#ifndef _GM3510_H_
#define _GM3510_H_
#include "sys.h"
#include "can.h"

void GM3510_CAN_Send(short * data);

short * GM3510_Feedback(unsigned char * data);

#endif
