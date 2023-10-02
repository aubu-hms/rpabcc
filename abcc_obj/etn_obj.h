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
** This is the public header file for the Ethernet IO object.
********************************************************************************
********************************************************************************
** Services:
**    ETN_ProcessCmdMsg()     - Processes commands sent to the ETN object.
**
********************************************************************************
********************************************************************************
*/

#ifndef ETN_H
#define ETN_H

/*******************************************************************************
** Constants
********************************************************************************
*/

/*******************************************************************************
** Typedefs
********************************************************************************
*/

#if ETN_IA_IP_CONFIGURATION_ENABLE
/*------------------------------------------------------------------------------
** Structure storing attribute #16 'IP configuration' data
**------------------------------------------------------------------------------
*/
typedef union ETN_IpConfigType
{
   UINT32 alIpConfig[ 3 ];
   struct
   {
      UINT32 lIpAddr;
      UINT32 lSnMask;
      UINT32 lGwAddr;
   }
   sAddr;
}
ETN_IpConfigType;
#endif /* #if ETN_IA_IP_CONFIGURATION_ENABLE */

/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Processes commands sent to the Ethernet object.
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ETN_ProcessCmdMsg( ABP_MsgType* psNewMessage );

#if ETN_IA_IP_CONFIGURATION_ENABLE
/*------------------------------------------------------------------------------
** Fetch the present content of the IP address cfg. structure.
**------------------------------------------------------------------------------
** Arguments:
**    psIpConfig  - Pointer to structure to store current IP configuration
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ETN_GetIpConfig( ETN_IpConfigType* psIpConfig );
#endif /* #if ETN_IA_IP_CONFIGURATION_ENABLE */

#if ETN_IA_NETWORK_STATUS_ENABLE
/*------------------------------------------------------------------------------
** Fetch the present content of the Network Status word.
**------------------------------------------------------------------------------
** Arguments:
**    piStatus    -  Pointer to storage for the status bitfield.
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ETN_GetNwStatus( UINT16* piNwStatus );
#endif /* #if ETN_IA_NETWORK_STATUS_ENABLE */

/*******************************************************************************
** Callbacks
********************************************************************************
*/

#if ETN_OBJ_USE_SET_ATTR_SUCCESS_CALLBACK
/*------------------------------------------------------------------------------
** Optional user callback function that needs to be implemented by user
** if ETN_OBJ_USE_SET_ATTR_SUCCESS_CALLBACK is set to TRUE.
**------------------------------------------------------------------------------
** Arguments:
**    iInstance   - Object instance number
**    bAttribute  - Object instance attribute number
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ETN_SetAttrSuccessCallback( UINT16 iInstance, UINT8 bAttribute );
#endif /* #if ETN_OBJ_USE_SET_ATTR_SUCCESS_CALLBACK */

#endif /* Inclusion lock */
