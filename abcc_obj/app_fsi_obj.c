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
** Source file for Application File System Interface object.                  **
**                                                                            **
** Not the complete Application File System Interface is implemented.         **
** Currently 1 instance is supported.                                         **                                                                        **
********************************************************************************
********************************************************************************
*/

#include "abcc_obj_cfg.h"

#if APP_FSI_OBJ_ENABLE

#include "abcc_td.h"
#include "abcc_sys_adapt.h"
#include "abcc_drv_cfg.h"
#include "app_obj.h"
#include "abcc.h"
#include "appl_abcc_handler.h"
#include "abcc_port.h"
#include "abp_fsi.h"
#include "anb_fsi_obj.h"
#include "app_fsi_obj.h"
#include "abcc_platform_cfg.h"
#include "appl_adi_config.h"

/*******************************************************************************
** Constants
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Object attribute values
**------------------------------------------------------------------------------
*/
#define APP_FSI_OA_NAME_VALUE   "Application File System Interface"
#define APP_FSI_OA_REV_VALUE    1

/*------------------------------------------------------------------------------
** Max number of instances in application.
**------------------------------------------------------------------------------
*/
#define APP_FSI_MAX_NUMBER_OF_INSTANCE 1

/*------------------------------------------------------------------------------
** Indicate that instance is free.
**------------------------------------------------------------------------------
*/
#define APP_FSI_INSTANCE_FREE 0x0000

/*------------------------------------------------------------------------------
** Indicate that instance is not allocated.
**------------------------------------------------------------------------------
*/
#define APP_FSI_INSTANCE_NOT_ALLOCATED 0xFFFE

/*------------------------------------------------------------------------------
** Indicate that instance is not found.
**------------------------------------------------------------------------------
*/
#define APP_FSI_INSTANCE_NOT_FOUND 0xFFFF

/*------------------------------------------------------------------------------
** Application file instance 1.
**------------------------------------------------------------------------------
*/
#define APP_FSI_INSTANCE_1 0x0001

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Structure describing Application File System Interface Object.
**------------------------------------------------------------------------------
*/

typedef struct app_fsi_Object
{
    const  char* pcName;
    const UINT8  bRevision;
    UINT16 iNumberOfInstances;
    UINT16 iHighestInstanceNo;
}
app_fsi_ObjectType;

/*******************************************************************************
** Forward declarations
********************************************************************************
*/

static UINT16 FindInstance( UINT16 iInst );
static UINT16 AllocateInst( UINT16 iInstToAllocate );
static void ReleaseInst( UINT16 iInst );
static void CreateInst( ABP_MsgType* psNewMessage );
static void DeleteInst( ABP_MsgType* psNewMessage );

/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*******************************************************************************
** Private Globals
********************************************************************************
*/

static app_fsi_ObjectType app_fsi_sObject =
{
   APP_FSI_OA_NAME_VALUE,   /* Name.                    */
   APP_FSI_OA_REV_VALUE,    /* Revision.                */
   0,                       /* Number of instances.     */
   0                        /* Highest instance number. */
};

static APP_FSI_FileType app_fsi_sFile;
static APP_FSI_FileInstanceType app_fsi_sFileInst[ APP_FSI_MAX_NUMBER_OF_INSTANCE ] = { { APP_FSI_INSTANCE_FREE,
                                                                                          &app_fsi_sFile,
                                                                                          ABP_FSI_TYPE_NONE } };
/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** FindInstance - finds the index containing the specified instance
**------------------------------------------------------------------------------
** Arguments:
**    iInst - instance to find.
**
** Returns:
**    APP_FSI_INSTANCE_NOT_FOUND if instance was not found,
**    else returns the index where the instance resides.
**------------------------------------------------------------------------------
*/
static UINT16 FindInstance( UINT16 iInst )
{
   UINT16 iIndex;

   for( iIndex = 0; iIndex < APP_FSI_MAX_NUMBER_OF_INSTANCE; iIndex++ )
   {
      if( app_fsi_sFileInst[ iIndex ].iInst == iInst )
      {
         return( iIndex );
      }
   }

   return( APP_FSI_INSTANCE_NOT_FOUND );
}

/*------------------------------------------------------------------------------
** AllocateInst - allocate a new Application File System Interface instance.
**------------------------------------------------------------------------------
** Arguments:
**    None.
**
** Returns:
**    If allocation was successful, return the new Application File System
**    Interface instance value.
**    Else return APP_FSI_INSTANCE_NOT_ALLOCATED.
**------------------------------------------------------------------------------
*/
static UINT16 AllocateInst( UINT16 iInstToAllocate )
{
   UINT16  iIndex;

   /*
   ** Find free entry in instance structure
   */
   iIndex = FindInstance( APP_FSI_INSTANCE_FREE );

   if( iIndex == APP_FSI_INSTANCE_NOT_FOUND )
   {
      /*
      ** No free entry available
      */
      return( APP_FSI_INSTANCE_NOT_ALLOCATED );
   }

   /*
   ** Get the lowest available instance
   */
   while( 1 )
   {
      if( FindInstance( iInstToAllocate ) == APP_FSI_INSTANCE_NOT_FOUND  )
      {
         break;
      }
      else
      {
         iInstToAllocate++;
      }
   }

   app_fsi_sFileInst[ iIndex ].iInst = iInstToAllocate;
   app_fsi_sObject.iNumberOfInstances++;

   return( iInstToAllocate );
}

