#ifndef __WKUP_H
#define __WKUP_H

#include "stm32f4xx.h"
  
u8 check_wkup(void); //���PA0�ŵ��ź�
void init_wkup(void); //PA0 WKUP���ѳ�ʼ��
void StandbyMode(void);//ϵͳ�������ģʽ

#endif

