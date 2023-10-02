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
** Source file for the BACnet Host Object.
********************************************************************************
********************************************************************************
*/

#include "abcc_td.h"
#include "abcc.h"
#include "abcc_obj_cfg.h"
#include "bac.h"
#include "abp.h"
#include "abp_bac.h"
#include "abcc_time.h"
#include "appl_adi_config.h"

#include "string.h"

#if BAC_OBJ_ENABLE

/*******************************************************************************
** Defines
********************************************************************************
*/

/*------------------------------------------------------------------------------
** BACnet-specific constants for the Advanced Mapping functions
**------------------------------------------------------------------------------
*/
#if BAC_IA_SUPPORT_ADV_MAPPING_ENABLE && BAC_IA_SUPPORT_ADV_MAPPING_VALUE

#define BAC_MAX_OBJECT_INSTANCE_NUMBERS            2040

#define BAC_OBJ_INST_NAME_LENGTH_MIN               1
#define BAC_OBJ_INST_NAME_LENGTH_MAX               252
#define BAC_CMD_GET_ALL_OBJ_INST_LENGTH            255

#endif /* BAC_IA_SUPPORT_ADV_MAPPING_ENABLE && BAC_IA_SUPPORT_ADV_MAPPING_VALUE */

/*------------------------------------------------------------------------------
** Object attribute values
**------------------------------------------------------------------------------
*/
#define BAC_OA_NAME_VALUE                          "BACnet"
#define BAC_OA_REV_VALUE                           1
#define BAC_OA_NUM_INST_VALUE                      1
#define BAC_OA_HIGHEST_INST_VALUE                  1

/*------------------------------------------------------------------------------
** Sanity checks against ABCC_CFG_MAX_MSG_SIZE
**------------------------------------------------------------------------------
*/
#if BAC_IA_OBJECT_NAME_ENABLE
#if ABCC_CFG_MAX_MSG_SIZE < ABP_BAC_IA_OBJECT_NAME_MAX_DS
#error ABCC_CFG_MAX_MSG_SIZE is too small to hold the BACnet Object Name value!
#endif
#endif

#if BAC_IA_VENDOR_NAME_ENABLE
#if ABCC_CFG_MAX_MSG_SIZE < ABP_BAC_IA_VENDOR_NAME_MAX_DS
#error ABCC_CFG_MAX_MSG_SIZE is too small to hold the BACnet Vendor Name value!
#endif
#endif

#if BAC_IA_VENDOR_IDENTIFIER_ENABLE
#if ABCC_CFG_MAX_MSG_SIZE < ABP_BAC_IA_VENDOR_IDENTIFIER_DS
#error ABCC_CFG_MAX_MSG_SIZE is too small to hold the BACnet Vendor Identifier value!
#endif
#endif

#if BAC_IA_MODEL_NAME_ENABLE
#if ABCC_CFG_MAX_MSG_SIZE < ABP_BAC_IA_MODEL_NAME_MAX_DS
#error ABCC_CFG_MAX_MSG_SIZE is too small to hold the BACnet Model Name value!
#endif
#endif

#if BAC_IA_FIRMWARE_REVISION_ENABLE
#if ABCC_CFG_MAX_MSG_SIZE < ABP_BAC_IA_FIRMWARE_REVISION_MAX_DS
#error ABCC_CFG_MAX_MSG_SIZE is too small to hold the BACnet Firmware Revision value!
#endif
#endif

#if BAC_IA_APP_SOFTWARE_VERSION_ENABLE
#if ABCC_CFG_MAX_MSG_SIZE < ABP_BAC_IA_APP_SOFTWARE_VERSION_MAX_DS
#error ABCC_CFG_MAX_MSG_SIZE is too small to hold the BACnet Software Version value!
#endif
#endif

