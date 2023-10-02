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
** COPYRIGHT NOTIFICATION (c) 2018 HMS Industrial Networks AB                 **
**                                                                            **
** This program is the property of HMS Industrial Networks AB.                **
** It may not be reproduced, distributed, or used without permission          **
** of an authorized company official.                                         **
********************************************************************************
********************************************************************************
** Assembly Mapping Object 0xEB - Public interfaces
********************************************************************************
********************************************************************************
** Services:
**   ASM_GetData()       - Get data for an assembly instance.
**   ASM_Init()          - Initializes the assembly mapping object.
**   ASM_ProcessCmdMsg() - Processes a message addressed to this object.
********************************************************************************
********************************************************************************
*/

#ifndef ASM_OBJ_H_
#define ASM_OBJ_H_

#include "abcc_td.h"

/*******************************************************************************
** Constants
********************************************************************************
*/

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Structure representing an assembly mapping instance.
**------------------------------------------------------------------------------
*/
typedef struct ASM_Instance
{
   const UINT32 lDescriptor;
#if ASM_SUPPORT_DYNAMIC_INSTANCES
   AD_MapType* psMap;
#else
   const AD_MapType* psMap;
#endif
#if ASM_IA_NAME_ENABLE
   const char* pacName;
#endif
#if ASM_IA_MAX_NUM_ADI_MAPS_ENABLE
   const UINT16 iMaxNumAdiMaps;
#endif
}
ASM_InstanceType;

/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Get assembly data for an instance.
**------------------------------------------------------------------------------
** Arguments:
**    iInstance     - Assembly mapping instance.
**    pxBuffer      - Buffer to put the assembly data in.
**    piOctetOffset - Offset in octets in pxBuffer where the data will be
**                    written to. This offset will be incremented according to
**                    the size written.
**
** Returns:
**    TRUE - Data was successfully written to destionation buffer.
**    FALSE - Invalid assembly mapping instance.
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL ASM_GetData( UINT16 iInstance,
                          void* pxBuffer,
                          UINT16* piOctetOffset );

/*------------------------------------------------------------------------------
** Initializes the Assembly Mapping object.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void ASM_Init( void );

/*------------------------------------------------------------------------------
** Process an object request against the Assembly Mapping Object.
**------------------------------------------------------------------------------
** Arguments:
**    psMsgBuffer - Pointer to ABCC command message.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void ASM_ProcessCmdMsg( ABP_MsgType* psNewMessage );

#endif  /* inclusion lock */
