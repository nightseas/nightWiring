/***************************************************
                nightWiring Project
        By Xiaohai Li  (haixiaolee@gmail.com)
***************************************************/

#include <stdint.h>

#include "nightWiring.h"
#include "nightWiringGPIO.h"
#include "nightWiringShift.h"

/*
 * shiftIn:
 *	Shift data in from a clocked source
 *********************************************************************************
 */

uint8_t shiftIn (uint8_t dPin, uint8_t cPin, uint8_t order)
{
  uint8_t value = 0;
  int8_t  i;
 
  if (order == MSBFIRST)
    for (i = 7; i >= 0; --i)
    {
      digitalWrite (cPin, HIGH);
      value |= digitalRead (dPin) << i;
      digitalWrite (cPin, LOW);
    }
  else
    for (i = 0; i < 8; ++i)
    {
      digitalWrite (cPin, HIGH);
      value |= digitalRead (dPin) << i;
      digitalWrite (cPin, LOW);
    }

  return value;
}

/*
 * shiftOut:
 *	Shift data out to a clocked source
 *********************************************************************************
 */

void shiftOut (uint8_t dPin, uint8_t cPin, uint8_t order, uint8_t val)
{
  int8_t i;

  if (order == MSBFIRST)
    for (i = 7; i >= 0; --i)
    {
      digitalWrite (dPin, val & (1 << i));
      digitalWrite (cPin, HIGH);
      digitalWrite (cPin, LOW);
    }
  else
    for (i = 0; i < 8; ++i)
    {
      digitalWrite (dPin, val & (1 << i));
      digitalWrite (cPin, HIGH);
      digitalWrite (cPin, LOW);
    }
}
