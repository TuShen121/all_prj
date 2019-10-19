#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <STC89C52.h>

sbit OLED_SCL=P0^1;//Ê±ÖÓ D0£¨SCLK?
sbit OLED_SDIN=P0^0;//D1£¨MOSI£© Êý¾Ý

sbit Kup=P2^6;
sbit Kdon=P2^0;

sbit Kp1=P0^3;
sbit Kp2=P0^5;
sbit Kp3=P0^7;

#endif