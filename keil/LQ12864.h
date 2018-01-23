#include<STC12C5A60S2.h>
sbit LCD_SCL=P1^7; //时钟 D0（SCLK）
sbit LCD_SDA=P1^5; //D1（MOSI） 数据
sbit LCD_RST=P1^4; //复位 
sbit LCD_DC =P1^3; //数据/命令控制
#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel	    ((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xCF 
#define X_WIDTH 128
#define Y_WIDTH 64

bit opposite_flag=0;
/*********************LCD 延时1ms************************************/
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
/*********************LCD写数据************************************/ 
void LCD_WrDat(unsigned char dat)	 
{
		LCD_DC=1;
	  SPDAT = dat;                //trigger SPI send
    while (!(SPSTAT & 0x80));   //wait send complete
    SPSTAT = 0xc0;       //clear SPI status
}
/*********************LCD写命令************************************/										
void LCD_WrCmd(unsigned char cmd)
{
		LCD_DC=0;
	  SPDAT = cmd;                //trigger SPI send
    while (!(SPSTAT & 0x80));   //wait send complete
    SPSTAT = 0xc0;       //clear SPI status
}
/*********************LCD 设置坐标************************************/
void LCD_Set_Pos(unsigned char x, unsigned char y) 
{ 
LCD_WrCmd(0xb0+y);
LCD_WrCmd(((x&0xf0)>>4)|0x10);
LCD_WrCmd((x&0x0f)|0x01); 
} 
/*********************LCD全屏************************************/
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
/*********************LCD复位************************************/
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
/*********************OLED SSD1306初始化************************************/
void LCD_Init(bit f)     
{  
	
	  SPDAT = 0;                  //initial SPI data
    SPSTAT = 0x80 | 0x40;       //clear SPI status
    SPCTL = 0xd0;        //master mode
	
LCD_SCL=1;
LCD_RST=0;
LCD_DLY_ms(50);
LCD_RST=1;       //从上电到下面开始初始化要有足够的时间，即等待RC复位完毕   
LCD_WrCmd(0xae);//--turn off oled panel
LCD_WrCmd(0x00);//---set low column address
LCD_WrCmd(0x10);//---set high column address
LCD_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
LCD_WrCmd(0x81);//--设置对比度
LCD_WrCmd(0xcf); // 设置对比度
	if(!f)
	{
		LCD_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
		LCD_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	}
	else
	{
		LCD_WrCmd(0xa0);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
		LCD_WrCmd(0xc0);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	}
LCD_WrCmd(0xa6);//--set normal display
LCD_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
LCD_WrCmd(0x3f);//--1/64 duty 若是128*32 LCD_WrCmd(0x1F);
LCD_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
LCD_WrCmd(0x00);//-not offset
LCD_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
LCD_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
LCD_WrCmd(0xd9);//--set pre-charge period
LCD_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
LCD_WrCmd(0xda);//--set com pins hardware configuration
LCD_WrCmd(0x12);
LCD_WrCmd(0xdb);//--set vcomh
LCD_WrCmd(0x40);//设置垂直偏移量(40-7f)
LCD_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)  若是128*32 LCD_WrCmd(0x02);*/ 
LCD_WrCmd(0x02);//
LCD_WrCmd(0x8d);//--set Charge Pump enable/disable
LCD_WrCmd(0x14);//--set(0x10) disable
LCD_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
LCD_WrCmd(0xa6);// 失能反色显示 (0xa6/a7) 
LCD_WrCmd(0xaf);//开启显示（AE/AF）
LCD_Fill(0x00);  //初始清屏
LCD_Set_Pos(0,0); 	
} 
 /*********************OLED SH1106初始化************************************/
/********	sh1106 显示范围4-132***************	 /
/**********************************************
void OLED_Init(void)     
{  
LCD_SCL=1;
LCD_RST=0;
LCD_DLY_ms(50);
LCD_RST=1;  
//从上电到下面开始初始化要有足够的时间，即等待RC复位完毕  

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
LCD_WrCmd(0x3F); //duty = 1/64  若是128*32 LCD_WrCmd(0x1F);
LCD_WrCmd(0xad); //set charge pump enable
LCD_WrCmd(0x8b); // 0x8a 外供VCC 
LCD_WrCmd(0x33); //0X30---0X33 set VPP 8V 
LCD_WrCmd(0xC8); //Com scan direction
LCD_WrCmd(0xD3); //set display offset
LCD_WrCmd(0x00); // 0x20 
LCD_WrCmd(0xD5); //set osc division
LCD_WrCmd(0x80);
LCD_WrCmd(0xD9); //set pre-charge period
LCD_WrCmd(0xf1); //0x22
LCD_WrCmd(0xDA); //set COM pins
LCD_WrCmd(0x20);  // 若是128*32 LCD_WrCmd(0x02);
LCD_WrCmd(0xdb); //set vcomh
LCD_WrCmd(0x40);
LCD_WrCmd(0xAF); //display ON
LCD_Set_Pos(0,0);	
LCD_Fill(0x00);  //初始清屏
} 
 ***************************************************/

/***************功能描述：显示6*8一组标准ASCII字符串	显示的坐标（x,y），y为页范围0～7****************/
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
/*******************功能描述：显示8*16一组标准ASCII字符串	 显示的坐标（x,y），y为页范围0～7****************/
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
/*****************功能描述：显示16*16点阵  显示的坐标（x,y），y为页范围0～7****************************/
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
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
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