#if BAC_IA_SUPPORT_ADV_MAPPING_ENABLE
#if ABCC_CFG_MAX_MSG_SIZE < ABP_BAC_IA_SUPPORT_ADV_MAPPING_DS
#error ABCC_CFG_MAX_MSG_SIZE is too small to hold the BACnet Advanced Mapping value!
#endif
#if ABCC_CFG_MAX_MSG_SIZE < ( ABP_UINT16_SIZEOF + ABP_UINT32_SIZEOF )
#error ABCC_CFG_MAX_MSG_SIZE is too small to hold the BACnet Get_ADI_By_BACnet_Object_Instance data!
#endif
#if ABCC_CFG_MAX_MSG_SIZE < BAC_OBJ_INST_NAME_LENGTH_MAX
#error ABCC_CFG_MAX_MSG_SIZE is too small to hold the BACnet Get_ADI_By_BACnet_Name data!
#endif
#if ABCC_CFG_MAX_MSG_SIZE < BAC_CMD_GET_ALL_OBJ_INST_LENGTH
#error ABCC_CFG_MAX_MSG_SIZE is too small to hold the BACnet Get_All_BACnet_Object_Instances data!
#endif
#if ABCC_CFG_MAX_MSG_SIZE < ( ABP_UINT16_SIZEOF + ABP_UINT32_SIZEOF )
#error ABCC_CFG_MAX_MSG_SIZE is too small to hold the BACnet Get_BACnet_Object_Instance_By_ADI data!
#endif
#endif

#if BAC_IA_CURR_DATE_AND_TIME_ENABLE
#if ABCC_CFG_MAX_MSG_SIZE < ABP_BAC_IA_CURRENT_DATE_AND_TIME_DS
#error ABCC_CFG_MAX_MSG_SIZE is too small to hold the BACnet Current Date And Time value!
#endif
#endif

#if BAC_IA_PASSWORD_ENABLE
#if ABCC_CFG_MAX_MSG_SIZE < ABP_BAC_IA_PASSWORD_MAX_DS
#error ABCC_CFG_MAX_MSG_SIZE is too small to hold the BACnet Password value!
#endif
#endif

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*******************************************************************************
** Forward declarations
********************************************************************************
*/
static void bac_InstanceCommand( ABP_MsgType* psNewMessage );
static void bac_ObjectCommand( ABP_MsgType* psNewMessage );

/*******************************************************************************
** Private Globals
********************************************************************************
*/
#if BAC_IA_OBJECT_NAME_ENABLE && BAC_IA_OBJECT_NAME_SET
/*
** PORTING ALERT!
**
** If the Object Name attribute supports SET local NVS is needed since the
** Object Name attribute is supposed to be non-volatile.
**
** In this example implementation a local buffer is used for simplicity
** reasons. Also note that this example does not contain any function that
** resets the Object Name value to its default, like if a 'return to factory
** defaults' is ordered via the front panel of the application device, this
** must be added by the implementor.
*/
static char bac_abObjectNameValue[ ABP_BAC_IA_OBJECT_NAME_MAX_DS + 1 ] = BAC_IA_OBJECT_NAME_VALUE;
#endif /* BAC_IA_OBJECT_NAME_ENABLE && BAC_IA_OBJECT_NAME_SET */

/*******************************************************************************
** Public Services
********************************************************************************
*/

void BAC_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
   /*
   ** This function processes commands to the BACnet Object and its Instance.
   */
   if( ABCC_GetMsgInstance( psNewMessage ) == ABP_INST_OBJ )
   {
      /*
      ** The BACnet object Command.
      */
      bac_ObjectCommand( psNewMessage );
   }
   else
   {
      /*
      ** The BACnet instance Command.
      */
      bac_InstanceCommand( psNewMessage );
   }

   ABCC_SendRespMsg( psNewMessage );
}

