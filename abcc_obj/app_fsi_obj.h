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
** **
********************************************************************************
********************************************************************************
**Services:
**           APP_FSI_ProcessCmdMsg() - Processes the commands sent to the
**                                     Application File System Interface
**                                     Object.
**
********************************************************************************
********************************************************************************
*/

#ifndef APP_FSI_OBJ_H_
#define APP_FSI_OBJ_H_

/*******************************************************************************
** Constants
********************************************************************************
*/

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Data type describing a file.
**------------------------------------------------------------------------------
*/
typedef struct app_fsi_File
{
   UINT8* pbFile;      /* Working file pointer where reads and writes are acted on */
   char*  pacName;     /* Represents the name of the file */
   UINT32 lSize;       /* Represents the actual file size of the file */
   UINT32 lChunkSize;  /* Represents the current chunk size. This is the maximum bytes available to
                          read/write in one iteration */
}
APP_FSI_FileType;

/*------------------------------------------------------------------------------
** Data type describing a file instance.
**------------------------------------------------------------------------------
*/
typedef struct app_fsi_FileInstance
{
   UINT16               iInst;     /* Instance number */
   APP_FSI_FileType*    psFile;    /* Pointer to file */
   UINT8                bInstType; /* Type of instance */
}
APP_FSI_FileInstanceType;

/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** APP_FSI_ProcessCmdMsg - Function that processes the commands sent to the
**                         Application File System Interface Object.
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage  - Pointer to a ABP_MsgType message.
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
EXTFUNC void APP_FSI_ProcessCmdMsg( ABP_MsgType* psNewMessage );


#endif /* APP_FSI_OBJ_H_ */
