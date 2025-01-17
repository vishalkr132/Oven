#ifndef DS18B20_H
#define DS18B20_H

/**
 * @copyright (c) 2024 Eaden Technology Pte Ltd. All rights reserved.
 * All trademarks are owned or licensed by Organization Name,
 * its subsidiaries or affiliated companies.
 *
 ******************************************************************************
 *
 * @file      ds18b20_sensor.h
 * @brief     This is the header file for ds18b20 temp sensor.
 *            Macros, structs, function prototypes defined here.
 ******************************************************************************
 */


/*==============================================================================
                              System Includes
==============================================================================*/
#include <stdint.h>
#include <syslog.h>
#include <stdbool.h>

/*==============================================================================
                              Project Includes
==============================================================================*/
/*==============================================================================
                              Defines
==============================================================================*/

/*==============================================================================
                             Type Defines
==============================================================================*/
typedef float float32_t;
typedef double float64_t;
/*==============================================================================
                              Macros
==============================================================================*/

#define MAX_BUFFER_SIZE    16
/*==============================================================================
                              Enums
==============================================================================*/

/*==============================================================================
                              Structures
==============================================================================*/

/*==============================================================================
                              functions prototypes
==============================================================================*/

int32_t HW_DS18B20_fnInitSensor(int8_t*);
int32_t HW_DS18B20_fnOpenSensor(int8_t*);
int32_t HW_DS18B20_fnReadTempVal(float32_t*);
float32_t HW_DS18B20_fnFilterData(float32_t*);
int32_t HW_DS18B20_fnCloseSensor(void);
/*==============================================================================
                              Footer
==============================================================================*/

/************ (C) COPYRIGHT (c) 2024 Eaden Technology Pte Ltd. *********END OF FILE****/

#endif /** DS18B20_H*/
