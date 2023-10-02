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
** Source file for the EtherCAT Object.
********************************************************************************
********************************************************************************
*/

#include "abcc_td.h"
#include "abcc.h"
#include "abcc_sys_adapt.h"
#include "abcc_obj_cfg.h"
#include "ect.h"
#include "abp.h"
#include "abp_ect.h"
#include "string.h"
#include "appl_abcc_handler.h"
#include "abcc_port.h"
#include "appl_adi_config.h"

#if ECT_OBJ_ENABLE

/*******************************************************************************
** Defines
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Object attribute values
**------------------------------------------------------------------------------
*/
#define ECT_OA_NAME_VALUE                          "EtherCAT"
#define ECT_OA_REV_VALUE                           1
#define ECT_OA_NUM_INST_VALUE                      1
#define ECT_OA_HIGHEST_INST_VALUE                  1

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Structure describing the EtherCAT Instance 1 attribute 13 ADI Translation
**------------------------------------------------------------------------------
*/
#if ECT_IA_ADI_TRANS_ENABLE
typedef struct ect_Instance1AdiTranslation
{
   UINT16 iADIInstanceNumber;
   UINT16 iADIObjectIndex;
}
ect_Instance1AdiTranslation;
#endif

/*------------------------------------------------------------------------------
** Structure describing the EtherCAT Instance 1 attribute 15 Object Subindex
** Translation
**------------------------------------------------------------------------------
*/
#if ECT_IA_OBJ_SUB_TRANS_ENABLE
typedef struct ect_Instance1ObjSubTrans
{
   UINT16 iADIInstanceNumber;
   UINT16 iADIObjectIndex;
   UINT8  bADIObjectIndexSubindex;
}
ect_Instance1ObjSubTrans;
#endif

/*------------------------------------------------------------------------------
** Structure describing the EtherCAT Instance 1 attributes.
**------------------------------------------------------------------------------
*/
#if( ECT_IA_WR_PD_ASSY_INST_TRANS_ENABLE || \
     ECT_IA_RD_PD_ASSY_INST_TRANS_ENABLE || \
     ECT_IA_ADI_TRANS_ENABLE ||             \
     ECT_IA_OBJ_SUB_TRANS_ENABLE ||         \
     ECT_IA_ENABLE_FOE_ENABLE ||            \
     ECT_IA_ENABLE_EOE_ENABLE ||            \
     ECT_IA_SET_DEV_ID_AS_CSA_ENABLE ||     \
     ECT_IA_STATE_TIMEOUTS_ENABLE ||        \
     ECT_IA_COMP_IDENT_LISTS_ENABLE ||      \
     ECT_IA_CLEAR_IDENT_AL_STS_ENABLE ||    \
     ECT_IA_DEF_TXPDO_ASSIGN_ENABLE ||      \
     ECT_IA_DEF_RXPDO_ASSIGN_ENABLE )
typedef struct ect_Instance
{
#if ECT_IA_WR_PD_ASSY_INST_TRANS_ENABLE
   UINT16 aiWrPdAssyInstTrans[ ECT_IA_WR_PD_ASSY_INST_TRANS_SIZE ];
#endif
#if ECT_IA_RD_PD_ASSY_INST_TRANS_ENABLE
   UINT16 aiRdPdAssyInstTrans[ ECT_IA_RD_PD_ASSY_INST_TRANS_SIZE ];
#endif
#if ECT_IA_ADI_TRANS_ENABLE
   ect_Instance1AdiTranslation asADITrans[ ECT_IA_ADI_TRANS_SIZE ];
#endif
#if ECT_IA_OBJ_SUB_TRANS_ENABLE
   ect_Instance1ObjSubTrans asObjSubTrans[ ECT_IA_OBJ_SUB_TRANS_SIZE ];
#endif
#if ECT_IA_ENABLE_FOE_ENABLE
   BOOL fEnableFoE;
#endif
#if ECT_IA_ENABLE_EOE_ENABLE
   BOOL fEnableEoE;
#endif
#if ECT_IA_SET_DEV_ID_AS_CSA_ENABLE
   BOOL fSetDevIDAsCSA;
#endif
#if ECT_IA_STATE_TIMEOUTS_ENABLE
   UINT32 alStateTransTimeouts[ 4 ];
#endif
#if ECT_IA_COMP_IDENT_LISTS_ENABLE
   BOOL fCompIDLists;
#endif
#if ECT_IA_CLEAR_IDENT_AL_STS_ENABLE
   BOOL fClearIdAlSts;
#endif
#if ECT_IA_DEF_TXPDO_ASSIGN_ENABLE
   UINT16 aiTxPdoAssignments[ ECT_IA_DEF_TXPDO_ASSIGN_SIZE ];
#endif
#if ECT_IA_DEF_RXPDO_ASSIGN_ENABLE
   UINT16 aiRxPdoAssignments[ ECT_IA_DEF_RXPDO_ASSIGN_SIZE ];
#endif
}
ect_InstanceType;
#endif /* if( ECT_IA_WR_PD_ASSY_INST_TRANS_ENABLE... */