/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Processes commands to BACnet host object Instances
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
static void bac_InstanceCommand( ABP_MsgType* psNewMessage )
{
   /*
   ** This function processes commands to the BACnet Instance 1.
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

      switch( ABCC_GetMsgCmdExt0( psNewMessage ) )
      {
#if BAC_IA_OBJECT_NAME_ENABLE
      case ABP_BAC_IA_OBJECT_NAME:
      {
         /*
         ** PORTING ALERT!
         **
         ** If the Object Name attribute only supports GET its value can come
         ** from a hard-coded string, but if it also supports SET its value
         ** will normally come from local NVS.
         **
         ** In this example a hard-coded value is used when only GET is
         ** supported, a RAM buffer is used if SET is supported.
         */
         UINT16 iStrLength;

#if BAC_IA_OBJECT_NAME_SET
         iStrLength = (UINT16)strnlen( bac_abObjectNameValue, ABP_BAC_IA_OBJECT_NAME_MAX_DS );
         ABCC_SetMsgString( psNewMessage, bac_abObjectNameValue, iStrLength, 0 );
#else
         iStrLength = (UINT16)strnlen( BAC_IA_OBJECT_NAME_VALUE, ABP_BAC_IA_OBJECT_NAME_MAX_DS );
         ABCC_SetMsgString( psNewMessage, BAC_IA_OBJECT_NAME_VALUE, iStrLength, 0 );
#endif
         ABP_SetMsgResponse( psNewMessage, iStrLength );

         break;
      }
#endif
#if BAC_IA_VENDOR_NAME_ENABLE
      case ABP_BAC_IA_VENDOR_NAME:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strnlen( BAC_IA_VENDOR_NAME_VALUE, ABP_BAC_IA_VENDOR_NAME_MAX_DS );
         ABCC_SetMsgString( psNewMessage, BAC_IA_VENDOR_NAME_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );

         break;
      }
#endif
#if BAC_IA_VENDOR_IDENTIFIER_ENABLE
      case ABP_BAC_IA_VENDOR_IDENTIFIER:
         ABCC_SetMsgData16( psNewMessage, BAC_IA_VENDOR_IDENTIFIER_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_BAC_IA_VENDOR_IDENTIFIER_DS );

         break;
#endif
#if BAC_IA_MODEL_NAME_ENABLE
      case ABP_BAC_IA_MODEL_NAME:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strnlen( BAC_IA_MODEL_NAME_VALUE, ABP_BAC_IA_MODEL_NAME_MAX_DS );
         ABCC_SetMsgString( psNewMessage, BAC_IA_MODEL_NAME_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );

         break;
      }
#endif
#if BAC_IA_FIRMWARE_REVISION_ENABLE
      case ABP_BAC_IA_FIRMWARE_REVISION:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strnlen( BAC_IA_FIRMWARE_REVISION_VALUE, ABP_BAC_IA_FIRMWARE_REVISION_MAX_DS );
         ABCC_SetMsgString( psNewMessage, BAC_IA_FIRMWARE_REVISION_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );

         break;
      }
#endif
#if BAC_IA_APP_SOFTWARE_VERSION_ENABLE
      case ABP_BAC_IA_APP_SOFTWARE_VERSION:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strnlen( BAC_IA_APP_SOFTWARE_VERSION_VALUE, ABP_BAC_IA_APP_SOFTWARE_VERSION_MAX_DS );
         ABCC_SetMsgString( psNewMessage, BAC_IA_APP_SOFTWARE_VERSION_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );

         break;
      }
#endif
#if BAC_IA_SUPPORT_ADV_MAPPING_ENABLE
      case ABP_BAC_IA_SUPPORT_ADV_MAPPING:
         ABCC_SetMsgData8( psNewMessage, BAC_IA_SUPPORT_ADV_MAPPING_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_BAC_IA_SUPPORT_ADV_MAPPING_DS );

         break;
#endif
#if BAC_IA_CURR_DATE_AND_TIME_ENABLE
      case ABP_BAC_IA_CURRENT_DATE_AND_TIME:
      {
         ABCC_TIME_DateAndTimeType sTime;
         memset( &sTime, 0, sizeof( sTime ) );

         if( !ABCC_TIME_GetDateAndTime( &sTime ) )
         {
            ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_GETABLE );
            break;
         }

         ABCC_SetMsgData16( psNewMessage, sTime.iYear, 0 );
         ABCC_SetMsgData8( psNewMessage, sTime.bMonth, 2 );
         ABCC_SetMsgData8( psNewMessage, sTime.bDay, 3 );
         ABCC_SetMsgData8( psNewMessage, sTime.bHour, 4 );
         ABCC_SetMsgData8( psNewMessage, sTime.bMinute, 5 );
         ABCC_SetMsgData8( psNewMessage, sTime.bSecond, 6 );

         ABP_SetMsgResponse( psNewMessage, ABP_BAC_IA_CURRENT_DATE_AND_TIME_DS );

         break;
      }
