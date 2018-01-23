#include<STC12C5A60S2.h>

unsigned long USBA,USBU,U2_5;
unsigned long voltage=0,current=0; //单位V A  扩大100倍
unsigned long  electric_mah=0;
unsigned long  electric_time=0;
unsigned long  time=0,temp_time=0;
bit Auto_switch=0;

/*********************ADC初始化*********************************/
void InitADC()
{
    P1ASF = 0x07;                   //Open 8 channels ADC function
    ADC_RES = 0;                    //Clear previous result
    ADC_CONTR = 0x80;
}


/************Get ADC result*********************************/
unsigned int GetADCResult(unsigned char ch)
{
	if(ch<3)
	{
    ADC_CONTR = 0x88 | ch;
    _nop_();                        //Must wait before inquiry
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & 0x10));//Wait complete flag
    ADC_CONTR &= 0xef;         //Close ADC

    return (ADC_RES<<2|ADC_RESL);                 //Return ADC result
	}
}

#define N 101//ADC中值滤波次数，必须为基数
void ADC_Result()
{
	unsigned char i,j,k;
	unsigned int value_buf[3][N];
	unsigned long temp=0;
	USBA=0;USBU=0;
	
	for(i=0;i<N;i++)
	{
		value_buf[0][i] = GetADCResult(0);	//2.48v基准电压
		value_buf[1][i] = GetADCResult(1);	//电流
		value_buf[2][i] = GetADCResult(2);	//电源电压
	}
	
	   for (j=0;j<N-1;j++)
   {
      for (i=0;i<N-j;i++)
      {
				for (k=0;k<3;k++)
         if ( value_buf[k][i]>value_buf[k][i+1] )
         {
            temp = value_buf[k][i];
            value_buf[k][i] = value_buf[k][i+1];
            value_buf[k][i+1] = temp;
         }
      }
   }
	 
	temp=value_buf[0][(N-1)/2];
	USBA=value_buf[1][(N-1)/2];
	USBU=value_buf[2][(N-1)/2];
	 
	U2_5=252927/temp;
	USBA=USBA*123/temp;
	USBU=USBU*494/temp;
}



void ADC()
{
		unsigned char i,temp;
		unsigned char res[]="I:0.00A";

		
		voltage=(USBU+U2_5)/2;//(long)(GetADCResult(2))*496/GetADCResult(0);
		res[0]='U';
		res[1]=':';
		res[2]=voltage/100+'0';
		res[3]='.';
		res[4]=voltage/10%10+'0';
		res[5]=voltage%10+'0';
		res[6]='V';
		LCD_P8x16Str(0,opposite_flag?0:2,res);


		current=USBA*2;//(long)GetADCResult(1)*225/GetADCResult(0);
		res[0]='I';
		res[1]=':';
		res[2]=current/100+'0';
		res[3]='.';
		res[4]=current/10%10+'0';
		res[5]=current%10+'0';
		res[6]='A';
		LCD_P8x16Str(0,opposite_flag?2:4,res);

		
		res[0]=electric_time/3600/10%10+'0';
		res[1]=electric_time/3600%10+'0';
		res[2]=':';
		res[3]=electric_time%3600/60/10+'0';
		res[4]=electric_time%3600/60%10+'0';
		res[5]=0;
		LCD_P8x16Str(80,opposite_flag?0:2,res);

		

		if(current!=0)
		LCD_P6x8Str(120,opposite_flag?1:3,"T");
		else
		LCD_P6x8Str(120,opposite_flag?1:3,"N");
		
		
		temp = (electric_mah/1000);//
		res[0]=temp/10000%10+'0';
		res[1]=temp/1000%10+'0';
		res[2]=temp/100%10+'0';
		res[3]=temp/10%10+'0';
		res[4]=temp%10+'0';
		res[5]=0;
		
		LCD_P8x16Str(71,opposite_flag?2:4,res);

		
		for(i=1;i<6;i++)
		LCD_WrDat(F6x8['m'-32][i]);
		for(i=0;i<6;i++)
		LCD_WrDat(F6x8['a'-32][i]);
		for(i=0;i<6;i++)
		LCD_WrDat(F6x8['h'-32][i]);
		
		
			LCD_P16x16Ch(0,6,4);
			LCD_P16x16Ch(16,6,5);
			LCD_P16x16Ch(32,6,6);
			LCD_P16x16Ch(48,6,7);
			LCD_P8x16Str(64,6,":");
			LCD_P16x16Ch(72,6,Auto_switch?8:9);

}