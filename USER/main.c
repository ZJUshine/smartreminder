#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "usmart.h"
#include "adc.h"
#include "adc3.h"
#include "lsens.h"
#include "key.h"
#include "beep.h"

int main(void)
{	
	short temp;
	int tempup;
	int tempdown;
		u8 key;
	tempup = 99;
	tempdown = 25;
	temp = 30;
    HAL_Init();                    	 	//初始化HAL库    
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//设置时钟,72M
	delay_init(72);               		//初始化延时函数
	uart_init(115200);					//初始化串口
	usmart_dev.init(84); 		   		//初始化USMART	
	LED_Init();							//初始化LED	
	KEY_Init();							//初始化按键
	BEEP_Init();						//初始化蜂鸣器
 	LCD_Init();           				//初始化LCD FSMC接口
	MY_ADC_Init();                  	//初始化ADC1通道1
	
	POINT_COLOR=RED; 
	LCD_ShowString(30,50,200,16,16,"STM32");	
	LCD_ShowString(30,70,200,16,16,"Temperature");
	LCD_ShowString(30,70,200,16,16,"made in 514");
	LCD_ShowString(30,110,200,16,16,"2022/12/9");	  
	POINT_COLOR=BLUE;//设置字体为蓝色      
	LCD_ShowString(30,140,200,16,16,"TEMPERATE: 00.00C");//先在固定位置显示小数点	
	LCD_ShowString(30,180,200,16,16,"TEMPERATEUP:");
	LCD_ShowString(30,200,200,16,16,"TEMPERATEDOWN:");
    while(1)
	{
		key=KEY_Scan(0);
				switch(key)
		{				 
			case WKUP_PRES:			//控制上限温度
				tempup++;
				break;
			case KEY2_PRES:			//控制上限温度
				tempup--;
				break;
			case KEY1_PRES:			//控制下限温度
				tempdown++;
				break;
			case KEY0_PRES:			//控制下限温度
			tempdown--;
			break;
		}
		
		temp=Get_Temprate();	//得到温度值 
		if(temp<0)
		{
			temp=-temp;
			LCD_ShowString(30+10*8,140,16,16,16,"-");	//显示负号
		}else LCD_ShowString(30+10*8,140,16,16,16," ");	//无符号
		
		
		LCD_ShowxNum(30+11*8,140,temp/100,2,16,0);		//显示整数部分
		LCD_ShowxNum(30+14*8,140,temp%100,2,16,0);		//显示小数部分 
		LCD_ShowxNum(30+14*8,180,tempup,2,16,0);		  //显示UP温度
		LCD_ShowxNum(30+14*8,200,tempdown,2,16,0);		//显示DOWN温度
		if (temp/100 < tempup && temp/100 > tempdown)         //如果温度超出范围报警
		{
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET); 	//BEEP引脚拉高， 等同BEEP=1;
		}
		else
		{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);   	//BEEP引脚拉低， 等同BEEP=0;
		}
		LED0=!LED0;
		delay_ms(25);	
	} 
}

