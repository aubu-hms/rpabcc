/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Driver version 5.08.01 (2021-12-09)                                   **
**                                                                            **
** Delivered with:                                                            **
**    ABP            8.03.01 (2022-12-12)                                     **
**                                                                            */
/*******************************************************************************
********************************************************************************
** COPYRIGHT NOTIFICATION (c) 2013 HMS Industrial Networks AB                 **
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
** ABCC driver error and debug functions.
********************************************************************************
********************************************************************************
*/
#include "abcc_drv_cfg.h"
#include "abcc.h"
#include "abcc_sw_port.h"
#include "abcc_td.h"
#include "abp.h"
#include "abcc_debug_err.h"

/*******************************************************************************
** Constants
********************************************************************************
*/

/*
** Max number of errors to log
*/
#ifndef ABCC_DRV_CFG_MAX_NUM_ERR_LOG
#define ABCC_DRV_CFG_MAX_NUM_ERR_LOG 1
#endif

/*------------------------------------------------------------------------------
** Help macros for debugging and error reporting.
**------------------------------------------------------------------------------
*/

/*
** Report error to application depending on ABCC_CFG_ERR_REPORTING_ENABLED
*/
#if( ABCC_CFG_ERR_REPORTING_ENABLED )
#define ABCC_REPORT_ERROR( eSeverity, eErrorCode, lAddInfo ) ABCC_CbfDriverError( eSeverity, eErrorCode, lAddInfo )
#else
#define ABCC_REPORT_ERROR( eSeverity, eErrorCode, lAddInfo )
#endif

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*******************************************************************************
** Public Globals
********************************************************************************
*/

#if( ABCC_CFG_DRV_SPI || ABCC_CFG_DRV_SERIAL )
#if( ABCC_CFG_DEBUG_CRC_ERROR_CNT )
UINT16   DEBUG_iCrcErrorCnt = 0;
#endif
#endif

/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*
** Array to hold the logged error information.
*/
static UINT8 abcc_bErrCount = 0;
static ABCC_ErrInfoType abcc_asErrInfo[ ABCC_DRV_CFG_MAX_NUM_ERR_LOG ];

#ifdef ABCC_SYS_16_BIT_CHAR
#define ABCC_GetMsgCmdField( psMsg )   ( ABCC_GetLowAddrOct( (psMsg)->sHeader.iCmdReserved ) )
#else
#define ABCC_GetMsgCmdField( psMsg )   ( (psMsg)->sHeader.bCmd )
#endif

