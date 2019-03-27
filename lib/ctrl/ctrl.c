#include <avr/io.h>
#include <util/delay.h>
#include "ctrl.h"

void CTRL_Init(void)
{
    DDRC |= _BV(5);
    PORTC &= ~_BV(5);
}

void CTRL_Set(void)
{
    PORTC |= _BV(5);
}

void CTRL_Reset(void)
{
    PORTC &= ~_BV(5);
}