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
** COPYRIGHT NOTIFICATION (c) 2022 HMS Industrial Networks AB                 **
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
** Source file for the CC-Link IE TSN Object.
********************************************************************************
********************************************************************************
*/

#include "abcc_td.h"
#include "abcc.h"
#include "abcc_obj_cfg.h"
#include "ciet.h"
#include "abp.h"
#include "abp_ciet.h"
#include "string.h"
#include "appl_abcc_handler.h"
#include "abcc_port.h"

#if CIET_OBJ_ENABLE

/*******************************************************************************
** Defines
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Object attribute values
**------------------------------------------------------------------------------
*/
#define CIET_OA_NAME_VALUE                          "CC-Link IE TSN"
#define CIET_OA_REV_VALUE                           1
#define CIET_OA_NUM_INST_VALUE                      1
#define CIET_OA_HIGHEST_INST_VALUE                  1

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Structure describing an CC-Link IE TSN Object.
**------------------------------------------------------------------------------
*/
typedef struct ciet_Object
{
   const  char* pcName;
   UINT8  bRevision;
   UINT16 iNumberOfInstances;
   UINT16 iHighestInstanceNo;
}
ciet_ObjectType;

/*------------------------------------------------------------------------------
** Forward declarations
**------------------------------------------------------------------------------
*/
static void InstanceCommand( ABP_MsgType* psNewMessage );
static void ObjectCommand( ABP_MsgType* psNewMessage );

/*******************************************************************************
** Private Globals
********************************************************************************
*/

static const ciet_ObjectType ciet_sObject =
{
   CIET_OA_NAME_VALUE,                           /* Name.                                              */
   CIET_OA_REV_VALUE,                            /* Revision.                                          */
   CIET_OA_NUM_INST_VALUE,                       /* Number of instances.                               */
   CIET_OA_HIGHEST_INST_VALUE                    /* Highest instance number.                           */
};

/*******************************************************************************
** Public Services
********************************************************************************
*/

void CIET_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
   /*
   ** This function processes commands to the CC-Link IE TSN Object and it's Instance.
   */
   if( ABCC_GetMsgInstance( psNewMessage ) == ABP_INST_OBJ )
   {
      /*
      ** CC-Link IE TSN object Command.
      */
      ObjectCommand( psNewMessage );
   }
   else
   {
      /*
      ** CC-Link IE TSN instance Command.
      */
      InstanceCommand( psNewMessage );
   }

   ABCC_SendRespMsg( psNewMessage );
}