/*------------------------------------------------------------------------------
** DeleteInst - routine to handle delete requests from the ABCC module.
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage - Pointer to a ABP_MsgType message.
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
static void DeleteInst( ABP_MsgType* psNewMessage )
{
   UINT16 iInst;

   iInst = ABCC_GetMsgCmdExt( psNewMessage );

   ReleaseInst( iInst );

   ABP_SetMsgResponse( psNewMessage, 0 );
}

/*------------------------------------------------------------------------------
** CreateInst - routine to handle create requests from the ABCC module.
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage - Pointer to a ABP_MsgType message.
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
static void CreateInst( ABP_MsgType* psNewMessage )
{
   UINT16 iInst;
   UINT16 iInstToAllocate;

   iInstToAllocate = APP_FSI_INSTANCE_1;

   iInst = AllocateInst( iInstToAllocate );

   if( iInst != APP_FSI_INSTANCE_NOT_ALLOCATED )
   {
      ABCC_SetMsgData16( psNewMessage, iInst, 0 );
      ABP_SetMsgResponse( psNewMessage, 2 );
   }
   else
   {
      /*
      ** Abort, indicate that an instance could not be created.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_NO_RESOURCES );
   }
}

/*------------------------------------------------------------------------------
** ReleaseInst - release an Application File System Interface instance.
**------------------------------------------------------------------------------
** Arguments:
**    iInst - instance to release.
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
static void ReleaseInst( UINT16 iInst )
{
   UINT16 iIndex;

   /*
   ** Find the instance.
   */
   iIndex = FindInstance( iInst );

   if( iIndex == APP_FSI_INSTANCE_NOT_FOUND )
   {

      /*
      ** An instance is being deleted that does not exist.
      */
      ABCC_ERROR( ABCC_SEV_FATAL, ABCC_EC_INTERNAL_ERROR, 0 );

      return;
   }

   /*
   ** File instance de-allocation.
   */
   app_fsi_sFileInst[ iIndex ].iInst = APP_FSI_INSTANCE_FREE;
   app_fsi_sFileInst[ iIndex ].bInstType = ABP_FSI_TYPE_NONE;

   if( app_fsi_sObject.iNumberOfInstances > 0 )
   {
      app_fsi_sObject.iNumberOfInstances--;
   }
   else
   {
      ABCC_ERROR( ABCC_SEV_FATAL, ABCC_EC_INTERNAL_ERROR, 0 );
   }
}


