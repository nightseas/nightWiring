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

#include "nightWiringSoftPwm.h"
#include "nightWiringSoftTone.h"

#include "nightWiring.h"
#include "nightWiringGPIO.h"


static volatile int    pinPass = -1;
static pthread_mutex_t pinMutex;

int *platformPinMap = NULL;
int platformPinNum = 0;

// gpioValueFds:
//	Map a file descriptor from the /sys/class/gpio/gpioX/value

static int gpioValueFds[NW_PIN_MAX] =
{
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

// ISR Data

static void (*isrFunctions[NW_PIN_MAX])(void);

//Internal function
static int gpioExport(int gpio);
//static int gpioUnexport(int gpio);


/*
 * pinMode:
 *	Sets the mode of a pin to be input, output or PWM output
 *********************************************************************************
 */

void pinMode (int pin, int mode)
{
  char fName[64];
  int fd;
  
  if((pin < 0) || (pin > (platformPinNum - 1)))
    nightWiringFailure (NW_FATAL, "pinMode: pin must be 0-%d (%d)\n", (platformPinNum - 1), pin);

  if(platformPinMap[pin] > -1)
  {
    sprintf (fName, "/sys/class/gpio/gpio%d/direction", platformPinMap[pin]);
    if((fd = open(fName, O_WRONLY)) < 0)
      (void)nightWiringFailure (NW_ALMOST, "Failed to set direction for pin%d, gpio%d: %s\n", pin, platformPinMap[pin], strerror (errno));

    if(mode == OUTPUT)
       write(fd, "out\n", 4);
    else
       write(fd, "in\n", 3);

    close(fd);
  }
  return;
}



/*
 * digitalRead:
 *	Read the value of a given Pin, returning HIGH or LOW
 *********************************************************************************
 */

int digitalRead (int pin)
{
  char c;
  
  if((pin < 0) || (pin > (platformPinNum - 1)))
    return nightWiringFailure (NW_FATAL, "digitalRead: pin must be 0-%d (%d)\n", (platformPinNum - 1), pin);

  if(gpioValueFds[pin] == -1)
	return LOW;

  lseek  (gpioValueFds[pin], 0L, SEEK_SET);
  read   (gpioValueFds[pin], &c, 1);
  return (c == '0') ? LOW : HIGH;
}


/*
 * digitalWrite:
 *	Set an output bit
 *********************************************************************************
 */

void digitalWrite (int pin, int value)
{
  if((pin < 0) || (pin > (platformPinNum - 1)))
    nightWiringFailure (NW_FATAL, "digitalWrite: pin must be 0-%d (%d)\n", (platformPinNum - 1), pin);
  
  if(gpioValueFds[pin] != -1)
  {
    if(value == LOW)
      write (gpioValueFds[pin], "0\n", 2);
    else
      write (gpioValueFds[pin], "1\n", 2);
  }
  return;
}


/*
 * waitForInterrupt:
 *	Pi Specific.
 *	Wait for Interrupt on a GPIO pin.
 *	This is actually done via the /sys/class/gpio interface regardless of
 *	the nightWiring access mode in-use. Maybe sometime it might get a better
 *	way for a bit more efficiency.
 *********************************************************************************
 */

int waitForInterrupt (int pin, int mS)
{
  #if 1
  int fd, ret;

  char dummyData ;
  struct pollfd polls ;
  
  if((pin < 0) || (pin > (platformPinNum - 1)))
    return nightWiringFailure (NW_FATAL, "digitalWrite: pin must be 0-%d (%d)\n", (platformPinNum - 1), pin);

  if((fd = gpioValueFds[pin]) == -1)
    return -2 ;

// Setup poll structure
  polls.fd = fd ;
  polls.events = POLLPRI ;	// Urgent data!

// Wait for it ...
  ret = poll (&polls, 1, mS);

// Do a dummy read to clear the interrupt
//	A one character read appars to be enough.
//	Followed by a seek to reset it.
  read (fd, &dummyData, 1);
  lseek (fd, 0, SEEK_SET);

  return ret ;
  #else
    return 0;
  #endif
}


/*
 * interruptHandler:
 *	This is a thread and gets started to wait for the interrupt we're
 *	hoping to catch. It will call the user-function when the interrupt
 *	fires.
 *********************************************************************************
 */

static void *interruptHandler (void *arg)
{
#if 1
  int myPin ;

  (void)nightWiringSetPri(55);	// Only effective if we run as root

  myPin = pinPass ;
  pinPass = -1 ;

  for (;;)
    if(waitForInterrupt(myPin, -1) > 0)
      isrFunctions[myPin]();
#endif
  return NULL ;
}


/*
 * nightWiringISR:
 *	Pi Specific.
 *	Take the details and create an interrupt handler that will do a call-
 *	back to the user supplied function.
 *********************************************************************************
 */

int nightWiringISR (int pin, int mode, void (*function)(void))
{
#if 1
  pthread_t threadId ;
  const char *modeS ;
  char fName[64];
  pid_t pid ;
  int   count, i, fd, len;
  char  dummyData;

if((pin < 0) || (pin > (platformPinNum - 1)))
    return nightWiringFailure (NW_FATAL, "nightWiringISR: pin must be 0-%d (%d)\n", (platformPinNum - 1), pin);

  // Now export the pin and set the edge
  if(mode == INT_EDGE_FALLING)
  {
    modeS = "falling";
    len = 7;
  }
  else if(mode == INT_EDGE_RISING)
  {
    modeS = "rising";
    len = 6;
  }
  else if(mode == INT_EDGE_BOTH)
  {
    modeS = "both";
    len = 4;
  }
  else if(mode == INT_EDGE_NONE)
  {
    modeS = "none";
    len = 4;    
  }  
  else
  {
    return nightWiringFailure (NW_FATAL, "nightWiringISR: invalid ISR mode, should be INT_EDGE_FALLING, INT_EDGE_RISING or INT_EDGE_BOTH\n");  
  }
  
  if ((pid = fork ()) < 0)	// Fail
    return nightWiringFailure (NW_FATAL, "nightWiringISR: fork failed: %s\n", strerror (errno)) ;

  if (pid == 0)
  {
    // The pin must be input mode
    pinMode(pin, INPUT);
    
    sprintf(fName, "/sys/class/gpio/gpio%d/edge", platformPinMap[pin]);
    if((fd = open(fName, O_RDWR)) < 0)
      return nightWiringFailure(NW_ALMOST, "Failed to open pin%d (gpio%d) for ISR edge control: %s\n", pin, platformPinMap[pin], strerror (errno));

    write(fd, modeS, len);
    close(fd);
  }
  else
    wait(NULL);

  // Clear any initial pending interrupt by doing a dummy read
  ioctl(gpioValueFds[pin], FIONREAD, &count);
  for(i = 0 ; i < count ; ++i)
    read(gpioValueFds[pin], &dummyData, 1);

  isrFunctions[pin] = function;

  pthread_mutex_lock(&pinMutex);
    pinPass = pin ;
    pthread_create(&threadId, NULL, interruptHandler, NULL);
    while(pinPass != -1)
      delay(1);
  pthread_mutex_unlock(&pinMutex);

#endif
  return 0 ;
}

/*
 * nightWiringGpioSetup:
 * Initialisation using the /sys/class/gpio
 *	interface to the GPIO systems - slightly slower, but always usable as
 *	a non-root user, assuming the devices are already exported and setup correctly.
 */

int nightWiringGpioSetup (int *pinMap, int pinNum)
{
  int pin;
  char fName[64];
  
  if(pinNum < 1 || pinNum > NW_PIN_MAX)
  {
    return nightWiringFailure(NW_FATAL, "Illegal number of pins (%d), max number is (%d)", pinNum, NW_PIN_MAX);
  }
  
  platformPinNum = pinNum;
  platformPinMap = pinMap;
  
  for (pin = 0; pin < platformPinNum; pin++)
  {
    if(platformPinMap[pin] > -1)
    {
      if(gpioExport(platformPinMap[pin]) != 0)
        return nightWiringFailure (NW_ALMOST, "Failed to export pin%d (gpio%d)\n", pin, platformPinMap[pin]);
    }
  }
  
  for (pin = 0; pin < platformPinNum; pin++)
  {
    if(platformPinMap[pin] > -1)
    {
      sprintf (fName, "/sys/class/gpio/gpio%d/value", platformPinMap[pin]);
      if((gpioValueFds[pin] = open(fName, O_RDWR)) == -1)
        return nightWiringFailure (NW_ALMOST, "Failed to open pin%d (gpio%d) for value control: %s\n", pin, platformPinMap[pin], strerror (errno));
    }
  }

  return 0;
}

static int gpioExport(int gpio)
{
	char gpioName[8];
	int fd, len;

  if((fd = open ("/sys/class/gpio/export", O_WRONLY)) < 0)
    return nightWiringFailure (NW_ALMOST, "Failed to open export for writing: %s\n", strerror (errno));
  
	len = snprintf(gpioName, 8, "%d", gpio);

	write(fd, gpioName, len);
	close(fd);

	return 0;
}

#if FALSE
static int gpioUnexport(int gpio)
{
  char gpioName[8];
  int fd, len;
  
  if((fd = open ("/sys/class/gpio/unexport", O_WRONLY)) < 0)
    return nightWiringFailure (NW_ALMOST, "Failed to open unexport for writing: %s\n", strerror (errno));

  len = snprintf(gpioName, 8, "%d", gpio);

  write(fd, gpioName, len);
  close(fd);

  return 0;
}
#endif
