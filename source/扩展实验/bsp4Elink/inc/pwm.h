#ifndef __PWM_H_
#define __PWM_H_

#include "stm32f4xx.h"

//arr���Զ���װֵ  psc��ʱ��Ԥ��Ƶ�� 
//TIM14_PWM_Init(500-1,84-1);  //��ʱ��ʱ��Ϊ168M����Ƶϵ��Ϊsc�����Լ���Ƶ�� 
//Ϊ168/psc,��װ��ֵarr������PWMƵ��Ϊ168/(psc+1)/(arr+1)  

void timer_pwm_init(u32 arr,u32 psc);


#endif
