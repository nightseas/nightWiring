/***************************************************
                nightWiring Project
        By Xiaohai Li  (haixiaolee@gmail.com)
***************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "nightWiring.h"
#include "nightWiringI2C.h"

// I2C definitions

#define I2C_SLAVE	0x0703
#define I2C_SMBUS	0x0720	/* SMBus-level access */

#define I2C_SMBUS_READ	1
#define I2C_SMBUS_WRITE	0

// SMBus transaction types

#define I2C_SMBUS_QUICK		    0
#define I2C_SMBUS_BYTE		    1
#define I2C_SMBUS_BYTE_DATA	    2 
#define I2C_SMBUS_WORD_DATA	    3
#define I2C_SMBUS_PROC_CALL	    4
#define I2C_SMBUS_BLOCK_DATA	    5
#define I2C_SMBUS_I2C_BLOCK_BROKEN  6
#define I2C_SMBUS_BLOCK_PROC_CALL   7		/* SMBus 2.0 */
#define I2C_SMBUS_I2C_BLOCK_DATA    8

// SMBus messages

#define I2C_SMBUS_BLOCK_MAX	32	/* As specified in SMBus standard */	
#define I2C_SMBUS_I2C_BLOCK_MAX	32	/* Not specified but we use same structure */

// Structures used in the ioctl() calls

union i2c_smbus_data
{
  uint8_t  byte;
  uint16_t word;
  uint8_t  block[I2C_SMBUS_BLOCK_MAX + 2];	// block[0] is used for length + one more for PEC
};

struct i2c_smbus_ioctl_data
{
  char read_write;
  uint8_t command;
  int size;
  union i2c_smbus_data *data;
};

static inline int i2c_smbus_access (int fd, char rw, uint8_t command, int size, union i2c_smbus_data *data)
{
  struct i2c_smbus_ioctl_data args;

  args.read_write = rw;
  args.command    = command;
  args.size       = size;
  args.data       = data;
  return ioctl (fd, I2C_SMBUS, &args);
}


/*
 * i2cRead:
 *	Simple device read
 *********************************************************************************
 */

int i2cRead (int fd)
{
  union i2c_smbus_data data;

  if (i2c_smbus_access (fd, I2C_SMBUS_READ, 0, I2C_SMBUS_BYTE, &data))
    return -1;
  else
    return data.byte & 0xFF;
}


/*
 * i2cReadReg8: i2cReadReg16:
 *	Read an 8 or 16-bit value from a regsiter on the device
 *********************************************************************************
 */

int i2cReadReg8 (int fd, int reg)
{
  union i2c_smbus_data data;

  if (i2c_smbus_access (fd, I2C_SMBUS_READ, reg, I2C_SMBUS_BYTE_DATA, &data))
    return -1;
  else
    return data.byte & 0xFF;
}

int i2cReadReg16 (int fd, int reg)
{
  union i2c_smbus_data data;

  if (i2c_smbus_access (fd, I2C_SMBUS_READ, reg, I2C_SMBUS_WORD_DATA, &data))
    return -1;
  else
    return data.word & 0xFFFF;
}


/*
 * i2cWrite:
 *	Simple device write
 *********************************************************************************
 */

int i2cWrite (int fd, int data)
{
  return i2c_smbus_access (fd, I2C_SMBUS_WRITE, data, I2C_SMBUS_BYTE, NULL);
}


/*
 * i2cWriteReg8: i2cWriteReg16:
 *	Write an 8 or 16-bit value to the given register
 *********************************************************************************
 */

int i2cWriteReg8 (int fd, int reg, int value)
{
  union i2c_smbus_data data;

  data.byte = value;
  return i2c_smbus_access (fd, I2C_SMBUS_WRITE, reg, I2C_SMBUS_BYTE_DATA, &data);
}

int i2cWriteReg16 (int fd, int reg, int value)
{
  union i2c_smbus_data data;

  data.word = value;
  return i2c_smbus_access (fd, I2C_SMBUS_WRITE, reg, I2C_SMBUS_WORD_DATA, &data);
}


/*
 * i2cSetupInterface:
 *	Undocumented access to set the interface explicitly - might be used
 *	for the Pi's 2nd I2C interface...
 *********************************************************************************
 */

int i2cSetupInterface (const char *device, int devId)
{
    int fd;
    
    if ((fd = open (device, O_RDWR)) < 0)
      return nightWiringFailure (NW_ALMOST, "Unable to open I2C device: %s\n", strerror (errno));
    
    if (ioctl (fd, I2C_SLAVE, devId) < 0)
      return nightWiringFailure (NW_ALMOST, "Unable to select I2C device: %s\n", strerror (errno));
    
    return fd;
}


/*
 * i2cSetup:
 *	Open the I2C device, and regsiter the target device
 *********************************************************************************
 */

int i2cSetup (const char *device, const int devId)
{
  return i2cSetupInterface (device, devId);
}
