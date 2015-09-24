#include "led.h"
#include "stm32f4xx.h"


void init_led(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  
	/* Enable the GPIO_LED Clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	//
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure the GPIO_LED pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_8);//�ر�LED1 
	GPIO_SetBits(GPIOB,GPIO_Pin_8);//�ر�LED2
	
}

void led_on(int ledx)
{
	if(LED1==ledx)
		//GPIO_ResetBits(GPIOA,GPIO_Pin_8);//����LED1 
	GPIO_ResetBits(GPIOD,GPIO_Pin_12);//����LED1 
	if(LED2==ledx)
		//GPIO_ResetBits(GPIOB,GPIO_Pin_8);//����LED2
	GPIO_ResetBits(GPIOD,GPIO_Pin_13);//����LED1 
}

void led_off(int ledx)
{
	/*
	if(LED1==ledx)
		GPIO_SetBits(GPIOA,GPIO_Pin_8);//�ر�LED1 
	if(LED2==ledx)
		GPIO_SetBits(GPIOB,GPIO_Pin_8);//�ر�LED2
	*/
	if(LED1==ledx)
		//GPIO_ResetBits(GPIOA,GPIO_Pin_8);//����LED1 
	GPIO_SetBits(GPIOD,GPIO_Pin_12);//����LED1 
	if(LED2==ledx)
		//GPIO_ResetBits(GPIOB,GPIO_Pin_8);//����LED2
	GPIO_SetBits(GPIOD,GPIO_Pin_13);//����LED1 
	
}

void Led_toggle(unsigned char _no)
{
	if (_no == LED1)
	{
		GPIO_ToggleBits(GPIOA,GPIO_Pin_8);
	}
	else if (_no == LED2)
	{
		GPIO_ToggleBits(GPIOB,GPIO_Pin_8);
	}
}

