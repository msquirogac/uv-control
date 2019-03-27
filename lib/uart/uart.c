#include "uart.h"
#include "uartcfg.h"
#include <avr/io.h>
#include <util/delay.h>

#define TX_BIT _BV(UART_TX_BIT)
#define BIT_US (1000000/UART_BAUD) /*!< Period */

void UART_init()
{
    UART_TX_DDR |= TX_BIT;
    UART_TX_PORT |= TX_BIT;
}

void UART_send(uint8_t byte)
{
    uint8_t mask = 1;
    /* Start bit */
    UART_TX_PORT &= ~TX_BIT;
    _delay_us(BIT_US);
    while (mask)
    {
        if (mask & byte)
            UART_TX_PORT |= TX_BIT;
        else
            UART_TX_PORT &= ~TX_BIT;
        _delay_us(BIT_US);
        mask <<= 1;
    }
    /* Stop bit */
    UART_TX_PORT |= TX_BIT;
    _delay_us(BIT_US);
}
