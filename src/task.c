/****************************
Yirat Romano                *
04/03/2024                  *
Daniel                      *
source file                 *
exercise: task              *
*****************************/
#include <stdlib.h>/*malloc()*/
#include <assert.h> /*assert*/
#include <time.h> /*time*/

#include "task.h" /*task_t*/

struct task
{
    task_action_func_t task_action_func;
    task_clean_func_t task_clean_func;
    void *action_params;
    void *cleanup_params;
    ilrd_uid_t uid;
    size_t interval;
    time_t exec_time;

};

task_t *TaskCreate(size_t interval, task_action_func_t action, 
void *action_params, task_clean_func_t cleanup, void *cleanup_params)
{
    task_t *task;

    assert(action);
    
    task = (task_t *)malloc(sizeof(task_t));
    if (NULL == task)
    {
        return NULL;
    }

    task->uid = UIDGenerate();
    if (UIDIsEqual(task->uid, bad_uid))
    {
        free(task);
        return NULL;
    }

    task->task_action_func = action;
    task->task_clean_func = cleanup;
    task->action_params = action_params;
    task->cleanup_params = cleanup_params;
    task->interval = interval;
    task->exec_time = time(NULL) + interval;

    return task;
} 

void TaskDestroy(task_t *task)
{
    assert(task);

    if(task->task_clean_func)
    {
        task->task_clean_func(task->cleanup_params);
    }

    task->task_action_func = NULL;
    task->task_clean_func = NULL;
    task->action_params = NULL;
    task->cleanup_params = NULL;

    free(task);
}

ilrd_uid_t TaskGetUID(const task_t *task)
{
    assert(task);

    return task->uid;
}

int TaskRun(task_t *task)
{
    int status = 0;
    assert(task);

    status = task->task_action_func(task->action_params);
    
    return status;
}

int TaskIsSame(const task_t *task1, const task_t *task2)
{
    assert(task1);
    assert(task2);

    return (UIDIsEqual(task1->uid, task2->uid));
}

time_t TaskGetTimeToRun(const task_t *task)
{
    assert(task);

    return task->exec_time;
}

void TaskUpdateTimeToRun(task_t *task)
{
    assert(task);

    task->exec_time += task->interval;
}