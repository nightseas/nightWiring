/***************************************************
                nightWiring Project
        By Xiaohai Li  (haixiaolee@gmail.com)
***************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <poll.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <asm/ioctl.h>

#include "nightWiring.h"

// Time for easy calculations
static uint64_t epochMilli, epochMicro;

// Debugging & Return codes
int nightWiringReturnCodes = FALSE;

static void initialiseEpoch (void);

/*
 * nightWiringSetup:
 *	Not really need to call this, unless you want to use timing funtions.
 *********************************************************************************
 */

int nightWiringSetup (void)
{
  static int alreadyCalled = FALSE;

// Make sure the setup function only be called once.
  if (alreadyCalled)
    (void)nightWiringFailure (NW_FATAL, "nightWiringSetup*: You must only call this once per program run. This is a fatal error. Please fix your code.\n");

  alreadyCalled = TRUE;

// Root check.
    if (geteuid () != 0)
      (void)nightWiringFailure (NW_FATAL, "nightWiringSetup: Must be root. (Did you forget sudo?)\n");

  initialiseEpoch ();

  return 0;
}

/*
 * nightWiringFailure:
 *	Fail. Or not.
 *********************************************************************************
 */

int nightWiringFailure (int fatal, const char *message, ...)
{
  va_list argp;
  char buffer[1024];

  if (!fatal && nightWiringReturnCodes)
    return -1;

  va_start (argp, message);
    vsnprintf (buffer, 1023, message, argp);
  va_end (argp);

  fprintf (stderr, "%s", buffer);
  exit (EXIT_FAILURE);

  return 0;
}



/*
 * initialiseEpoch:
 *	Initialise our start-of-time variable to be the current unix
 *	time in milliseconds and microseconds.
 *********************************************************************************
 */

static void initialiseEpoch (void)
{
  struct timeval tv;

  gettimeofday (&tv, NULL);
  epochMilli = (uint64_t)tv.tv_sec * (uint64_t)1000    + (uint64_t)(tv.tv_usec / 1000);
  epochMicro = (uint64_t)tv.tv_sec * (uint64_t)1000000 + (uint64_t)(tv.tv_usec);
}


/*
 * delay:
 *	Wait for some number of milliseconds
 *********************************************************************************
 */

void delay (unsigned int howLong)
{
  struct timespec sleeper, dummy;

  sleeper.tv_sec  = (time_t)(howLong / 1000);
  sleeper.tv_nsec = (long)(howLong % 1000) * 1000000;

  nanosleep (&sleeper, &dummy);
}


/*
 * delayMicroseconds:
 *	This is somewhat intersting. It seems that on the Pi, a single call
 *	to nanosleep takes some 80 to 130 microseconds anyway, so while
 *	obeying the standards (may take longer), it's not always what we
 *	want!
 *
 *	So what I'll do now is if the delay is less than 100uS we'll do it
 *	in a hard loop, watching a built-in counter on the ARM chip. This is
 *	somewhat sub-optimal in that it uses 100% CPU, something not an issue
 *	in a microcontroller, but under a multi-tasking, multi-user OS, it's
 *	wastefull, however we've no real choice )-:
 *
 *      Plan B: It seems all might not be well with that plan, so changing it
 *      to use gettimeofday () and poll on that instead...
 *********************************************************************************
 */

void delayMicrosecondsHard (unsigned int howLong)
{
  struct timeval tNow, tLong, tEnd;

  gettimeofday (&tNow, NULL);
  tLong.tv_sec  = howLong / 1000000;
  tLong.tv_usec = howLong % 1000000;
  timeradd (&tNow, &tLong, &tEnd);

  while (timercmp (&tNow, &tEnd, <))
    gettimeofday (&tNow, NULL);
}

void delayMicroseconds (unsigned int howLong)
{
  struct timespec sleeper;
  unsigned int uSecs = howLong % 1000000;
  unsigned int wSecs = howLong / 1000000;

  /**/ if (howLong ==   0)
    return;
  else if (howLong  < 100)
    delayMicrosecondsHard (howLong);
  else
  {
    sleeper.tv_sec  = wSecs;
    sleeper.tv_nsec = (long)(uSecs * 1000L);
    nanosleep (&sleeper, NULL);
  }
}


/*
 * millis:
 *	Return a number of milliseconds as an unsigned int.
 *********************************************************************************
 */

unsigned int millis (void)
{
  struct timeval tv;
  uint64_t now;

  gettimeofday (&tv, NULL);
  now  = (uint64_t)tv.tv_sec * (uint64_t)1000 + (uint64_t)(tv.tv_usec / 1000);

  return (uint32_t)(now - epochMilli);
}


/*
 * micros:
 *	Return a number of microseconds as an unsigned int.
 *********************************************************************************
 */

unsigned int micros (void)
{
  struct timeval tv;
  uint64_t now;

  gettimeofday (&tv, NULL);
  now  = (uint64_t)tv.tv_sec * (uint64_t)1000000 + (uint64_t)tv.tv_usec;

  return (uint32_t)(now - epochMicro);
}