#endif
#if BAC_IA_PASSWORD_ENABLE
      case ABP_BAC_IA_PASSWORD:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strnlen( BAC_IA_PASSWORD_VALUE, ABP_BAC_IA_PASSWORD_MAX_DS );
         ABCC_SetMsgString( psNewMessage, BAC_IA_PASSWORD_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );

         break;
      }
#endif

      default:
         /*
         ** Unsupported attribute.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );

         break;

      } /* End of switch( attribute number )" */

      break; /* End of ABP_CMD_GET_ATTR */

   case ABP_CMD_SET_ATTR:

      switch( ABCC_GetMsgCmdExt0( psNewMessage ) )
      {
#if BAC_IA_OBJECT_NAME_ENABLE
      case ABP_BAC_IA_OBJECT_NAME:
      {
         /*
         ** PORTING ALERT!
         **
         ** If the Object Name attribute supports SET the new value should be
         ** saved in local NVS.
         **
         ** In this example a local buffer is used to hold the value, it is
         ** not saved to NVS.
         */
#if BAC_IA_OBJECT_NAME_SET
         UINT16 iStrLength;

         iStrLength = ABCC_GetMsgDataSize( psNewMessage );
         if( iStrLength > ABP_BAC_IA_OBJECT_NAME_MAX_DS )
         {
            ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_TOO_MUCH_DATA );
         }
         else if( iStrLength < 1 )
         {
            ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_NOT_ENOUGH_DATA );
         }
         else
         {
            ABCC_GetMsgString( psNewMessage, bac_abObjectNameValue, iStrLength, 0 );
            bac_abObjectNameValue[ iStrLength ] = 0;
            ABP_SetMsgResponse( psNewMessage, 0 );
         }
#else
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
#endif
         break;
      }
#endif
#if BAC_IA_CURR_DATE_AND_TIME_ENABLE
      case ABP_BAC_IA_CURRENT_DATE_AND_TIME:
      {

#if BAC_IA_CURR_DATE_AND_TIME_SET
         ABCC_TIME_DateAndTimeType sTime;
         memset( &sTime, 0, sizeof( sTime ) );

         ABCC_GetMsgData16( psNewMessage, &sTime.iYear, 0 );
         ABCC_GetMsgData8( psNewMessage, &sTime.bMonth, 2 );
         ABCC_GetMsgData8( psNewMessage, &sTime.bDay, 3 );
         ABCC_GetMsgData8( psNewMessage, &sTime.bHour, 4 );
         ABCC_GetMsgData8( psNewMessage, &sTime.bMinute, 5 );
         ABCC_GetMsgData8( psNewMessage, &sTime.bSecond, 6 );

         if( ABCC_TIME_SetDateAndTime( &sTime ) )
         {
            ABP_SetMsgResponse( psNewMessage, 0 );
         }
         else
         {
            ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         }
#else
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
#endif
         break;
      }
#endif
#if BAC_IA_VENDOR_NAME_ENABLE
      case ABP_BAC_IA_VENDOR_NAME:
#endif
#if BAC_IA_VENDOR_IDENTIFIER_ENABLE
      case ABP_BAC_IA_VENDOR_IDENTIFIER:
#endif
#if BAC_IA_MODEL_NAME_ENABLE
      case ABP_BAC_IA_MODEL_NAME:
#endif
#if BAC_IA_FIRMWARE_REVISION_ENABLE
      case ABP_BAC_IA_FIRMWARE_REVISION:
#endif
#if BAC_IA_APP_SOFTWARE_VERSION_ENABLE
      case ABP_BAC_IA_APP_SOFTWARE_VERSION:
