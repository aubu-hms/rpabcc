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
** Source file for Anybus File System Interface object.                       **
**                                                                            **
** Implements basic support for Anybus File System Interface object.          **
** Not complete object is implemented.                                        **
********************************************************************************
********************************************************************************
*/

#include "abcc_obj_cfg.h"

#if ANB_FSI_OBJ_ENABLE

#include "anb_fsi_obj.h"
#include "abp_fsi.h"
#include "appl_abcc_handler.h"
#include "abcc_port.h"
#include "abcc_sw_port.h"
#include "abcc_platform_cfg.h"

/*******************************************************************************
** Constants
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Max length of data, when data is written to file.
**------------------------------------------------------------------------------
*/
#define ANB_FSI_WRITE_MAX_DATA_LEN 255

/*------------------------------------------------------------------------------
** Max length for filename.
**------------------------------------------------------------------------------
*/
#define ANB_FSI_MAX_LEN_FILENAME 128

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*******************************************************************************
** Forward declarations
********************************************************************************
*/
static ABCC_CmdSeqCmdStatusType CreateFSIInstCmd( ABP_MsgType* psMsg );
static ABCC_CmdSeqCmdStatusType DeleteCmd( ABP_MsgType* psMsg );
static ABCC_CmdSeqCmdStatusType FileCloseCmd( ABP_MsgType* psMsg );
static ABCC_CmdSeqCmdStatusType FileOpenCmd( ABP_MsgType* psMsg );
static ABCC_CmdSeqCmdStatusType FileWriteCmd( ABP_MsgType* psMsg );
static ABCC_CmdSeqCmdStatusType InvalidateFirmwareFileCmd( ABP_MsgType* psMsg );

static ABCC_CmdSeqRespStatusType HandleFSIInstRsp( ABP_MsgType* psMsg );
static ABCC_CmdSeqRespStatusType FileWriteRsp( ABP_MsgType* psMsg );
static ABCC_CmdSeqRespStatusType ResponseHandler( ABP_MsgType* psMsg );

/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*******************************************************************************
** Private Globals
********************************************************************************
*/
static UINT8  anb_fsi_abFsiWriteData[ ANB_FSI_WRITE_MAX_DATA_LEN ];
static UINT16 anb_fsi_iInstance;
static UINT16 anb_fsi_iFsiWriteOffset;
static UINT16 anb_fsi_iFsiWriteSize;

static char  anb_fsi_acFilename[ ANB_FSI_MAX_LEN_FILENAME ];
static UINT8 anb_fsi_bFilenameLength;
static UINT8 anb_fsi_bAccessMode;
static BOOL anb_fsi_fInstanceAccessLocked;

/*------------------------------------------------------------------------------
** Command sequence to create an FSI instance and open a file
** on the ABCC.
**------------------------------------------------------------------------------
*/
static const ABCC_CmdSeqType anb_fsi_asFileOpenSeq[] =
{
   ABCC_CMD_SEQ( CreateFSIInstCmd, HandleFSIInstRsp ),
   ABCC_CMD_SEQ( FileOpenCmd, ResponseHandler ),
   ABCC_CMD_SEQ_END()
};

/*------------------------------------------------------------------------------
** Command sequence to write data to a file on the ABCC.
**------------------------------------------------------------------------------
*/
static const ABCC_CmdSeqType anb_fsi_asFileWriteSeq[] =
{
   ABCC_CMD_SEQ( FileWriteCmd, FileWriteRsp ),
   ABCC_CMD_SEQ_END()
};

/*------------------------------------------------------------------------------
** Command sequence to close the opened file and delete the FSI instance.
**------------------------------------------------------------------------------
*/
static const ABCC_CmdSeqType anb_fsi_asCloseSeq[] =
{
   ABCC_CMD_SEQ( FileCloseCmd, ResponseHandler ),
   ABCC_CMD_SEQ( DeleteCmd, ResponseHandler ),
   ABCC_CMD_SEQ_END()
};

