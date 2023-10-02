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
** BACnet-specific ADI example.
**
** If the BACnet Advanced Mapping should be enabled the following definitions
** in "abcc_obj_cfg.h" should be set:
** BAC_OBJ_ENABLE                            TRUE
** BAC_IA_SUPPORT_ADV_MAPPING_ENABLE         TRUE
** BAC_IA_SUPPORT_ADV_MAPPING_VALUE          TRUE
**
** In "abcc_drv_cfg.h" make sure that the following definitions are set:
** ABCC_CFG_STRUCT_DATA_TYPE                 FALSE
** ABCC_CFG_ADI_GET_SET_CALLBACK             FALSE
** AD_IA_MIN_MAX_DEFAULT_ENABLE              TRUE
********************************************************************************
********************************************************************************
*/

#include "abcc_obj_cfg.h"
#include "appl_adi_config.h"
#include "bac.h"

#if ( APPL_ACTIVE_ADI_SETUP == APPL_ADI_SETUP_BACNET )

#if( ABCC_CFG_STRUCT_DATA_TYPE || ABCC_CFG_ADI_GET_SET_CALLBACK  )
#error ABCC_CFG_STRUCT_DATA_TYPE must be set to FALSE in order to run this example
#error ABCC_CFG_ADI_GET_SET_CALLBACK must be set to FALSE in order to run this example
#endif

/*
** In order for enum values to be properly set up this define must be set to TRUE
*/
#if( !AD_IA_MIN_MAX_DEFAULT_ENABLE  )
#error AD_IA_MIN_MAX_DEFAULT_ENABLE must be set to TRUE in order to run this example
#endif

/*******************************************************************************
** Constants
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Access descriptor for the ADIs
**------------------------------------------------------------------------------
*/
#define APPL_READ_MAP_READ_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS |           \
                                         ABP_APPD_DESCR_MAPPABLE_READ_PD )

#define APPL_READ_MAP_WRITE_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS |          \
                                          ABP_APPD_DESCR_SET_ACCESS |          \
                                          ABP_APPD_DESCR_MAPPABLE_READ_PD )

#define APPL_WRITE_MAP_READ_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS |          \
                                          ABP_APPD_DESCR_MAPPABLE_WRITE_PD )

#define APPL_NOT_MAP_READ_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS )

#define APPL_NOT_MAP_WRITE_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS |           \
                                         ABP_APPD_DESCR_SET_ACCESS )

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Properties for the ENUM ADIs.
**------------------------------------------------------------------------------
*/
/*------------------------------------------------------------------------------
** Enum strings.
**
** NOTE:
** In order to be able to retrieve the enumstrings via BACnet
** AD_IA_MIN_MAX_DEFAULT_ENABLE must be set to TRUE.
**
** See the comments in "abcc_obj_cfg" for more information.
**------------------------------------------------------------------------------
*/
static AD_ENUMStrType asEnumStrings[ 3 ] = { { 0, "No" }, { 1, "Maybe" }, { 2, "Yes" } };
static AD_ENUMType sEnumProps = { { 0, 2, 0 }, 3, asEnumStrings };

/*------------------------------------------------------------------------------
** Storage space for the ADI values.
**------------------------------------------------------------------------------
*/
static FLOAT32    lFloatSum = 0.0f;
static FLOAT32    lFloatA = 0.0f;
static FLOAT32    lFloatB = 0.0f;

static BOOL       fBoolAnd = 0;
static BOOL       fBoolOr = 0;
static BOOL       fBoolA = 0;
static BOOL       fBoolB = 0;

static UINT8      bEnumProd = 0;
static UINT8      bEnumCons = 0;

static UINT16     iDontCare = 0;

#if BAC_IA_SUPPORT_ADV_MAPPING_ENABLE && BAC_IA_SUPPORT_ADV_MAPPING_VALUE
/*
** This table defines the BACnet object type values and instance number values
** for the available ADIs. If a certain ADI should not have a corresponding
** BACnet object instance use the BAC_OBJ_TYPE_IGNORE in this list.
**
** NOTE:
** This list must have the same number of elements as the "APPL_asAdiEntryList"
** and its contents must match the ADI list in a 1:1 fashion. I.e. index 'X'
** in both lists must refer to data for the same ADI.
**
*/
const BAC_ObjectListType BAC_asObjectList[] =
{
   { BAC_OBJ_TYPE_AN_VAL,  10 }, /* ADI 1, analog_sum_a_b  */
   { BAC_OBJ_TYPE_AN_VAL,  11 }, /* ADI 2, analog_a        */
   { BAC_OBJ_TYPE_AN_VAL,  12 }, /* ADI 3, analog_b        */
   { BAC_OBJ_TYPE_BIN_VAL, 20 }, /* ADI 4, binary_a_and_b  */
   { BAC_OBJ_TYPE_BIN_VAL, 21 }, /* ADI 5, binary_a_or_b   */
   { BAC_OBJ_TYPE_BIN_VAL, 22 }, /* ADI 6, binary_a        */
   { BAC_OBJ_TYPE_BIN_VAL, 23 }, /* ADI 7, binary_b        */
   { BAC_OBJ_TYPE_MS_VAL,  30 }, /* ADI 8, multistate_prod */
   { BAC_OBJ_TYPE_MS_VAL,  31 }, /* ADI 9, multistate_cons */
   { BAC_OBJ_TYPE_IGNORE      }  /* ADI 10, dont_care      */
};
#endif /* BAC_IA_SUPPORT_ADV_MAPPING_ENABLE && BAC_IA_SUPPORT_ADV_MAPPING_VALUE */