#if ECT_OBJ_CMD_GET_OBJ_DESC_ENABLE
/*------------------------------------------------------------------------------
** Enumeration with EtherCAT defined object codes for use with the
** Get_Object_Description command.
**------------------------------------------------------------------------------
*/
typedef enum ect_ObjCode
{
   ECT_OBJCODE_VAR    = 0x07,
   ECT_OBJCODE_ARRAY  = 0x08,
   ECT_OBJCODE_RECORD = 0x09
}
ect_ObjCodeType;

/*------------------------------------------------------------------------------
** Enumeration with EtherCAT base data types and pre-defined record object data
** types for use with the Get_Object_Description command.
**------------------------------------------------------------------------------
*/
typedef enum ect_DataType
{
   ECT_DATATYPE_BOOLEAN            = 0x0001,
   ECT_DATATYPE_INTEGER8           = 0x0002,
   ECT_DATATYPE_INTEGER16          = 0x0003,
   ECT_DATATYPE_INTEGER32          = 0x0004,
   ECT_DATATYPE_UNSIGNED8          = 0x0005,
   ECT_DATATYPE_UNSIGNED16         = 0x0006,
   ECT_DATATYPE_UNSIGNED32         = 0x0007,
   ECT_DATATYPE_REAL32             = 0x0008,
   ECT_DATATYPE_VISIBLE_STRING     = 0x0009,
   ECT_DATATYPE_OCTET_STRING       = 0x000A,
   ECT_DATATYPE_UNICODE_STRING     = 0x000B,
   ECT_DATATYPE_INTEGER24          = 0x0010,
   ECT_DATATYPE_REAL64             = 0x0011,
   ECT_DATATYPE_INTEGER40          = 0x0012,
   ECT_DATATYPE_INTEGER48          = 0x0013,
   ECT_DATATYPE_INTEGER56          = 0x0014,
   ECT_DATATYPE_INTEGER64          = 0x0015,
   ECT_DATATYPE_UNSIGNED40         = 0x0018,
   ECT_DATATYPE_UNSIGNED48         = 0x0019,
   ECT_DATATYPE_UNSIGNED56         = 0x001A,
   ECT_DATATYPE_UNSIGNED64         = 0x001B,
   ECT_DATATYPE_GUID               = 0x001D,
   ECT_DATATYPE_BYTE               = 0x001E,
   ECT_DATATYPE_WORD               = 0x001F,
   ECT_DATATYPE_DWORD              = 0x0020,
   ECT_DATATYPE_BITARR8            = 0x002D,
   ECT_DATATYPE_BITARR16           = 0x002E,
   ECT_DATATYPE_BITARR32           = 0x002F,
   ECT_DATATYPE_BIT1               = 0x0030,
   ECT_DATATYPE_BIT2               = 0x0031,
   ECT_DATATYPE_BIT3               = 0x0032,
   ECT_DATATYPE_BIT4               = 0x0033,
   ECT_DATATYPE_BIT5               = 0x0034,
   ECT_DATATYPE_BIT6               = 0x0035,
   ECT_DATATYPE_BIT7               = 0x0036,
   ECT_DATATYPE_BIT8               = 0x0037,
   ECT_DATATYPE_ARRAY_OF_INT       = 0x0260,
   ECT_DATATYPE_ARRAY_OF_SINT      = 0x0261,
   ECT_DATATYPE_ARRAY_OF_DINT      = 0x0262,
   ECT_DATATYPE_ARRAY_OF_UDINT     = 0x0263,

   /* Pre-defined record object datatypes */
   ECT_DATATYPE_PDO_MAPPING        = 0x0021,
   ECT_DATATYPE_IDENTITY           = 0x0023,
   ECT_DATATYPE_COMMAND_PAR        = 0x0025,
   ECT_DATATYPE_PDO_PARAMETER      = 0x0027,
   ECT_DATATYPE_ENUM               = 0x0028,
   ECT_DATATYPE_SM_SYNCHRONISATION = 0x0029,
   ECT_DATATYPE_RECORD             = 0x002A,
   ECT_DATATYPE_BACKUP_PARAMETER   = 0x002B,
   ECT_DATATYPE_MOD_DEV_PROFILE    = 0x002C,
   ECT_DATATYPE_ERROR_SETTING      = 0x0281,
   ECT_DATATYPE_DIAGNOSIS_HISTORY  = 0x0282,
   ECT_DATATYPE_EXT_SYNC_STATUS    = 0x0283,
   ECT_DATATYPE_EXT_SYNC_SETTINGS  = 0x0284,
   ECT_DATATYPE_DEFTYPE_FSOEFRAME  = 0x0285,
   ECT_DATATYPE_DEFTYPE_FSOECOMPAR = 0x0286
}
ect_DataTypeType;

/*------------------------------------------------------------------------------
** Structure describing an entry used in the EtherCAT Object
** Get_Object_Description command.
**------------------------------------------------------------------------------
*/
typedef struct ect_GetObjectDesc
{
   UINT16           iObjectIndex;
   ect_DataTypeType eEctDataType;
   ect_ObjCodeType  eObjectCode;
}
ect_GetObjectDescType;
#endif


