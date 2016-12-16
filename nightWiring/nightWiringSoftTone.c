/***************************************************
                nightWiring Project
        By Xiaohai Li  (haixiaolee@gmail.com)
***************************************************/

#include <stdio.h>
#include <pthread.h>

#include "nightWiring.h"
#include "nightWiringGPIO.h"
#include "nightWiringSoftTone.h"

#define	PULSE_TIME	100

static int freqs        [NW_PIN_MAX];
static pthread_t threads[NW_PIN_MAX];

static int newPin = -1;


/*
 * softToneThread:
 *	Thread to do the actual PWM output
 *********************************************************************************
 */

static NW_THREAD (softToneThread)
{
  int pin, freq, halfPeriod;
  struct sched_param param;

  param.sched_priority = sched_get_priority_max (SCHED_RR);
  pthread_setschedparam (pthread_self (), SCHED_RR, &param);

  pin    = newPin;
  newPin = -1;

  nightWiringSetPri (50);

  for (;;)
  {
    freq = freqs[pin];
    if (freq == 0)
      delay (1);
    else
    {
      halfPeriod = 500000 / freq;

      digitalWrite (pin, HIGH);
      delayMicroseconds (halfPeriod);

      digitalWrite (pin, LOW);
      delayMicroseconds (halfPeriod);
    }
  }

  return NULL;
}


/*
 * softToneWrite:
 *	Write a frequency value to the given pin
 *********************************************************************************
 */

void softToneWrite (int pin, int freq)
{
  pin &= 63;

  /**/ if (freq < 0)
    freq = 0;
  else if (freq > 5000)	// Max 5KHz
    freq = 5000;

  freqs[pin] = freq;
}


/*
 * softToneCreate:
 *	Create a new tone thread.
 *********************************************************************************
 */

int softToneCreate (int pin)
{
  int res;
  pthread_t myThread;

  pinMode      (pin, OUTPUT);
  digitalWrite (pin, LOW);

  if (threads[pin] != 0)
    return -1;

  freqs[pin] = 0;

  newPin = pin;
  res    = pthread_create (&myThread, NULL, softToneThread, NULL);

  while (newPin != -1)
    delay (1);

  threads[pin] = myThread;

  return res;
}


/*
 * softToneStop:
 *	Stop an existing softTone thread
 *********************************************************************************
 */

void softToneStop (int pin)
{
  if (threads[pin] != 0)
  {
    pthread_cancel (threads[pin]);
    pthread_join   (threads[pin], NULL);
    threads[pin] = 0;
    digitalWrite (pin, LOW);
  }
}
