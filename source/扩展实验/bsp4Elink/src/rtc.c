#include "rtc.h"
#include <stdio.h>


/* RTC Configuration ******************************************************/
void rtc_config(void)
{	
	/* RTC_ReadBackupRegister(RTC_BKP_DR0) */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);//��PWRʱ��
	
	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);//�����üĴ���Ȩ��
	
	/* Clear Wakeup flag */
	PWR_ClearFlag(PWR_FLAG_WU);

    /* Reset Backup Domain */
    RCC_BackupResetCmd(ENABLE);
    RCC_BackupResetCmd(DISABLE);
	
	
    /* Enable the LSE */ 
    RCC_LSEConfig(RCC_LSE_ON);//ʹ���ⲿ����
  	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET){}//�ȴ��ⲿ����׼����
	
    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//ѡ��RTCʱ��Դ  
	
    /* Enable the RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC APB registers synchronisation (needed after start-up from Reset)*/
    RTC_WaitForSynchro();//�ȴ�RTC APB�Ĵ���ͬ��
		
}


void rtc_time_init(void)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	RTC_InitTypeDef RTC_InitStructure;
	struct RtcTime time = {0x14,0x09,0x10,0x03,0x0,0x0,0x0};
	
	//if the time not set(first boot)
	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2){
		//printf("init...\r\n");
		rtc_config();
		RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
		RTC_TimeStructure.RTC_Hours = time.hour;
		RTC_TimeStructure.RTC_Minutes = time.min;
		RTC_TimeStructure.RTC_Seconds = time.sec;
		RTC_DateStructure.RTC_Year = time.year;
		RTC_DateStructure.RTC_Month = time.month;
		RTC_DateStructure.RTC_Date = time.date;
		RTC_DateStructure.RTC_WeekDay = time.week;
	
		// prescaler factor
		RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
		RTC_InitStructure.RTC_SynchPrediv = 0XFF;
		
		// Specifies the RTC Hour Format
		RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	
		RTC_Init(&RTC_InitStructure);
		RTC_SetTime(RTC_Format_BCD,&RTC_TimeStructure);
		RTC_SetDate(RTC_Format_BCD,&RTC_DateStructure);
		
		//Writes a data in a specified RTC Backup data register
		RTC_WriteBackupRegister(RTC_BKP_DR0,0x32F2);
	}//the time already set(reboot)
	else{   
		//printf("reset..\r\n");
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	    PWR_BackupAccessCmd(ENABLE);
	    RTC_WaitForSynchro();
  	}
	
}

void get_rtc_time(void)
{
	struct RtcTime CurrentTime;
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	
	RTC_GetTime(RTC_Format_BCD,&RTC_TimeStructure); //������
	RTC_GetDate(RTC_Format_BCD,&RTC_DateStructure); //��ʱ��
	
	CurrentTime.hour = RTC_TimeStructure.RTC_Hours;
	CurrentTime.min = RTC_TimeStructure.RTC_Minutes;
	CurrentTime.sec = RTC_TimeStructure.RTC_Seconds;
	CurrentTime.year = RTC_DateStructure.RTC_Year;
	CurrentTime.month = RTC_DateStructure.RTC_Month;
	CurrentTime.date = RTC_DateStructure.RTC_Date;
	CurrentTime.week = RTC_DateStructure.RTC_WeekDay;
	
	printf("20%02x-%02x-%02x %02x:%02x:%02x\r\n",CurrentTime.year,
		CurrentTime.month,CurrentTime.date,CurrentTime.hour,CurrentTime.min,CurrentTime.sec);
}


void rtc_alarm_config(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RTC_AlarmTypeDef RTC_AlarmStructure;

	/* ��ʼ���ⲿ�ж�17���ο������ֲᣩ */
	EXTI_ClearITPendingBit(EXTI_Line17);
	EXTI_InitStructure.EXTI_Line=EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* ��ʼ��RTC_Alarm_IRQn�ж� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* RTC���ӳ�ʼ���� ÿ��5�����Ӵ��� */
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE); 
	RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x10;
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
	
	/* ��ѡ���������ڣ����ں�ʱ�䣬���ȫ�����Σ�ÿ��һ�����һ������ */
	//RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
	RTC_AlarmStructure.RTC_AlarmMask=RTC_AlarmMask_DateWeekDay| RTC_AlarmMask_Hours|RTC_AlarmMask_Minutes;

	/* ���ñ���ʱ�� */
	RTC_AlarmStructure.RTC_AlarmTime.RTC_H12 = RTC_H12_AM;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = 0x0;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0x0;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0x5;
	RTC_SetAlarm(RTC_Format_BCD,RTC_Alarm_A,&RTC_AlarmStructure);
	
	/* ����RTC�ж�Դ */
	RTC_ITConfig(RTC_IT_ALRA,ENABLE);
	/* ʹ��RTC�����ж�*/
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
	
	/* ������ӵȴ� */
	RTC_ClearFlag(RTC_FLAG_ALRAF);
	/* ����ж���17�ȴ����ڴ�����������*/
	EXTI_ClearITPendingBit(EXTI_Line17);

	get_alarm_time();
}

void get_alarm_time(void)
{
	RTC_AlarmTypeDef RTC_AlarmStruct;
	
	RTC_GetAlarm(RTC_Format_BCD,RTC_Alarm_A, &RTC_AlarmStruct);
	printf("The alarm time %02x:%02x:%02x\r\n",RTC_AlarmStruct.RTC_AlarmTime.RTC_Hours,
			RTC_AlarmStruct.RTC_AlarmTime.RTC_Minutes,RTC_AlarmStruct.RTC_AlarmTime.RTC_Seconds);
}


void RTC_Alarm_IRQHandler(void)
{
	//����Ƿ�ָ�����жϷ���
	if(RTC_GetITStatus(RTC_IT_ALRA)!=RESET)
	{
		printf("alarm interrupt handler:");
		get_rtc_time(); 
		PWR_BackupAccessCmd(ENABLE);//�����У�����RTC_ClearITPendingBit ����ɹ�
		RTC_ClearITPendingBit(RTC_IT_ALRA); 
		PWR_BackupAccessCmd(DISABLE); 
		EXTI_ClearITPendingBit(EXTI_Line17);
	}
}
