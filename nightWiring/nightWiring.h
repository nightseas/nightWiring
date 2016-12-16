/***************************************************
                nightWiring Project
        By Xiaohai Li  (haixiaolee@gmail.com)
***************************************************/

#ifndef	__NIGHT_WIRING_H__
#define	__NIGHT_WIRING_H__

// True, false models
#ifndef	TRUE
#  define	TRUE	(1==1)
#  define	FALSE	(!TRUE)
#endif

// Threads
#define	NW_THREAD(X)	void *X (void *dummy)

// Failure modes
#define	NW_FATAL	(1==1)
#define	NW_ALMOST	(1==2)



// Function prototypes
#ifdef __cplusplus
extern "C" {
#endif

// Core
extern int nightWiringSetup (void);

// Error handling
extern int nightWiringFailure (int fatal, const char *message, ...);

// Threads
extern int  nightWiringThreadCreate      (void *(*fn)(void *));
extern void nightWiringLock              (int key);
extern void nightWiringUnlock            (int key);

// Schedulling priority
extern int nightWiringSetPri (const int pri);

// Timing
extern void         delay             (unsigned int howLong);
extern void         delayMicroseconds (unsigned int howLong);
extern unsigned int millis            (void);
extern unsigned int micros            (void);

#ifdef __cplusplus
}
#endif

#endif
