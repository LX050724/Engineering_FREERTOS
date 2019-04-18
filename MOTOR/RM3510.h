#ifndef _RM3510_H_
#define _RM3510_H_
#include "sys.h"
#include "can.h"

void RM3510_CAN_Send(short * data);
short RM3510_Feedback(unsigned char * data);

#endif
