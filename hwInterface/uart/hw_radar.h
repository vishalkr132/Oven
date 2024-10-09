/**
 *
 * @copyright (c) 2024 Eaden Technology Pte Ltd. All rights reserved.
 * All trademarks are owned or licensed by Eaden Technology Pte Ltd,
 * its subsidiaries or affiliated companies.
 *
 ******************************************************************************
 * @file      mmWave_radar.h
 * @brief     This is the header file for mmWave radar for detecting object 
 *          

 ******************************************************************************
 */
#ifndef MMWAVE_RADAR_H
#define MMWAVE_RADAR_H

/*===================================================================================================
*         System includes
======================================================================================================*/
#include <stdint.h>
#define ODOR_OK 0
#define ODOR_ERR -1

/*==============================================================================
                              functions prototypes
==============================================================================*/

/*!
@brief		<b>Initialize the mmWave radar. </b>
@param[in]	i8_pUartPort - uart port or serial port path 
@param[out]	None
@retval EOK: mmWave radar initialization successful.
@retval errno: mmWave radar initialization failed.
*/
int32_t HW_RADAR_fnInit(int8_t *i8_pUartPort , int32_t i32_baudrate);
/*!
@brief		<b>read date from the mmWave radar. </b>
@param[in]	None
@param[out]	None
@retval float: distence
@retval errno: if mmWave radar failed to read.
*/
float HW_RADAR_fnReadData();

/*!
@brief		<b>close uart port for mmWave radar. </b>
@param[in]	None
@param[out]	None
@retval None
*/
void HW_RADAR_fnCloseUart(void);


#endif /* MMWAVE_RADAR_H */
