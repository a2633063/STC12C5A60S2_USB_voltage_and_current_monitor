#ifndef _UART_H_
#define _UART_H_

/******************************
*函数名:SendOneByte
*功  能：51串口输出值
*输  入：输出值c
*返回值：无
******************************/
unsigned char SendOneByte(unsigned char c)
{
	ES=0;					//串口中断
    SBUF = c;
    while(!TI);
    TI = 0;
	ES=1;					//串口中断
	return c;
}

/******************************
*函数名:SendString
*功  能：51串口输出字符串
*输  入：输出字符串*Str
*返回值：无
******************************/
void SendString(unsigned char *Str)
{
	//判断Str指向的数据是否有效.
	while( *Str!= '\0')
	{
	//是否是回车字符 如果是,则发送相应的回车 0x0d 0x0a
	if(*Str=='\n'){SendOneByte(0x0d);SendOneByte(0x0a);}
		else	SendOneByte(*Str);
	//指针++ 指向下一个字节.
	Str++;
	}
}

/******************************
*函数名:SendHex16
*功  能：将无符号字符型值以16进制字符串形式输出
*输  入：输出值tmp
*返回值：无
******************************/
void SendHex16(unsigned char tmp)
{
	unsigned char z[]="0123456789ABCDEF";
	SendOneByte(z[tmp/16]);
	SendOneByte(z[tmp%16]);
	SendOneByte(' ');
}

/******************************
*函数名:SendHex10
*功  能：将无符号字符型值以10进制字符串形式输出
*输  入：输出值tmp,显示位数x(以方便对齐显示)
*返回值：无
******************************/
void SendHex10(unsigned long tmp,unsigned char x)
{
	unsigned char i;
	unsigned char z[10];
	z[0]=tmp%10000000000/1000000000;
	z[1]=tmp%1000000000/100000000;
	z[2]=tmp%100000000/10000000;
	z[3]=tmp%10000000/1000000;
	z[4]=tmp%1000000/100000;
	z[5]=tmp%100000/10000;
	z[6]=tmp%10000/1000;
	z[7]=tmp%1000/100;
	z[8]=tmp%100/10;
	z[9]=tmp%10;
	for(i=0;i<10-x;i++)
	{
		if(z[i]!=0)break;
	}
	if(i>=10) 	SendOneByte(0x30);
	else
	for(i;i<10;i++)
	{
	SendOneByte(z[i]+0x30);	
	}
}

/*
/******************************
*函数名:UARTInterrupt
*功  能：串口接收中断
*输  入：无
*返回值：无
******************************
void UARTInterrupt(void) interrupt 4	
{
    unsigned char temp
    if(RI)
    {
        RI = 0;
	temp= SBUF;  				
    }
    else
      TI = 0;
}
*/
#endif