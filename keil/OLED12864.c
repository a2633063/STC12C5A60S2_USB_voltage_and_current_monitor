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
/*********************������************************************/
void main(void) 
{
unsigned char i ;
unsigned int 	temp; 
	EX0 = 1;        //ʹ�� INT0 �ⲿ�ж�
	TCON = 0x01;    //������ʽΪ���帺���ش���
	
/*****����*********************/
	PCON |= 0x80;		//ʹ�ܲ����ʱ���λSMOD  //22.1184M 57600
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR &= 0xFB;		//���������ʷ�����ʱ��ΪFosc/12,��12T
	BRT = 0xFe;		//�趨���������ʷ�������װֵ
	AUXR |= 0x01;		//����1ѡ����������ʷ�����Ϊ�����ʷ�����
	AUXR |= 0x10;		//�������������ʷ�����
	
	
		//20����@22.1184MHz
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	TMOD = 0x11;		//���ö�ʱ��ģʽ
	TL0 = 0x00;		//���ö�ʱ��ֵ//20ms
	TH0 = 0x70;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	
	TL1 = 0x00;		//���ö�ʱ��ֵ
	TH1 = 0x70;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 0;		//��ʱ��1��ʼ��ʱ
	
	
	ET1 = 1;			//��ʱ��1�ж�
	ET0 = 1;			//��ʱ��0�ж�
	ES=1;					//�����ж�
	EA = 1;
	
if(!K)  opposite_flag=~opposite_flag;	
	SendString("Ready\n");
	InitADC();
	SendString("ADC Init\n");
	LCD_Init(opposite_flag); //oled ��ʼ��  
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
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0x70;		//���ö�ʱ��ֵ
	
	if(++t>=50)
	{
		t=0;
		time+=1;
		if(current!=0)
		electric_time+=1;        //Ϊ������� ��ʱ������60��(+60)
		electric_mah += current*25/9; //*1000/3600
	}
}

void Timer1Interrupt(void) interrupt 3
{
	static unsigned int tim=0;
	TL1 = 0xCD;		//���ö�ʱ��ֵ
	TH1 = 0xF8;		//���ö�ʱ��ֵ
	if(!K)
	{
		tim++;
		if(tim>1500)
		{
			key=Key_short;
			tim=0;
			TR1 = 0;		//��ʱ��1��ʼ��ʱ
			EX0=1;
		}
	}
	else
	{
		if(tim>1500)
		{
		}
		else
		{//�̰�
			key=Key_short;
		}
			tim=0;
			TR1 = 0;		//��ʱ��1��ʼ��ʱ
			EX0=1;
	}

}
void EX_IN() interrupt 0 using 0
{
	EX0 = 0;
			Delay(3);
	if(!K)
	{
			TR1 = 1;		//��ʱ��1��ʼ��ʱ
	}
	else EX0=1;

}

void UARTInterrupt(void) interrupt 4//�����ж�
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
