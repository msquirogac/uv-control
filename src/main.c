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

typedef enum
{
    STATE1,
    STATE11,
    STATE2,
    STATE21,
    STATE22,
    STATE221,
    STATE23,
    STATE231,
    STATE24,
    STATE241,
    STATE25,
    STATE251,
    STATE26,
    STATE_MAX
} state_t;

typedef enum
{
    EVENTNONE,
    EVENT1,
    EVENT2,
    EVENT3,
    EVENT_MAX
} event_t;

typedef struct
{
    uint8_t flag;
    int16_t rtime;
    int16_t tseg;
} param_t;

uint8_t display[2][18];
#define fill "                "
#define TIME_MAX 30000
#define STEP1 1
#define STEP2 10
#define STEP3 100
#define STEP4 1000

int16_t limit(int16_t min, int16_t max, int16_t var)
{
    var = (var < min) ? min : var;
    var = (var > max) ? max : var;
    return var;
}

typedef state_t(*stateHandler)(void*);

/******************************************************************************/
state_t state1e0(void *param)
{
    param_t *msg = param;
    (void) msg;
    snprintf((char*) display[0], 17, ">Run%s", fill);
    snprintf((char*) display[1], 17, " Set time%s", fill);
    return STATE1;
}

state_t state1e1(void *param)
{
    param_t *msg = param;
    msg->flag = 1;
    msg->rtime = msg->tseg;
    return STATE11;
}

state_t state1e2(void *param)
{
    param_t *msg = param;
    (void) msg;
    return STATE2;
}

#define state0e3 state1e2

/******************************************************************************/
state_t state11e0(void *param)
{
    param_t *msg = param;
    snprintf((char*) display[0], 17, "Set:  %5d seg%s", msg->tseg, fill);
    snprintf((char*) display[1], 17, "Left: %5d seg%s", msg->rtime, fill);
    return STATE11;
}

state_t state11e1(void *param)
{
    param_t *msg = param;
    msg->flag = 0;
    msg->rtime = 0;
    return STATE1;
}

/******************************************************************************/
state_t state2e0(void *param)
{
    param_t *msg = param;
    (void) msg;
    snprintf((char*) display[0], 17, " Run%s", fill);
    snprintf((char*) display[1], 17, ">Set time%s", fill);
    return STATE2;
}

state_t state2e1(void *param)
{
    param_t *msg = param;
    (void) msg;
    return STATE21;
}

state_t state2e2(void *param)
{
    param_t *msg = param;
    (void) msg;
    return STATE1;
}

#define state2e3 state2e2

/******************************************************************************/
state_t state21e0(void *param)
{
    param_t *msg = param;
    snprintf((char*) display[0], 17, "Set:  %5d seg%s", msg->tseg, fill);
    snprintf((char*) display[1], 17, ">Back%s", fill);
    return STATE21;
}

state_t state21e1(void *param)
{
    param_t *msg = param;
    (void) msg;
    return STATE2;
}

state_t state21e2(void *param)
{
    param_t *msg = param;
    (void) msg;
    return STATE22;
}

state_t state21e3(void *param)
{
    param_t *msg = param;
    (void) msg;
    return STATE26;
}

/******************************************************************************/
state_t state22e0(void *param)
{
    param_t *msg = param;
    snprintf((char*) display[0], 17, "Set:  %5d seg%s", msg->tseg, fill);
    snprintf((char*) display[1], 17, ">1x%s", fill);
    return STATE22;
}

state_t state22e1(void *param)
{
    param_t *msg = param;
    (void) msg;
    return STATE221;
}

state_t state22e2(void *param)
{
    param_t *msg = param;
    (void) msg;
    return STATE23;
}

state_t state22e3(void *param)
{
    param_t *msg = param;
    (void) msg;
    return STATE21;
}

/******************************************************************************/
state_t state221e0(void *param)
{
    param_t *msg = param;
    snprintf((char*) display[0], 17, "Set:  %5d seg%s", msg->tseg, fill);
    snprintf((char*) display[1], 17, "Step:     1%s", fill);
    return STATE221;
}

state_t state221e1(void *param)
{
    param_t *msg = param;
    (void) msg;
    return STATE22;
}

state_t state221e2(void *param)
{
    param_t *msg = param;
    msg->tseg = msg->tseg + STEP1;
    msg->tseg = limit(0, TIME_MAX, msg->tseg);
    return STATE221;
}

state_t state221e3(void *param)
{
    param_t *msg = param;
    msg->tseg = msg->tseg - STEP1;
    msg->tseg = limit(0, TIME_MAX, msg->tseg);
    return STATE221;
}

/******************************************************************************/
state_t state23e0(void *param)
{
    param_t *msg = param;
    snprintf((char*) display[0], 17, "Set:  %5d seg%s", msg->tseg, fill);
    snprintf((char*) display[1], 17, ">10x%s", fill);
    return STATE23;
}

state_t state23e1(void *param)
{
    param_t *msg = param;
    (void) msg;
    return STATE231;
}

state_t state23e2(void *param)
{
    param_t *msg = param;
    (void) msg;
    return STATE24;
}

state_t state23e3(void *param)
{
    param_t *msg = param;
    (void) msg;
    return STATE22;
}

/******************************************************************************/
state_t state231e0(void *param)
{
    param_t *msg = param;
    snprintf((char*) display[0], 17, "Set:  %5d seg%s", msg->tseg, fill);
    snprintf((char*) display[1], 17, "Step:    10%s", fill);
    return STATE231;
}