/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*------------------------------------------------------------------------------
** ADI list for this example.
**
** NOTE:
** If Advanced Mapping (BAC_IA_SUPPORT_ADV_MAPPING_ENABLE) is enabled the ADI
** list must have the same number of elemens as the "BAC_asObjectList"
** and its contents must match that list in a 1:1 fashion. I.e. index 'X' in
** both lists must refer to data for the same ADI.
**
** See the comments in "bac.h" for more information.
**------------------------------------------------------------------------------
*/
/*----------------------------------------------------------------------------------------
** iInstance | pabName | bDataType | bNumOfElements | bDesc | pxValuePtr | pxValuePropPtr|
**----------------------------------------------------------------------------------------
*/
const AD_AdiEntryType APPL_asAdiEntryList[] =
{
   {  1, "analog_sum_a_b",  ABP_FLOAT,  1, APPL_WRITE_MAP_READ_ACCESS_DESC, { &lFloatSum, NULL        } },
   {  2, "analog_a",        ABP_FLOAT,  1, APPL_NOT_MAP_WRITE_ACCESS_DESC,  { &lFloatA,   NULL        } },
   {  3, "analog_b",        ABP_FLOAT,  1, APPL_NOT_MAP_WRITE_ACCESS_DESC,  { &lFloatB,   NULL        } },

   {  4, "binary_a_and_b",  ABP_BOOL,   1, APPL_WRITE_MAP_READ_ACCESS_DESC, { &fBoolAnd,  NULL        } },
   {  5, "binary_a_or_b",   ABP_BOOL,   1, APPL_WRITE_MAP_READ_ACCESS_DESC, { &fBoolOr,   NULL        } },
   {  6, "binary_a",        ABP_BOOL,   1, APPL_NOT_MAP_WRITE_ACCESS_DESC,  { &fBoolA,    NULL        } },
   {  7, "binary_b",        ABP_BOOL,   1, APPL_NOT_MAP_WRITE_ACCESS_DESC,  { &fBoolB,    NULL        } },

   {  8, "multistate_prod", ABP_ENUM,   1, APPL_WRITE_MAP_READ_ACCESS_DESC, { &bEnumProd, &sEnumProps } },
   {  9, "multistate_cons", ABP_ENUM,   1, APPL_NOT_MAP_WRITE_ACCESS_DESC,  { &bEnumCons, &sEnumProps } },

   { 10, "dont_care",       ABP_UINT16, 1, APPL_NOT_MAP_WRITE_ACCESS_DESC,  { &iDontCare, NULL        } }
};

/*------------------------------------------------------------------------------
** Process Data map for this example.
**
** An ADI must be mapped as PD_WRITE For COV notification to be possible, in
** this example all 'producing' ADIs are mapped.
**------------------------------------------------------------------------------
*/
/*-------------------------------------------------------------------
** 1. AD instance | 2. Direction | 3. Num elements | 4. Start index |
**-------------------------------------------------------------------
*/
const AD_MapType APPL_asAdObjDefaultMap[] =
{
   { 0x0001,  PD_WRITE,  AD_MAP_ALL_ELEM, 0 },
   { 0x0004,  PD_WRITE,  AD_MAP_ALL_ELEM, 0 },
   { 0x0005,  PD_WRITE,  AD_MAP_ALL_ELEM, 0 },
   { 0x0008,  PD_WRITE,  AD_MAP_ALL_ELEM, 0 },
   { AD_MAP_END_ENTRY }
};

/*******************************************************************************
** Private Services
********************************************************************************
*/

/*******************************************************************************
** Public Services
********************************************************************************
*/

UINT16 APPL_GetNumAdi( void )
{
   return( sizeof( APPL_asAdiEntryList ) / sizeof( AD_AdiEntryType ) );
}

void APPL_CyclicalProcessing( void )
{
   /*
   ** This function is called when read and write data have been updated.
   */

   lFloatSum = lFloatA + lFloatB;

   fBoolAnd = fBoolA && fBoolB;
   fBoolOr = fBoolA || fBoolB;

   bEnumProd = bEnumCons;

}

/*******************************************************************************
** Tasks
********************************************************************************
*/
#endif