/*------------------------------------------------------------------------------
** Structure describing an EtherCAT Object.
**------------------------------------------------------------------------------
*/
typedef struct ect_Object
{
    const char*           pcName;
    UINT8                 bRevision;
    UINT16                iNumberOfInstances;
    UINT16                iHighestInstanceNo;
#if ECT_OBJ_CMD_GET_OBJ_DESC_ENABLE
    ect_GetObjectDescType asObjectDescriptions[ ECT_GET_OBJ_DESC_NUM_OBJ ];
#endif
}
ect_ObjectType;

/*******************************************************************************
** Private Globals
********************************************************************************
*/
#if ECT_IA_ETHERCAT_STATE_ENABLE
   static UINT8 ect_bEtherCATState;
#endif

#if ECT_IA_FSOE_STATUS_IND_ENABLE
   static UINT8 ect_eFSoEStatusIndicator;
#endif

static const ect_ObjectType ect_sObject =
{
   ECT_OA_NAME_VALUE,               /* Name.                                                  */
   ECT_OA_REV_VALUE,                /* Revision.                                              */
   ECT_OA_NUM_INST_VALUE,           /* Number of instances.                                   */
   ECT_OA_HIGHEST_INST_VALUE        /* Highest instance number.                               */
#if ECT_OBJ_CMD_GET_OBJ_DESC_ENABLE
   ,
   ECT_GET_OBJ_DESC_CONTENT         /* Object descriptions for Get_Object_Description command */
#endif
};

#if( ECT_IA_WR_PD_ASSY_INST_TRANS_ENABLE || \
     ECT_IA_RD_PD_ASSY_INST_TRANS_ENABLE || \
     ECT_IA_ADI_TRANS_ENABLE ||             \
     ECT_IA_OBJ_SUB_TRANS_ENABLE ||         \
     ECT_IA_ENABLE_FOE_ENABLE ||            \
     ECT_IA_ENABLE_EOE_ENABLE ||            \
     ECT_IA_SET_DEV_ID_AS_CSA_ENABLE ||     \
     ECT_IA_STATE_TIMEOUTS_ENABLE ||        \
     ECT_IA_COMP_IDENT_LISTS_ENABLE ||      \
     ECT_IA_CLEAR_IDENT_AL_STS_ENABLE ||    \
     ECT_IA_DEF_TXPDO_ASSIGN_ENABLE ||      \
     ECT_IA_DEF_RXPDO_ASSIGN_ENABLE )
static const ect_InstanceType ect_sInstance =
{
#if ECT_IA_WR_PD_ASSY_INST_TRANS_ENABLE
   ECT_IA_WR_PD_ASSY_INST_TRANS_VALUE,
#endif
#if ECT_IA_RD_PD_ASSY_INST_TRANS_ENABLE
   ECT_IA_RD_PD_ASSY_INST_TRANS_VALUE,
#endif
#if ECT_IA_ADI_TRANS_ENABLE
   ECT_IA_ADI_TRANS_VALUE,
#endif
#if ECT_IA_OBJ_SUB_TRANS_ENABLE
   ECT_IA_OBJ_SUB_TRANS_VALUE,
#endif
#if ECT_IA_ENABLE_FOE_ENABLE
   ECT_IA_ENABLE_FOE_VALUE,
#endif
#if ECT_IA_ENABLE_EOE_ENABLE
   ECT_IA_ENABLE_EOE_VALUE,
#endif
#if ECT_IA_SET_DEV_ID_AS_CSA_ENABLE
   ECT_IA_SET_DEV_ID_AS_CSA_VALUE,
#endif
#if ECT_IA_STATE_TIMEOUTS_ENABLE
   ECT_IA_STATE_TIMEOUTS_VALUE,
#endif
#if ECT_IA_COMP_IDENT_LISTS_ENABLE
   ECT_IA_COMP_IDENT_LISTS_VALUE,
#endif
#if ECT_IA_CLEAR_IDENT_AL_STS_ENABLE
   ECT_IA_CLEAR_IDENT_AL_STS_VALUE,
#endif
#if ECT_IA_DEF_TXPDO_ASSIGN_ENABLE
   ECT_IA_DEF_TXPDO_ASSIGN_VALUE,
#endif
#if ECT_IA_DEF_RXPDO_ASSIGN_ENABLE
   ECT_IA_DEF_RXPDO_ASSIGN_VALUE
#endif
};

#endif /* #if( ECT_IA_WR_PD_ASSY_INST_TRANS_ENABLE... */

/*******************************************************************************
** Private Services
********************************************************************************
*/

