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
** Platform dependent macros and functions required by the ABCC driver and
** Anybus objects implementation to be platform independent.
********************************************************************************
********************************************************************************
*/

#include "abcc_sw_port.h"
#include "abcc.h"

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
** Private Globals
********************************************************************************
*/

/*******************************************************************************
** Private Services
********************************************************************************
*/

/*******************************************************************************
** Public Services
********************************************************************************
*/

#ifdef ABCC_SYS_16_BIT_CHAR
void ABCC_PORT_CopyOctetsImpl( void* pxDest, UINT16 iDestOctetOffset,
                               const void* pxSrc, UINT16 iSrcOctetOffset,
                               UINT16 iNumOctets )
{
   UINT16 i;
   UINT16 iData;
   BOOL fOddDestOctet;
   BOOL fOddSrcOctet;
   UINT16* piDest;
   UINT16* piSrc;

   fOddDestOctet = iDestOctetOffset & 1;
   fOddSrcOctet =  iSrcOctetOffset & 1;
   piDest =   (UINT16*)pxDest + ( iDestOctetOffset >> 1 );
   piSrc =    (UINT16*)pxSrc + ( iSrcOctetOffset >> 1 );

   for( i = 0; i < iNumOctets; i++ )
   {
      if( fOddSrcOctet )
      {
         iData = ABCC_GetHighAddrOct( *piSrc );
         piSrc++;
      }
      else
      {
         iData = ABCC_GetLowAddrOct( *piSrc );
      }
      fOddSrcOctet ^= 1;

      if( fOddDestOctet )
      {
         ABCC_SetHighAddrOct( *piDest, iData );
         piDest++;
      }
      else
      {
         ABCC_SetLowAddrOct( *piDest, iData );
      }
      fOddDestOctet ^= 1;
   }
}

void ABCC_PORT_StrCpyToNativeImpl( void* pxDest, const void* pxSrc,
                                   UINT16 iSrcOctetOffset, UINT16 iNbrOfChars )
{
   UINT16*        piDest;
   const UINT16*  piSrc;
   BOOL           fOddSrc;

   piDest = pxDest;
   piSrc = (UINT16*)pxSrc + ( iSrcOctetOffset >> 1 );
   fOddSrc = ( iSrcOctetOffset & 1 ) == 1;

   while( iNbrOfChars > 0 )
   {
      if( fOddSrc )
      {
         *piDest = ABCC_GetHighAddrOct( *piSrc );
         piSrc++;
      }
      else
      {
         *piDest = ABCC_GetLowAddrOct( *piSrc );
      }
      piDest++;
      fOddSrc = !fOddSrc;
      iNbrOfChars--;
   }
}

void ABCC_PORT_StrCpyToPackedImpl( void* pxDest, UINT16 iDestOctetOffset,
                                   const void* pxSrc, UINT16 iNbrOfChars )
{
   UINT16*        piDest;
   const UINT16*  piSrc;
   BOOL           fOddDest;

   piDest = (UINT16*)pxDest + ( iDestOctetOffset >> 1 );
   piSrc = pxSrc;
   fOddDest = ( iDestOctetOffset & 1 ) == 1;

   while( iNbrOfChars > 0 )
   {
      if( fOddDest )
      {
         ABCC_SetHighAddrOct( *piDest, *piSrc );
         piDest++;
      }
      else
      {
         ABCC_SetLowAddrOct( *piDest, *piSrc );
      }
      piSrc++;
      fOddDest = !fOddDest;
      iNbrOfChars--;
   }
}
#else
#if ABCC_CFG_PAR_EXT_BUS_ENDIAN_DIFF
void * ABCC_PORT_CopyImpl( void *pbDest, const void *pbSource, int iNbrOfOctets )
{
   UINT8 *pbDst;
   const UINT8 *pbSrc;

   pbDst = pbDest;
   pbSrc = pbSource;

   for ( ; iNbrOfOctets > 0; --iNbrOfOctets )
   {
      *pbDst++ = *pbSrc++;
   }

   return( pbDest );
}
#endif
#endif

