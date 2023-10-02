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
*/

#include "abcc_drv_cfg.h"
#include "abcc_port.h"
#include "abcc_sys_adapt.h"
#include "abcc_sys_adapt_spi.h"
#include "abcc_sys_adapt_par.h"
#include "abcc_sys_adapt_ser.h"
#include "abcc_debug_err.h"

#include "bcm2835.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/types.h>


#if( ABCC_CFG_DRV_SPI )
static ABCC_SYS_SpiDataReceivedCbfType pnDataReadyCbf;
#endif

#if( ABCC_CFG_DRV_SERIAL )
static ABCC_SYS_SerDataReceivedCbfType pnSerDataReadyCbf;
static int xUart0fd = -1;
static struct termios options;
#endif

BOOL ABCC_SYS_HwInit( void )
{
   /*
   ** GPIO17 connected to CompactCom Reset.
   */

   bcm2835_gpio_write( RPI_BPLUS_GPIO_J8_11, LOW );
   bcm2835_gpio_fsel( RPI_BPLUS_GPIO_J8_11, BCM2835_GPIO_FSEL_OUTP );

   /*
   ** GPIO5, 6, 13, 19 connected to CompactCom OM0-OM3.
   */

   bcm2835_gpio_fsel( RPI_BPLUS_GPIO_J8_29, BCM2835_GPIO_FSEL_OUTP );
   bcm2835_gpio_fsel( RPI_BPLUS_GPIO_J8_31, BCM2835_GPIO_FSEL_OUTP );
   bcm2835_gpio_fsel( RPI_BPLUS_GPIO_J8_33, BCM2835_GPIO_FSEL_OUTP );
   bcm2835_gpio_fsel( RPI_BPLUS_GPIO_J8_35, BCM2835_GPIO_FSEL_OUTP );

   /*
   ** GPIO26 connected to CompactCom IRQ.
   ** Pull-up enabled.
   */

   bcm2835_gpio_set_pud( RPI_BPLUS_GPIO_J8_37, BCM2835_GPIO_PUD_UP );
   bcm2835_gpio_fsel( RPI_BPLUS_GPIO_J8_37, BCM2835_GPIO_FSEL_INPT );

#if ABCC_CFG_MODULE_ID_PINS_CONN
   /*
   ** GPIO23 and GPIO24 connected to CompactCom MI0 and MI1.
   */

   bcm2835_gpio_fsel( RPI_BPLUS_GPIO_J8_16, BCM2835_GPIO_FSEL_INPT );
   bcm2835_gpio_fsel( RPI_BPLUS_GPIO_J8_18, BCM2835_GPIO_FSEL_INPT );
#endif

#if ABCC_CFG_MOD_DETECT_PINS_CONN
   /*
   ** GPIO27 and GPIO22 connected to CompactCom MD0 and MD1.
   ** Pull-up enabled.
   */

   bcm2835_gpio_set_pud( RPI_BPLUS_GPIO_J8_13, BCM2835_GPIO_PUD_UP );
   bcm2835_gpio_set_pud( RPI_BPLUS_GPIO_J8_15, BCM2835_GPIO_PUD_UP );

   bcm2835_gpio_fsel( RPI_BPLUS_GPIO_J8_13, BCM2835_GPIO_FSEL_INPT );
   bcm2835_gpio_fsel( RPI_BPLUS_GPIO_J8_15, BCM2835_GPIO_FSEL_INPT );
#endif

   return( TRUE );
}


