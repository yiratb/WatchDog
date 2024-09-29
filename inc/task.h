/****************************
Yirat Romano                *
04/03/2024                  *
Daniel                      *
header file                 *
exercise: task              *
*****************************/
/*
This code defines a task management system where tasks are created with 
specified intervals, actions, and cleanup functions.
*/
#ifndef TASK_H
#define TASK_H

#include "uid.h" /* ilrd_uid_t */

/* Define task structure */
typedef struct task task_t;
/* Function pointer type for the action function of a task */
typedef int (*task_action_func_t)(void* param);
/* Function pointer type for the cleanup function of a task */
typedef void (*task_clean_func_t)(void* param);

/**
 * Function: TaskCreate
 * ---------------------
 * Creates a new task.
 * 
 * interval: The interval at which the task should be executed.
 * action: Pointer to the function that represents the action to be performed by the task.
 * action_params: Pointer to the parameters to be passed to the action function.
 * cleanup: Pointer to the function that represents the cleanup action to be performed by the task.
 * cleanup_params: Pointer to the parameters to be passed to the cleanup function.
 * 
 * Returns: A pointer to the newly created task, or NULL if memory allocation fails.
 * 
 * Complexity: O(1)
 * 
 * Warning: action must not be NULL. 
 */
task_t* TaskCreate(size_t interval, task_action_func_t action, 
void *action_params, task_clean_func_t cleanup, void *cleanup_params);

/**
 * Function: TaskDestroy
 * ----------------------
 * Destroys a task, freeing all allocated memory.
 * 
 * task: Pointer to the task to be destroyed.
 * 
 * Complexity: O(1)
 * 
 * Warning: task must not be NULL. 
 */
void TaskDestroy(task_t *task);


/**
 * Function: TaskGetUID
 * ---------------------
 * Retrieves the UID of a task.
 * 
 * task: Pointer to the task.
 * 
 * Returns: The UID of the task.
 * 
 * Complexity: O(1)
 * 
 * Warning: task must not be NULL.
 */
ilrd_uid_t TaskGetUID(const task_t *task);

/* Function: TaskRun
 * ------------------
 * Executes the action function of a task.
 * 
 * task: Pointer to the task.
 * 
 * Returns: The return value of the action function.
 * 
 * Complexity: Depends on the runtime of the function 
 * 
 * Warning: task must not be NULL.
 */
int TaskRun(task_t *task);

/* Function: TaskIsSame
 * ---------------------
 * Checks if two tasks have the same unique identifier.
 * 
 * task1: Pointer to the first task.
 * task2: Pointer to the second task.
 * 
 * Returns: 1 if the tasks have the same unique identifier, 0 otherwise.
 * 
 * Complexity: O(1)
 * 
 * Warning: task1 and task2 must not be NULL.
 */
int TaskIsSame(const task_t *task1, const task_t *task2);

/* Function: TaskGetTimeToRun
 * ---------------------------
 * Retrieves the time when a task is scheduled to run.
 * 
 * task: Pointer to the task.
 * 
 * Returns: The time when the task is scheduled to run.
 * 
 * Complexity: O(1)
 * 
 * Warning: task must not be NULL.
 */
time_t TaskGetTimeToRun(const task_t *task);

/* Function: TaskUpdateTimeToRun
 * ------------------------------
 * Updates the time when a task is scheduled to run.
 * 
 * task: Pointer to the task.
 * 
 * Complexity: O(1)
 * 
 * Warning: task must not be NULL.
 */
void TaskUpdateTimeToRun(task_t *task);

#endif /*TASK_H*/