/*------------------------------------------------------------------------------
** GetInstAttributes - routine to handle get instance attributes command.
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage - Pointer to a ABP_MsgType message.
**    iIndex       - Instance index.
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
static void GetInstAttributes( ABP_MsgType* psNewMessage, UINT16 iIndex )
{

   /*
   ** Get Instance Attribute service.
   */
   switch( ABCC_GetMsgCmdExt0( psNewMessage ) )
   {
   case ABP_FSI_IA_TYPE:

      /*
      ** Get the instance type.
      */
      ABCC_SetMsgData8( psNewMessage, app_fsi_sFileInst[ iIndex ].bInstType, 0 );

      ABP_SetMsgResponse( psNewMessage, ABP_FSI_IA_TYPE_DS );

      break;

   case ABP_FSI_IA_FILE_SIZE:

      /*
      ** Get the instance's file size.
      */
      if( app_fsi_sFileInst[ iIndex ].bInstType == ABP_FSI_TYPE_FILE )
      {
         if( app_fsi_sFileInst[ iIndex ].psFile )
         {
            ABCC_SetMsgData32( psNewMessage,
                               app_fsi_sFileInst[ iIndex ].psFile->lSize,
                               0 );

            ABP_SetMsgResponse( psNewMessage, ABP_FSI_IA_FILE_SIZE_DS );
         }
         else
         {
            ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_INST );
         }
      }
      else
      {
         ABCC_SetMsgData32( psNewMessage, 0, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_FSI_IA_FILE_SIZE_DS );
      }

      break;

   case ABP_FSI_IA_PATH:
   default:

      /*
      ** Unsupported attribute.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );

      break;
   }
}

/*------------------------------------------------------------------------------
** The function that processes the commands to the Application File System
** Interface Object (instance 0).
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
   UINT16 iStrLength;

   switch( ABCC_GetMsgCmdBits( psNewMessage ) )
   {
   case ABP_CMD_GET_ATTR:
   {
      switch( ABCC_GetMsgCmdExt0( psNewMessage ) )
      {
      case ABP_OA_NAME:

         /*
         ** The 'name' attribute is requested.
         ** Copy the attribute to a response message.
         */
         iStrLength = (UINT16)strlen( app_fsi_sObject.pcName );
         ABCC_SetMsgString( psNewMessage, app_fsi_sObject.pcName, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;

      case ABP_OA_REV:

         /*
         ** The 'revision' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData8( psNewMessage, app_fsi_sObject.bRevision, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_REV_DS );
         break;

      case ABP_OA_NUM_INST:

         /*
         ** The 'Number of Instances' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData16( psNewMessage, app_fsi_sObject.iNumberOfInstances, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_NUM_INST_DS );
         break;

      case ABP_OA_HIGHEST_INST:

         /*
         ** The 'Highest Instance Number' attribute is requested.
         ** Copy the attribute to a response message.
         */
      {
         UINT16 iHighestInst;
         UINT16 iIndex;

         iHighestInst = 0;

         for( iIndex = 0; iIndex < APP_FSI_MAX_NUMBER_OF_INSTANCE; iIndex++ )
         {
            if( iHighestInst < app_fsi_sFileInst[ iIndex ].iInst )
            {
               iHighestInst = app_fsi_sFileInst[ iIndex ].iInst;
            }
         }

         ABCC_SetMsgData16( psNewMessage, iHighestInst, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_HIGHEST_INST_DS );

         break;
      }

      case ABP_FSI_OA_MAX_INST:

         ABCC_SetMsgData16( psNewMessage, APP_FSI_MAX_NUMBER_OF_INSTANCE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_FSI_OA_MAX_INST_DS );

         break;

      default:

         /*
         ** Unsupported attribute.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;
      }
      break;
   }

   case ABP_CMD_CREATE:

      /*
      ** Create a new object instance for file transfer.
      */
      CreateInst( psNewMessage );

      break;

   case ABP_CMD_DELETE:

      /*
      ** Delete the object instance for file transfer.
      */
      DeleteInst( psNewMessage );

      break;

   default:

      /*
      ** Unsupported command.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );

      break;

   }
}

/*------------------------------------------------------------------------------
** The function that processes the commands to the Application File
** System Interface Object instance.
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
   UINT16 iIndex;

   iIndex = FindInstance( ABCC_GetMsgInstance( psNewMessage ) );

   if( iIndex == APP_FSI_INSTANCE_NOT_FOUND )
   {
      /*
      ** Instance does not exist.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_INST );

      return;
   }

   switch( ABCC_GetMsgCmdBits( psNewMessage ) )
   {
   case ABP_CMD_GET_ATTR:

      /*
      ** Get instance attribute.
      */
      GetInstAttributes( psNewMessage, iIndex );

      break;

   case ABP_FSI_CMD_FILE_OPEN:

      /*
      ** Open requested file.
      */
      APPL_FileOpen( psNewMessage, &app_fsi_sFileInst[ iIndex ] );

      break;

   case ABP_FSI_CMD_FILE_CLOSE:

      /*
      ** Close requested file.
      */
      APPL_FileClose( psNewMessage, &app_fsi_sFileInst[ iIndex ] );

      break;

   case ABP_FSI_CMD_FILE_WRITE:

      /*
      ** Write to opened file instance.
      */
      APPL_FileWrite( psNewMessage, &app_fsi_sFileInst[ iIndex ] );

      break;

   case ABP_FSI_CMD_FILE_READ:
   case ABP_FSI_CMD_DIRECTORY_OPEN:
   case ABP_FSI_CMD_DIRECTORY_CLOSE:
   case ABP_FSI_CMD_DIRECTORY_READ:
   case ABP_FSI_CMD_FILE_COPY:
   case ABP_FSI_CMD_FILE_RENAME:
   case ABP_FSI_CMD_FILE_DELETE:
   case ABP_FSI_CMD_DIRECTORY_CREATE:
   case ABP_FSI_CMD_DIRECTORY_CHANGE:
   case ABP_FSI_CMD_DIRECTORY_DELETE:
   default:

         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );

      break;
   }
}

/*******************************************************************************
** Public Services
********************************************************************************
*/

void APP_FSI_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
   if( ABCC_GetMsgInstance( psNewMessage ) == ABP_INST_OBJ )
   {
      /*
      ** Process the Application File System
      ** Interface Object Command.
      */
      ObjectCommand( psNewMessage );
   }
   else
   {
      /*
      ** Process the Application File System
      ** Interface Instance Command.
      */
      InstanceCommand( psNewMessage );
   }

   ABCC_SendRespMsg( psNewMessage );
}

#endif /* APP_FSI_OBJ_ENABLE */