/*------------------------------------------------------------------------------
**  Command sequence to write an invalid firmware file to the ABCC.
**  Useful for network-specific error indication of firmware update failure
**  when attempting to update host firmware.
**------------------------------------------------------------------------------
*/
static const ABCC_CmdSeqType anb_fsi_asInvalidateAbccFwSeq[] =
{
   ABCC_CMD_SEQ( FileCloseCmd, ResponseHandler ),
   ABCC_CMD_SEQ( FileOpenCmd, ResponseHandler ),
   ABCC_CMD_SEQ( InvalidateFirmwareFileCmd, FileWriteRsp ),
   ABCC_CMD_SEQ( FileCloseCmd, ResponseHandler ),
   ABCC_CMD_SEQ( DeleteCmd, ResponseHandler ),
   ABCC_CMD_SEQ_END()
};

/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Lock instance so that data is written sequentially.
**------------------------------------------------------------------------------
** Arguments:
**    fLock                    - TRUE to lock instance. FALSE to unlock.
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
static void SetInstanceLocked( BOOL fLock )
{
   anb_fsi_fInstanceAccessLocked = fLock;
}

/*------------------------------------------------------------------------------
** Creates an FSI instance.
**------------------------------------------------------------------------------
** Arguments:
**    psMsg                    - Message to send to ABCC.
**
** Returns:
**    ABCC_CmdSeqCmdStatusType - Response for command callback.
**------------------------------------------------------------------------------
*/
static ABCC_CmdSeqCmdStatusType CreateFSIInstCmd( ABP_MsgType* psMsg )
{
   ABCC_SetMsgHeader( psMsg,
                      ABP_OBJ_NUM_FSI,
                      0,
                      0,
                      (ABP_MsgCmdType)( ABP_MSG_HEADER_C_BIT | ABP_CMD_CREATE ),
                      0,
                      ABCC_GetNewSourceId() );

   return( ABCC_SEND_COMMAND );
}

/*------------------------------------------------------------------------------
** Processes a response to a CreateFSIInstCmd. Reads in and stores the new
** FSI instance number that was generated by the CreateFSIInstCmd.
**------------------------------------------------------------------------------
** Arguments:
**    psMsg                    - Message received from the ABCC.
**
** Returns:
**    ABCC_CmdSeqCmdStatusType - Response for command callback.
**------------------------------------------------------------------------------
*/
static ABCC_CmdSeqRespStatusType HandleFSIInstRsp( ABP_MsgType* psMsg )
{
   if( ABCC_VerifyMessage( psMsg ) == ABCC_EC_NO_ERROR )
   {
      /*
      ** Successfully created a file system instance
      ** Store the created instance
      */
      ABCC_GetMsgData16( psMsg, &anb_fsi_iInstance, 0 );

      /*
      ** Initialize the write offset
      */
      anb_fsi_iFsiWriteOffset = 0;

      return( ABCC_EXEC_NEXT_COMMAND );
   }

   /*
   ** Something went wrong
   */
   return( ABCC_RESP_ABORT_SEQ );
}

/*------------------------------------------------------------------------------
** Deletes the current instance of FSI on ABCC.
**------------------------------------------------------------------------------
** Arguments:
**    psMsg                    - Message to send to ABCC
**
** Returns:
**    ABCC_CmdSeqCmdStatusType - Response for command callback
**------------------------------------------------------------------------------
*/
static ABCC_CmdSeqCmdStatusType DeleteCmd( ABP_MsgType* psMsg )
{
   ABCC_SetMsgHeader( psMsg,
                      ABP_OBJ_NUM_FSI,
                      0,
                      0,
                      (ABP_MsgCmdType)( ABP_MSG_HEADER_C_BIT | ABP_CMD_DELETE ),
                      0,
                      ABCC_GetNewSourceId() );

   ABCC_SetMsgCmdExt( psMsg, anb_fsi_iInstance );

   return( ABCC_SEND_COMMAND );
}

