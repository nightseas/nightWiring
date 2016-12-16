/***************************************************
                nightWiring Project
        By Xiaohai Li  (haixiaolee@gmail.com)
***************************************************/

#ifndef	__WIRING_PI_H__
#define	__WIRING_PI_H__

// C doesn't have true/false by default and I can never remember which
//	way round they are, so ...

#ifndef	TRUE
#  define	TRUE	(1==1)
#  define	FALSE	(!TRUE)
#endif

// Handy defines

// nightWiring GPIO definitons
#ifndef	NW_PIN_MAX
#  define	NW_PIN_MAX      64
#endif

#define	WPI_MODE_PINS		 0
#define	WPI_MODE_GPIO		 1
#define	WPI_MODE_GPIO_SYS	 2
#define	WPI_MODE_PHYS		 3
#define	WPI_MODE_PIFACE		 4
#define	WPI_MODE_UNINITIALISED	-1

// Pin modes
#define	INPUT           0
#define	OUTPUT          1

#define	LOW             0
#define	HIGH            1

// Pull up/down/none (reserved)
#define	PUD_OFF         0
#define	PUD_DOWN        1
#define	PUD_UP          2

// Interrupt levels
#define	INT_EDGE_NONE     0
#define	INT_EDGE_FALLING  1
#define	INT_EDGE_RISING   2
#define	INT_EDGE_BOTH     3

// Function prototypes
#ifdef __cplusplus
extern "C" {
#endif

// Core GPIO functions
extern int nightWiringGpioSetup (int *pinMap, int pinNum);

extern void pinMode             (int pin, int mode);
extern int  digitalRead         (int pin);
extern void digitalWrite        (int pin, int value);

// Interrupts
extern int  waitForInterrupt    (int pin, int mS);
extern int  nightWiringISR      (int pin, int mode, void (*function)(void));

#ifdef __cplusplus
}
#endif

#endif
