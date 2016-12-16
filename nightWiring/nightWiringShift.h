/***************************************************
                nightWiring Project
        By Xiaohai Li  (haixiaolee@gmail.com)
***************************************************/

#define	LSBFIRST	0
#define	MSBFIRST	1

#ifndef	_STDINT_H
#  include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t shiftIn      (uint8_t dPin, uint8_t cPin, uint8_t order);
extern void    shiftOut     (uint8_t dPin, uint8_t cPin, uint8_t order, uint8_t val);

#ifdef __cplusplus
}
#endif