/*******************************************************************************
** Private Services
********************************************************************************
*/
/*------------------------------------------------------------------------------
** Log error to memory.
** More human readable information will be available if
** ABCC_CFG_DEBUG_ERR_ENABLED is set to TRUE (see ABCC_ErrInfoType) but this
** will have significant impact on the code size.
**------------------------------------------------------------------------------
** Arguments:
**    eSeverity         - Severity (see ABCC_SeverityType)
**    eErrorCode        - Error code (see ABCC_ErrorCodeType)
**    lAddInfo          - Additional info relevant for error
**    pacSeverity       - Pointer to severity in text format
**    pacErrorCode      - Pointer to error code in text format
**    pacAddInfo        - Pointer to additional info in text format
**    pacLoaction       - Pointer to file and line info text format
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#if ABCC_CFG_DEBUG_ERR_ENABLED
static void LogError( ABCC_SeverityType eSeverity,
                      ABCC_ErrorCodeType eErrorCode,
                      UINT32 lAddInfo,
                      char* pacSeverity,
                      char* pacErrorCode,
                      char* pacAddInfo,
                      char* pacFile,
                      INT32 lLine )
#else
static void LogError( ABCC_SeverityType eSeverity,
                      ABCC_ErrorCodeType eErrorCode,
                      UINT32 lAddInfo )
#endif
{
   abcc_asErrInfo[ abcc_bErrCount ].eErrorCode = eErrorCode;
   abcc_asErrInfo[ abcc_bErrCount ].eSeverity  = eSeverity;
   abcc_asErrInfo[ abcc_bErrCount ].lAddInfo   = lAddInfo;
#if ABCC_CFG_DEBUG_ERR_ENABLED
   abcc_asErrInfo[ abcc_bErrCount ].pacSeverity = pacSeverity;
   abcc_asErrInfo[ abcc_bErrCount ].pacErrorCode = pacErrorCode;
   abcc_asErrInfo[ abcc_bErrCount ].pacAddInfo = pacAddInfo;
   abcc_asErrInfo[ abcc_bErrCount ].pacFile = pacFile;
   abcc_asErrInfo[ abcc_bErrCount ].lLine = lLine;
#endif

   if( abcc_bErrCount != ( ABCC_DRV_CFG_MAX_NUM_ERR_LOG - 1 ) )
   {
      abcc_bErrCount++;
   }
}

/*******************************************************************************
** Public Services
********************************************************************************
*/
#if ABCC_CFG_DEBUG_MESSAGING
void ABCC_DebugPrintMsg( char* pcInfo, ABP_MsgType* psMsg )
{
   UINT16 i;
   UINT16 iDataSize;
   UINT8 bData;

   iDataSize = ABCC_GetMsgDataSize( psMsg );

   ABCC_PORT_DebugPrint( ( "\n%s:\n", pcInfo ) );
   ABCC_PORT_DebugPrint( ( "[ MsgBuf:0x%p Size:0x%04x SrcId  :0x%02x DestObj:0x%02x\n  Inst  :0x%04x     Cmd :0x%02x   CmdExt0:0x%02x CmdExt1:0x%02x ]\n",
                          psMsg,
                          ABCC_GetMsgDataSize( psMsg ),
                          ABCC_GetMsgSourceId( psMsg ),
                          ABCC_GetMsgDestObj( psMsg ),
                          ABCC_GetMsgInstance( psMsg ),
                          ABCC_GetMsgCmdField( psMsg ),
                          ABCC_GetMsgCmdExt0( psMsg ),
                          ABCC_GetMsgCmdExt1( psMsg ) ) );

   ABCC_PORT_DebugPrint( ("[ ") );
   for( i = 0; i < iDataSize; i++ )
   {
      if( ( i % 16 ) == 15 )
      {
         ABCC_PORT_DebugPrint( ("\n  ") );
      }

      ABCC_GetMsgData8( psMsg, &bData, i );
      ABCC_PORT_DebugPrint( ("0x%02x ", bData ) );
   }

   ABCC_PORT_DebugPrint( ( "]\n\n") );
}

void ABCC_DebugPrintMsgEvent( char* pcInfo, ABP_MsgType* psMsg )
{
   ABCC_PORT_DebugPrint( ( "%s: MsgBuf:0x%p SrcId:0x%02x\n",
                          pcInfo, psMsg,
                          ABCC_GetMsgSourceId( psMsg ) ) );
}
#endif


UINT8 ABCC_GetDriverError( ABCC_ErrInfoType** ppacErrInfo )
{
   if( ppacErrInfo != NULL )
   {
      *ppacErrInfo = abcc_asErrInfo;
   }
   return( abcc_bErrCount );
}

#if ABCC_CFG_DEBUG_ERR_ENABLED
void ABCC_ErrorHandler( ABCC_SeverityType eSeverity,
                        ABCC_ErrorCodeType eErrorCode,
                        UINT32 lAddInfo,
                        char* pacSeverity,
                        char* pacErrorCode,
                        char* pacAddInfo,
                        char* pacFile,
                        INT32 lLine )
#else
void ABCC_ErrorHandler( ABCC_SeverityType eSeverity,
                        ABCC_ErrorCodeType eErrorCode,
                        UINT32 lAddInfo )
