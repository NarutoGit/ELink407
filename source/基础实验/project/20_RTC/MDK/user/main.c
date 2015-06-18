#include "stm32f4xx.h"
#include "delay.h"
#include "uart.h"
#include "rtc.h"

int main()
{
	
	init_uart();
	
	//����systick�ж���ʱ Delay_ms()
	init_delay();
	//SysTick_Configuration();
	
	/*configuration rtc*/
	rtc_time_init();
	
	/*configuration alarm time*/
	rtc_alarm_config();//������rtc��ʼ��֮��
	
	//get current time
	while(1){
		delay_ms(500);
		delay_ms(500);
		get_rtc_time();
		//get_alarm_time();
	}
}