#endif
#if BAC_IA_SUPPORT_ADV_MAPPING_ENABLE
      case ABP_BAC_IA_SUPPORT_ADV_MAPPING:
#endif
#if BAC_IA_PASSWORD_ENABLE
      case ABP_BAC_IA_PASSWORD:
#endif
#if ( BAC_IA_VENDOR_NAME_ENABLE ) ||            \
    ( BAC_IA_VENDOR_IDENTIFIER_ENABLE ) ||      \
    ( BAC_IA_MODEL_NAME_ENABLE ) ||             \
    ( BAC_IA_FIRMWARE_REVISION_ENABLE ) ||      \
    ( BAC_IA_APP_SOFTWARE_VERSION_ENABLE ) ||   \
    ( BAC_IA_SUPPORT_ADV_MAPPING_ENABLE ) ||    \
    ( BAC_IA_PASSWORD_ENABLE )
         /*
         ** ABP_ERR_ATTR_NOT_SETABLE response for all GET-only attributes.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );

         break;
#endif

      default:
         /*
         ** Unsupported attribute.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );

         break;

      } /* End of switch( attribute number )" */

      break; /* End of ABP_CMD_SET_ATTR */

   default:
      /*
      ** Unsupported command.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );

      break;

   } /* End of switch( command number ) */
}

static void bac_ObjectCommandGetAttr( ABP_MsgType* psNewMessage )
{
   switch( ABCC_GetMsgCmdExt0( psNewMessage ) )
   {
   case ABP_OA_NAME:
   {
      UINT16   iStrLength;

      iStrLength = (UINT16)strlen( BAC_OA_NAME_VALUE );
      ABCC_SetMsgString( psNewMessage, BAC_OA_NAME_VALUE, iStrLength, 0 );
      ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );

      break;
   }

   case ABP_OA_REV:
      ABCC_SetMsgData8( psNewMessage, BAC_OA_REV_VALUE, 0 );
      ABP_SetMsgResponse( psNewMessage, ABP_OA_REV_DS );

      break;

   case ABP_OA_NUM_INST:
      ABCC_SetMsgData16( psNewMessage, BAC_OA_NUM_INST_VALUE, 0 );
      ABP_SetMsgResponse( psNewMessage, ABP_OA_NUM_INST_DS );

      break;

   case ABP_OA_HIGHEST_INST:
      ABCC_SetMsgData16( psNewMessage, BAC_OA_HIGHEST_INST_VALUE, 0 );
      ABP_SetMsgResponse( psNewMessage, ABP_OA_HIGHEST_INST_DS );

      break;

   default:
      /*
      ** Unsupported attribute.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );

      break;

   } /* End of switch( attribute number ) */
}

#if BAC_IA_SUPPORT_ADV_MAPPING_ENABLE && BAC_IA_SUPPORT_ADV_MAPPING_VALUE

static void bac_ObjectCommandGetAdiByBacnetObjInst( ABP_MsgType* psNewMessage )
{
   UINT16   iReqObjType;
   UINT32   lReqInstance;
   BOOL     fMatch;
   UINT16   iNumOfAdis;
   UINT16   i;

   ABCC_GetMsgData16( psNewMessage, &iReqObjType, 0 );
   ABCC_GetMsgData32( psNewMessage, &lReqInstance, 2 );

   fMatch = FALSE;
   if( lReqInstance < BAC_MAX_OBJECT_INSTANCE_NUMBERS )
   {
      iNumOfAdis = APPL_GetNumAdi();
      for( i = 0; i < iNumOfAdis; i++ )
      {
         if( ( BAC_asObjectList[ i ].iObjType == iReqObjType ) &&
             ( BAC_asObjectList[ i ].lInstance == lReqInstance ) )
         {
            fMatch = TRUE;
            break;
         }
      }
   }

   if( fMatch )
   {
      ABCC_SetMsgData16( psNewMessage, APPL_asAdiEntryList[ i ].iInstance, 0 );
      ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF );
   }
   else
   {
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_OUT_OF_RANGE );
   }
}