/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Processes commands to CIET Instances
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
static void InstanceCommand( ABP_MsgType* psNewMessage )
{
   /*
   ** This function processes commands to the CC-Link IE TSN Instance.
   */
   if( ABCC_GetMsgInstance( psNewMessage ) != 1 )
   {
      /*
      ** The Instance does not exist.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_INST );
      return;
   }

   switch( ABCC_GetMsgCmdBits( psNewMessage ) )
   {
   case ABP_CMD_GET_ATTR:
   {
      switch( ABCC_GetMsgCmdExt0( psNewMessage ) )
      {
#if CIET_IA_VENDOR_CODE_ENABLE
      case ABP_CIET_IA_VENDOR_CODE:

         /*
         ** Copy the 1st Instance 1 attribute (Vendor Code) to the message.
         */
         ABCC_SetMsgData16( psNewMessage, CIET_IA_VENDOR_CODE_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_CIET_IA_VENDOR_CODE_DS );
         break;
#endif
#if CIET_IA_VENDOR_NAME_ENABLE
      case ABP_CIET_IA_VENDOR_NAME:
      {
         UINT16 iStrLength;

         /*
         ** Copy the 2nd Instance 1 attribute (Vendor Name) to the message.
         */
         iStrLength = (UINT16)strlen( CIET_IA_VENDOR_NAME_VALUE );
         ABCC_SetMsgString( psNewMessage, CIET_IA_VENDOR_NAME_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;
      }
#endif
#if CIET_IA_MODEL_CODE_ENABLE
      case ABP_CIET_IA_MODEL_CODE:

         /*
         ** Copy the 3rd Instance 1 attribute (Model Code) to the message.
         */
         ABCC_SetMsgData32( psNewMessage, CIET_IA_MODEL_CODE_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_CIET_IA_MODEL_CODE_DS );
         break;
#endif
#if CIET_IA_EXPANSION_MODEL_CODE_ENABLE
      case ABP_CIET_IA_EXPANSION_MODEL_CODE:
      {
         /*
         ** Copy the 4th Instance 1 attribute (Expansion Model Code) to the message.
         */
         ABCC_SetMsgData16( psNewMessage, CIET_IA_EXPANSION_MODEL_CODE_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_CIET_IA_EXPANSION_MODEL_CODE_DS );
         break;
      }
#endif
#if CIET_IA_MODEL_NAME_ENABLE
      case ABP_CIET_IA_MODEL_NAME:
      {
         UINT16 iStrLength;

         /*
         ** Copy the 5th Instance 1 attribute (Model Name) to the message.
         */
         iStrLength = (UINT16)strlen( CIET_IA_MODEL_NAME_VALUE );
         ABCC_SetMsgString( psNewMessage, CIET_IA_MODEL_NAME_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;
      }
#endif
#if CIET_IA_DEVICE_TYPE_ENABLE
      case ABP_CIET_IA_DEVICE_TYPE:

         /*
         ** Copy the 6th Instance 1 attribute (Device Type) to the message.
         */
         ABCC_SetMsgData16( psNewMessage, CIET_IA_DEVICE_TYPE_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_CIET_IA_DEVICE_TYPE_DS );
         break;
#endif
#if CIET_IA_DEVICE_VERSION_ENABLE
      case ABP_CIET_IA_DEVICE_VERSION:

         /*
         ** Copy the 7th Instance 1 attribute (Device Version) to the message.
         */
         ABCC_SetMsgData16( psNewMessage, CIET_IA_DEVICE_VERSION_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_CIET_IA_DEVICE_VERSION_DS );
         break;
#endif
#if CIET_IA_FW_VERSION_ENABLE
      case ABP_CIET_IA_FW_VERSION:

         /*
         ** Copy the 8th Instance 1 attribute (Firmware Version) to the message.
         */
         ABCC_SetMsgData16( psNewMessage, CIET_IA_FW_VERSION_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_CIET_IA_FW_VERSION_DS );
         break;
#endif
#if CIET_IA_HW_VERSION_ENABLE
      case ABP_CIET_IA_HW_VERSION:

         /*
         ** Copy the 9th Instance 1 attribute (Hardware Version) to the message.
         */
         ABCC_SetMsgData8( psNewMessage, CIET_IA_HW_VERSION_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_CIET_IA_HW_VERSION_DS );
         break;
#endif
#if CIET_IA_SERIAL_NUMBER_ENABLE
      case ABP_CIET_IA_SERIAL_NUMBER:
      {
         UINT16 iStrLength;

         /*
         ** Copy the 10th Instance 1 attribute (Serial Number) to the message.
         */
         iStrLength = (UINT16)strlen( CIET_IA_SERIAL_NUMBER_VALUE );
         ABCC_SetMsgString( psNewMessage, CIET_IA_SERIAL_NUMBER_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;
      }
#endif
      default:

         /*
         ** Unsupported attribute.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;
      }
      break;
   }
   default:

      /*
      ** Unsupported command.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;

   } /* End of switch( Command number ) */
}

/*------------------------------------------------------------------------------
** Processes commands to the CIET Object (Instance 0)
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
static void ObjectCommand( ABP_MsgType* psNewMessage )
{
   /*
   ** This function processes commands to the CC-Link IE TSN Object (Instance 0).
   */
   switch( ABCC_GetMsgCmdBits( psNewMessage ) )
   {
   case ABP_CMD_GET_ATTR:
   {
      switch( ABCC_GetMsgCmdExt0( psNewMessage ) )
      {
      case ABP_OA_NAME:
      {
         UINT16 iStrLength;

         /*
         ** Copy the attribute to a message.
         */
         iStrLength = (UINT16)strlen( ciet_sObject.pcName );
         ABCC_SetMsgString( psNewMessage, ciet_sObject.pcName, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;
      }

      case ABP_OA_REV:

         /*
         ** Copy the attribute to a message.
         */
         ABCC_SetMsgData8( psNewMessage, ciet_sObject.bRevision, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_REV_DS );
         break;

      case ABP_OA_NUM_INST:

         /*
         ** Copy the attribute to a message.
         */
         ABCC_SetMsgData16( psNewMessage, ciet_sObject.iNumberOfInstances, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_NUM_INST_DS );
         break;

      case ABP_OA_HIGHEST_INST:

         /*
         ** Copy the attribute to a message.
         */
         ABCC_SetMsgData16( psNewMessage, ciet_sObject.iHighestInstanceNo, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_HIGHEST_INST_DS );
         break;

      default:

         /*
         ** Unsupported attribute.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;

      } /* End of switch( Attribute number ) */

      break;
   }

   default:

      /*
      ** Unsupported command.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;

   } /* End of switch( Command number ) */
}

#endif /* CIET_OBJ_ENABLE */
