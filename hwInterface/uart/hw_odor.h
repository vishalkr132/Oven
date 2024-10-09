#ifndef _H_ODOR_H
#define _H_ODOR_H

#include <stdint.h>

#define ODOR_OK 0
#define ODOR_ERR -1
#define ODOR_DATA_SIZE 26

typedef struct 
{
    uint8_t header;
    int32_t temp;
    int32_t voltage1;
    int32_t voltage2;
    int32_t threshould1;
    int32_t threshould2;
    uint8_t gas;
    uint8_t preheatSignal;
    uint8_t fault;
    uint8_t status;
    uint8_t checksum;

}st_Odor;


int32_t ODOR_fnInit(int32_t odor_baudrate , const char *odor_path);
int32_t ODOR_fnReadData(st_Odor *st_Odor_t);
int32_t ODOR_fnClose();

#endif