static void bac_ObjectCommandGetAdiByBacnetObjInstName( ABP_MsgType* psNewMessage )
{
   UINT8    bErrCode;
   BOOL     fMatch;
   UINT16   iNumOfAdis;
   UINT8    bTmpChar;
   UINT16   iStrLen;
   UINT16   i;
   UINT16   j;

   bErrCode = ABP_ERR_NO_ERROR;

   iStrLen = iLeTOi( psNewMessage->sHeader.iDataSize );
   if( iStrLen > BAC_OBJ_INST_NAME_LENGTH_MAX )
   {
      bErrCode = ABP_ERR_TOO_MUCH_DATA;
   }
   else if( iStrLen < BAC_OBJ_INST_NAME_LENGTH_MIN )
   {
      bErrCode = ABP_ERR_NOT_ENOUGH_DATA;
   }
   else
   {
      ABCC_GetMsgData8( psNewMessage, &bTmpChar, 0 );
      if( bTmpChar == 0 )
      {
         bErrCode = ABP_ERR_NOT_ENOUGH_DATA;
      }
   }

   if( bErrCode == ABP_ERR_NO_ERROR )
   {
      fMatch = FALSE;
      iNumOfAdis = APPL_GetNumAdi();
      for( i = 0; i < iNumOfAdis; i++ )
      {
         if( APPL_asAdiEntryList[ i ].pacName != NULL )
         {
            if( APPL_asAdiEntryList[ i ].pacName[ 0 ] != 0 )
            {
               /*
               ** Manual strcmp() on the message data. Using the standard
               ** library strcmp() would require copying the message data
               ** to a local temporary buffer, but a BACnet object name can
               ** be up to 252 bytes, which might be a bit too much to
               ** allocate from the stack in an embedded system.
               */
               fMatch = TRUE;
               for( j = 0; j < iStrLen; j++ )
               {
                  ABCC_GetMsgData8( psNewMessage, &bTmpChar, j );
                  if( APPL_asAdiEntryList[ i ].pacName[ j ] != bTmpChar )
                  {
                     fMatch = FALSE;
                     break;
                  }
               }
               if( fMatch )
               {
                  if( APPL_asAdiEntryList[ i ].pacName[ j ] != 0 )
                  {
                     fMatch = FALSE;
                  }
               }
            }
         }
         if( fMatch )
         {
            break;
         }
      }

      if( fMatch )
      {
         ABCC_SetMsgData16( psNewMessage, APPL_asAdiEntryList[ i ].iInstance, 0 );
         ABCC_SetMsgData16( psNewMessage, BAC_asObjectList[ i ].iObjType, 2 );
         ABCC_SetMsgData32( psNewMessage, BAC_asObjectList[ i ].lInstance, 4 );
         ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF + ABP_UINT16_SIZEOF + ABP_UINT32_SIZEOF );
      }
      else
      {
         bErrCode = ABP_ERR_OUT_OF_RANGE;
      }
   }

   if( bErrCode != ABP_ERR_NO_ERROR )
   {
      ABP_SetMsgErrorResponse( psNewMessage, 1, bErrCode );
   }
}

static void bac_ObjectCommandGetAllBacnetObjInstances( ABP_MsgType* psNewMessage )
{
   UINT16   iReqObjType;
   UINT16   iNumOfAdis;
   UINT16   i;
   UINT8    bByteOffset;
   UINT8    bValue;

   ABCC_GetMsgData16( psNewMessage, &iReqObjType, 0 );

   /*
   ** The response contains BAC_CMD_GET_ALL_OBJ_INST_LENGTH bytes of bitmap,
   ** clear this so that we only need to set the correct bits in the next loop.
   */
   for( i = 0; i < BAC_CMD_GET_ALL_OBJ_INST_LENGTH; i++ )
   {
      ABCC_SetMsgData8( psNewMessage, 0, i );
   }
   ABP_SetMsgResponse( psNewMessage, BAC_CMD_GET_ALL_OBJ_INST_LENGTH );

   /*
   ** Each bit represents one BACnet object, step through the object list
   ** and set the corresponding bit in the message data field for objects
   ** of the given type.
   */
   iNumOfAdis = APPL_GetNumAdi();
   for( i = 0; i < iNumOfAdis; i++ )
   {
      if( BAC_asObjectList[ i ].iObjType == iReqObjType )
      {
         if( BAC_asObjectList[ i ].lInstance < BAC_MAX_OBJECT_INSTANCE_NUMBERS )
         {
            bByteOffset = BAC_asObjectList[ i ].lInstance >> 3;
            ABCC_GetMsgData8( psNewMessage, &bValue, bByteOffset );
            bValue = bValue | 1 << ( BAC_asObjectList[ i ].lInstance - ( bByteOffset << 3 ) );
            ABCC_SetMsgData8( psNewMessage, bValue, bByteOffset );
         }
      }
   }
}