BOOL ABCC_SYS_Init( void )
{
   UINT8 bDesiredOpmode = 255;
   UINT8 bModuleId;

   bModuleId = ABCC_ReadModuleId();
   switch( bModuleId )
   {
   case ABP_MODULE_ID_ACTIVE_ABCC30:
      bDesiredOpmode = ABCC_CFG_ABCC_OP_MODE_30;
      break;

   case ABP_MODULE_ID_ACTIVE_ABCC40:
      bDesiredOpmode = ABCC_CFG_ABCC_OP_MODE_40;
      break;

   case ABP_MODULE_ID_PASSIVE_ABCC:
   case ABP_MODULE_ID_CUSTOMER_SPECIFIC:
   default:
      ABCC_PORT_DebugPrint( ( "Module type not supported!" ) );
      ABCC_ERROR( ABCC_SEV_INFORMATION, ABCC_EC_MODULE_ID_NOT_SUPPORTED, (UINT32)bModuleId );
      return( FALSE );
      break;
   }

   switch( bDesiredOpmode )
   {
#if( ABCC_CFG_DRV_SPI )
   case ABP_OP_MODE_SPI:

      /*
      ** Check if running as root
      */
      if( geteuid() != 0 )
      {
         fprintf( stderr, "WARNING! To use SPI as communication interface, this program must be run as root!\n\n" );
         return( FALSE );
      }

      if( !bcm2835_spi_begin() )
      {
         ABCC_PORT_DebugPrint( ( "SPI initialization failed!\n" ) );
         return( FALSE );
      }
      else
      {
         bcm2835_spi_setBitOrder( BCM2835_SPI_BIT_ORDER_MSBFIRST );
         bcm2835_spi_setDataMode( BCM2835_SPI_MODE0 );
         bcm2835_spi_setClockDivider( BCM2835_SPI_CLOCK_DIVIDER_32 ); /* 12,5MHz */
         bcm2835_spi_chipSelect( BCM2835_SPI_CS0 );
         bcm2835_spi_setChipSelectPolarity( BCM2835_SPI_CS0, LOW );
      }
      break;
#endif
#if( ABCC_CFG_DRV_SERIAL )
   case ABP_OP_MODE_SERIAL_19_2:
   case ABP_OP_MODE_SERIAL_57_6:
   case ABP_OP_MODE_SERIAL_115_2:
   case ABP_OP_MODE_SERIAL_625:

      xUart0fd = open( "/dev/serial0", O_RDWR | O_NOCTTY );
      if ( xUart0fd == -1 )
      {
         /*
         ** ERROR - Can not open serial port.
         */
         if( errno == ENOENT )
         {
            /*
            ** No such file or directory
            */
            ABCC_PORT_DebugPrint( ( "Error - Unable to open UART. Make sure it is not used for the Serial Console. Check 'Preferences->Raspberry Pi Configuration'.\n" ) );
         }
         else
         {
            ABCC_PORT_DebugPrint( ( "Error - Unable to open UART. Ensure it is not in use by another application.\n" ) );
         }
         return( FALSE );
      }

      memset( &options, 0, sizeof( options ) );
      options.c_cflag = CS8 | CLOCAL | CREAD;
      options.c_iflag = IGNPAR | IGNBRK;
      options.c_oflag = 0;
      options.c_lflag = 0;
      options.c_cc[ VMIN ] = 0;
      tcflush( xUart0fd, TCIOFLUSH );

      /*
      ** Update any interface-dependent settings that depends on the OpMode.
      */
      switch( bDesiredOpmode )
      {
      case ABP_OP_MODE_SERIAL_19_2:
         options.c_cflag |= B19200;
         options.c_cc[ VTIME ] = 4;
         break;
      case ABP_OP_MODE_SERIAL_57_6:
         options.c_cflag |= B57600;
         options.c_cc[ VTIME ] = 2;
         break;
      case ABP_OP_MODE_SERIAL_115_2:
         options.c_cflag |= B115200;
         options.c_cc[ VTIME ] = 1;
         break;
      case ABP_OP_MODE_SERIAL_625:
      default:
         ABCC_ERROR( ABCC_SEV_FATAL, ABCC_EC_INCORRECT_OPERATING_MODE, 0 );
         return( FALSE );
         break;
      }

      if( tcsetattr( xUart0fd, TCSANOW, &options) < 0 )
      {
         ABCC_PORT_DebugPrint( ( "Could not set serial port attributes! perror=%d", perror ) );
         return( FALSE );
      }
      break;
#endif
   case ABP_OP_MODE_SHIFT_REGISTER:
   case ABP_OP_MODE_16_BIT_PARALLEL:
   case ABP_OP_MODE_8_BIT_PARALLEL:
   default:
      ABCC_ERROR( ABCC_SEV_INFORMATION, ABCC_EC_INCORRECT_OPERATING_MODE, (UINT32)bDesiredOpmode );
      return( FALSE );
      break;
   }

   return( TRUE );
}

