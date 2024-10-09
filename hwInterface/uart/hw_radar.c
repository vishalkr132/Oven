/**
 *
 * @copyright (c) 2024 Eaden Technology Pte Ltd. All rights reserved.
 * All trademarks are owned or licensed by Eaden Technology Pte Ltd,
 * its subsidiaries or affiliated companies.
 *
 ******************************************************************************
 * @file      mmWave_radar.c
 * @brief     This is the source file for mmWave radar for detecting object 
 *          

 ******************************************************************************
 */


/*===================================================================================================
*         System includes
======================================================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

/*==============================================================================
                              Private Includes
==============================================================================*/
#include "hw_radar.h"

/*==============================================================================
*         Project includes
================================================================================*/

/*==============================================================================
                              Static Variables
==============================================================================*/
static int32_t sgi32_UartFd; 

static int32_t sai32_BaudRateSpeed[8] = {B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300};
static int32_t sai32_BaudRateName[8] = {115200, 38400, 19200, 9600, 4800, 2400, 1200, 300};

/*=============================================================================
                              Private Macros
==============================================================================*/

/*==============================================================================
                            see header for function prtotype
==============================================================================*/
int32_t HW_RADAR_fnInit(int8_t *i8_pUartPort , int32_t i32_baudrate)
{
    struct termios uart_config;
    int32_t i32_loop = 0;
    int BAUD_RATE = 0;
    sgi32_UartFd = open(i8_pUartPort, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (-1 == sgi32_UartFd) 
    {
        return sgi32_UartFd;
    }
    
    if (0 != tcgetattr(sgi32_UartFd, &uart_config)) 
    {
        close(sgi32_UartFd);
        return ODOR_ERR; 
    }

    for(i32_loop = 0; i32_loop < 8; i32_loop++)
    {
            if(sai32_BaudRateName[i32_loop] == i32_baudrate)
            {
                BAUD_RATE = sai32_BaudRateSpeed[i32_loop];
                break;
            }
    }


    /* Set serial port parameters */
    cfsetospeed(&uart_config, BAUD_RATE);
    cfsetispeed(&uart_config, BAUD_RATE);
    uart_config.c_cflag &= ~PARENB; /* Disable parity */
    uart_config.c_cflag &= ~CSTOPB; /* 1 stop bit */
    uart_config.c_cflag &= ~CSIZE;  /* Clear mask */
    uart_config.c_cflag |= CS8;     /* 8 data bits */
    uart_config.c_cflag |= CREAD;   /* Enable receiver */
    uart_config.c_cflag |= CLOCAL;  /* Ignore control lines */
    
    /* Set raw mode for input*/
    uart_config.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    uart_config.c_oflag &= ~OPOST;
    
    /* Set timeout */
    uart_config.c_cc[VMIN] = 0; /* Non-blocking read */
    uart_config.c_cc[VTIME] = 10; /* 1 second timeout */
    
    if (0 != tcsetattr(sgi32_UartFd, TCSANOW, &uart_config)) 
    {
        close(sgi32_UartFd);
        return ODOR_ERR; 
    }
    
    return ODOR_OK;
}


/*==============================================================================
                            see header for function prtotype
==============================================================================*/
float HW_RADAR_fnReadData(void) 
{
    int32_t i32_BytesRead;
    int8_t i8_pBuffer[255];
    float f_distance;
    float f_magnitude;

    /* Attempt to read data */
    i32_BytesRead = read(sgi32_UartFd, i8_pBuffer, sizeof(i8_pBuffer));
    
    /* Check for errors */
    if (0 > i32_BytesRead) 
    {
        tcflush(sgi32_UartFd, TCIOFLUSH);
        return ODOR_ERR;
    } 

    /* Parse received data and extract distance and magnitude */
    i8_pBuffer[i32_BytesRead] = '\0'; 
    sscanf(i8_pBuffer, "{\"dis\":%f,\"mag\":%f", &f_distance, &f_magnitude);

    return f_distance;
 
}

/*==============================================================================
                            see header for function prtotype
==============================================================================*/
void HW_RADAR_fnCloseUart(void) 
{
    if(-1 != sgi32_UartFd)
    {
        close(sgi32_UartFd);
        sgi32_UartFd = -1;
    }

}


/************ (C) COPYRIGHT (c) 2024 Eaden Technology Pte Ltd *********END OF FILE****/
