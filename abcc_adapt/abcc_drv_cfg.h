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
** User configuration of the ABCC driver. The configuration parameters are
** documented in the driver's public interface abcc_cfg.h.
**
** All configurations in this file should be possible to override if defined
** in abcc_platform_cfg.h. This allows the target platform to adapt the
** product configuration to realize its implementation for specific needs.
********************************************************************************
********************************************************************************
*/

#ifndef ABCC_DRV_CFG_H_
#define ABCC_DRV_CFG_H_

#include "abcc_platform_cfg.h"
#include "abcc_td.h"
#include "abp.h"

/*******************************************************************************
** Constants
********************************************************************************
*/

/*------------------------------------------------------------------------------
** All interface drivers are supported.
**
** Check the descriptions in "./abcc_drv/inc/abcc_cfg.h" for more information
** about the purpose of each separate 'define'.
**------------------------------------------------------------------------------
*/
#ifndef ABCC_CFG_DRV_PARALLEL
   #define ABCC_CFG_DRV_PARALLEL                      ( FALSE )
#endif
#ifndef ABCC_CFG_DRV_SPI
   #define ABCC_CFG_DRV_SPI                           ( TRUE )
#endif
#ifndef ABCC_CFG_DRV_SERIAL
   #define ABCC_CFG_DRV_SERIAL                        ( TRUE )
#endif
#ifndef ABCC_CFG_DRV_PARALLEL_30
   #define ABCC_CFG_DRV_PARALLEL_30                   ( FALSE )
#endif

/*------------------------------------------------------------------------------
** The software can both get the operating mode from external resource and set
** the selected operating mode to the ABCC host connector.
**
** Check the descriptions in "./abcc_drv/inc/abcc_cfg.h" for more information
** about the purpose of each separate 'define'.
**------------------------------------------------------------------------------
*/
#ifndef ABCC_CFG_OP_MODE_GETTABLE
   #define ABCC_CFG_OP_MODE_GETTABLE                  ( FALSE )
#endif
#ifndef ABCC_CFG_OP_MODE_SETTABLE
   #define ABCC_CFG_OP_MODE_SETTABLE                  ( TRUE )
#endif
#if( !ABCC_CFG_OP_MODE_GETTABLE )
   #ifndef ABCC_CFG_ABCC_OP_MODE_30
      #define ABCC_CFG_ABCC_OP_MODE_30    ABP_OP_MODE_SERIAL_115_2
   #endif
   #ifndef ABCC_CFG_ABCC_OP_MODE_40
      #define ABCC_CFG_ABCC_OP_MODE_40    ABP_OP_MODE_SPI
   #endif
#endif

/*------------------------------------------------------------------------------
** SPI frame configuration
**
** Check the descriptions in "./abcc_drv/inc/abcc_cfg.h" for more information
** about the purpose of each separate 'define'.
**------------------------------------------------------------------------------
*/
#if( ABCC_CFG_DRV_SPI )
   #ifndef ABCC_CFG_SPI_MSG_FRAG_LEN
      #define ABCC_CFG_SPI_MSG_FRAG_LEN                  ( 16 )
   #endif
#endif

/*------------------------------------------------------------------------------
** ABCC memory access configuration for parallel operating modes
**
** Check the descriptions in "./abcc_drv/inc/abcc_cfg.h" for more information
** about the purpose of each separate 'define'.
**------------------------------------------------------------------------------
*/
#ifndef ABCC_CFG_MEMORY_MAPPED_ACCESS
   #define ABCC_CFG_MEMORY_MAPPED_ACCESS                 ( FALSE )
#endif
#if( ABCC_CFG_MEMORY_MAPPED_ACCESS )
   #ifndef ABCC_CFG_PARALLEL_BASE_ADR
      #define ABCC_CFG_PARALLEL_BASE_ADR                 ( 0x00000000 )
   #endif
#endif

/*------------------------------------------------------------------------------
** Telegram-cyle timeout times in milliseconds for the asynchronous serial
** application interface.
**
** Check the descriptions in "./abcc_drv/inc/abcc_cfg.h" for more information
** about the purpose of each separate 'define'.
**------------------------------------------------------------------------------
*/
#if( ABCC_CFG_DRV_SERIAL )
   #ifndef ABCC_CFG_SERIAL_TMO_19_2
      #define ABCC_CFG_SERIAL_TMO_19_2                   ( 350 )
   #endif
   #ifndef ABCC_CFG_SERIAL_TMO_57_6
      #define ABCC_CFG_SERIAL_TMO_57_6                   ( 120 )
   #endif
   #ifndef ABCC_CFG_SERIAL_TMO_115_2
      #define ABCC_CFG_SERIAL_TMO_115_2                  ( 60 )
   #endif
   #ifndef ABCC_CFG_SERIAL_TMO_625
      #define ABCC_CFG_SERIAL_TMO_625                    ( 20 )
   #endif
#endif

