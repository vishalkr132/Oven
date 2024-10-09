#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <ioctl.h>

#include "i2c.h"

int32_t i2c_fd;
int32_t I2C_fnInit(int32_t i2c_bus)
{
    char filepath[20];
    i2c_fd = 0;
    snprintf(filepath , 19 , "/dev/i2c-%d" , i2c_bus);

    i2c_fd = open(filepath , O_RDWR);

    if(i2c_fd < 0)
    {
        printf("failed to open i2c\n");
        return -1;
    }

    if(ioctl(i2c_fd , I2C_SLAVE , SLAVE_ADD) != 0)
    {
        printf("failed to open i2c\n");
    }

    return 0;
}

int32_t I2C_fnWrite()
{
    char buff[3];
    buff[0] = 0x40;
    buff[1] = 0x10;
    buff[2] = 0x20;

    int writeByte = write(i2c_fd , buff , sizeof(buff));
}

int32_t I2C_fnRead()
{
    char readBuff[3];
    int readBytes = read(i2c_fd , readBuff , 2);
}

int32_t I2c_fnClose()
{
    if(i2c_fd != -1)
    {
        close(i2c_fd);
        i2c_fd = -1;
        return 0;
    }
    return -1;
}