/*------------------------------------------------------------------------------
** Close the file currently open on the ABCC.
**------------------------------------------------------------------------------
** Arguments:
**    psMsg                    - Message to send to ABCC.
**
** Returns:
**    ABCC_CmdSeqCmdStatusType - Response for command callback.
**------------------------------------------------------------------------------
*/
static ABCC_CmdSeqCmdStatusType FileCloseCmd( ABP_MsgType* psMsg )
{
   /*
   ** Reset the file offset, in case of a repeated open-file on the same
   ** instance is performed
   */
   anb_fsi_iFsiWriteOffset = 0;

   ABCC_SetMsgHeader( psMsg,
                      ABP_OBJ_NUM_FSI,
                      anb_fsi_iInstance,
                      0,
                      (ABP_MsgCmdType)( ABP_MSG_HEADER_C_BIT | ABP_FSI_CMD_FILE_CLOSE ),
                      0,
                      ABCC_GetNewSourceId() );

   return( ABCC_SEND_COMMAND );
}

/*------------------------------------------------------------------------------
** Opens a file on the ABCC
**------------------------------------------------------------------------------
** Arguments:
**    psMsg                    - Message to send to ABCC.
**
** Returns:
**    ABCC_CmdSeqCmdStatusType - Response for command callback.
**------------------------------------------------------------------------------
*/
static ABCC_CmdSeqCmdStatusType FileOpenCmd( ABP_MsgType* psMsg )
{
   ABCC_SetMsgHeader( psMsg,
                      ABP_OBJ_NUM_FSI,
                      anb_fsi_iInstance,
                      0,
                      (ABP_MsgCmdType)( ABP_MSG_HEADER_C_BIT | ABP_FSI_CMD_FILE_OPEN ),
                      (UINT16)anb_fsi_bFilenameLength,
                      ABCC_GetNewSourceId() );

   ABCC_SetMsgCmdExt( psMsg, anb_fsi_bAccessMode );
   ABCC_SetMsgString( psMsg, anb_fsi_acFilename, (UINT16)anb_fsi_bFilenameLength, 0 );

   return( ABCC_SEND_COMMAND );
}

/*------------------------------------------------------------------------------
** Writes a block of data to the open FSI file.
**------------------------------------------------------------------------------
** Arguments:
**    psMsg                    - Message to send to ABCC.
**
** Returns:
**    ABCC_CmdSeqCmdStatusType - Response for command callback.
**------------------------------------------------------------------------------
*/
static ABCC_CmdSeqCmdStatusType FileWriteCmd( ABP_MsgType* psMsg )
{
   UINT16 iMaxChunkSize;
   UINT16 iDataSize;

   /*
   ** Lock the FSI instance so that the data is written sequentially
   */
   SetInstanceLocked( TRUE );

   iMaxChunkSize = sizeof( anb_fsi_abFsiWriteData );
   iDataSize = anb_fsi_iFsiWriteSize;

   if( iDataSize > iMaxChunkSize )
   {
      iDataSize = iMaxChunkSize;
   }

   ABCC_SetMsgHeader( psMsg,
                      ABP_OBJ_NUM_FSI,
                      anb_fsi_iInstance,
                      0,
                      (ABP_MsgCmdType)( ABP_MSG_HEADER_C_BIT | ABP_FSI_CMD_FILE_WRITE ),
                      iDataSize,
                      ABCC_GetNewSourceId() );

   ABCC_PORT_CopyOctets( ABCC_GetMsgDataPtr( psMsg ),
                         0,
                         anb_fsi_abFsiWriteData,
                         0,
                         iDataSize );

   return( ABCC_SEND_COMMAND );
}