#if ECT_IA_ENUM_ADIS_ENABLE
/*------------------------------------------------------------------------------
** Generates the ADI list that instance attribute 9 represents, i.e. the list
** of all ABP_ENUM ADIs.
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**    None              - The psNewMessage is set to the appropriate response
**                        by this function depending on the outcome.
**------------------------------------------------------------------------------
*/
static void GenerateEnumAdiList( ABP_MsgType* psNewMessage )
{
   UINT16   iAdiIndex;
   UINT16   iMsgOffset;
   UINT16   iNumOfAdis;
   UINT8    bErrCode;

   iMsgOffset = 0;
   iNumOfAdis = APPL_GetNumAdi();
   bErrCode = ABP_ERR_NO_ERROR;

   /*
   ** Search the ADI list for ABP_ENUM ADIs and copy their instance number(s)
   ** to the message data field.
   */
   for( iAdiIndex = 0; iAdiIndex < iNumOfAdis; iAdiIndex++ )
   {
#if( ABCC_CFG_STRUCT_DATA_TYPE )
      /*
      ** Ignore struct ADIs, ABP_ENUMs are not allowed in them.
      */
      if( ( APPL_asAdiEntryList[ iAdiIndex ].psStruct == NULL ) &&
          ( APPL_asAdiEntryList[ iAdiIndex ].bDataType == ABP_ENUM ) )
#else
      if( APPL_asAdiEntryList[ iAdiIndex ].bDataType == ABP_ENUM )
#endif
      {
         if( iMsgOffset >= ( ABCC_CFG_MAX_MSG_SIZE - 1 ) )
         {
            /*
            ** There are more ABP_ENUM ADIs than what we can fit into the
            ** message data field. Notify the user and send an error response
            ** to prevent an 'Only half of the ABP_ENUM ADIs shows up as ENUM!'
            ** situation.
            */
            ABCC_ERROR( ABCC_SEV_WARNING,
                        ABCC_EC_NO_RESOURCES,
                        (UINT32)APPL_asAdiEntryList[ iAdiIndex ].iInstance );
            bErrCode = ABP_ERR_NO_RESOURCES;

            break; /* Break for( ... ) */
         }
         ABCC_SetMsgData16( psNewMessage,
                              APPL_asAdiEntryList[ iAdiIndex ].iInstance,
                              iMsgOffset );
         iMsgOffset += ABP_UINT16_SIZEOF;
      }
   }

   if( bErrCode == ABP_ERR_NO_ERROR )
   {
      ABP_SetMsgResponse( psNewMessage, iMsgOffset );
   }
   else
   {
      ABP_SetMsgErrorResponse( psNewMessage, 1, bErrCode );
   }
}
#endif /* ECT_IA_ENUM_ADIS_ENABLE */

