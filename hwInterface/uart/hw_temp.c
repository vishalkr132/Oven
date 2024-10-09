/**
 * @copyright (c) 2024 Eaden Technology Pte Ltd. All rights reserved.
 * All trademarks are owned or licensed by Organization Name,
 * its subsidiaries or affiliated companies.
 *
 ******************************************************************************
 *
 * @file      ds18b20_sensor.c
 * @brief     This is the module application source file for ds18b20 temp sensor.
 *            open sensor, read sensor, close sensor and return temperature functionality implemented.
 ******************************************************************************
 */

/*==============================================================================
                              System Includes
==============================================================================*/
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

/*=============================================================================
                              Project Includes
==============================================================================*/

/*==============================================================================
                              Private Includes
==============================================================================*/
#include "hw_temp.h"

/*=============================================================================
                              Private Macros
==============================================================================*/

/*==============================================================================
                              External/Public Variables
==============================================================================*/

/*==============================================================================
                              Static Variables
==============================================================================*/
static int32_t sgi32_fd = -1;
static int32_t sgi32_isInit = 0;
/*==============================================================================
                              Static Functions
==============================================================================*/

/*==============================================================================
                              DEFINE Functions
==============================================================================*/

/*==============================================================================
                              Local Function Prototypes
==============================================================================*/

/*!
 * @brief <b> This function is for opening the specified path and
 *            get the file descriptor to the DS18B20 Temp Sensor. </b>
 *
 * @param[in] pi8_path Pointer to a string containing the path to DS18B20 sensor
 * @param[out] none
 *
 * @return int32_t - 0 on success or -1 on failure
 */
int32_t HW_DS18B20_fnOpenSensor(int8_t *pi8_path)
{
    sgi32_fd = open(pi8_path, O_RDWR | O_NDELAY | O_NOCTTY);
    if (-1 == sgi32_fd)
    {
        printf("LOG_ERR: DS18B20_OpenSensor: Open failed,"
                        "ret = %d , errno %d\n", sgi32_fd, errno);
        return sgi32_fd;
    }

    // printf("LOG_DEBUG: DS18B20_OpenSensor: Open successful,"
    //                 "ret = %d !\n", sgi32_fd);
    return 0;
}

/*!
 * @brief <b> This function is for initializing DS18B20 Temperature
 *            Sensor by opening the specified path. </b>
 *
 * @param[in] none
 * @param[out] none
 *
 * @return int32_t - 0 on success or -1 on failure of the initialization
 */
int32_t HW_DS18B20_fnInitSensor(int8_t* i8_path)
{
    int32_t i32_ret = -1;

    i32_ret = HW_DS18B20_fnOpenSensor(i8_path);
    if(0 == i32_ret)
    {
        printf("LOG_DEBUG: DS18B20_InitSensor: Initalizations completed\n");
    }
    else
    {
        printf("LOG_ERR: DS18B20_InitSensor: Initalizations failed\n");
    }
    return i32_ret;
}

/*!
 * @brief <b> This function is for filter the temperature data using a moving average approach. </b>
 *
 * @param[in] p_temp_buffer pointer to an array containing temperature values to be filtered
 * @param[out] none
 *
 * @return float32_t - Returns a float representing the filtered temperature value
 */
float32_t HW_DS18B20_fnFilterData(float32_t* pf32_TempBuffer)
{
    int32_t i, j, i32_len = 0;
    float32_t f32_val, f32_sum = 0;

    for(i = 0; i < MAX_BUFFER_SIZE; i++)
    {
        for(j = 0; j < MAX_BUFFER_SIZE; j++)
        {
            if(pf32_TempBuffer[i] > pf32_TempBuffer[j])
            {
                f32_val = pf32_TempBuffer[i];
                pf32_TempBuffer[i] = pf32_TempBuffer[j];
                pf32_TempBuffer[j] = f32_val;
            }
        }
    }

    for(i = MAX_BUFFER_SIZE / 4; i < (MAX_BUFFER_SIZE * 3) / 4; i++)
    {
        f32_sum += pf32_TempBuffer[i];
        i32_len++;
    }

    return (f32_sum / i32_len);
}

/*!
 * @brief <b> This function is for read temperature values from the DS18B20 sensor
 *            and filters the results using a moving average. </b>
 *
 * @param[in] none
 * @param[out] pf32_result pointer to a float return temperature values
 *
 * @return int32_t - int32_t - 0 on success or -1 on failure
 */
int32_t HW_DS18B20_fnReadTempVal(float32_t* pf32_result)
{
    uint8_t ui8_buff[2] = {0};
    float32_t f32_TempBuffer[MAX_BUFFER_SIZE];

    uint16_t ui16_temp = 0;
    int32_t i;
    int32_t i32_ret = -1;

    if (-1 != sgi32_fd)
    {
        for (i = 0; i < MAX_BUFFER_SIZE; i++)
        {
            memset(ui8_buff, 0, sizeof(ui8_buff));
            int32_t i32_BytesRead = read(sgi32_fd, ui8_buff, sizeof(ui8_buff));
            if(-1 != i32_BytesRead)
            {
                //printf("LOG_DEBUG: buff[0]: 0x%02X and buff[1]: 0x%02X\n", ui8_buff[0], ui8_buff[1]); 
                if(255 != ui8_buff[0] && 255 != ui8_buff[1])
                {
                    ui16_temp = ((uint16_t) ui8_buff[1]) << 8;
                    ui16_temp |= (uint16_t) ui8_buff[0];
                    *pf32_result = 0.0625 * ((float32_t) ui16_temp);
                    f32_TempBuffer[i] = *pf32_result;
                    if (MAX_BUFFER_SIZE - 1 != i)
                    {
                        usleep(50000);
                    }
                }
            }
            else
            {
                printf("LOG_ERR: Error reading data from sensor, errno %d\n", errno);
                close(sgi32_fd);
                return -1;
            }
        }
        float32_t f32_filteredData = HW_DS18B20_fnFilterData(f32_TempBuffer);
        pf32_result = &f32_filteredData;
        i32_ret = 0;
    }
    else
    {
        printf("LOG_ERR: DS18B20_OpenSensor: Read temp failed,"
                        "ret = %d , errno %d\n", sgi32_fd, errno);
    }

    return i32_ret;
}

/*!
 * @brief <b> This function is for close the file descriptor associated
 *            with the DS18B20 sensor and reset . </b>
 *
 * @param[in] none
 * @param[out] none
 *
 * @return none
 */
int32_t HW_DS18B20_fnCloseSensor(void)
{
    int32_t i32_ret = 0;

    if (-1 != sgi32_fd)
    {
        int32_t i32_CloseReturn = close(sgi32_fd);
        if(0 == i32_CloseReturn)
        {
            sgi32_fd = -1;
            sgi32_isInit = 0;
            printf("LOG_DEBUG: DS18B20_CloseSensor: Close successful,"
                            "ret = %d !\n", sgi32_fd);
            return i32_ret;
        }
        else
        {
            printf("LOG_ERR: DS18B20_CloseSensor: Close failed,"
                            "ret = %d , errno %d\n", sgi32_fd, errno);
            return i32_ret;
        }
    }
    else
    {
        printf("LOG_ERR: DS18B20_CloseSensor: Nothing to close\n");
        return i32_ret;
    }
}

/************ (C) COPYRIGHT (c) 2024 Eaden Technology Pte Ltd. *********END OF FILE****/
