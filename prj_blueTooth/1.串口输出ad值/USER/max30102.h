#ifndef _MAX30102_H_
#define _MAX30102_H_

#include "stm32f10x.h"
#include "bsp_i2c_gpio.h"
#include <stdio.h>
unsigned long test_max30102_fun(void);
u8 max30102_Bus_Read(u8 Register_Address);
void max30102_init(void);
#endif