void ABCC_SYS_Close( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
}

#if( ABCC_CFG_OP_MODE_SETTABLE )
void ABCC_SYS_SetOpmode( UINT8 bOpmode )
{
   /*
   ** Update the OM pins according to configured OpMode.
   */

   bcm2835_gpio_write( RPI_BPLUS_GPIO_J8_29, LOW );  /* OM0 */
   bcm2835_gpio_write( RPI_BPLUS_GPIO_J8_31, LOW );  /* OM1 */
   bcm2835_gpio_write( RPI_BPLUS_GPIO_J8_33, LOW );  /* OM2 */
   bcm2835_gpio_write( RPI_BPLUS_GPIO_J8_35, LOW );  /* OM3 */

   /* OM0 */
   if( ( bOpmode & 0x01 ) == 0x01 )
   {
      bcm2835_gpio_write( RPI_BPLUS_GPIO_J8_29, HIGH );
   }

   /* OM1 */
   if( ( bOpmode & 0x02 ) == 0x02 )
   {
      bcm2835_gpio_write( RPI_BPLUS_GPIO_J8_31, HIGH );
   }

   /* OM2 */
   if( ( bOpmode & 0x04 ) == 0x04 )
   {
      bcm2835_gpio_write( RPI_BPLUS_GPIO_J8_33, HIGH );
   }

   /* OM3 */
   if( ( bOpmode & 0x08 ) == 0x08 )
   {
      bcm2835_gpio_write( RPI_BPLUS_GPIO_J8_35, HIGH );
   }
}
#endif


#if( ABCC_CFG_OP_MODE_GETTABLE )
UINT8 ABCC_SYS_GetOpmode( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
}
#endif


void ABCC_SYS_HWReset( void )
{
   bcm2835_gpio_write( RPI_BPLUS_GPIO_J8_11, LOW );
}


void ABCC_SYS_HWReleaseReset( void )
{
   bcm2835_gpio_write( RPI_BPLUS_GPIO_J8_11, HIGH );
}


#ifndef ABCC_CFG_ABCC_MODULE_ID
UINT8 ABCC_SYS_ReadModuleId( void )
{
   UINT8 bRet = 0x00;

   if( bcm2835_gpio_lev( RPI_BPLUS_GPIO_J8_16 ) )
   {
      bRet |= 0x01;
   }

   if( bcm2835_gpio_lev( RPI_BPLUS_GPIO_J8_18 ) )
   {
      bRet |= 0x02;
   }

   return bRet;
}
#endif


#if( ABCC_CFG_MOD_DETECT_PINS_CONN )
BOOL ABCC_SYS_ModuleDetect( void )
{
   if( ( bcm2835_gpio_lev( RPI_BPLUS_GPIO_J8_13 ) == LOW ) &&
       ( bcm2835_gpio_lev( RPI_BPLUS_GPIO_J8_15 ) == LOW ) )
   {
      return ( TRUE );
   }
   else
   {
      return ( FALSE );
   }
}
#endif

#if( ABCC_CFG_SYNC_ENABLE && ABCC_CFG_USE_ABCC_SYNC_SIGNAL )
void ABCC_SYS_SyncInterruptEnable( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
}
#endif

#if( ABCC_CFG_SYNC_ENABLE && ABCC_CFG_USE_ABCC_SYNC_SIGNAL )
void ABCC_SYS_SyncInterruptDisable( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
}
#endif


#if( ABCC_CFG_INT_ENABLED )
void ABCC_SYS_AbccInterruptEnable( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
}


void ABCC_SYS_AbccInterruptDisable( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
}
#endif


#if( ABCC_CFG_POLL_ABCC_IRQ_PIN )
BOOL ABCC_SYS_IsAbccInterruptActive( void )
{
   if( bcm2835_gpio_lev( RPI_BPLUS_GPIO_J8_37 ) )
   {
      return( FALSE );
   }
   else
   {
      return( TRUE );
   }
}
#endif


