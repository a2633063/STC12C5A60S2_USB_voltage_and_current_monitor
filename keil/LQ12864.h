#include<STC12C5A60S2.h>
sbit LCD_SCL=P1^7; //ʱ�� D0��SCLK��
sbit LCD_SDA=P1^5; //D1��MOSI�� ����
sbit LCD_RST=P1^4; //��λ 
sbit LCD_DC =P1^3; //����/�������
#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel	    ((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xCF 
#define X_WIDTH 128
#define Y_WIDTH 64

bit opposite_flag=0;
/*********************LCD ��ʱ1ms************************************/
void LCD_DLY_ms(unsigned int ms)
{                         
unsigned int a;
while(ms)
{
a=1800;
while(a--);
ms--;
}
return;
}
unsigned char Dat(unsigned char com)
{  unsigned char a,b=0,i,temp;
	a=com;  
   for(i=0;i<8;i++)
   {  
	   temp=a&0x01;  
	   a=a>>1;  
	   b=b+temp;  
	   if(i != 7)  
	   b=b<<1;  
   } 
   return b; 	
}
/*********************LCDд����************************************/ 
void LCD_WrDat(unsigned char dat)	 
{
		LCD_DC=1;
	  SPDAT = dat;                //trigger SPI send
    while (!(SPSTAT & 0x80));   //wait send complete
    SPSTAT = 0xc0;       //clear SPI status
}
/*********************LCDд����************************************/										
void LCD_WrCmd(unsigned char cmd)
{
		LCD_DC=0;
	  SPDAT = cmd;                //trigger SPI send
    while (!(SPSTAT & 0x80));   //wait send complete
    SPSTAT = 0xc0;       //clear SPI status
}
/*********************LCD ��������************************************/
void LCD_Set_Pos(unsigned char x, unsigned char y) 
{ 
LCD_WrCmd(0xb0+y);
LCD_WrCmd(((x&0xf0)>>4)|0x10);
LCD_WrCmd((x&0x0f)|0x01); 
} 
/*********************LCDȫ��************************************/
void LCD_Fill(unsigned char bmp_dat) 
{
unsigned char y,x;
for(y=0;y<8;y++)
{
LCD_WrCmd(0xb0+y);
LCD_WrCmd(0x01);
LCD_WrCmd(0x10);
for(x=0;x<X_WIDTH;x++)
LCD_WrDat(bmp_dat);
}
}
/*********************LCD��λ************************************/
void LCD_CLS(void)
{
unsigned char y,x;	
for(y=0;y<8;y++)
{
LCD_WrCmd(0xb0+y);
LCD_WrCmd(0x01);
LCD_WrCmd(0x10); 
for(x=0;x<X_WIDTH;x++)
LCD_WrDat(0);
}
}
/*********************OLED SSD1306��ʼ��************************************/
void LCD_Init(bit f)     
{  
	
	  SPDAT = 0;                  //initial SPI data
    SPSTAT = 0x80 | 0x40;       //clear SPI status
    SPCTL = 0xd0;        //master mode
	
LCD_SCL=1;
LCD_RST=0;
LCD_DLY_ms(50);
LCD_RST=1;       //���ϵ絽���濪ʼ��ʼ��Ҫ���㹻��ʱ�䣬���ȴ�RC��λ���   
LCD_WrCmd(0xae);//--turn off oled panel
LCD_WrCmd(0x00);//---set low column address
LCD_WrCmd(0x10);//---set high column address
LCD_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
LCD_WrCmd(0x81);//--���öԱȶ�
LCD_WrCmd(0xcf); // ���öԱȶ�
	if(!f)
	{
		LCD_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
		LCD_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	}
	else
	{
		LCD_WrCmd(0xa0);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
		LCD_WrCmd(0xc0);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	}
LCD_WrCmd(0xa6);//--set normal display
LCD_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
LCD_WrCmd(0x3f);//--1/64 duty ����128*32 LCD_WrCmd(0x1F);
LCD_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
LCD_WrCmd(0x00);//-not offset
LCD_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
LCD_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
LCD_WrCmd(0xd9);//--set pre-charge period
LCD_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
LCD_WrCmd(0xda);//--set com pins hardware configuration
LCD_WrCmd(0x12);
LCD_WrCmd(0xdb);//--set vcomh
LCD_WrCmd(0x40);//���ô�ֱƫ����(40-7f)
LCD_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)  ����128*32 LCD_WrCmd(0x02);*/ 
LCD_WrCmd(0x02);//
LCD_WrCmd(0x8d);//--set Charge Pump enable/disable
LCD_WrCmd(0x14);//--set(0x10) disable
LCD_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
LCD_WrCmd(0xa6);// ʧ�ܷ�ɫ��ʾ (0xa6/a7) 
LCD_WrCmd(0xaf);//������ʾ��AE/AF��
LCD_Fill(0x00);  //��ʼ����
LCD_Set_Pos(0,0); 	
} 
 /*********************OLED SH1106��ʼ��************************************/
