/*//////////////////////////////////
File Name: wd_client.c
File Type: source
Made By: Yirat Romano
Review By: Itamar
Last Modefy Date: 20/04/24
////////////////////////////////////*/

#define _POSIX_C_SOURCE 200809L

#include <sys/wait.h>  /*wait*/
#include <stdlib.h> /*NULL env*/
#include <unistd.h> /*fork getpid*/
#include <signal.h> /*SIGUSR1*/
#include <assert.h> /*assert*/
#include <string.h> /*strcmp*/
#include <pthread.h> /*pthread_t*/
#include <stdio.h> /*printf for debug*/
#include <semaphore.h> /*sem_t*/
#include <stdatomic.h> /*atomic_int*/
#include <fcntl.h> 

#include "scheduler.h" /*scheduler_t*/
#include "uid.h" /*ilrd_uid_t*/
#include "wd_client.h" /*SUCCESS_WD*/

#ifndef NDEBUG
    #define DEBUG_EXPR(x) (x) 
#else
    #define DEBUG_EXPR(x) 
#endif

#define USER_SEM_NAME ("USER_SEMA")
#define WD_SEM_NAME ("WD_SEMA")
#define FILE_NAME_WD ("./wd")
#define CAPACITY_FAIL (5)

typedef enum side_sem
{
    CURR_SEM = 0,
    OTHER_SEM
} side_sem_t;


pthread_t thread;
pid_t id_of_other_proc = 0; /*the pid of the other side - where to send signals*/
atomic_int stop_flag = NOT_FINISH; /*check if the critical section done success*/
atomic_int counter_fail = 0; /*counter how many times the signal not recive from the other side*/
scheduler_t *sched = NULL;
sem_t *semaphores[2];
char file_name[30] = "";

/*****************Helper_Function***************/
static void *StartRun(void *argc);
static scheduler_t *CreateSchedAndTask(const char **cmd);
static void InitSigaction(struct sigaction sa_usr1, struct sigaction sa_usr2);
static wd_status_t Revive(const char **cmd);
static wd_status_t Fork(const char **cmd);
static wd_status_t InitSemaphores();
static void DestroySemaphores();

/****************Task***************/
static int SendSignal1(void *param);
static int CheckCounter(void *param);
static int StopRun(void *param);

/****************Handler***************/
static void handle_sigusr1(int signo, siginfo_t *info, void *context);
static void handle_sigusr2(int signo, siginfo_t *info, void *context);

wd_status_t WDStart(const char **cmd)
{
    wd_status_t status = WDSUCCESS;
    struct sigaction sa_usr1 = {0}, sa_usr2 = {0};

    assert(cmd);
    assert(cmd[0]);

    strcpy(file_name, cmd[0]);
    DEBUG_EXPR(printf("%s pid: %d\n", file_name, getpid()));

    InitSigaction(sa_usr1, sa_usr2);
    if (WDFAILURE == InitSemaphores())
    {
        return (WDFAILURE);
    }

    sched = CreateSchedAndTask(cmd);
    if (NULL == sched)
    {
        DEBUG_EXPR(printf("create sched fail\n"));
        DestroySemaphores();
        return (WDFAILURE);
    }
    
    if (0 == strcmp(cmd[0], FILE_NAME_WD))
    {
        id_of_other_proc = getppid();

        StartRun(NULL);
    }
    else
    {
        /*if is eqaul to NULL its mean that we do restart to 
        the user, and that we not need to do fork to WD becuase he exict*/
        if (NULL == getenv("WD_PID"))
        {
            status = Fork(cmd);
            if (WDFAILURE == status)
            {
                DestroySemaphores();
                SchedDestroy(sched);

                return (WDFAILURE);
            }
        }
        else
        {
            id_of_other_proc = getppid();
        }

        status = pthread_create(&thread, NULL, &StartRun, NULL);
        if (WDSUCCESS != status)
        {
            DestroySemaphores();
            SchedDestroy(sched);

            DEBUG_EXPR(printf("Pthread create fail\n"));
            return (WDFAILURE);
        }
    }
    return (WDSUCCESS);
}

void WDStop(void)
{
    DEBUG_EXPR(printf("%d WDStop - %d\n", getpid(), id_of_other_proc));

    kill(id_of_other_proc, SIGUSR2);

    sem_wait(semaphores[CURR_SEM]);
    SchedStop(sched);

    DestroySemaphores();

    pthread_join(thread, NULL);
}

/************************************************/
/**************Helper_Function*******************/
/************************************************/

static void InitSigaction(struct sigaction sa_usr1, struct sigaction sa_usr2)
{
    sa_usr1.sa_sigaction = &handle_sigusr1;
    sa_usr1.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &sa_usr1, NULL);

    sa_usr2.sa_sigaction = &handle_sigusr2;
    sa_usr2.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR2, &sa_usr2, NULL);
}

static void *StartRun(void *argc)
{
    (void)argc;

    sem_post(semaphores[OTHER_SEM]);
    sem_wait(semaphores[CURR_SEM]);

    SchedRun(sched);
    SchedDestroy(sched);

    return (NULL);
}

