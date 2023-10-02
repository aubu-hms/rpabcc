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
** Header file for LogPrint
********************************************************************************
********************************************************************************
** Services:
**    LOGPRINT_SetConsoleStream() - Set the stream to use for 'console' ouput.
**    LOGPRINT_SetLogFileStream() - Set the stream to use for 'log file' ouput.
**    LOGPRINT_Printf()           - printf() equivalent that sends output to
**                                  one or more streams.
**    LOGPRINT_TimeStamp()        - Adds a timestamp with the local/wall time
**                                  to the output streams.
********************************************************************************
********************************************************************************
*/

#ifndef LOGPRINT_H_
#define LOGPRINT_H_

/*******************************************************************************
** Public constants.
********************************************************************************
*/

/*******************************************************************************
** Public typedefs.
********************************************************************************
*/

/*******************************************************************************
** Public globals.
********************************************************************************
*/

/*******************************************************************************
** Public services.
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Sets the stream to use for the console, normally 'stdout' or 'stderr'.
** Setting the stream to NULL will disable printouts.
**------------------------------------------------------------------------------
** Arguments:
**    pxStream - Stream to use for the console.
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
EXTFUNC void LOGPRINT_SetConsoleStream( FILE* pxStream );

/*------------------------------------------------------------------------------
** Sets the stream to use for the log file, the caller is responsible for
** opening and closing the the stream. Setting the stream to NULL will disable
** printouts.
**------------------------------------------------------------------------------
** Arguments:
**    pxStream - Stream to use for the logfile.
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
EXTFUNC void LOGPRINT_SetLogFileStream( FILE* pxStream );

/*------------------------------------------------------------------------------
** printf()-equivalent that sends the output to one or more streams.
**------------------------------------------------------------------------------
** Arguments:
**    printf()-style arguments.
**
** Returns:
**    Number of printed characters.
**------------------------------------------------------------------------------
*/
EXTFUNC int LOGPRINT_Printf( const char *pacFormat, ... );

/*------------------------------------------------------------------------------
** Produces a platform-dependent timestamp on the output streams.
**------------------------------------------------------------------------------
** Arguments:
**    None.
**
** Returns:
**    TRUE on success, FALSE otherwise.
**------------------------------------------------------------------------------
*/
#if ( defined( __linux__ ) && defined( __GNUC__ ) ) || \
    ( defined( _WIN32 )    && defined( _MSC_VER ) )
EXTFUNC BOOL LOGPRINT_TimeStamp( void );
#endif

#endif /* LOGPRINT_H */
