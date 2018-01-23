#define Key_short 1
#define Key_long 2

#include<STC12C5A60S2.h>
#include "intrins.h"
#include <string.h>
#include "codetab.h"
#include "LQ12864.h"
#include "UART.h"
#include "ADC.h"
sbit K=P3^2;
sbit Ralay=P2^5;
unsigned char key=0;
unsigned char RI_dat[20];
unsigned char light=0xcf;
void Delay(unsigned int n)
{
    unsigned int x;

    while (n--)
    {
        x = 5000;
        while (x--);
    }
}
/*********************主函数************************************/
void main(void) 
{
unsigned char i ;
unsigned int 	temp; 
	EX0 = 1;        //使能 INT0 外部中断
	TCON = 0x01;    //触发方式为脉冲负边沿触发
	
/*****串口*********************/
	PCON |= 0x80;		//使能波特率倍速位SMOD  //22.1184M 57600
	SCON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xFB;		//独立波特率发生器时钟为Fosc/12,即12T
	BRT = 0xFe;		//设定独立波特率发生器重装值
	AUXR |= 0x01;		//串口1选择独立波特率发生器为波特率发生器
	AUXR |= 0x10;		//启动独立波特率发生器
	
	
		//20毫秒@22.1184MHz
	AUXR &= 0x7F;		//定时器时钟12T模式
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD = 0x11;		//设置定时器模式
	TL0 = 0x00;		//设置定时初值//20ms
	TH0 = 0x70;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	
	TL1 = 0x00;		//设置定时初值
	TH1 = 0x70;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 0;		//定时器1开始计时
	
	
	ET1 = 1;			//定时器1中断
	ET0 = 1;			//定时器0中断
	ES=1;					//串口中断
	EA = 1;
	
if(!K)  opposite_flag=~opposite_flag;	
	SendString("Ready\n");
	InitADC();
	SendString("ADC Init\n");
	LCD_Init(opposite_flag); //oled 初始化  
	SendString("LCD Init\n");
	P0M0=0XFF;

	/*
	LCD_Set_Pos(0,0);
	for(i=0;i<255;i++)
	LCD_WrDat(i);
	
	LCD_Set_Pos(0,1);
	for(i=0;i<128;i++)
	LCD_WrDat(i);*/

if(opposite_flag)
	{
  LCD_P16x16Ch(24,6,0);
  LCD_P16x16Ch(40,6,1);
  LCD_P16x16Ch(56,6,0);
  LCD_P16x16Ch(72,6,2);
  LCD_P16x16Ch(88,6,3);
	}
	else
	{
  LCD_P16x16Ch(24,0,0);
  LCD_P16x16Ch(40,0,1);
  LCD_P16x16Ch(56,0,0);
  LCD_P16x16Ch(72,0,2);
  LCD_P16x16Ch(88,0,3);
	}
	

//	LCD_P6x8Str(40,6,"012345");
	ADC_Result();
	ADC();

	Ralay=0;
	Delay(400);

	while(1)
	{
	ADC_Result();
	ADC();

	if(time>60&&current<30&&current!=0) Ralay=1;
	if(current>250&&Auto_switch==0) Ralay=1;
	//	Delay(100);

  if(key==Key_long)
	{
			Ralay=~Ralay;time=0;
			electric_mah=electric_time=0;
		  key=0;
	}
	else if(key==Key_short)
	{
		Auto_switch=~Auto_switch;

		key=0;
	}
	


	}

}


void Timer0Interrupt(void) interrupt 1
{
	static unsigned char t;
	TL0 = 0x00;		//设置定时初值
	TH0 = 0x70;		//设置定时初值
	
	if(++t>=50)
	{
		t=0;
		time+=1;
		if(current!=0)
		electric_time+=1;        //为方便调试 将时间扩大60倍(+60)
		electric_mah += current*25/9; //*1000/3600
	}
}

void Timer1Interrupt(void) interrupt 3
{
	static unsigned int tim=0;
	TL1 = 0xCD;		//设置定时初值
	TH1 = 0xF8;		//设置定时初值
	if(!K)
	{
		tim++;
		if(tim>1500)
		{
			key=Key_short;
			tim=0;
			TR1 = 0;		//定时器1开始计时
			EX0=1;
		}
	}
	else
	{
		if(tim>1500)
		{
		}
		else
		{//短按
			key=Key_short;
		}
			tim=0;
			TR1 = 0;		//定时器1开始计时
			EX0=1;
	}

}
void EX_IN() interrupt 0 using 0
{
	EX0 = 0;
			Delay(3);
	if(!K)
	{
			TR1 = 1;		//定时器1开始计时
	}
	else EX0=1;

}

void UARTInterrupt(void) interrupt 4//串口中断
{
	unsigned char temp;
	

    if(RI)
    {
      RI = 0;
			temp=SBUF;
		//	SendHex16(temp);
			if(temp==0xa8)	IAP_CONTR=0X60;
			//else LCD_WrCmd(temp);

    }
    else
        TI = 0;
}
