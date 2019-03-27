/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   lcdcfg.h
 * Author: tuxihc
 *
 * Created on 22 de marzo de 2019, 12:56
 */

#ifndef LCDCFG_H
#define LCDCFG_H

#ifdef __cplusplus
extern "C" {
#endif

#define LCD_RS_DDR  DDRD
#define LCD_RW_DDR  DDRD
#define LCD_EN_DDR  DDRD
#define LCD_DB4_DDR DDRB
#define LCD_DB5_DDR DDRB
#define LCD_DB6_DDR DDRD
#define LCD_DB7_DDR DDRD
#define LCD_RS_WREG  PORTD
#define LCD_RW_WREG  PORTD
#define LCD_EN_WREG  PORTD
#define LCD_DB4_WREG PORTB
#define LCD_DB5_WREG PORTB
#define LCD_DB6_WREG PORTD
#define LCD_DB7_WREG PORTD
#define LCD_RS_RREG  PIND
#define LCD_RW_RREG  PIND
#define LCD_EN_RREG  PIND
#define LCD_DB4_RREG PINB
#define LCD_DB5_RREG PINB
#define LCD_DB6_RREG PIND
#define LCD_DB7_RREG PIND
#define LCD_RS_BIT  _BV(0)
#define LCD_RW_BIT  _BV(1)
#define LCD_EN_BIT  _BV(2)
#define LCD_DB4_BIT _BV(6)
#define LCD_DB5_BIT _BV(7)
#define LCD_DB6_BIT _BV(5)
#define LCD_DB7_BIT _BV(6)

#ifdef __cplusplus
}
#endif

#endif /* LCDCFG_H */

