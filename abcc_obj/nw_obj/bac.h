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
** This is the public header file for the BACnet object.
********************************************************************************
********************************************************************************
** Services:
**
** Public Services:
**    BAC_ProcessCmdMsg()     - Processes commands sent to the BACIP object.
********************************************************************************
********************************************************************************
*/
#ifndef BAC_H
#define BAC_H

#include "abcc_td.h"

/*******************************************************************************
** Defines
********************************************************************************
*/

/*------------------------------------------------------------------------------
** BACnet-specific constants for the Advanced Mapping functions
**------------------------------------------------------------------------------
*/
#if BAC_IA_SUPPORT_ADV_MAPPING_ENABLE && BAC_IA_SUPPORT_ADV_MAPPING_VALUE

#define BAC_OBJ_TYPE_AN_VAL                        2
#define BAC_OBJ_TYPE_BIN_VAL                       5
#define BAC_OBJ_TYPE_MS_VAL                        19
/*
** The ABCC only supports object instance numbers between 0 and 2039, so
** 0xFFFFFFFF is used as a 'do not assign' value.
*/
#define BAC_OBJ_TYPE_IGNORE                        0xFFFF, 0xFFFFFFFF

#endif /* BAC_IA_SUPPORT_ADV_MAPPING_ENABLE && BAC_IA_SUPPORT_ADV_MAPPING_VALUE */

/*******************************************************************************
** Typedefs
********************************************************************************
*/
#if BAC_IA_SUPPORT_ADV_MAPPING_ENABLE && BAC_IA_SUPPORT_ADV_MAPPING_VALUE

typedef struct BAC_ObjectListTag
{
   UINT16   iObjType;
   UINT32   lInstance;
}
BAC_ObjectListType;

#endif /* BAC_IA_SUPPORT_ADV_MAPPING_ENABLE && BAC_IA_SUPPORT_ADV_MAPPING_VALUE */


/*------------------------------------------------------------------------------
** Process Data map for mapping ADIs to BACnet objects
**
** Maps ADIs to BACnet objects of the given type and instance number.
**
** PORTING ALERT!
** Only used if Advanced Mapping is enabled.
**
** If Advanced Mapping (BAC_IA_SUPPORT_ADV_MAPPING_VALUE) is enabled the ADI
** list must have the same number of elemens as this list and its contents must
** match that list in a 1:1 fashion. I.e. index 'X' in both lists must refer to
** data for the same ADI.
**------------------------------------------------------------------------------
*/
#if BAC_IA_SUPPORT_ADV_MAPPING_ENABLE && BAC_IA_SUPPORT_ADV_MAPPING_VALUE

EXTVAR const BAC_ObjectListType BAC_asObjectList[];

#endif /* BAC_IA_SUPPORT_ADV_MAPPING_ENABLE && BAC_IA_SUPPORT_ADV_MAPPING_VALUE */

/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Processes commands sent to the BACnet object.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
**------------------------------------------------------------------------------
*/
EXTFUNC void BAC_ProcessCmdMsg( ABP_MsgType* psNewMessage );
#endif  /* inclusion lock */
