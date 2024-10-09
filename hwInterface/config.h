#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdio.h>
#define CONFIG_PATH "configfile.json"

typedef struct
{
    struct 
    {
        int32_t odor_baudrate;
        uint8_t odor_path[50];
    }st_odor;
    struct 
    {
        int32_t radar_baudrate;
        uint8_t radar_path[50];
    }st_radar;

    struct 
    {
        uint8_t temp_path[50];
    }st_temp;

}st_config;


int32_t CONFIG_fnRead(st_config *config_t);
void add();

#endif
