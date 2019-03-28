#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <uart.h>
#include <ctrl.h>
#include <lcd.h>
#include <enc.h>
#include <task.h>
#include <stdio.h>
#include <string.h>

#define DEBUG

void print(void *data)
{
#ifdef DEBUG
    uint8_t *tmpreg = data;
    while (*tmpreg != 0x00)
    {
        UART_send(*tmpreg);
        tmpreg++;
    }
#endif
}

void TIM_init(void)
{
    TCCR1A |= 0;
    TCCR1B |= _BV(WGM12) | _BV(CS01);
    TCNT1 = 0;
    OCR1A = 1023;
    TIMSK1 |= _BV(OCIE1A);
    sei();
}

volatile uint8_t ticks;

ISR(TIMER1_COMPA_vect)
{
    ticks++;
}

uint8_t taskTicks(void)
{
    return ticks;
}

enum
{
    STATE0,
    STATE0A,
    STATE1,
    STATE1A,
} menu_t;

typedef struct
{
    uint8_t flag;
    int16_t rtime;
} param_t;

uint8_t display[2][18];
#define fill "                "

void core_task(void *param)
{
    param_t *msg = param;
    static uint8_t state = STATE0;
    static int16_t tseg;

    uint8_t bt = ENC_GetButton();
    uint8_t enc = ENC_GetEncoder();
    uint8_t bflag = bt & 0x01;
    uint8_t eflag = enc & 0x01;
    uint8_t edir = enc & 0x02;
    switch (state)
    {
    case STATE0:
    {
        snprintf((char*) display[0], 17, ">Run%s", fill);
        snprintf((char*) display[1], 17, " Set time%s", fill);
        if (bflag)
        {
            msg->flag = 1;
            msg->rtime = tseg;
            state = STATE0A;
        }
        if (eflag)
        {
            if (edir)
                state = STATE1;
            else
                state = STATE1;
        }
        break;
    }
    case STATE0A:
    {
        snprintf((char*) display[0], 17, "Left: %5d seg%s", msg->rtime, fill);
        snprintf((char*) display[1], 17, "Set:  %5d seg%s", tseg, fill);
        if (bflag)
        {
            msg->flag = 0;
            msg->rtime = 0;
            state = STATE0;
        }
        break;
    }
    case STATE1:
    {
        snprintf((char*) display[0], 17, " Run%s", fill);
        snprintf((char*) display[1], 17, ">Set time%s", fill);
        if (bflag)
        {
            state = STATE1A;
        }
        if (eflag)
        {
            if (edir)
                state = STATE0;
            else
                state = STATE0;
        }
        break;
    }
    case STATE1A:
    {
        snprintf((char*) display[0], 17, "%s", fill);
        snprintf((char*) display[1], 17, "Set:  %5d seg%s", tseg, fill);;
        if (bflag)
        {
            state = STATE1;
        }
        if (eflag)
        {
            if (edir)
                tseg++;
            else
                tseg--;
            tseg = (tseg < 0) ? 0 : tseg;
            tseg = (tseg > 1000) ? 1000 : tseg;
        }
        break;
    }
    }
}

void ctrl_task(void *param)
{
    param_t *msg = param;
    static uint8_t count;
    if (msg->flag && msg->rtime)
    {
        CTRL_Set();
        count = (count + 1) % 10;
        if (!count)
        {
            msg->rtime--;
            msg->rtime = (msg->rtime < 0) ? 0 : msg->rtime;
        }
    }
    else
    {
        CTRL_Reset();
        count = 0;
    }
}

void lcd_task(void *param)
{
    LCD_ClearDisplay();
    LCD_ReturnHome();
    for (size_t i = 0; i < 16; i++)
    {
        LCD_PutChar(display[0][i]);
    }

    LCD_SetCursor(0x40);
    for (size_t i = 0; i < 16; i++)
    {
        LCD_PutChar(display[1][i]);
    }
}

void print_task(void *param)
{
    print(">");
    print(display[0]);
    print("<>");
    print(display[1]);
    print(">");
    print("\r");
}

task_t task0, task1, task2, task3;

int main(void)
{
    UART_init();
    CTRL_Init();
    TIM_init();
    ENC_Init();
    LCD_Init();
    LCD_WConfig();
    print("\n");

    uint8_t msg;
    taskSetup(&task0, core_task, 20, &msg);
    taskSetup(&task1, ctrl_task, 100, &msg);
    taskSetup(&task2, lcd_task, 250, NULL);
    taskSetup(&task3, print_task, 100, NULL);
    taskStart(&task0);
    taskStart(&task1);
    taskStart(&task2);
    taskStart(&task3);
    while (1)
    {
        taskRun(&task0);
        taskRun(&task1);
        taskRun(&task2);
        taskRun(&task3);
    }
}

