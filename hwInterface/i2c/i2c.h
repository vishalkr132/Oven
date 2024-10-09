#ifndef _I2C_H
#define _I2C_H

#include <stdint.h>

#define SLAVE_ADD 0x40

int32_t I2C_fnInit(int32_t i2c_bus);
int32_t I2C_fnWrite();
int32_t I2C_fnRead();
int32_t I2c_fnClose();

#endif