static scheduler_t *CreateSchedAndTask(const char **cmd)
{
    ilrd_uid_t uid1, uid2, uid3;
    scheduler_t *sched =  SchedCreate();
    if (NULL == sched)
    {
       return (NULL);
    }

    uid1 = SchedAddTask(sched, 1, SendSignal1, NULL, NULL, NULL);
    if (UIDIsEqual(uid1, bad_uid))
    {
        return (NULL);
    }

    uid2 = SchedAddTask(sched, 2, CheckCounter, cmd, NULL, NULL);
    if (UIDIsEqual(uid2, bad_uid))
    {
        return (NULL);
    }
    uid3 = SchedAddTask(sched, 3, StopRun, NULL, NULL, NULL);
    if (UIDIsEqual(uid3, bad_uid))
    {
        return (NULL);
    }

    return (sched);
}

static wd_status_t Revive(const char **cmd)
{
    pid_t pid = 0;
    char pid_str[20];
    
    pid = fork();
    if (-1 == pid)
    {
        return (WDFAILURE);
    }
    if (0 == pid)
    {
        if (0 == strcmp(cmd[0], FILE_NAME_WD))
        {
            pid = getpid();

            /*Convert the PID to a string*/
            snprintf(pid_str, sizeof(pid_str), "%d", pid);

            /* Set the environment variable "WD_PID" to the PID string */
            if (setenv("WD_PID", pid_str, 1) != 0) 
            {
                perror("Error setting environment variable");
                return (WDFAILURE);
            }

            execvp(((char **)cmd[1])[0], (char **)cmd[1]);
        }
        execvp(FILE_NAME_WD, (char * const *)cmd[1]);
    }

    id_of_other_proc = pid;

    sem_post(semaphores[OTHER_SEM]);
    sem_wait(semaphores[CURR_SEM]);
    
    return (WDSUCCESS);
}

static wd_status_t Fork(const char **cmd)
{
    wd_status_t status = WDSUCCESS;
    pid_t child_pid = fork();

    if (WDFAILURE == child_pid)
    {
        DEBUG_EXPR(printf("Error in fork()\n"));
        return (WDFAILURE);
    }
    if (0 == child_pid)
    {
        status = execvp(FILE_NAME_WD, (char * const *)cmd);
        if (WDFAILURE == status)
        {
            DEBUG_EXPR(printf("execlp fail\n"));
            return (WDFAILURE);
        }
    }

    id_of_other_proc = child_pid;

    return (WDSUCCESS);
}

static wd_status_t InitSemaphores()
{
    side_sem_t user_sem = CURR_SEM;
    side_sem_t wd_sem = OTHER_SEM;

    if (0 == strcmp(file_name, FILE_NAME_WD))
    {
        user_sem = OTHER_SEM;
        wd_sem = CURR_SEM;
    }

    semaphores[user_sem] = sem_open(USER_SEM_NAME, O_CREAT, 0666, 0);
    if (semaphores[user_sem] == SEM_FAILED)
    {
        DEBUG_EXPR(printf("sem open failed\n"));
        return (WDFAILURE);
    }

    semaphores[wd_sem] = sem_open(WD_SEM_NAME, O_CREAT, 0666, 0);
    if (semaphores[wd_sem] == SEM_FAILED)
    {
        DEBUG_EXPR(printf("sem open failed\n"));
        return (WDFAILURE);
    }

    return (WDSUCCESS);
}

static void DestroySemaphores()
{
    sem_close(semaphores[CURR_SEM]);
    sem_close(semaphores[OTHER_SEM]);

    sem_unlink(USER_SEM_NAME);
    sem_unlink(WD_SEM_NAME);
}

/************************************************/
/*********************TASK***********************/
/************************************************/
static int SendSignal1(void *param)
{
    (void)param;

    DEBUG_EXPR(printf("%d Task1\n", getpid()));
    DEBUG_EXPR(printf("%d send signal to: %d\n", getpid(), id_of_other_proc));

    kill(id_of_other_proc, SIGUSR1);
    ++counter_fail;
    
    return (REPEAT);
}

static int CheckCounter(void *param)
{
    DEBUG_EXPR(printf("%d Task2 %d\n", getpid(), counter_fail));

    if (counter_fail >= CAPACITY_FAIL)
    {
        DEBUG_EXPR(printf("Revive to  %d\n", id_of_other_proc));

        atomic_exchange(&counter_fail, 0);

        if (WDFAILURE == Revive(param))
        {
            DEBUG_EXPR(printf("Revive Fail\n"));
            return(ERROR);
        }
    }

    return (REPEAT);
}

static int StopRun(void *param)
{
    (void)param;

    DEBUG_EXPR(printf("%d Task3\n", getpid()));

    if (FINISH == stop_flag)
    {
        sem_post(semaphores[OTHER_SEM]);

        SchedStop(sched);

        if (unsetenv("WD_PID") != 0) 
        {
            perror("unsetenv");
            return (ERROR);
        }
        return (STOP);
    }
    
    return (REPEAT);
}

/************************************************/
/*******************HENDLER**********************/
/************************************************/

void handle_sigusr1(int signo, siginfo_t *info, void *context)
{
    (void)context;
    (void)signo;

    if (info->si_pid == id_of_other_proc)
    {
        DEBUG_EXPR(printf("%d recive signal1\n", getpid()));
        atomic_exchange(&counter_fail, 0);
    }
}

void handle_sigusr2(int signo, siginfo_t *info, void *context)
{
    (void)context;
    (void)signo;

    if (info->si_pid == id_of_other_proc)
    {
        DEBUG_EXPR(printf("%d recive signal2\n", getpid()));
        atomic_exchange(&stop_flag, FINISH);
    }
}