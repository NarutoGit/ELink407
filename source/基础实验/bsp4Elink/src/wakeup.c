    #include "wakeup.h"
    #include "led.h"
	#include "stm32f4xx.h"
    #include "delay.h"
	#include "stdio.h"
   
     
 //�������ģʽ
 void StandbyMode(void)
 {	
	//�رղ���Ҫ������
		
	RCC_AHB1PeriphResetCmd(0X01FF,ENABLE);  //��λ���� IO ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//ʹ�� PWR ʱ��
	PWR_ClearFlag(PWR_FLAG_WU);//��� Wake-up  ��־
	PWR_WakeUpPinCmd(ENABLE);//���� WKUP ���ڻ���
	PWR_EnterSTANDBYMode();//�������ģʽ
		
 }
	
	
u8 Check_WKUP(void)
{
	u32 t=0;
		
	led_on(LED1); //����LED1
		
	while(1)
	{
		if(0==(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)&0x01))//wakeup������
		{
			t++;
		}

		if(t>=600000)//������ʱ
		{
			printf("����\r\n");
			return 2; 
		}
	}
}
	
	
    //�ж�,��⵽PA0�ŵ�һ��������.
    //�ж���0���ϵ��жϼ��
    void EXTI0_IRQHandler(void)
    {
		EXTI->PR=1<<0; //���LINE10�ϵ��жϱ�־λ
		if(2==Check_WKUP())//����?
		{	
			StandbyMode();
		}
    }
    
	
//PA0 WKUP���ѳ�ʼ��
void init_wkup(void)
{	
		
	GPIO_InitTypeDef   GPIO_InitStructure;
	
	EXTI_InitTypeDef    EXTI_InitStructure;
	
	NVIC_InitTypeDef    NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ�� GPIOA ʱ�� 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ�� SYSCFG ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//���� 
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ�� LINE0
	EXTI_Init(&EXTI_InitStructure);//����
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);//PA0 ���ӵ��� 0

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//�ⲿ�ж� 0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ� 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//��Ӧ���ȼ� 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//���� NVIC
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ����� 2
}

