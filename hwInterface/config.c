#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include "config.h"
#include "json/cJSON.h"

void add();

int32_t CONFIG_fnRead(st_config *config_t)
{
    FILE *file = fopen(CONFIG_PATH , "r");
    if(file == NULL)
    {
        printf("failed to open");
        return -1;
    }

    char buffer[1024];
    int readLen = fread(buffer , 1 , sizeof(buffer) , file);
    fclose(file);

    cJSON *json = cJSON_Parse(buffer);

    if(json == NULL)
    {
        printf("failed to parse\n");
        return -1;
    }

    cJSON *odor_uart = cJSON_GetObjectItemCaseSensitive(json , "odor_path");
    if(cJSON_IsString(odor_uart) && odor_uart->valuestring != NULL)
    {
        strncpy(config_t->st_odor.odor_path , odor_uart->valuestring , sizeof(config_t->st_odor.odor_path));
    }

    cJSON *odor_rate = cJSON_GetObjectItemCaseSensitive(json , "odor_baudrate");
    if(cJSON_IsNumber(odor_rate))
    {
        config_t->st_odor.odor_baudrate = odor_rate->valueint;
    }

    printf("odor path %s\n" , config_t->st_odor.odor_path);
    printf("odor baudrate %d\n" , config_t->st_odor.odor_baudrate);

}


void add()
{
     cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json , "odor_path" , "/dev/ttyCH9344USB6");
    cJSON_AddNumberToObject(json , "odor_baudrate" , 2400);
    cJSON_AddStringToObject(json , "radar_path" , "/dev/ttyCH9344USB5");
    cJSON_AddNumberToObject(json , "radar_baudrate" , 115200);
    cJSON_AddStringToObject(json , "temp_path" , "/dev/ds18b20");

    char *json_str = cJSON_Print(json);

    FILE *fd = fopen(CONFIG_PATH , "w");
    if(fd == NULL)
    {
        printf("failed to open %s\n" , CONFIG_PATH);
    }
    
    fputs(json_str , fd);
    fclose(fd);

    cJSON_free(json_str);
    cJSON_Delete(json);
}


//#if 0

int main()
{
    st_config config_t;
     add();
     CONFIG_fnRead(&config_t);
    return 0;
}

//#endif