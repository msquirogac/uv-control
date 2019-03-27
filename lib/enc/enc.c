#include <avr/io.h>
#include <avr/interrupt.h>
#include "enc.h"

inline uint8_t pinstate(volatile uint8_t *port, uint8_t pin, uint8_t mask)
{
    return ((*port & _BV(pin)) ? mask : 0);
}

volatile uint8_t state;
volatile uint8_t bcount;
volatile uint8_t ecount;

void ENC_Init(void)
{
    PCIFR |= _BV(PCIF0);
    PCMSK0 |= _BV(PCINT0);
    PCMSK0 |= _BV(PCINT1);
    PCMSK0 |= _BV(PCINT2);
    PCICR |= _BV(PCIE0);

    DDRB &= ~_BV(0);
    DDRB &= ~_BV(1);
    DDRB &= ~_BV(2);
    state |= pinstate(&PINB, 0, 0x01);
    state |= pinstate(&PINB, 1, 0x02);
    state |= pinstate(&PINB, 2, 0x04);
    sei();
}

ISR(PCINT0_vect)
{
    uint8_t tmp = 0x00;
    tmp |= pinstate(&PINB, 0, 0x01);
    tmp |= pinstate(&PINB, 1, 0x02);
    tmp |= pinstate(&PINB, 2, 0x04);
    uint8_t iflag = state ^ tmp;
    state = tmp;
    if (iflag & 0x01)
    {
        if (state & 0x01)
        {
            bcount++;
        }
    }
    if (iflag & 0x02)
    {
        if (state & 0x02)
        {
            if (state & 0x04)
            {
                ecount--;
            }
        }
    }
    if (iflag & 0x04)
    {
        if (state & 0x04)
        {
            if (state & 0x02)
            {
                ecount++;
            }
        }
    }
}

inline uint8_t ENC_Button(void)
{
    return bcount;
}

inline uint8_t ENC_Encoder(void)
{
    return ecount;
}

uint8_t ENC_GetButton(void)
{
    static uint8_t button;
    uint8_t cbutton = ENC_Button();
    uint8_t bflag = (cbutton != button);
    button = cbutton;
    return bflag;
}

uint8_t ENC_GetEncoder(void)
{
    static uint8_t encoder;
    uint8_t cencoder = ENC_Encoder();
    uint8_t eflag = (cencoder != encoder) ? 0x01 : 0x00;
    uint8_t edir = ((uint8_t) (encoder - cencoder) > 1) ? 0x02 : 0x00;
    encoder = cencoder;
    return eflag | edir;
}