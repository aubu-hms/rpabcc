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
** File Description:
** This is the public header file for the EtherCat object.
********************************************************************************
********************************************************************************
** Services:
**
** Public Services:
**    ECT_ProcessCmdMsg()     - Processes commands sent to the ECT object.
********************************************************************************
********************************************************************************
*/
#ifndef ECT_H
#define ECT_H

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** EtherCAT states
**------------------------------------------------------------------------------
*/
#if ECT_IA_ETHERCAT_STATE_ENABLE

typedef enum ECT_State
{
  ECT_STATE_INIT    = 0x01,
  ECT_STATE_PREOP   = 0x02,
  ECT_STATE_BOOT    = 0x03,
  ECT_STATE_SAFEOP  = 0x04,
  ECT_STATE_OP      = 0x08
}
ECT_StateType;

#endif
/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void ECT_ProcessCmdMsg()
** Processes commands sent to the ECT object.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       ECT_ProcessCmdMsg( &sMessage );
**------------------------------------------------------------------------------
*/
EXTFUNC void ECT_ProcessCmdMsg( ABP_MsgType* psNewMessage );

#if ECT_IA_ETHERCAT_STATE_ENABLE
/*------------------------------------------------------------------------------
** ECT_StateType ECT_GetActualEctState()
** Retreives the last EtherCAT state set by the ABCC.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       The last EtherCAT state set by ABCC.
**
** Usage:
**       ECT_GetActualEctState( );
**------------------------------------------------------------------------------
*/
EXTFUNC ECT_StateType ECT_GetActualEctState( void );
#endif

#endif  /* inclusion lock */
