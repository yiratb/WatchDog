/****************************
Yirat Romano                *
29/02/2024                  *
sharon                      *
header file                 *
exercise: uid               *
*****************************/
#ifndef ILRD_UID_H
#define ILRD_UID_H

#include <stddef.h> /*size_t*/
#include <sys/types.h> /*pid_t*/
#include <time.h> /*time_t*/

/*Struct representing a unique identifier composed of a counter, process ID, and timestamp.*/
typedef struct ilrd_uid
{
    size_t counter;
    pid_t pid;
    time_t time;
} ilrd_uid_t;

/*External declaration of a constant representing a bad UID.
   A bad UID has counter, pid, and time set to invalid values.*/
extern const ilrd_uid_t bad_uid;

/**
 * Function: UIDGenerate
 * ---------------------
 * Generates a new unique identifier.
 * 
 * Returns: A newly generated unique identifier.
 */
ilrd_uid_t UIDGenerate(void);

/**
 * Function: UIDIsEqual
 * ---------------------
 * Checks if two unique identifiers are equal.
 * 
 * one: The first unique identifier.
 * other: The second unique identifier.
 * 
 * Returns: 1 if the identifiers are equal, 0 otherwise.
 */
int UIDIsEqual(ilrd_uid_t one, ilrd_uid_t other);

#endif /*ILRD_UID_H*/
