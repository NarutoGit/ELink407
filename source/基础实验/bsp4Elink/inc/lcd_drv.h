
#ifndef __LCD_H
#define __LCD_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stm32f4xx.h"


#define LCD_W     (240)
#define LCD_H     (400)

//LCD REG
#define LCD_REG_HADDR 0x200
#define LCD_REG_VADDR 0x201
#define LCD_REG_DATA  0x202


/* LCD color */
#define White          0xFFFF //��ɫ
#define Black          0x0000 //��ɫ
#define Grey           0xF7DE //��ɫ
#define Blue           0x001F //��ɫ
#define Blue2          0x051F //����ɫ
#define Red            0xF800 //��ɫ
#define Magenta        0xF81F //Ʒ��
#define Green          0x07E0 //��ɫ
#define Cyan           0x7FFF //����ɫ
#define Yellow         0xFFE0 //��ɫ


void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue);

__inline void LCD_WriteRAM_Prepare(void);
__inline void LCD_REG_Select(uint16_t REG_Code);
__inline void LCD_WriteRAM(uint16_t RGB_Code);


void LCD_Init(void);

void LCD_Clear(uint16_t Color);

#ifdef __cplusplus
}
#endif


#endif /* __LCD_H */