/*------------------------------------------------------------------------------
** If Module ID pins are connected no soft configuration is required.
**
** Check the descriptions in "./abcc_drv/inc/abcc_cfg.h" for more information
** about the purpose of each separate 'define'.
**------------------------------------------------------------------------------
*/
#ifndef ABCC_CFG_MODULE_ID_PINS_CONN
   #define ABCC_CFG_MODULE_ID_PINS_CONN               ( TRUE )
#endif

#if( !ABCC_CFG_MODULE_ID_PINS_CONN )
   #ifndef ABCC_CFG_ABCC_MODULE_ID
      #define ABCC_CFG_ABCC_MODULE_ID       ABP_MODULE_ID_ACTIVE_ABCC40
   #endif
#endif

/*------------------------------------------------------------------------------
** Module detection configuration
**
** Check the descriptions in "./abcc_drv/inc/abcc_cfg.h" for more information
** about the purpose of each separate 'define'.
**------------------------------------------------------------------------------
*/
#ifndef ABCC_CFG_MOD_DETECT_PINS_CONN
   #define ABCC_CFG_MOD_DETECT_PINS_CONN              ( TRUE )
#endif

/*------------------------------------------------------------------------------
** Configuration of message handling
**
** Check the descriptions in "./abcc_drv/inc/abcc_cfg.h" for more information
** about the purpose of each separate 'define'.
**------------------------------------------------------------------------------
*/
#ifndef ABCC_CFG_MAX_NUM_APPL_CMDS
   #define ABCC_CFG_MAX_NUM_APPL_CMDS                 ( 4 )
#endif
#ifndef ABCC_CFG_MAX_NUM_ABCC_CMDS
   #define ABCC_CFG_MAX_NUM_ABCC_CMDS                 ( 4 )
#endif
#ifndef ABCC_CFG_MAX_MSG_SIZE
   #define ABCC_CFG_MAX_MSG_SIZE                      ( ABP_MAX_MSG_DATA_BYTES )
#endif
#ifndef ABCC_CFG_MAX_PROCESS_DATA_SIZE
   #define ABCC_CFG_MAX_PROCESS_DATA_SIZE             ( 4096 )
#endif

/*------------------------------------------------------------------------------
** Sync configuration
**
** Check the descriptions in "./abcc_drv/inc/abcc_cfg.h" for more information
** about the purpose of each separate 'define'.
**------------------------------------------------------------------------------
*/
#ifndef ABCC_CFG_SYNC_ENABLE
   #define ABCC_CFG_SYNC_ENABLE                       ( FALSE )
#endif
#if( ABCC_CFG_SYNC_ENABLE )
   #ifndef ABCC_CFG_USE_ABCC_SYNC_SIGNAL
      #define ABCC_CFG_USE_ABCC_SYNC_SIGNAL           ( TRUE )
   #endif
#endif
#ifndef ABCC_CFG_SYNC_MEASUREMENT_IP
   #define ABCC_CFG_SYNC_MEASUREMENT_IP               ( FALSE )
#endif
#ifndef ABCC_CFG_SYNC_MEASUREMENT_OP
   #define ABCC_CFG_SYNC_MEASUREMENT_OP               ( FALSE )
#endif

/*------------------------------------------------------------------------------
** Interrupt configuration excluding sync
**
** Check the descriptions in "./abcc_drv/inc/abcc_cfg.h" for more information
** about the purpose of each separate 'define'.
**------------------------------------------------------------------------------
*/
#ifndef ABCC_CFG_POLL_ABCC_IRQ_PIN
   #define ABCC_CFG_POLL_ABCC_IRQ_PIN                 ( TRUE )
#endif
#ifndef ABCC_CFG_INT_ENABLED
   #define ABCC_CFG_INT_ENABLED                       ( FALSE )
#endif
#if( ABCC_CFG_INT_ENABLED )
   #ifndef ABCC_CFG_INT_ENABLE_MASK_PAR
      #define ABCC_CFG_INT_ENABLE_MASK_PAR            ( ABP_INTMASK_RDPDIEN | ABP_INTMASK_STATUSIEN | ABP_INTMASK_RDMSGIEN | ABP_INTMASK_WRMSGIEN | ABP_INTMASK_ANBRIEN  )
   #endif
   #ifndef ABCC_CFG_INT_ENABLE_MASK_SPI
      #define ABCC_CFG_INT_ENABLE_MASK_SPI            ( 0 )
   #endif
   #ifndef ABCC_CFG_HANDLE_INT_IN_ISR_MASK
      #define ABCC_CFG_HANDLE_INT_IN_ISR_MASK         ( ABP_INTMASK_RDPDIEN )
   #endif
#endif

/*------------------------------------------------------------------------------
** Watchdog configuration
**
** Check the descriptions in "./abcc_drv/inc/abcc_cfg.h" for more information
** about the purpose of each separate 'define'.
**------------------------------------------------------------------------------
*/
#ifndef ABCC_CFG_WD_TIMEOUT_MS
   #define ABCC_CFG_WD_TIMEOUT_MS                     ( 1000 )
