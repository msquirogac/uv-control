/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   task.h
 * Author: tuxihc
 *
 * Created on 27 de marzo de 2019, 1:32
 */

#ifndef TASK_H
#define TASK_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*taskHandler)(void*);

typedef struct
{
    taskHandler func;
    uint8_t limit;
    uint8_t ticks;
    uint8_t enable;
    void *param;
} task_t;

extern uint8_t taskTicks(void);
void taskSetup(task_t *task, taskHandler func, uint8_t limit, void *param);
void taskStart(task_t *task);
void taskStop(task_t *task);
void taskRun(task_t *task);

#ifdef __cplusplus
}
#endif

#endif /* TASK_H */

