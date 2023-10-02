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
** Platform dependent implementation of abcc_time functions
********************************************************************************
********************************************************************************
*/

#include "abcc_obj_cfg.h"
#include "abcc_time.h"

/*******************************************************************************
** Constants
********************************************************************************
*/

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*******************************************************************************
** Private Services
********************************************************************************
*/

/*******************************************************************************
** Public Services
********************************************************************************
*/

#if BAC_IA_CURR_DATE_AND_TIME_ENABLE
BOOL ABCC_TIME_GetDateAndTime( ABCC_TIME_DateAndTimeType* psTime )
{
   /*
   ** PORTING ALERT!
   ** Needs to be implemented in order for get date and time over BACnet to work
   */
   #error PORTING ALERT!

   return FALSE;
}
#endif /* BAC_IA_CURR_DATE_AND_TIME_ENABLE */

#if BAC_IA_CURR_DATE_AND_TIME_ENABLE && BAC_IA_CURR_DATE_AND_TIME_SET
BOOL ABCC_TIME_SetDateAndTime( ABCC_TIME_DateAndTimeType* psTime )
{
   /*
   ** PORTING ALERT!
   ** Needs to be implemented in order for set date and time over BACnet to work
   */
   #error PORTING ALERT!

   return FALSE;
}
#endif /* BAC_IA_CURR_DATE_AND_TIME_ENABLE && BAC_IA_CURR_DATE_AND_TIME_SET */

/*******************************************************************************
** Tasks
********************************************************************************
*/
