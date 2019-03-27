#include "lcd.h"
#include "lcdcfg.h"
#include <avr/io.h>
#include <util/delay.h>

void LCD_UpdateBusMode(uint8_t mode)
{
    if (mode)
    {
        LCD_DB4_DDR &= ~LCD_DB4_BIT;
        LCD_DB5_DDR &= ~LCD_DB5_BIT;
        LCD_DB6_DDR &= ~LCD_DB6_BIT;
        LCD_DB7_DDR &= ~LCD_DB7_BIT;
    }
    else
    {
        LCD_DB4_DDR |= LCD_DB4_BIT;
        LCD_DB5_DDR |= LCD_DB5_BIT;
        LCD_DB6_DDR |= LCD_DB6_BIT;
        LCD_DB7_DDR |= LCD_DB7_BIT;
    }
}

void LCD_Init(void)
{
    LCD_RS_DDR |= LCD_RS_BIT;
    LCD_RW_DDR |= LCD_RW_BIT;
    LCD_EN_DDR |= LCD_EN_BIT;
    LCD_UpdateBusMode(0);
    LCD_RS_WREG &= ~LCD_RS_BIT;
    LCD_RW_WREG &= ~LCD_RW_BIT;
    LCD_EN_WREG &= ~LCD_EN_BIT;
    LCD_DB4_WREG &= ~LCD_DB4_BIT;
    LCD_DB5_WREG &= ~LCD_DB5_BIT;
    LCD_DB6_WREG &= ~LCD_DB6_BIT;
    LCD_DB7_WREG &= ~LCD_DB7_BIT;
}

void LCD_W4Bits(uint8_t mode, uint8_t data)
{
    uint32_t tmp = (data & 0x0f);
    LCD_RW_WREG &= ~LCD_RW_BIT;
    LCD_UpdateBusMode(0);
    if (mode)
        LCD_RS_WREG |= LCD_RS_BIT;
    else
        LCD_RS_WREG &= ~LCD_RS_BIT;

    LCD_DB4_WREG &= ~LCD_DB4_BIT;
    LCD_DB5_WREG &= ~LCD_DB5_BIT;
    LCD_DB6_WREG &= ~LCD_DB6_BIT;
    LCD_DB7_WREG &= ~LCD_DB7_BIT;
    LCD_EN_WREG |= LCD_EN_BIT;
    LCD_DB4_WREG |= (tmp & 0x01) ? LCD_DB4_BIT : 0;
    LCD_DB5_WREG |= (tmp & 0x02) ? LCD_DB5_BIT : 0;
    LCD_DB6_WREG |= (tmp & 0x04) ? LCD_DB6_BIT : 0;
    LCD_DB7_WREG |= (tmp & 0x08) ? LCD_DB7_BIT : 0;
    LCD_EN_WREG &= ~LCD_EN_BIT;
}

uint8_t LCD_R4Bits(uint8_t mode)
{
    uint8_t tmp = 0x00;
    LCD_RW_WREG |= LCD_RW_BIT;
    LCD_UpdateBusMode(1);
    if (mode)
        LCD_RS_WREG |= LCD_RS_BIT;
    else
        LCD_RS_WREG &= ~LCD_RS_BIT;

    LCD_EN_WREG |= LCD_EN_BIT;
    _delay_us(2);
    tmp |= (LCD_DB4_RREG & LCD_DB4_BIT) ? 0x01 : 0;
    tmp |= (LCD_DB5_RREG & LCD_DB5_BIT) ? 0x02 : 0;
    tmp |= (LCD_DB6_RREG & LCD_DB6_BIT) ? 0x04 : 0;
    tmp |= (LCD_DB7_RREG & LCD_DB7_BIT) ? 0x08 : 0;
    LCD_EN_WREG &= ~LCD_EN_BIT;
    return tmp;
}

void LCD_WConfig(void)
{
    LCD_W4Bits(0, 0x03);
    _delay_ms(10);
    LCD_W4Bits(0, 0x03);
    _delay_ms(10);
    LCD_W4Bits(0, 0x03);
    _delay_ms(10);
    LCD_W4Bits(0, 0x02);
    _delay_ms(1);

    LCD_Command(LCD_FUNC | LCD_FUNC_N);
    _delay_ms(1);
    LCD_Command(0x00);
    _delay_ms(1);
    LCD_Command(LCD_POWER | LCD_POWER_D);
    _delay_ms(1);
    LCD_Command(0x00);
    _delay_ms(1);
    LCD_Command(LCD_CLEAR);
    _delay_ms(1);
}

void LCD_Command(uint8_t cmd)
{
    LCD_W4Bits(0, cmd >> 4);
    LCD_W4Bits(0, cmd >> 0);
}

uint8_t LCD_Status(void)
{
    uint8_t tmp = 0x00;
    tmp |= (LCD_R4Bits(0) << 4);
    tmp |= (LCD_R4Bits(0) << 0);
    return tmp;
}

void LCD_WriteRegister(uint8_t data)
{
    LCD_W4Bits(1, data >> 4);
    LCD_W4Bits(1, data >> 0);
}

uint8_t LCD_ReadRegister(void)
{
    uint8_t tmp = 0x00;
    tmp |= (LCD_R4Bits(1) << 4);
    tmp |= (LCD_R4Bits(1) << 0);
    return tmp;
}

void LCD_ClearDisplay(void)
{
    LCD_Command(LCD_CLEAR);
    _delay_ms(2);
}

void LCD_ReturnHome(void)
{
    LCD_Command(LCD_HOME);
    _delay_ms(2);
}

void LCD_SetCursor(uint8_t address)
{
    LCD_Command(LCD_DDRAM | address);
    _delay_us(50);
}

void LCD_PutChar(uint8_t data)
{
    LCD_WriteRegister(data);
    _delay_us(50);
}

uint8_t LCD_GetChar(void)
{
    return LCD_ReadRegister();
}