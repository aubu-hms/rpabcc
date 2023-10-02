/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Starter Kit version 285f43c (2023-07-06)                              **
**                                                                            **
** Delivered with:                                                            **
**    ABP            8.03.01 (2022-12-12)                                     **
**    ABCC Driver    5.08.01 (2021-12-09)                                     **
**                                                                            */
/*******************************************************************************
********************************************************************************
** COPYRIGHT NOTIFICATION (c) 2019 HMS Industrial Networks AB                 **
**                                                                            **
** This code is the property of HMS Industrial Networks AB.                   **
** The source code may not be reproduced, distributed, or used without        **
** permission. When used together with a product from HMS, permission is      **
** granted to modify, reproduce and distribute the code in binary form        **
** without any restrictions.                                                  **
**                                                                            **
** THE CODE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND. HMS DOES NOT    **
** WARRANT THAT THE FUNCTIONS OF THE CODE WILL MEET YOUR REQUIREMENTS, OR     **
** THAT THE OPERATION OF THE CODE WILL BE UNINTERRUPTED OR ERROR-FREE, OR     **
** THAT DEFECTS IN IT CAN BE CORRECTED.                                       **
********************************************************************************
********************************************************************************
** File Description:
** This is the public header file for time operations. The time should be local
** time but the device may use another time format if this is specified.
********************************************************************************
********************************************************************************
** Services:
**
** Public Services:
**    ABCC_TIME_Get() - Get system time.
**    ABCC_TIME_Set() - Set system time.
********************************************************************************
********************************************************************************
*/
#ifndef ABCC_TIME_H
#define ABCC_TIME_H

#include "abcc_td.h"

/*******************************************************************************
** Typedefs
********************************************************************************
*/
/*
** 24-hour format time.
** Ex: 2019-11-29T10:24:05
** iYear - 2019
** bMonth - 11
** bDay - 29
** bHour - 10
** bMinute - 24
** bSecond - 05
*/
typedef struct ABCC_TIME_DateAndTime
{
   UINT16 iYear;
   UINT8 bMonth;
   UINT8 bDay;
   UINT8 bHour;
   UINT8 bMinute;
   UINT8 bSecond;
}
ABCC_TIME_DateAndTimeType;

/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Get system time.
**------------------------------------------------------------------------------
** Arguments:
**       psTime - The time structure to be filled with system time data.
**
** Returns:
**       Returns TRUE if the operation was successful, otherwise FALSE.
**
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL ABCC_TIME_GetDateAndTime( ABCC_TIME_DateAndTimeType* psTime );

/*------------------------------------------------------------------------------
** Set system time.
**------------------------------------------------------------------------------
** Arguments:
**       psTime - The time to be set.
**
** Returns:
**       Returns TRUE if the operation was successful, otherwise FALSE.
**
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL ABCC_TIME_SetDateAndTime( ABCC_TIME_DateAndTimeType* psTime );

#endif  /* inclusion lock */
