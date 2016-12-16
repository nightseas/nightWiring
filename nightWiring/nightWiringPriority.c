/***************************************************
                nightWiring Project
        By Xiaohai Li  (haixiaolee@gmail.com)
***************************************************/

#include <sched.h>
#include <string.h>

#include "nightWiring.h"


/*
 * piHiPri:
 *	Attempt to set a high priority schedulling for the running program
 *********************************************************************************
 */

int nightWiringSetPri (const int pri)
{
  struct sched_param sched;

  memset (&sched, 0, sizeof(sched));

  if (pri > sched_get_priority_max (SCHED_RR))
    sched.sched_priority = sched_get_priority_max (SCHED_RR);
  else
    sched.sched_priority = pri;

  return sched_setscheduler (0, SCHED_RR, &sched);
}
