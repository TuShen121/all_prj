#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "reg52.h"
sbit OLED_SCL=P0^1;//Ê±ÖÓ D0£¨SCLK?
sbit OLED_SDIN=P0^0;//D1£¨MOSI£© Êý¾Ý

sbit LED=P0^2;
sbit KEY=P2^6;
sbit BEEP=P0^7;
#endif