static void bac_ObjectCommandGetBacnetObjInstByAdi( ABP_MsgType* psNewMessage )
{
   UINT16   iReqAdi;
   UINT16   iNumOfAdis;
   BOOL     fMatch;
   UINT16   i;

   ABCC_GetMsgData16( psNewMessage, &iReqAdi, 0 );

   fMatch = FALSE;
   iNumOfAdis = APPL_GetNumAdi();
   if( iReqAdi < iNumOfAdis )
   {
      for( i = 0; i < iNumOfAdis; i++ )
      {
         if( APPL_asAdiEntryList[ i ].iInstance == iReqAdi )
         {
            fMatch = TRUE;
            break;
         }
      }
   }

   if( fMatch )
   {
      ABCC_SetMsgData16( psNewMessage, BAC_asObjectList[ i ].iObjType, 0 );
      ABCC_SetMsgData32( psNewMessage, BAC_asObjectList[ i ].lInstance, 2 );
      ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF + ABP_UINT32_SIZEOF );
   }
   else
   {
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_OUT_OF_RANGE );
   }
}
#endif /* BAC_IA_SUPPORT_ADV_MAPPING_ENABLE && BAC_IA_SUPPORT_ADV_MAPPING_VALUE */

/*------------------------------------------------------------------------------
** Processes commands to the BACnet host object (Instance 0)
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
static void bac_ObjectCommand( ABP_MsgType* psNewMessage )
{
   /*
   ** This function processes commands to the BACnet Object (Instance 0).
   */
   switch( ABCC_GetMsgCmdBits( psNewMessage ) )
   {
   case ABP_CMD_GET_ATTR:
      bac_ObjectCommandGetAttr( psNewMessage );
      break;

#if BAC_IA_SUPPORT_ADV_MAPPING_ENABLE && BAC_IA_SUPPORT_ADV_MAPPING_VALUE
      /*
      ** PORTING ALERT!
      **
      ** If the support for Advanced Mapping is enabled these four services *may*
      ** require extra adaptation or tweaking. They shall work 'as is' if the
      ** the formats and functions of the BACnet-related data structures in this
      ** file and bac.h are not changed.
      */

   case ABP_BAC_CMD_GET_ADI_BY_BACNET_OBJ_INST:
      bac_ObjectCommandGetAdiByBacnetObjInst( psNewMessage );
      break;

   case ABP_BAC_CMD_GET_ADI_BY_BACNET_OBJ_INST_NAME:
      bac_ObjectCommandGetAdiByBacnetObjInstName( psNewMessage );
      break;

   case ABP_BAC_CMD_GET_ALL_BACNET_OBJ_INSTANCES:
      bac_ObjectCommandGetAllBacnetObjInstances( psNewMessage );
      break;

   case ABP_BAC_CMD_GET_BACNET_OBJ_INST_BY_ADI:
      bac_ObjectCommandGetBacnetObjInstByAdi( psNewMessage );
      break;

#endif /* BAC_IA_SUPPORT_ADV_MAPPING_ENABLE && BAC_IA_SUPPORT_ADV_MAPPING_VALUE */

   default:
      /*
      ** Unsupported command.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;

   } /* End of switch( command number ) */
}

#endif /* BAC_OBJ_ENABLE */
