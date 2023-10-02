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
** This is an example implementation of an application handler to control the
** ABCC module. It has support to initialize, run and reset/shutdown the ABCC.
** The state machine shall be executed cyclically and it returns its current
** status after every execution.
********************************************************************************
********************************************************************************
** Services:
**    APPL_HandleAbcc()              - Runs the state machine controlling the
**                                     ABCC module
**    APPL_RestartAbcc()             - Forces the state machine to restart the
**                                     ABCC module
**    APPL_Shutdown()                - Forces the state machine to shut down the
**                                     ABCC module
**    APPL_Reset()                   - Forces the state machine to reset the
**                                     ABCC module
**    APPL_UnexpectedError()         - Application has encountered an unexpected
**                                     error
**    APPL_SetAddress()              - Set ABCC module address
**    APPL_SetBaudrate()             - Set ABCC module baud rate
**    APPL_SetCommSettings()         - Set ABCC module comm settings
********************************************************************************
********************************************************************************
*/

#ifndef APPL_ABCC_HANDLER_H
#define APPL_ABCC_HANDLER_H
#include "abcc_td.h"
#include "string.h"

/*******************************************************************************
** Constants
********************************************************************************
*/

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Status reported by the ABCC handler controlling the ABCC module
**------------------------------------------------------------------------------
*/
typedef enum APPL_AbccHandlerStatus
{
   APPL_MODULE_NO_ERROR,         /* Module OK */
   APPL_MODULE_NOT_DETECTED,     /* No module plugged */
   APPL_MODULE_NOT_SUPPORTED,    /* Unsupported module detected */
   APPL_MODULE_NOT_ANSWERING,    /* Possible reasons: Wrong API selected, defect module */
   APPL_MODULE_RESET,            /* Reset requested from ABCC */
   APPL_MODULE_SHUTDOWN,         /* Shutdown requested */
   APPL_MODULE_UNEXPECTED_ERROR  /* Unexpected error occurred */
}
APPL_AbccHandlerStatusType;


/*------------------------------------------------------------------------------
**  Error codes for example application:
**
**  APPL_NO_ERROR                  - No error
**  APPL_AD_PD_REDA_SIZE_ERR       - Total process data read size too large
**                                   Check ABCC_CFG_MAX_PROCESS_DATA_SIZE
**  APPL_AD_PD_WRITE_SIZE_ERR      - Total process data write too large
**                                   Check ABCC_CFG_MAX_PROCESS_DATA_SIZE
**  APPL_AD_TOO_MANY_READ_MAPPINGS - Read process data map has too many entries
**                                   Check AD_MAX_OF_READ_WRITE_TO_MAP.
**  APPL_AD_TOO_MANY_WRITE_MAPPINGS- Write process data map has too many entries
**                                   Check AD_MAX_OF_WRITE_WRITE_TO_MAP
**  APPL_AD_UNKNOWN_ADI            - Requested ADI could not be found
**------------------------------------------------------------------------------
*/
typedef enum APPL_ErrCode
{
   APPL_NO_ERROR = 0,
   APPL_AD_PD_READ_SIZE_ERR,
   APPL_AD_PD_WRITE_SIZE_ERR,
   APPL_AD_TOO_MANY_READ_MAPPINGS,
   APPL_AD_TOO_MANY_WRITE_MAPPINGS,
   APPL_AD_UNKNOWN_ADI
}
APPL_ErrCodeType;

/*------------------------------------------------------------------------------
**  Comm settings:
**
**  APPL_COMM_SETTING_AUTO         - Auto negotiation
**  APPL_COMM_SETTING_10HDX        - 10 Mbit/s half duplex
**  APPL_COMM_SETTING_10FDX        - 10 Mbit/s full duplex
**  APPL_COMM_SETTING_100HDX       - 100 Mbit/s half duplex
**  APPL_COMM_SETTING_100FDX       - 100 Mbit/s full duplex
**  APPL_COMM_SETTING_AUTO_100FDX  - Auto negotiation with only 100 Mbit/s full
*                                    duplex in advertisment
**  APPL_COMM_SETTING_AUTO_1000FDX - Auto negotiation with only 1 Gbit/s full
*                                    duplex in advertisment
**------------------------------------------------------------------------------
*/
typedef enum APPL_CommSetting
{
   APPL_COMM_SETTING_AUTO = 0,
   APPL_COMM_SETTING_10HDX,
   APPL_COMM_SETTING_10FDX,
   APPL_COMM_SETTING_100HDX,
   APPL_COMM_SETTING_100FDX,
   APPL_COMM_SETTING_AUTO_100FDX,
   APPL_COMM_SETTING_AUTO_1000FDX,
}
APPL_CommSettingType;

/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** This function shall be called on cyclic bases from the main loop to handle
** the ABCC. It includes a state machine for handling reset, run, and shutdown
** of the driver.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    State of the ABCC handler
**------------------------------------------------------------------------------
*/
EXTFUNC APPL_AbccHandlerStatusType APPL_HandleAbcc( void );

/*------------------------------------------------------------------------------
** This function will force the ABCC handler to restart the ABCC module
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void APPL_RestartAbcc( void );

/*------------------------------------------------------------------------------
** This function will force the ABCC handler to shutdown the ABCC module
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void APPL_Shutdown( void );

/*------------------------------------------------------------------------------
** This function will force the ABCC handler to reset the ABCC
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void APPL_Reset( void );

/*------------------------------------------------------------------------------
** Application has encountered an unexpected error.
** The abcc main state machine will halt and indicate error.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void APPL_UnexpectedError( void );

/*------------------------------------------------------------------------------
** Set HW switch 1 value to the application. The value is used to generate the
** the node address or the IP address (192.168.0.X) depending on network.
**
** NOTE: For networks that uses an IP address and if this function is called
**       with address 0 no address will be set to the ABCC.
**------------------------------------------------------------------------------
** Arguments:
**    bSwitchValue/iSwitchValue - Switch 1 value either as an 8 bit or 16 bit
**                                value depending on configuration.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#if APPL_USE_16BIT_NODE_ADDR
EXTFUNC void APPL_SetAddress( UINT16 iSwitchValue );
#else
EXTFUNC void APPL_SetAddress( UINT8 bSwitchValue );
#endif

/*------------------------------------------------------------------------------
** Set HW switch 2 value to the application. The value is used to generate the
** baud rate for networks that implements network configuration object instance
** 2.
**------------------------------------------------------------------------------
** Arguments:
**    bSwitchValue - Switch 2 value
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void APPL_SetBaudrate( UINT8 bSwitchValue );

/*------------------------------------------------------------------------------
** Set Comm settings (speed/duplex) for ports.
**------------------------------------------------------------------------------
** Arguments:
**
**    eCommSettings1 - Settings for port 1
**    eCommSettings2 - Settings for port 2
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void APPL_SetCommSettings( APPL_CommSettingType eCommSettings1,
                                   APPL_CommSettingType eCommSettings2 );

#endif  /* inclusion lock */
