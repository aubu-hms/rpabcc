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
** Interface for error handling and debugging.
********************************************************************************
********************************************************************************
** Services :
** ABCC_DebugPrintMsg()             - Debug print of ABCC message
** ABCC_DebugPrintMsgEvent()        - Debug print of ABCC message event
********************************************************************************
********************************************************************************
*/

#ifndef ABCC_DEBUG_ERR_H_
#define ABCC_DEBUG_ERR_H_

#include "abcc_drv_cfg.h"
#include "abcc_td.h"
#include "abp.h"
#include "abcc_sys_adapt.h"
#include "abcc.h"
#include "abcc_port.h"
#include "abcc_handler.h"

/*******************************************************************************
** Public Constants
********************************************************************************
*/

/*
** DEBUG Level macros.
*/
#if( ABCC_CFG_DEBUG_EVENT_ENABLED )
#define DEBUG_EVENT( args ) ABCC_PORT_DebugPrint( args )
#else
#define DEBUG_EVENT( args )
#endif

/*******************************************************************************
** Public Typedefs
********************************************************************************
*/

/*******************************************************************************
** Public Globals
********************************************************************************
*/

#if( ABCC_CFG_DRV_SPI || ABCC_CFG_DRV_SERIAL )
#if( ABCC_CFG_DEBUG_CRC_ERROR_CNT )
/*------------------------------------------------------------------------------
** CRC error counter for the SPI and UART application interfaces. It will be
** incremented every time a CRC mismatch is seen, it will roll over to zero
** when it reaches 0xFFFF, and is not cleared in case of a HW reset of the ABCC.
** The application can clear it at runtime by writing zero to it.
**------------------------------------------------------------------------------
*/
EXTVAR UINT16 DEBUG_iCrcErrorCnt;
#endif
#endif

/*******************************************************************************
** Public Services
********************************************************************************
*/

#if ABCC_CFG_DEBUG_MESSAGING
/*------------------------------------------------------------------------------
** Prints ABCC message content using ABCC_PORT_DebugPrint().
** Prints: Message buffer address, message header and message data
**------------------------------------------------------------------------------
** Arguments:
**    pcInfo - General information about the debug print.
**    psMsg  - ABCC message
** Returns:
**    None
**------------------------------------------------------------------------------
*/
void ABCC_DebugPrintMsg( char* pcInfo, ABP_MsgType* psMsg );

/*------------------------------------------------------------------------------
** Prints buffer address and source id for an ABCC message.
**------------------------------------------------------------------------------
** Arguments:
**    pcInfo - General information about the debug print.
**    psMsg  - ABCC message
** Returns:
**    None
**------------------------------------------------------------------------------
*/
void ABCC_DebugPrintMsgEvent( char* pcInfo, ABP_MsgType* psMsg );

#define ABCC_DEBUG_MSG_DATA( pcInfo, psMsg )  ABCC_DebugPrintMsg( ( pcInfo ), ( psMsg ) )
#define ABCC_DEBUG_MSG_EVENT( pcInfo, psMsg ) ABCC_DebugPrintMsgEvent( ( pcInfo ), ( psMsg ) )
#define ABCC_DEBUG_MSG_GENERAL( pcInfo )      ABCC_PORT_DebugPrint( pcInfo )
#else
#define ABCC_DEBUG_MSG_DATA( pcInfo, psMsg )
#define ABCC_DEBUG_MSG_EVENT( pcInfo, psMsg )
#define ABCC_DEBUG_MSG_GENERAL( pcInfo )
#endif

/*------------------------------------------------------------------------------
** Prints hexdumps of message, SPI, and UART data.
**------------------------------------------------------------------------------
** Arguments:
**    pcInfo - Tag to prepend to the output.
**
**    psMsg - Pointer to ABCC message buffer.
**
**    piData       - Pointer to SPI frame data.
**    iSizeInWords - SPI frame size in words.
**
**    pbData       - Pointer to UART telegram data.
**    iSizeInBytes - UART telegram size in bytes.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#if ABCC_CFG_DEBUG_HEXDUMP_MSG
EXTFUNC void ABCC_DebugHexdumpMsg( char* pcInfo, ABP_MsgType* psMsg );
#define ABCC_DEBUG_HEXDUMP_MSG( pcInfo, psMsg ) ABCC_DebugHexdumpMsg( pcInfo, psMsg )
#else
#define ABCC_DEBUG_HEXDUMP_MSG( pcInfo, psMsg )
#endif

#if ABCC_CFG_DEBUG_HEXDUMP_SPI
EXTFUNC void ABCC_DebugHexdumpSpi( char* pcInfo, UINT16* piData, UINT16 iSizeInWords );
#define ABCC_DEBUG_HEXDUMP_SPI( pcInfo, piData, iSizeInWords ) ABCC_DebugHexdumpSpi( pcInfo, piData, iSizeInWords )
#else
#define ABCC_DEBUG_HEXDUMP_SPI( pcInfo, piData, iSizeInWords )
#endif

#if ABCC_CFG_DEBUG_HEXDUMP_UART
EXTFUNC void ABCC_DebugHexdumpUart( char* pcInfo, UINT8* pbData, UINT16 iSizeInBytes );
#define ABCC_DEBUG_HEXDUMP_UART( pcInfo, pbData, iSizeInBytes ) ABCC_DebugHexdumpUart( pcInfo, pbData, iSizeInBytes )
#else
#define ABCC_DEBUG_HEXDUMP_UART( pcInfo, pbData, iSizeInBytes )
#endif

#endif
