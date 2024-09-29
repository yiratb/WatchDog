/****************************
Yirat Romano                *
29/02/2024                  *
sharon                      *
source file                 *
exercise: uid               *
*****************************/
#include "uid.h" /*ilrd_uid_t, bad_uid*/
#include <unistd.h> /*getpid()*/
#include <pthread.h>

#define UNVALID_TIME (-1)

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
const ilrd_uid_t bad_uid = {0, UNVALID_TIME, UNVALID_TIME};

ilrd_uid_t UIDGenerate(void)
{
    static size_t static_counter = 0;
    ilrd_uid_t new_uid;

    if (0 != pthread_mutex_lock(&mutex))
    {
        return (bad_uid);
    }
    new_uid.counter = ++static_counter;
    if (0 != pthread_mutex_unlock(&mutex))
    {
        return (bad_uid);
    }

    new_uid.pid = getpid();

    new_uid.time = time(NULL);
    if (UNVALID_TIME == new_uid.time)
    {
        return (bad_uid);
    }

    return (new_uid);
}

int UIDIsEqual(ilrd_uid_t one, ilrd_uid_t other)
{
    return (one.counter == other.counter && one.pid == other.pid
                    && one.time == other.time);
}