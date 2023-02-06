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
    HAL_Init();                    	 	//��ʼ��HAL��    
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//����ʱ��,72M
	delay_init(72);               		//��ʼ����ʱ����
	uart_init(115200);					//��ʼ������
	usmart_dev.init(84); 		   		//��ʼ��USMART	
	LED_Init();							//��ʼ��LED	
	KEY_Init();							//��ʼ������
	BEEP_Init();						//��ʼ��������
 	LCD_Init();           				//��ʼ��LCD FSMC�ӿ�
	MY_ADC_Init();                  	//��ʼ��ADC1ͨ��1
	
	POINT_COLOR=RED; 
	LCD_ShowString(30,50,200,16,16,"STM32");	
	LCD_ShowString(30,70,200,16,16,"Temperature");
	LCD_ShowString(30,70,200,16,16,"made in 514");
	LCD_ShowString(30,110,200,16,16,"2022/12/9");	  
	POINT_COLOR=BLUE;//��������Ϊ��ɫ      
	LCD_ShowString(30,140,200,16,16,"TEMPERATE: 00.00C");//���ڹ̶�λ����ʾС����	
	LCD_ShowString(30,180,200,16,16,"TEMPERATEUP:");
	LCD_ShowString(30,200,200,16,16,"TEMPERATEDOWN:");
    while(1)
	{
		key=KEY_Scan(0);
				switch(key)
		{				 
			case WKUP_PRES:			//���������¶�
				tempup++;
				break;
			case KEY2_PRES:			//���������¶�
				tempup--;
				break;
			case KEY1_PRES:			//���������¶�
				tempdown++;
				break;
			case KEY0_PRES:			//���������¶�
			tempdown--;
			break;
		}
		
		temp=Get_Temprate();	//�õ��¶�ֵ 
		if(temp<0)
		{
			temp=-temp;
			LCD_ShowString(30+10*8,140,16,16,16,"-");	//��ʾ����
		}else LCD_ShowString(30+10*8,140,16,16,16," ");	//�޷���
		
		
		LCD_ShowxNum(30+11*8,140,temp/100,2,16,0);		//��ʾ��������
		LCD_ShowxNum(30+14*8,140,temp%100,2,16,0);		//��ʾС������ 
		LCD_ShowxNum(30+14*8,180,tempup,2,16,0);		  //��ʾUP�¶�
		LCD_ShowxNum(30+14*8,200,tempdown,2,16,0);		//��ʾDOWN�¶�
		if (temp/100 < tempup && temp/100 > tempdown)         //����¶ȳ�����Χ����
		{
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET); 	//BEEP�������ߣ� ��ͬBEEP=1;
		}
		else
		{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);   	//BEEP�������ͣ� ��ͬBEEP=0;
		}
		LED0=!LED0;
		delay_ms(25);	
	} 
}