state_t state231e1(void *param)
{
    param_t *msg = param;
    (void) msg;
    return STATE23;
}

state_t state231e2(void *param)
{
    param_t *msg = param;
    msg->tseg = msg->tseg + STEP2;
    msg->tseg = limit(0, TIME_MAX, msg->tseg);
    return STATE231;
}

state_t state231e3(void *param)
{
    param_t *msg = param;
    msg->tseg = msg->tseg - STEP2;
    msg->tseg = limit(0, TIME_MAX, msg->tseg);
    return STATE231;
}

/******************************************************************************/
state_t state24e0(void *param)
{
    param_t *msg = param;
    snprintf((char*) display[0], 17, "Set:  %5d seg%s", msg->tseg, fill);
    snprintf((char*) display[1], 17, ">100x%s", fill);
    return STATE24;
}

state_t state24e1(void *param)
{
    param_t *msg = param;
    (void) msg;
    return STATE241;
}

state_t state24e2(void *param)
{
    param_t *msg = param;
    (void) msg;
    return STATE25;
}

state_t state24e3(void *param)
{
    param_t *msg = param;
    (void) msg;
    return STATE23;
}

/******************************************************************************/
state_t state241e0(void *param)
{
    param_t *msg = param;
    snprintf((char*) display[0], 17, "Set:  %5d seg%s", msg->tseg, fill);
    snprintf((char*) display[1], 17, "Step:   100%s", fill);
    return STATE241;
}

state_t state241e1(void *param)
{
    param_t *msg = param;
    (void) msg;
    return STATE24;
}

state_t state241e2(void *param)
{
    param_t *msg = param;
    msg->tseg = msg->tseg + STEP3;
    msg->tseg = limit(0, TIME_MAX, msg->tseg);
    return STATE241;
}

state_t state241e3(void *param)
{
    param_t *msg = param;
    msg->tseg = msg->tseg - STEP3;
    msg->tseg = limit(0, TIME_MAX, msg->tseg);
    return STATE241;
}

/******************************************************************************/
state_t state25e0(void *param)
{
    param_t *msg = param;
    snprintf((char*) display[0], 17, "Set:  %5d seg%s", msg->tseg, fill);
    snprintf((char*) display[1], 17, ">1000x%s", fill);
    return STATE25;
}

state_t state25e1(void *param)
{
    param_t *msg = param;
    (void) msg;
    return STATE251;
}

state_t state25e2(void *param)
{
    param_t *msg = param;
    (void) msg;
    return STATE26;
}

state_t state25e3(void *param)
{
    param_t *msg = param;
    (void) msg;
    return STATE24;
}

/******************************************************************************/
state_t state251e0(void *param)
{
    param_t *msg = param;
    snprintf((char*) display[0], 17, "Set:  %5d seg%s", msg->tseg, fill);
    snprintf((char*) display[1], 17, "Step:  1000%s", fill);
    return STATE251;
}

state_t state251e1(void *param)
{
    param_t *msg = param;
    (void) msg;
    return STATE25;
}

state_t state251e2(void *param)
{
    param_t *msg = param;
    msg->tseg = msg->tseg + STEP4;
    msg->tseg = limit(0, TIME_MAX, msg->tseg);
    return STATE251;
}

state_t state251e3(void *param)
{
    param_t *msg = param;
    msg->tseg = msg->tseg - STEP4;
    msg->tseg = limit(0, TIME_MAX, msg->tseg);
    return STATE251;
}

/******************************************************************************/
state_t state26e0(void *param)
{
    param_t *msg = param;
    snprintf((char*) display[0], 17, "Set:  %5d seg%s", msg->tseg, fill);
    snprintf((char*) display[1], 17, ">Clear%s", fill);
    return STATE26;
}

state_t state26e1(void *param)
{
    param_t *msg = param;
    msg->tseg = 0;
    return STATE26;
}

state_t state26e2(void *param)
{
    param_t *msg = param;
    (void) msg;
    return STATE21;
}

state_t state26e3(void *param)
{
    param_t *msg = param;
    (void) msg;
    return STATE25;
}

/******************************************************************************/
stateHandler stateVector[STATE_MAX][EVENT_MAX] = {
    {state1e0, state1e1, state1e2, state0e3},
    {state11e0, state11e1, state11e0, state11e0},
    {state2e0, state2e1, state2e2, state2e3},
    {state21e0, state21e1, state21e2, state21e3},
    {state22e0, state22e1, state22e2, state22e3},
    {state221e0, state221e1, state221e2, state221e3},
    {state23e0, state23e1, state23e2, state23e3},
    {state231e0, state231e1, state231e2, state231e3},
    {state24e0, state24e1, state24e2, state24e3},
    {state241e0, state241e1, state241e2, state241e3},
    {state25e0, state25e1, state25e2, state25e3},
    {state251e0, state251e1, state251e2, state251e3},
    {state26e0, state26e1, state26e2, state26e3},
};

void core_task(void *param)
{
    static uint8_t state = STATE1;
    uint8_t bt = ENC_GetButton();
    uint8_t enc = ENC_GetEncoder();
    uint8_t event;
    event = (bt & 0x01) ? EVENT1 : ((enc & 0x01) ? ((enc & 0x02) ? EVENT2 : EVENT3) : EVENTNONE);
    state = (*stateVector[state][event])(param);
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
            msg->rtime = limit(0, TIME_MAX, msg->rtime);
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

    param_t msg = {0, 0, 0};
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

