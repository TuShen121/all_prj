#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "sys.h"

#define BEEP PCout(1)	
#define HuoYan PCin(6)  
#define YanWu PAin(11)  
#define KEY PBin(0) 
#define RenTi PCin(0)

void Io_CONFIG_Init(void);
#endif





