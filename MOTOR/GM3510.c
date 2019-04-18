#include "GM3510.h"

void GM3510_CAN_Send(short * data)
{
	unsigned char buff[8];
	buff[0] = (unsigned char)((data[0]>>8)&0xff);
	buff[1] = (unsigned char)((data[0])&0xff);
	buff[2] = (unsigned char)((data[1]>>8)&0xff);
	buff[3] = (unsigned char)((data[1])&0xff);
	buff[4] = (unsigned char)((data[2]>>8)&0xff);
	buff[5] = (unsigned char)((data[2])&0xff);
	CAN1_Send_Msg(0x1FF,buff);
}

short * GM3510_Feedback(unsigned char * data)
{
	static short buff[2];
	buff[0]|= data[0];
	buff[0] = (buff[0]<<8)|data[1];
	buff[1] = data[2];
	buff[1] = (buff[1]<<8)|data[2];
	return buff;
}