#endif
{
#if ABCC_CFG_DEBUG_ERR_ENABLED
   LogError( eSeverity, eErrorCode, lAddInfo,
             pacSeverity, pacErrorCode, pacAddInfo, pacFile, lLine );

   ABCC_PORT_DebugPrint( ( "Severity  : %s\n"
                           "Error code: %s (ErrNo: %d)\n"
                           "Add info  : %s (0x%x)\n"
                           "File      : %s (Line:%d)\n",
                           pacSeverity, pacErrorCode, eErrorCode,
                           pacAddInfo, lAddInfo, pacFile, lLine ) );
#else
   LogError( eSeverity, eErrorCode, lAddInfo );
#endif
   ABCC_REPORT_ERROR( eSeverity, eErrorCode, lAddInfo );

   if( eSeverity != ABCC_SEV_INFORMATION )
   {
     ABCC_SetMainStateError();
   }

   if( eSeverity == ABCC_SEV_FATAL )
   {
      volatile UINT8 bErrCount;
      volatile ABCC_ErrInfoType* pasErrInfo;
      bErrCount = ABCC_GetDriverError( (ABCC_ErrInfoType**)&pasErrInfo );
      ABCC_PORT_DebugPrint(( "FATAL: Enter endless loop\n%s", FileLine ));
      (void)bErrCount;
      while( 1 ){}
   }
}


#if( ABCC_CFG_DEBUG_HEXDUMP_MSG )
void ABCC_DebugHexdumpMsg( char* pcInfo, ABP_MsgType* psMsg )
{
   UINT16   iIndex;
   UINT16   iSizeInBytes;

   ABCC_PORT_DebugPrint(( "%s", pcInfo ));

   iSizeInBytes = ABCC_GetMsgDataSize( psMsg );

#ifdef ABCC_SYS_16_BIT_CHAR
   iSizeInBytes += sizeof( ABP_MsgHeaderType16 );

   iIndex = 0;
   while( iSizeInBytes > 0 )
   {
      UINT16   iData;

      iData = iLeTOi( ((UINT16*)psMsg)[ iIndex ] );

      ABCC_PORT_DebugPrint(( "%02x", (UINT8)(iData & 0xFF) ));
      iSizeInBytes--;
      if( iSizeInBytes > 0 )
      {
         ABCC_PORT_DebugPrint(( "%02x", (UINT8)(( iData >> 8 ) & 0xFF) ));
         iSizeInBytes--;
      }

      iIndex++;
   }
#else
   iSizeInBytes += sizeof( ABP_MsgHeaderType );

   for( iIndex = 0; iIndex < iSizeInBytes; iIndex++ )
   {
      ABCC_PORT_DebugPrint(( "%02x", ((UINT8*)psMsg)[ iIndex ] ));
   }
#endif

   ABCC_PORT_DebugPrint(( "\n" ));
}
#endif


#if ABCC_CFG_DEBUG_HEXDUMP_SPI
void ABCC_DebugHexdumpSpi( char* pcInfo, UINT16* piData, UINT16 iSizeInWords )
{
   ABCC_PORT_DebugPrint(( "%s", pcInfo ));

   while( iSizeInWords )
   {
      ABCC_PORT_DebugPrint(( "%04x", iTOiBe( *piData ) ));
      piData++;
      iSizeInWords--;
   }

   ABCC_PORT_DebugPrint(( "\n" ));
}
#endif


#if ABCC_CFG_DEBUG_HEXDUMP_UART
void ABCC_DebugHexdumpUart( char* pcInfo, UINT8* pbData, UINT16 iSizeInBytes )
{
   ABCC_PORT_DebugPrint(( "%s", pcInfo ));

   while( iSizeInBytes )
   {
      ABCC_PORT_DebugPrint(( "%02x", *pbData ));
      pbData++;
      iSizeInBytes--;
   }

   ABCC_PORT_DebugPrint(( "\n" ));
}
#endif
