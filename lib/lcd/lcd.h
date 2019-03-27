/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   lcd.h
 * Author: tuxihc
 *
 * Created on 22 de marzo de 2019, 12:57
 */

#ifndef LCD_H
#define LCD_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LCD_CLEAR    0x01
#define LCD_HOME     0x02
#define LCD_MODE     0x04
#define LCD_POWER    0x08
#define LCD_MOVE     0x10
#define LCD_FUNC     0x20
#define LCD_CGRAM    0x40
#define LCD_DDRAM    0x80

#define LCD_MODE_S   0x01
#define LCD_MODE_I   0x02
#define LCD_POWER_B  0x01
#define LCD_POWER_C  0x02
#define LCD_POWER_D  0x04
#define LCD_MOVE_R   0x04
#define LCD_MOVE_S   0x08
#define LCD_FUNC_DL  0x10
#define LCD_FUNC_N   0x08
#define LCD_FUNC_F   0x04
  
void LCD_Init(void);
void LCD_W4Bits(uint8_t mode, uint8_t data);
uint8_t LCD_R4Bits(uint8_t mode);
void LCD_WConfig(void);

void LCD_Command(uint8_t cmd);
uint8_t LCD_Status(void);
void LCD_WriteRegister(uint8_t data);
uint8_t LCD_ReadRegister(void);

void LCD_SetCursor(uint8_t address);
void LCD_ClearDisplay(void);
void LCD_ReturnHome(void);

void LCD_PutChar(uint8_t data);
uint8_t LCD_GetChar(void);

#ifdef __cplusplus
}
#endif

#endif /* LCD_H */