/********	sh1106 ��ʾ��Χ4-132***************	 /
/**********************************************
void OLED_Init(void)     
{  
LCD_SCL=1;
LCD_RST=0;
LCD_DLY_ms(50);
LCD_RST=1;  
//���ϵ絽���濪ʼ��ʼ��Ҫ���㹻��ʱ�䣬���ȴ�RC��λ���  

LCD_WrCmd(0xAE); //display off
LCD_WrCmd(0x02); //set lower column address
LCD_WrCmd(0x10); //set higher column address
LCD_WrCmd(0x40); //set display start line
LCD_WrCmd(0xB0); //set page address*
LCD_WrCmd(0x81); //contract control
LCD_WrCmd(0x2f); //128
LCD_WrCmd(0xA1); //set segment remap
LCD_WrCmd(0xA6); //normal / reverse
LCD_WrCmd(0xA8); //multiplex ratio
LCD_WrCmd(0x3F); //duty = 1/64  ����128*32 LCD_WrCmd(0x1F);
LCD_WrCmd(0xad); //set charge pump enable
LCD_WrCmd(0x8b); // 0x8a �⹩VCC 
LCD_WrCmd(0x33); //0X30---0X33 set VPP 8V 
LCD_WrCmd(0xC8); //Com scan direction
LCD_WrCmd(0xD3); //set display offset
LCD_WrCmd(0x00); // 0x20 
LCD_WrCmd(0xD5); //set osc division
LCD_WrCmd(0x80);
LCD_WrCmd(0xD9); //set pre-charge period
LCD_WrCmd(0xf1); //0x22
LCD_WrCmd(0xDA); //set COM pins
LCD_WrCmd(0x20);  // ����128*32 LCD_WrCmd(0x02);
LCD_WrCmd(0xdb); //set vcomh
LCD_WrCmd(0x40);
LCD_WrCmd(0xAF); //display ON
LCD_Set_Pos(0,0);	
LCD_Fill(0x00);  //��ʼ����
} 
 ***************************************************/

/***************������������ʾ6*8һ���׼ASCII�ַ���	��ʾ�����꣨x,y����yΪҳ��Χ0��7****************/
void LCD_P6x8Str(unsigned char x, y,unsigned char ch[])
{
unsigned char c=0,i=0,j=0;      
while (ch[j]!='\0')
{    
c =ch[j]-32;
if(x>126){x=0;y++;}
LCD_Set_Pos(x,y);    
for(i=0;i<6;i++)     
LCD_WrDat(F6x8[c][i]);  
x+=6;
j++;
}
}
/*******************������������ʾ8*16һ���׼ASCII�ַ���	 ��ʾ�����꣨x,y����yΪҳ��Χ0��7****************/
void LCD_P8x16Str(unsigned char x, y,unsigned char ch[])
{
unsigned char c=0,i=0,j=0;
while (ch[j]!='\0')
{    
c =ch[j]-32;
if(x>120){x=0;y++;}
LCD_Set_Pos(x,y);    
for(i=0;i<8;i++)     
LCD_WrDat(F8X16[c*16+i]);
LCD_Set_Pos(x,y+1);    
for(i=0;i<8;i++)     
LCD_WrDat(F8X16[c*16+i+8]);  
x+=8;
j++;
}
}
/*****************������������ʾ16*16����  ��ʾ�����꣨x,y����yΪҳ��Χ0��7****************************/
void LCD_P16x16Ch(unsigned char x, y, N)
{
unsigned char wm=0;
unsigned int adder=32*N;  //  	
LCD_Set_Pos(x , y);
for(wm = 0;wm < 16;wm++)  //             
{
LCD_WrDat(F16x16[adder]);	
adder += 1;
}      
LCD_Set_Pos(x,y + 1); 
for(wm = 0;wm < 16;wm++) //         
{
LCD_WrDat(F16x16[adder]);
adder += 1;
} 	  	
}
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void Draw_BMP(unsigned char x0, y0,x1, y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		LCD_Set_Pos(x0,y);				
    for(x=x0;x<x1;x++)
	    {      
	    	LCD_WrDat(BMP[j++]);	    	
	    }
	}
} 
