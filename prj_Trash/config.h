#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "REG51.h"

sbit LED = P0^0;//=0ÁÁ
sbit BEEP = P0^4;//=0Ïì

sbit KEY_UP = P2^4;
sbit KEY_DWON = P2^0;

sbit OLED_SCL=P1^5;//Ê±ÖÓ D0£¨SCLK?
sbit OLED_SDIN=P1^6;//D1£¨MOSI£© Êý¾Ý

sbit TRIG = P1^1;
sbit ECHO = P1^2;

sbit RED = P0^6;

sbit DSPORT = P1^0;

sbit SERVO = P3^7;



/*Declare SFR associated with the PCA */
sfr CCON        =   0xD8;           //PCA control register
sbit CCF0       =   CCON^0;         //PCA module-0 interrupt flag
sbit CCF1       =   CCON^1;         //PCA module-1 interrupt flag
sbit CR         =   CCON^6;         //PCA timer run control bit
sbit CF         =   CCON^7;         //PCA timer overflow flag
sfr CMOD        =   0xD9;           //PCA mode register
sfr CL          =   0xE9;           //PCA base timer LOW
sfr CH          =   0xF9;           //PCA base timer HIGH
sfr CCAPM0      =   0xDA;           //PCA module-0 mode register
sfr CCAP0L      =   0xEA;           //PCA module-0 capture register LOW
sfr CCAP0H      =   0xFA;           //PCA module-0 capture register HIGH
sfr CCAPM1      =   0xDB;           //PCA module-1 mode register
sfr CCAP1L      =   0xEB;           //PCA module-1 capture register LOW
sfr CCAP1H      =   0xFB;           //PCA module-1 capture register HIGH
sfr PCAPWM0     =   0xf2;
sfr PCAPWM1     =   0xf3;


sfr P0M0 = 0x94; //                                                                         0000,0000
sfr P0M1 = 0x93; //  
#endif