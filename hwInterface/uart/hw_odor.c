#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <error.h>
#include <termios.h>

#include "hw_odor.h"

int32_t odor_fd;
int32_t baudrate[7] = {B2400, B4800, B9600, B19200, B38400, B57600, B115200};
int32_t baud_rate[7] = {2400 , 4800 , 9600, 19200 , 38400 , 57600 , 115200};

int8_t odor_checksum(uint8_t *check_data , int32_t checksum_len);
int32_t odor_reverse(uint8_t *reverse_data);



int32_t ODOR_fnInit(int32_t odor_baudrate , const char *odor_path)
{
    struct termios tty;
    int32_t ODOR_BAUDRATE = 0;

    odor_fd = open(odor_path , O_RDWR);
    if(-1 == odor_fd)
    {
        printf("failed to open uart port %d\n" , odor_fd);
        return ODOR_ERR;
    }

    for(int32_t loop = 0; loop < 7; loop++)
    {
        if(odor_baudrate == baud_rate[loop])
        {
            ODOR_BAUDRATE = baudrate[loop];
            break;
        }
    }

    if(tcgetattr(odor_fd , &tty) != 0)
    {
        return ODOR_ERR;
    }

    cfsetspeed(&tty , ODOR_BAUDRATE);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= CREAD | CLOCAL;
    tty.c_cc[VMIN] = 26;
    tty.c_cc[VTIME] = 0;

    if(tcsetattr(odor_fd , TCSANOW , &tty) != 0 )
    {
        return ODOR_ERR;
    }
    return ODOR_OK;
}

int8_t odor_checksum(uint8_t *check_data , int32_t checksum_len)
{
    int32_t result = 0;
    for(int i=0; i<checksum_len;i++)
    {
        result += check_data[i];
    }

    return result & 0XFF;
}

int32_t odor_reverse(uint8_t *reverse_data)
{
    uint32_t result = 0;
    result = (reverse_data[3] << 24) | (reverse_data[2] << 16) | (reverse_data[1] << 8) | reverse_data[0];
    return result/1000;
}


int32_t ODOR_fnReadData(st_Odor *st_Odor_t)
{
    char odor_buffer[ODOR_DATA_SIZE];
    int32_t odor_read = 0;
    uint8_t checksum;

    memset(&odor_buffer , 0 , ODOR_DATA_SIZE);
    odor_read = read(odor_fd , odor_buffer , ODOR_DATA_SIZE);

    if(odor_read < 0)
    {
        printf("failed to read odor data %d\n" , odor_read);
        return ODOR_ERR;
    }

    for(int i=0;i<ODOR_DATA_SIZE;i++)
    {
        printf("%02X " , odor_buffer[i]);
    }
    printf("\n");

    checksum = odor_buffer[ODOR_DATA_SIZE - 1];
    uint8_t calculated_checksum = odor_checksum(odor_buffer , ODOR_DATA_SIZE - 1);

    if(checksum == calculated_checksum)
    {
        st_Odor_t->temp = odor_reverse(odor_buffer + 1);
        st_Odor_t->voltage1 = odor_reverse(odor_buffer + 5);
        st_Odor_t->voltage2 = odor_reverse(odor_buffer + 6);
    }

    return ODOR_OK;
}

int32_t ODOR_fnClose()
{

    if(odor_fd != -1)
    {
        close(odor_fd);
        odor_fd = -1;
        return ODOR_OK;
    }

    return ODOR_ERR;

}