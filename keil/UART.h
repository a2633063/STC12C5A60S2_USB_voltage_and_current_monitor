#ifndef _UART_H_
#define _UART_H_

/******************************
*������:SendOneByte
*��  �ܣ�51�������ֵ
*��  �룺���ֵc
*����ֵ����
******************************/
unsigned char SendOneByte(unsigned char c)
{
	ES=0;					//�����ж�
    SBUF = c;
    while(!TI);
    TI = 0;
	ES=1;					//�����ж�
	return c;
}

/******************************
*������:SendString
*��  �ܣ�51��������ַ���
*��  �룺����ַ���*Str
*����ֵ����
******************************/
void SendString(unsigned char *Str)
{
	//�ж�Strָ��������Ƿ���Ч.
	while( *Str!= '\0')
	{
	//�Ƿ��ǻس��ַ� �����,������Ӧ�Ļس� 0x0d 0x0a
	if(*Str=='\n'){SendOneByte(0x0d);SendOneByte(0x0a);}
		else	SendOneByte(*Str);
	//ָ��++ ָ����һ���ֽ�.
	Str++;
	}
}

/******************************
*������:SendHex16
*��  �ܣ����޷����ַ���ֵ��16�����ַ�����ʽ���
*��  �룺���ֵtmp
*����ֵ����
******************************/
void SendHex16(unsigned char tmp)
{
	unsigned char z[]="0123456789ABCDEF";
	SendOneByte(z[tmp/16]);
	SendOneByte(z[tmp%16]);
	SendOneByte(' ');
}

/******************************
*������:SendHex10
*��  �ܣ����޷����ַ���ֵ��10�����ַ�����ʽ���
*��  �룺���ֵtmp,��ʾλ��x(�Է��������ʾ)
*����ֵ����
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
*������:UARTInterrupt
*��  �ܣ����ڽ����ж�
*��  �룺��
*����ֵ����
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