#if( ABCC_CFG_DRV_SPI )
void ABCC_SYS_SpiRegDataReceived( ABCC_SYS_SpiDataReceivedCbfType pnDataReceived  )
{
   pnDataReadyCbf = pnDataReceived;
}


void ABCC_SYS_SpiSendReceive( void* pxSendDataBuffer, void* pxReceiveDataBuffer, UINT16 iLength )
{
   bcm2835_spi_transfernb( pxSendDataBuffer, pxReceiveDataBuffer, iLength );
   pnDataReadyCbf();
}
#endif


#if( ( ABCC_CFG_DRV_PARALLEL || ABCC_CFG_DRV_PARALLEL_30 ) && !ABCC_CFG_MEMORY_MAPPED_ACCESS )
void ABCC_SYS_ParallelRead( UINT16 iMemOffset, void* pxData, UINT16 iLength )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}


#if( ABCC_CFG_DRV_PARALLEL_30 )
UINT8 ABCC_SYS_ParallelRead8( UINT16 iMemOffset )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}
#endif


#if( ABCC_CFG_DRV_PARALLEL )
UINT16 ABCC_SYS_ParallelRead16( UINT16 iMemOffset )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}
#endif


void ABCC_SYS_ParallelWrite( UINT16 iMemOffset, void* pxData, UINT16 iLength )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}


#if( ABCC_CFG_DRV_PARALLEL_30 )
void ABCC_SYS_ParallelWrite8( UINT16 iMemOffset, UINT8 pbData )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}
#endif


#if( ABCC_CFG_DRV_PARALLEL )
void ABCC_SYS_ParallelWrite16( UINT16 iMemOffset, UINT16 piData )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}
#endif

void* ABCC_SYS_ParallelGetRdPdBuffer( void )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}


void* ABCC_SYS_ParallelGetWrPdBuffer( void )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}
#endif


#if( ABCC_CFG_DRV_SERIAL )
void ABCC_SYS_SerRegDataReceived( ABCC_SYS_SpiDataReceivedCbfType pnDataReceived  )
{
   if( pnDataReceived != NULL )
   {
      pnSerDataReadyCbf = pnDataReceived;
      return;
   }

   ABCC_ERROR( ABCC_SEV_WARNING, ABCC_EC_PARAMETER_NOT_VALID, (UINT32)pnDataReceived );
   return;
}


void ABCC_SYS_SerSendReceive( void* pxTxDataBuffer, void* pxRxDataBuffer, UINT16 iTxSize, UINT16 iRxSize )
{
   UINT8* pbDataBuffer;

   /*
   ** Start the UART transmission
   */
   if ( xUart0fd != -1 )
   {
      pbDataBuffer = (UINT8*)pxTxDataBuffer;
      while( iTxSize > 0 )
      {
         int count = write( xUart0fd, pbDataBuffer, iTxSize );
         if( count < 0 )
         {
            ABCC_PORT_DebugPrint( ( "UART TX error. perror=%d\n", perror ) );
            return;
         }
         pbDataBuffer += count;
         iTxSize -= count;
      }
   }

   /*
   ** Start the UART reception
   */
   if ( xUart0fd != -1 )
   {
      pbDataBuffer = (UINT8*)pxRxDataBuffer;

      while( iRxSize > 0 )
      {
         int rx_length = read( xUart0fd, pbDataBuffer, iRxSize );

         if ( rx_length < 0 )
         {
            /*
            ** An error occured.
            */
            ABCC_PORT_DebugPrint( ( "UART RX error. perror=%d\n", perror ) );
            return;
         }
         else if ( rx_length == 0 )
         {
            /*
            ** No data waiting to be read.
            */
            break;
         }
         else
         {
            /*
            ** Data received
            */
            pbDataBuffer += rx_length;
            iRxSize -= rx_length;
         }
      }
      if( iRxSize == 0 )
      {
         pnSerDataReadyCbf();
      }
   }
}

void ABCC_SYS_SerRestart( void )
{
   if ( xUart0fd != -1 )
   {
      tcflush( xUart0fd, TCIOFLUSH );
   }
   return;
}
#endif
