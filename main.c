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
** Main program
********************************************************************************
********************************************************************************
*/

#include "abcc_td.h"
#include "abcc.h"
#include "abcc_sys_adapt.h"
#include "ad_obj.h"
#include "appl_abcc_handler.h"
#include "abcc_versions.h"

#include "bcm2835.h"
#include "logprint.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define CONSOLE_INPUT_BUFFER_SIZE     16

static bool vSetRawConsoleMode( bool fGetSet )
{
   static struct termios   sOriginalTermIOSet;
   struct termios          sRawTermIOSet;

   if( fGetSet )
   {
      if( tcgetattr( STDIN_FILENO, &sOriginalTermIOSet ) < 0 )
      {
         perror( "tcgetattr() failed when reading 'sOriginalTermIOSet'" );
         return( false );
      }

      if( tcgetattr( STDIN_FILENO, &sRawTermIOSet ) < 0 )
      {
         perror( "tcgetattr() failed when reading 'sRawTermIOSet'" );
         return( false );
      }

      sRawTermIOSet.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
      sRawTermIOSet.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
      sRawTermIOSet.c_cflag &= ~(CSIZE | PARENB);
      sRawTermIOSet.c_cflag |= CS8;

      sRawTermIOSet.c_cc[ VMIN ] = 0;
      sRawTermIOSet.c_cc[ VTIME ] = 0;

      if( tcsetattr( STDIN_FILENO, TCSANOW, &sRawTermIOSet ) < 0 )
      {
         perror( "tcsetattr() failed when writing 'sRawTermIOSet'" );
         return( false );
      }
   }
   else
   {
      if( tcsetattr( STDIN_FILENO, TCSANOW, &sOriginalTermIOSet ) < 0 )
      {
         perror( "tcsetattr() failed when writing 'sOriginalTermIOSet'" );
         return( false );
      }
   }

   return( true );
}

static int Init( FILE** const ppxDebugFile )
{
   LOGPRINT_SetConsoleStream( stderr );
   *ppxDebugFile = fopen( "log_file.txt", "a" );
   if( *ppxDebugFile == NULL )
   {
      fprintf( stderr, "failed to open log_file.txt\n" );
      exit( EXIT_FAILURE );
   }
   LOGPRINT_SetLogFileStream( *ppxDebugFile );

   if( !bcm2835_init() )
   {
      fprintf( stderr, "bcm2835_init failed!\n" );
      exit( EXIT_FAILURE );
   }

   LOGPRINT_Printf( "-------------------------------------------------\n" );
   LOGPRINT_Printf( "Program started at: " );
   LOGPRINT_TimeStamp();
   LOGPRINT_Printf( "\n" );
   LOGPRINT_Printf( "-------------------------------------------------\n" );
   LOGPRINT_Printf( "HMS Industrial Networks\n" );
   LOGPRINT_Printf( "Anybus CompactCom Host Application Example Code\n" );
   LOGPRINT_Printf( "Raspberry Pi\n" );
   LOGPRINT_Printf( ABCC_STARTER_KIT_VER_STRING );
   LOGPRINT_Printf( "\n\n" );

   if( !vSetRawConsoleMode( true ) )
   {
      fprintf( stderr, "vSetRawConsoleMode( true ) failed\n" );
      exit( EXIT_FAILURE );
   }

   if( ABCC_HwInit() != ABCC_EC_NO_ERROR )
   {
      // We need to do cleanup, so do not exit directly.
      return( EXIT_FAILURE );
   }

   return( EXIT_SUCCESS );
}

static void HandleTimer( uint64_t* const pllTickThen )
{
   /*
   ** Poll the system timer and inform ABCC_RunTimerSystem() about how much
   ** time that has elapsed.
   */
   const uint64_t llTickNow = bcm2835_st_read();
   uint64_t llTickDiff = llTickNow - *pllTickThen;

   if( llTickDiff > 1000 )
   {
      *pllTickThen = llTickNow;
      llTickDiff /= 1000;

      /*
      ** Make sure that *all* elapsed time is accounted for.
      */
      while( llTickDiff > 0 )
      {
         if( llTickDiff > ABP_SINT16_MAX )
         {
            ABCC_RunTimerSystem( ABP_SINT16_MAX );
            llTickDiff -= ABP_SINT16_MAX;
         }
         else
         {
            ABCC_RunTimerSystem( (INT16)llTickDiff );
            llTickDiff = 0;
         }
      }
   }
}

static bool HandleInput( int* const pxReturnVal )
{
   /*
   ** Check if someone has pressed a key.
   */
   static uint8_t abConsoleInput[ CONSOLE_INPUT_BUFFER_SIZE ];
   const ssize_t xCount = read( STDIN_FILENO, abConsoleInput, CONSOLE_INPUT_BUFFER_SIZE );

   if( xCount > 0 )
   {
      int i;
      for( i = 0; i < xCount; i++ )
      {
         if( abConsoleInput[ i ] == 'q' || abConsoleInput[ i ] == 'Q' )
         {
            return( false );
         }
      }
   }
   else if( xCount == 0 )
   {
      /*
      ** Empty
      */
   }
   else // if( xCount < 0 )
   {
      perror( "read( STDIN ) failed" );
      *pxReturnVal = EXIT_FAILURE;
      return( false );
   }

   return( true );
}

int main()
{
   FILE*    pxDebugFile = NULL;
   int      xReturnVal  = Init( &pxDebugFile );
   bool     fRun        = xReturnVal == EXIT_SUCCESS;
   uint64_t llTickThen  = bcm2835_st_read();

   while( fRun )
   {
      const APPL_AbccHandlerStatusType eAbccHandlerStatus = APPL_HandleAbcc();
      switch( eAbccHandlerStatus )
      {
      case APPL_MODULE_NO_ERROR:
         break;
      case APPL_MODULE_RESET:
         APPL_RestartAbcc();
         break;
      case APPL_MODULE_NOT_DETECTED:
      case APPL_MODULE_NOT_SUPPORTED:
      case APPL_MODULE_NOT_ANSWERING:
      case APPL_MODULE_SHUTDOWN:
      case APPL_MODULE_UNEXPECTED_ERROR:
      default:
         printf( "\nAPPL_HandleAbcc() returned status code: %d\n", eAbccHandlerStatus );
         xReturnVal = EXIT_FAILURE;
         fRun = false;
         continue;
      }

      HandleTimer( &llTickThen );
      fRun = HandleInput( &xReturnVal );
   }

   ABCC_HWReset();
   bcm2835_spi_end();

   if( !vSetRawConsoleMode( false ) )
   {
      fprintf( stderr, "vSetRawConsoleMode( false ) failed\n" );
      xReturnVal = EXIT_FAILURE;
   }

   LOGPRINT_Printf( "-------------------------------------------------\n" );
   LOGPRINT_Printf( "Program ended at: " );
   LOGPRINT_TimeStamp();
   LOGPRINT_Printf( "\n" );
   LOGPRINT_Printf( "-------------------------------------------------\n" );

   fclose( pxDebugFile );

   return( xReturnVal );
}