#endif

/*------------------------------------------------------------------------------
** Default startup time
**
** Check the descriptions in "./abcc_drv/inc/abcc_cfg.h" for more information
** about the purpose of each separate 'define'.
**------------------------------------------------------------------------------
*/
#ifndef ABCC_CFG_STARTUP_TIME_MS
   #define ABCC_CFG_STARTUP_TIME_MS                   ( 1500 )
#endif

/*------------------------------------------------------------------------------
** Remap support configuration
**
** Check the descriptions in "./abcc_drv/inc/abcc_cfg.h" for more information
** about the purpose of each separate 'define'.
**------------------------------------------------------------------------------
*/
#ifndef ABCC_CFG_REMAP_SUPPORT_ENABLED
   #define ABCC_CFG_REMAP_SUPPORT_ENABLED             ( FALSE )
#endif
/*------------------------------------------------------------------------------
** The Application data object ADI support configuration
**
** Check the descriptions in "./abcc_drv/inc/abcc_cfg.h" for more information
** about the purpose of each separate 'define'.
**------------------------------------------------------------------------------
*/
#ifndef ABCC_CFG_STRUCT_DATA_TYPE
   #define ABCC_CFG_STRUCT_DATA_TYPE                  ( FALSE )
#endif
#ifndef ABCC_CFG_ADI_GET_SET_CALLBACK
   #define ABCC_CFG_ADI_GET_SET_CALLBACK              ( FALSE )
#endif
#ifndef ABCC_CFG_ADI_TRANS_SET_CALLBACK
   #define ABCC_CFG_ADI_TRANS_SET_CALLBACK            ( FALSE )
#endif
#ifndef ABCC_CFG_64BIT_ADI_SUPPORT
   #define ABCC_CFG_64BIT_ADI_SUPPORT                 ( TRUE )
#endif
#ifndef ABCC_CFG_DOUBLE_ADI_SUPPORT
   #define ABCC_CFG_DOUBLE_ADI_SUPPORT                ( TRUE )
#endif

/*------------------------------------------------------------------------------
** Command sequencer configuration
**
** Check the descriptions in "./abcc_drv/inc/abcc_cfg.h" for more information
** about the purpose of each separate 'define'.
**------------------------------------------------------------------------------
*/
#ifndef ABCC_CFG_DRV_CMD_SEQ_ENABLE
   #define ABCC_CFG_DRV_CMD_SEQ_ENABLE                ( TRUE )
#endif
#ifndef ABCC_CFG_MAX_NUM_CMD_SEQ
   #define ABCC_CFG_MAX_NUM_CMD_SEQ                   ( 2 )
#endif
#ifndef ABCC_CFG_CMD_SEQ_MAX_NUM_RETRIES
   #define ABCC_CFG_CMD_SEQ_MAX_NUM_RETRIES           ( 0 )
#endif

/*------------------------------------------------------------------------------
** Debug and error macro configuration
**
** Check the descriptions in "./abcc_drv/inc/abcc_cfg.h" for more information
** about the purpose of each separate 'define'.
**------------------------------------------------------------------------------
*/
#ifndef ABCC_CFG_ERR_REPORTING_ENABLED
   #define ABCC_CFG_ERR_REPORTING_ENABLED             ( TRUE )
#endif
#ifndef ABCC_CFG_DEBUG_EVENT_ENABLED
   #define ABCC_CFG_DEBUG_EVENT_ENABLED               ( TRUE )
#endif
#ifndef ABCC_CFG_DEBUG_ERR_ENABLED
   #define ABCC_CFG_DEBUG_ERR_ENABLED                 ( TRUE )
#endif
#ifndef ABCC_CFG_DEBUG_MESSAGING
   #define ABCC_CFG_DEBUG_MESSAGING                   ( FALSE )
#endif
#ifndef ABCC_CFG_DEBUG_CMD_SEQ_ENABLED
   #define ABCC_CFG_DEBUG_CMD_SEQ_ENABLED             ( FALSE )
#endif
#ifndef ABCC_CFG_DEBUG_HEXDUMP_MSG
   #define ABCC_CFG_DEBUG_HEXDUMP_MSG                 ( FALSE )
#endif
#ifndef ABCC_CFG_DEBUG_HEXDUMP_SPI
   #define ABCC_CFG_DEBUG_HEXDUMP_SPI                 ( FALSE )
#endif
#ifndef ABCC_CFG_DEBUG_HEXDUMP_UART
   #define ABCC_CFG_DEBUG_HEXDUMP_UART                ( FALSE )
#endif
#ifndef ABCC_CFG_DEBUG_CRC_ERROR_CNT
   #define ABCC_CFG_DEBUG_CRC_ERROR_CNT               ( FALSE )
#endif

#endif  /* inclusion lock */
