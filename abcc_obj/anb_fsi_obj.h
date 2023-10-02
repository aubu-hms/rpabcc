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
** This is the public header file for the Anybus File System Interface object.**
********************************************************************************
********************************************************************************
**Services:
**       ANB_FSI_FileOpen()                 - Creates an FSI instance, and
**                                            opens a file on the ABCC.
**       ANB_FSI_FileWrite()                - Writes data to the open file.
**       ANB_FSI_Close()                    - Closes the current file and
**                                            deletes the FSI instance.
**       ANB_FSI_InvalidateUploadedAbccFw() - Corrupt the firmware file on the
**                                            ABCC, by sending dummy data to
**                                            the .hiff file.
********************************************************************************
********************************************************************************
*/
#ifndef ANB_FSI_OBJ_H
#define ANB_FSI_OBJ_H

#include "abcc_td.h"
#include "abcc.h"
#include "abcc_sys_adapt.h"
#include "abcc_obj_cfg.h"
#include "abcc_cmd_seq_if.h"
#include "abp.h"

/*******************************************************************************
** Constants
********************************************************************************
*/

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Creates an FSI instance, and opens a file on the ABCC.
**------------------------------------------------------------------------------
** Arguments:
**    pcFilename      - Pointer to filename
**    bFilenameLength - Length of filename.
**    bAccessMode     - Access mode on file that shall be open.
**                      see abp_fsi.h for access mode.
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ANB_FSI_FileOpen( const char* pcFilename, UINT8 bFilenameLength, UINT8 bAccessMode );

/*------------------------------------------------------------------------------
** Writes data to the open file.
**------------------------------------------------------------------------------
** Arguments:
**   pbData             - Pointer to data to write.
**   iDataToWriteLength - Length of data to write.
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ANB_FSI_FileWrite ( const UINT8* pbData, UINT16 iDataToWriteLength );

/*------------------------------------------------------------------------------
** Closes the current file and deletes the FSI instance.
**------------------------------------------------------------------------------
** Arguments:
**   None.
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ANB_FSI_Close( void );

/*------------------------------------------------------------------------------
** This function will invalidate the previously uploaded firmware upgrade
** file (.hiff file). The function will send dummy data to the the uploaded
** .hiff file. The ABCC will not be able to use this corrupt .hiff file for
** firmware upgrade.
**------------------------------------------------------------------------------
** Arguments:
**    None.
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ANB_FSI_InvalidateUploadedAbccFw( void );

/*------------------------------------------------------------------------------
** Get if instance is locked so that data is written sequentially.
**------------------------------------------------------------------------------
** Arguments:
**   None.
**
** Returns:
**    TRUE if instance is locked. FALSE if instance in unlocked.
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL ANB_FSI_GetInstanceLocked( void );

#endif  /* inclusion lock */
