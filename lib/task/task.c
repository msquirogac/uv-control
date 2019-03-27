#include <stdint.h>
#include "task.h"

void taskSetup(task_t *task, taskHandler func, uint8_t limit, void *param)
{
    task->func = func;
    task->limit = limit;
    task->param = param;
}

void taskStart(task_t *task)
{
    task->enable = 1;
    task->ticks = taskTicks();
}

void taskStop(task_t *task)
{
    task->enable = 0;
}

void taskRun(task_t *task)
{
    uint8_t cticks = taskTicks();
    if (task->enable)
    {
        if ((uint8_t) (cticks - task->ticks) >= task->limit)
        {
            task->func(task->param);
            task->ticks = cticks;
        }
    }
}