/*------------------------------------------------------------------------------
** Processes commands to ECT Instances
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
static void InstanceCommand( ABP_MsgType* psNewMessage )
{
   /*
   ** This function processes commands to the EtherCAT Instance.
   */
   if( ABCC_GetMsgInstance( psNewMessage ) != 1 )
   {
      /*
      ** The Instance does not exist.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_INST );

      return;
   }

   switch( ABCC_GetMsgCmdBits( psNewMessage ) )
   {
   case ABP_CMD_GET_ATTR:
   {
      switch( ABCC_GetMsgCmdExt0( psNewMessage ) )
      {
#if ECT_IA_VENDOR_ID_ENABLE
      case ABP_ECT_IA_VENDOR_ID:

         /*
         ** Copy the 1st Instance 1 attribute (Vendor ID) to the message.
         */
         ABCC_SetMsgData32( psNewMessage, ECT_IA_VENDOR_ID_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_VENDOR_ID_DS );
         break;
#endif
#if ECT_IA_PRODUCT_CODE_ENABLE
      case ABP_ECT_IA_PRODUCT_CODE:

         /*
         ** Copy the 2nd Instance 1 attribute (Product code) to the message.
         */
         ABCC_SetMsgData32( psNewMessage, ECT_IA_PRODUCT_CODE_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_PRODUCT_CODE_DS );
         break;
#endif
#if ECT_IA_MAJOR_REV_ENABLE
      case ABP_ECT_IA_MAJOR_REV:

         /*
         ** Copy the 3rd Instance 1 attribute (Revision high word) to the
         ** message.
         */
         ABCC_SetMsgData16( psNewMessage, ECT_IA_REVISION_HW_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_MAJOR_REV_DS );
         break;
#endif
#if ECT_IA_MINOR_REV_ENABLE
      case ABP_ECT_IA_MINOR_REV:

         /*
         ** Copy the 4th Instance 1 attribute (Revision low word) to the message.
         */
         ABCC_SetMsgData16( psNewMessage, ECT_IA_REVISION_LW_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_MINOR_REV_DS );
         break;
#endif
#if ECT_IA_SERIAL_NUMBER_ENABLE
      case ABP_ECT_IA_SERIAL_NUMBER:

         /*
         ** Copy the 5th Instance 1 attribute (Serial number) to the message.
         */
         ABCC_SetMsgData32( psNewMessage, ECT_IA_SERIAL_NUMBER_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_SERIAL_NUMBER_DS );
         break;
#endif
#if ECT_IA_MANF_DEVICE_NAME_ENABLE
      case ABP_ECT_IA_MANF_DEV_NAME:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( ECT_IA_MANF_DEVICE_NAME_VALUE );

         /*
         ** Copy the 6th Instance 1 attribute (Manufacturer device name) to the
         ** message.
         */
         ABCC_SetMsgString( psNewMessage, ECT_IA_MANF_DEVICE_NAME_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;
      }
#endif
#if ECT_IA_MANF_HW_VERSION_ENABLE
      case ABP_ECT_IA_MANF_HW_VER:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( ECT_IA_MANF_HW_VERSION_VALUE );

         /*
         ** Copy the 7th Instance 1 attribute (Manufacturer hardware version) to
         ** the message.
         */
         ABCC_SetMsgString( psNewMessage, ECT_IA_MANF_HW_VERSION_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
      }
      break;
#endif
#if ECT_IA_MANF_SW_VERSION_ENABLE
      case ABP_ECT_IA_MANF_SW_VER:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( ECT_IA_MANF_SW_VERSION_VALUE );

         /*
         ** Copy the 8th Instance 1 attribute (Manufacturer software version) to
         ** the message.
         */
         ABCC_SetMsgString( psNewMessage, ECT_IA_MANF_SW_VERSION_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
      }
      break;
#endif
#if ECT_IA_ENUM_ADIS_ENABLE
      case ABP_ECT_IA_ENUM_ADIS:
      {
         GenerateEnumAdiList( psNewMessage );
         break;
      }
#endif
#if ECT_IA_DEVICE_TYPE_ENABLE
      case ABP_ECT_IA_DEVICE_TYPE:

         /*
         ** Copy the 10th Instance 1 attribute (Device type) to the message.
         */
         ABCC_SetMsgData32( psNewMessage, ECT_IA_DEVICE_TYPE_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_DEVICE_TYPE_DS );
         break;
#endif
#if ECT_IA_WR_PD_ASSY_INST_TRANS_ENABLE
      case ABP_ECT_IA_WR_PD_ASSY_INST_TRANS:
      {
         UINT16  iIndex;

         /*
         ** Copy the 11th Instance 1 attribute (Write PD assembly instance
         ** translation) to the message.
         */
         for( iIndex = 0; iIndex < ECT_IA_WR_PD_ASSY_INST_TRANS_SIZE; ++iIndex )
         {
            ABCC_SetMsgData16( psNewMessage,
                               ect_sInstance.aiWrPdAssyInstTrans[ iIndex ],
                               ( ABP_UINT16_SIZEOF * iIndex ) );
         }

         ABP_SetMsgResponse( psNewMessage, ( ECT_IA_WR_PD_ASSY_INST_TRANS_SIZE * ABP_UINT16_SIZEOF ) );
         break;
      }
#endif
#if ECT_IA_RD_PD_ASSY_INST_TRANS_ENABLE
      case ABP_ECT_IA_RD_PD_ASSY_INST_TRANS:
      {
         UINT16  iIndex;

         /*
         ** Copy the 12th Instance 1 attribute (Read PD assembly instance
         ** translation) to the message.
         */
         for( iIndex = 0; iIndex < ECT_IA_RD_PD_ASSY_INST_TRANS_SIZE; ++iIndex )
         {
            ABCC_SetMsgData16( psNewMessage,
                               ect_sInstance.aiRdPdAssyInstTrans[ iIndex ],
                               ( ABP_UINT16_SIZEOF * iIndex ) );
         }

         ABP_SetMsgResponse( psNewMessage, ( ECT_IA_RD_PD_ASSY_INST_TRANS_SIZE * ABP_UINT16_SIZEOF ) );
         break;
      }
#endif
#if ECT_IA_ADI_TRANS_ENABLE
      case ABP_ECT_IA_ADI_TRANS:
      {
         UINT16  iIndex;

         /*
         ** Copy the 13th Instance 1 attribute (ADI translation) to the message.
         */
         for( iIndex = 0; iIndex < ECT_IA_ADI_TRANS_SIZE; ++iIndex )
         {
            ABCC_SetMsgData16( psNewMessage,
                               ect_sInstance.asADITrans[ iIndex ].iADIInstanceNumber,
                               ( ( ABP_UINT16_SIZEOF + ABP_UINT16_SIZEOF ) * iIndex ) );
            ABCC_SetMsgData16( psNewMessage,
                               ect_sInstance.asADITrans[ iIndex ].iADIObjectIndex,
                               ( ( ABP_UINT16_SIZEOF + ABP_UINT16_SIZEOF ) * iIndex ) + ABP_UINT16_SIZEOF );
         }

         ABP_SetMsgResponse( psNewMessage, ( ECT_IA_ADI_TRANS_SIZE * ( ABP_UINT16_SIZEOF + ABP_UINT16_SIZEOF ) ) );
         break;
      }
#endif
#if ECT_IA_OBJ_SUB_TRANS_ENABLE
      case ABP_ECT_IA_OBJ_SUB_TRANS:
      {
         UINT16  iIndex;

         /*
         ** Copy the 13th Instance 1 attribute (ADI translation) to the message.
         */
         for( iIndex = 0; iIndex < ECT_IA_OBJ_SUB_TRANS_SIZE; ++iIndex )
         {
            ABCC_SetMsgData16( psNewMessage,
                               ect_sInstance.asObjSubTrans[ iIndex ].iADIInstanceNumber,
                               ( ( ABP_UINT16_SIZEOF + ABP_UINT16_SIZEOF + ABP_UINT8_SIZEOF ) * iIndex ) );
            ABCC_SetMsgData16( psNewMessage,
                               ect_sInstance.asObjSubTrans[ iIndex ].iADIObjectIndex,
                               ( ( ABP_UINT16_SIZEOF + ABP_UINT16_SIZEOF + ABP_UINT8_SIZEOF ) * iIndex ) + ABP_UINT16_SIZEOF );
            ABCC_SetMsgData8( psNewMessage,
                               ect_sInstance.asObjSubTrans[ iIndex ].bADIObjectIndexSubindex,
                               ( ( ABP_UINT16_SIZEOF + ABP_UINT16_SIZEOF + ABP_UINT8_SIZEOF ) * iIndex ) + ABP_UINT16_SIZEOF + ABP_UINT16_SIZEOF );
         }

         ABP_SetMsgResponse( psNewMessage, ( ECT_IA_OBJ_SUB_TRANS_SIZE * ( ABP_UINT16_SIZEOF + ABP_UINT16_SIZEOF + ABP_UINT8_SIZEOF ) ) );
         break;
      }
#endif
#if ECT_IA_ENABLE_FOE_ENABLE
      case ABP_ECT_IA_ENABLE_FOE:
      {
         /*
         ** The 'FoE enable' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData8( psNewMessage, ect_sInstance.fEnableFoE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_ENABLE_FOE_DS );
         break;
      }
#endif
#if ECT_IA_ENABLE_EOE_ENABLE
      case ABP_ECT_IA_ENABLE_EOE:
      {
         /*
         ** The 'EoE' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData8( psNewMessage, ect_sInstance.fEnableEoE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_ENABLE_EOE_DS );
         break;
      }
#endif
#if ECT_IA_SET_DEV_ID_AS_CSA_ENABLE
      case ABP_ECT_IA_SET_DEV_ID_AS_CSA:
      {
         /*
         ** The 'Set Device ID as Configured State Alias' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData8( psNewMessage, ect_sInstance.fSetDevIDAsCSA, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_SET_DEV_ID_AS_CSA_DS );
         break;
      }
#endif
#if ECT_IA_ETHERCAT_STATE_ENABLE
      case ABP_ECT_IA_ETHERCAT_STATE:
      {
         /*
         ** The 'EtherCAT state' attribute is requested.
         ** Attribute is 'Set only'. Return error code 'Attribute not gettable'.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_GETABLE );
         break;
      }
#endif
#if ECT_IA_STATE_TIMEOUTS_ENABLE
      case ABP_ECT_IA_STATE_TIMEOUTS:
      {
         /*
         ** The 'State Transitions Timeout' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData32( psNewMessage, ect_sInstance.alStateTransTimeouts[ 0 ], 0 );
         ABCC_SetMsgData32( psNewMessage, ect_sInstance.alStateTransTimeouts[ 1 ], 4 );
         ABCC_SetMsgData32( psNewMessage, ect_sInstance.alStateTransTimeouts[ 2 ], 8 );
         ABCC_SetMsgData32( psNewMessage, ect_sInstance.alStateTransTimeouts[ 3 ], 12 );
         ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_STATE_TIMEOUTS_DS );
         break;
      }
#endif
#if ECT_IA_COMP_IDENT_LISTS_ENABLE
      case ABP_ECT_IA_COMP_IDENT_LISTS:
      {
         /*
         ** The 'Compare Identity Lists' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData8( psNewMessage, ect_sInstance.fCompIDLists, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_COMP_IDENT_LISTS_DS );
         break;
      }
#endif
#if ECT_IA_FSOE_STATUS_IND_ENABLE
      case ABP_ECT_IA_FSOE_STATUS_IND:
      {
         /*
         ** The 'FsOE status indicator' attribute is requested.
         ** Attribute is 'Set only'. Return error code 'Attribute not gettable'.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_GETABLE );
         break;
      }
#endif
#if ECT_IA_CLEAR_IDENT_AL_STS_ENABLE
      case ABP_ECT_IA_CLEAR_IDENT_AL_STS:
      {
         /*
         ** The 'Clear IdentALSts' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData8( psNewMessage, ect_sInstance.fClearIdAlSts, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_CLEAR_IDENT_AL_STS_DS );
         break;
      }
#endif
#if ECT_IA_SII_ORDER_NUM_ENABLE
      case ABP_ECT_IA_SII_ORDER_NUM:
      {
         UINT16 iStrLength = (UINT16)strlen( ECT_IA_SII_ORDER_NUM_VALUE );

         if( iStrLength > ABP_ECT_IA_SII_ORDER_NUM_MAX_DS )
         {
            iStrLength = ABP_ECT_IA_SII_ORDER_NUM_MAX_DS;
         }

         ABCC_SetMsgString( psNewMessage, ECT_IA_SII_ORDER_NUM_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );

         break;
      }
#endif
#if ECT_IA_SII_DEV_NAME_ENABLE
      case ABP_ECT_IA_SII_DEV_NAME:
      {
         UINT16 iStrLength = (UINT16)strlen( ECT_IA_SII_DEV_NAME_VALUE );

         if( iStrLength > ABP_ECT_IA_SII_DEV_NAME_MAX_DS )
         {
            iStrLength = ABP_ECT_IA_SII_DEV_NAME_MAX_DS;
         }

         ABCC_SetMsgString( psNewMessage, ECT_IA_SII_DEV_NAME_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );

         break;
      }
#endif
#if ECT_IA_DEF_TXPDO_ASSIGN_ENABLE
      case ABP_ECT_IA_DEF_TXPDO_ASSIGN:
      {
         UINT16 iIndex;

         for( iIndex = 0; iIndex < ECT_IA_DEF_TXPDO_ASSIGN_SIZE; ++iIndex )
         {
            ABCC_SetMsgData16( psNewMessage,
                               ect_sInstance.aiTxPdoAssignments[ iIndex ],
                               ( ABP_UINT16_SIZEOF * iIndex ) );
         }

         ABP_SetMsgResponse( psNewMessage, ( ECT_IA_DEF_TXPDO_ASSIGN_SIZE * ABP_UINT16_SIZEOF ) );

         break;
      }
#endif
#if ECT_IA_DEF_RXPDO_ASSIGN_ENABLE
      case ABP_ECT_IA_DEF_RXPDO_ASSIGN:
      {
         UINT16 iIndex;

         for( iIndex = 0; iIndex < ECT_IA_DEF_RXPDO_ASSIGN_SIZE; ++iIndex )
         {
            ABCC_SetMsgData16( psNewMessage,
                               ect_sInstance.aiRxPdoAssignments[ iIndex ],
                               ( ABP_UINT16_SIZEOF * iIndex ) );
         }

         ABP_SetMsgResponse( psNewMessage, ( ECT_IA_DEF_RXPDO_ASSIGN_SIZE * ABP_UINT16_SIZEOF ) );

         break;
      }
#endif
      default:

         /*
         ** Unsupported attribute.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;
      }

      break;
   }
   case ABP_CMD_SET_ATTR:
   {
      switch( ABCC_GetMsgCmdExt0( psNewMessage ) )
      {
#if ECT_IA_ETHERCAT_STATE_ENABLE
      case ABP_ECT_IA_ETHERCAT_STATE:
      {
         /*
         ** Save the new state in the local variable.
         */
         ABCC_GetMsgData8( psNewMessage, &ect_bEtherCATState, 0 );
         ABP_SetMsgResponse( psNewMessage, 0 );
         break;
      }
#endif
#if ECT_IA_FSOE_STATUS_IND_ENABLE
      case ABP_ECT_IA_FSOE_STATUS_IND:
      {
         /*
         ** Save the new state of the FsOE status indicator.
         */
         ABCC_GetMsgData8( psNewMessage, &ect_eFSoEStatusIndicator, 0 );
         ABP_SetMsgResponse( psNewMessage, 0 );
         break;
      }
#endif
#if( ECT_IA_VENDOR_ID_ENABLE || ECT_IA_PRODUCT_CODE_ENABLE || ECT_IA_MAJOR_REV_ENABLE || ECT_IA_MINOR_REV_ENABLE || ECT_IA_SERIAL_NUMBER_ENABLE || ECT_IA_MANF_DEVICE_NAME_ENABLE || ECT_IA_MANF_HW_VERSION_ENABLE || ECT_IA_MANF_SW_VERSION_ENABLE || ECT_IA_ENUM_ADIS_ENABLE || ECT_IA_DEVICE_TYPE_ENABLE || ECT_IA_WR_PD_ASSY_INST_TRANS_ENABLE || ECT_IA_RD_PD_ASSY_INST_TRANS_ENABLE || ECT_IA_ADI_TRANS_ENABLE || ECT_IA_OBJ_SUB_TRANS_ENABLE || ECT_IA_ENABLE_FOE_ENABLE || ECT_IA_ENABLE_EOE_ENABLE || ECT_IA_SET_DEV_ID_AS_CSA_ENABLE || ECT_IA_STATE_TIMEOUTS_ENABLE || ECT_IA_COMP_IDENT_LISTS_ENABLE || ECT_IA_CLEAR_IDENT_AL_STS_ENABLE )
#if ECT_IA_VENDOR_ID_ENABLE
      case ABP_ECT_IA_VENDOR_ID:
#endif
#if ECT_IA_PRODUCT_CODE_ENABLE
      case ABP_ECT_IA_PRODUCT_CODE:
#endif
#if ECT_IA_MAJOR_REV_ENABLE
      case ABP_ECT_IA_MAJOR_REV:
#endif
#if ECT_IA_MINOR_REV_ENABLE
      case ABP_ECT_IA_MINOR_REV:
#endif
#if ECT_IA_SERIAL_NUMBER_ENABLE
      case ABP_ECT_IA_SERIAL_NUMBER:
#endif
#if ECT_IA_MANF_DEVICE_NAME_ENABLE
      case ABP_ECT_IA_MANF_DEV_NAME:
#endif
#if ECT_IA_MANF_HW_VERSION_ENABLE
      case ABP_ECT_IA_MANF_HW_VER:
#endif
#if ECT_IA_MANF_SW_VERSION_ENABLE
      case ABP_ECT_IA_MANF_SW_VER:
#endif
#if ECT_IA_ENUM_ADIS_ENABLE
      case ABP_ECT_IA_ENUM_ADIS:
#endif
#if ECT_IA_DEVICE_TYPE_ENABLE
      case ABP_ECT_IA_DEVICE_TYPE:
#endif
#if ECT_IA_WR_PD_ASSY_INST_TRANS_ENABLE
      case ABP_ECT_IA_WR_PD_ASSY_INST_TRANS:
#endif
#if ECT_IA_RD_PD_ASSY_INST_TRANS_ENABLE
      case ABP_ECT_IA_RD_PD_ASSY_INST_TRANS:
#endif
#if ECT_IA_ADI_TRANS_ENABLE
      case ABP_ECT_IA_ADI_TRANS:
#endif
#if ECT_IA_OBJ_SUB_TRANS_ENABLE
      case ABP_ECT_IA_OBJ_SUB_TRANS:
#endif
#if ECT_IA_ENABLE_FOE_ENABLE
      case ABP_ECT_IA_ENABLE_FOE:
#endif
#if ECT_IA_ENABLE_EOE_ENABLE
      case ABP_ECT_IA_ENABLE_EOE:
#endif
#if ECT_IA_SET_DEV_ID_AS_CSA_ENABLE
      case ABP_ECT_IA_SET_DEV_ID_AS_CSA:
#endif
#if ECT_IA_STATE_TIMEOUTS_ENABLE
      case ABP_ECT_IA_STATE_TIMEOUTS:
#endif
#if ECT_IA_COMP_IDENT_LISTS_ENABLE
      case ABP_ECT_IA_COMP_IDENT_LISTS:
#endif
#if ECT_IA_CLEAR_IDENT_AL_STS_ENABLE
      case ABP_ECT_IA_CLEAR_IDENT_AL_STS:
#endif
      {
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;
      }
#endif
      default:

         /*
         ** Unsupported attribute.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;
      }
      break;
   }
   default:

      /*
      ** Unsupported command.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;
   }
}

/*------------------------------------------------------------------------------
** Processes commands to the ECT Object (Instance 0)
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
static void ObjectCommand( ABP_MsgType* psNewMessage )
{
   /*
   ** This function processes commands to the EtherCAT Object
   ** (Instance 0).
   */
   switch( ABCC_GetMsgCmdBits( psNewMessage ) )
   {
   case ABP_CMD_GET_ATTR:

      switch( ABCC_GetMsgCmdExt0( psNewMessage ) )
      {
      case ABP_OA_NAME:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( ECT_OA_NAME_VALUE );

         /*
         ** Copy the attribute to a message.
         */
         ABCC_SetMsgString( psNewMessage, ECT_OA_NAME_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;
      }
      case ABP_OA_REV:

         /*
         ** Copy the attribute to a message.
         */

         ABCC_SetMsgData8( psNewMessage, ect_sObject.bRevision, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_UINT8_SIZEOF );
         break;

      case ABP_OA_NUM_INST:

         /*
         ** Copy the attribute to a message.
         */
         ABCC_SetMsgData16( psNewMessage, ect_sObject.iNumberOfInstances, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF );
         break;

      case ABP_OA_HIGHEST_INST:

         /*
         ** Copy the attribute to a message.
         */
         ABCC_SetMsgData16( psNewMessage, ect_sObject.iHighestInstanceNo, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF );
         break;

      default:

         /*
         ** Unsupported attribute.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;

      } /* End of switch( Attribute number ) */

      break;

#if ECT_OBJ_CMD_GET_OBJ_DESC_ENABLE
   case ABP_ECT_CMD_GET_OBJECT_DESC:
      {
         UINT16 iArrayIndex;
         UINT16 iRequestedObjIndex;


         iRequestedObjIndex = ABCC_GetMsgCmdExt( psNewMessage );

         for( iArrayIndex = 0; iArrayIndex < ECT_GET_OBJ_DESC_NUM_OBJ; iArrayIndex++ )
         {
            if( ect_sObject.asObjectDescriptions[ iArrayIndex ].iObjectIndex == iRequestedObjIndex )
            {
               ABCC_SetMsgData16( psNewMessage,
                                  ect_sObject.asObjectDescriptions[ iArrayIndex ].eEctDataType,
                                  0 );

               ABCC_SetMsgData8( psNewMessage,
                                 ect_sObject.asObjectDescriptions[ iArrayIndex ].eObjectCode,
                                 ABP_UINT16_SIZEOF );

               ABP_SetMsgResponse( psNewMessage, ( ABP_UINT16_SIZEOF + ABP_UINT8_SIZEOF ) );

               return;
            }
         }

         ABP_SetMsgErrorResponse( psNewMessage, ABP_UINT8_SIZEOF, ABP_ERR_DATA_NOT_AVAILABLE );
      }
      break;
#endif
   default:

      /*
      ** Unsupported command.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;

   } /* End of switch( Command number ) */
}

/*******************************************************************************
** Public Services
********************************************************************************
*/

void ECT_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
   /*
   ** This function processes commands to the EtherCAT Object and its Instance.
   */
   if( ABCC_GetMsgInstance( psNewMessage ) == ABP_INST_OBJ )
   {
      /*
      ** EtherCAT object Command.
      */
      ObjectCommand( psNewMessage );
   }
   else
   {
      /*
      ** EtherCAT instance Command.
      */
      InstanceCommand( psNewMessage );
   }

   ABCC_SendRespMsg( psNewMessage );
}

#if ECT_IA_ETHERCAT_STATE_ENABLE
ECT_StateType ECT_GetActualEctState( void )
{
   return( (ECT_StateType)ect_bEtherCATState );
}
#endif

#endif
