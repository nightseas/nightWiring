/***************************************************
                nightWiring Project
        By Xiaohai Li  (haixiaolee@gmail.com)
***************************************************/

#include <pthread.h>
#include "nightWiring.h"

static pthread_mutex_t nightWiringMutexes[4];



/*
 * nightWiringThreadCreate:
 *	Create and start a thread
 *********************************************************************************
 */

int nightWiringThreadCreate (void *(*fn)(void *))
{
  pthread_t myThread;

  return pthread_create (&myThread, NULL, fn, NULL);
}

/*
 * nightWiringLock: nightWiringUnlock:
 *	Activate/Deactivate a mutex.
 *	We're keeping things simple here and only tracking 4 mutexes which
 *	is more than enough for out entry-level pthread programming
 *********************************************************************************
 */

void nightWiringLock (int key)
{
  pthread_mutex_lock (&nightWiringMutexes[key]);
}

void nightWiringUnlock (int key)
{
  pthread_mutex_unlock (&nightWiringMutexes[key]);
}