/*------------------------------------------------------------------------------
** Processes the ABCC's response to the FileWriteCmd.
**------------------------------------------------------------------------------
** Arguments:
**    psMsg                    - Message received from the ABCC.
**
** Returns:
**    ABCC_CmdSeqCmdStatusType - Response for command callback.
**------------------------------------------------------------------------------
*/
static ABCC_CmdSeqRespStatusType FileWriteRsp( ABP_MsgType* psMsg )
{
   /*
   ** Update the write offset based on the number of bytes that the
   ** ABCC successfully wrote to the file.
   */
   anb_fsi_iFsiWriteOffset += ABCC_GetMsgCmdExt0( psMsg );

   /*
   ** Check if the ABCC wrote the full chunk or not and update
   ** the state variables appropriately.
   */
   if( anb_fsi_iFsiWriteOffset < anb_fsi_iFsiWriteSize )
   {
      return( ABCC_EXEC_CURR_COMMAND );
   }
   else if( ABCC_VerifyMessage( psMsg ) == ABCC_EC_NO_ERROR )
   {
      SetInstanceLocked( FALSE );
      anb_fsi_iFsiWriteOffset = 0;

      return( ABCC_EXEC_NEXT_COMMAND );
   }

   /*
   ** Something went wrong
   */
   return( ABCC_RESP_ABORT_SEQ );
}

/*------------------------------------------------------------------------------
** FSI Generic Response Handler.
**------------------------------------------------------------------------------
** Arguments:
**    psMsg                    - Message received from the ABCC
**
** Returns:
**    ABCC_CmdSeqCmdStatusType - Response for command callback
**------------------------------------------------------------------------------
*/
static ABCC_CmdSeqRespStatusType ResponseHandler( ABP_MsgType* psMsg )
{
   if( ABCC_VerifyMessage( psMsg ) == ABCC_EC_NO_ERROR )
   {
      return( ABCC_EXEC_NEXT_COMMAND );
   }

   /*
   ** Something went wrong
   */
   return( ABCC_RESP_ABORT_SEQ );
}

/*------------------------------------------------------------------------------
** Corrupt a file on the ABCC by writing dummy data to the
** .hiff file instance.
**------------------------------------------------------------------------------
** Arguments:
**    psMsg                    - Message to send to ABCC
**
** Returns:
**    ABCC_CmdSeqCmdStatusType - Response for command callback
**------------------------------------------------------------------------------
*/
static ABCC_CmdSeqCmdStatusType InvalidateFirmwareFileCmd( ABP_MsgType* psMsg )
{
   const UINT8 abDummyData[] = {0x48, 0x49, 0x46, 0x46}; /* "HIFF" */

   anb_fsi_iFsiWriteSize = sizeof( abDummyData );

   ABCC_PORT_CopyOctets( anb_fsi_abFsiWriteData,
                         0,
                         abDummyData,
                         0,
                         anb_fsi_iFsiWriteSize );

   return( FileWriteCmd( psMsg ) );
}

/*******************************************************************************
** Public Services
********************************************************************************
*/
void ANB_FSI_FileOpen( const char* pcFilename, UINT8 bFilenameLength, UINT8 bAccessMode )
{
   ABCC_PORT_MemCpy( anb_fsi_acFilename, pcFilename, bFilenameLength );
   anb_fsi_bAccessMode = bAccessMode;
   anb_fsi_bFilenameLength = bFilenameLength;

   ABCC_AddCmdSeq( anb_fsi_asFileOpenSeq, NULL );
}

void ANB_FSI_FileWrite( const UINT8* pbData, UINT16 iDataToWriteLength )
{
   ABCC_PORT_MemCpy( anb_fsi_abFsiWriteData, pbData, iDataToWriteLength );
   anb_fsi_iFsiWriteSize = iDataToWriteLength;

   ABCC_AddCmdSeq( anb_fsi_asFileWriteSeq, NULL );
}

void ANB_FSI_Close( void )
{
   ABCC_AddCmdSeq( anb_fsi_asCloseSeq, NULL );
}

void ANB_FSI_InvalidateUploadedAbccFw( void )
{
   ABCC_AddCmdSeq( anb_fsi_asInvalidateAbccFwSeq, NULL );
}

BOOL ANB_FSI_GetInstanceLocked( void )
{
   return( anb_fsi_fInstanceAccessLocked );
}

#endif /* ANB_FSI_OBJ_ENABLE */
