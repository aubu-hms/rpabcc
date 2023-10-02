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
** COPYRIGHT NOTIFICATION (c) 2015 HMS Industrial Networks AB                 **
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
** This is the public header file for the DeviceNet Object.
********************************************************************************
********************************************************************************
** Services:
**    DEV_ProcessCmdMsg()     - Processes commands sent to the DEV object.
**
********************************************************************************
********************************************************************************
*/

#ifndef DEV_H
#define DEV_H

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
#if ASM_OBJ_ENABLE

/*------------------------------------------------------------------------------
** Variable declarations for attribute #21(Producing Instance Number list) and
** attribute #22(Consuming Instance Number list).
** If the Assembly object is enabled, those variables must be defined elsewhere.
** An example is available in "/example_app/appl_adimap_asm.c".
**------------------------------------------------------------------------------
*/
EXTVAR const UINT16 DEV_aiProdInstNumList[];
EXTVAR const UINT16 DEV_iProdInstNumListSize;

EXTVAR const UINT16 DEV_aiConsInstNumList[];
EXTVAR const UINT16 DEV_iConsInstNumListSize;

#endif /* ASM_OBJ_ENABLE */

/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Processes commands sent to the DEV object
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
EXTFUNC void DEV_ProcessCmdMsg( ABP_MsgType* psNewMessage );

#endif  /* inclusion lock */
