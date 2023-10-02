/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Driver version 5.08.01 (2021-12-09)                                   **
**                                                                            **
** Delivered with:                                                            **
**    ABP            8.03.01 (2022-12-12)                                     **
**                                                                            */
/*******************************************************************************
********************************************************************************
** COPYRIGHT NOTIFICATION (c) 2013 HMS Industrial Networks AB                 **
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
** ABCC driver API used by the the application.
********************************************************************************
********************************************************************************
** Services:
**    ABCC_PORT_DebugPrint()          - Prints a string
**    ABCC_PORT_UseCritical()         - Used if any preparation is needed before
**                                      calling "ABCC_PORT_EnterCritical()".
**    ABCC_PORT_EnterCritical()       - Disables all interrupts, if they are not
**                                      already disabled.
**    ABCC_PORT_ExitCritical()        - Restore interrupts to the state they were
**                                      in when "ABCC_PORT_EnterCritical()" was
**                                      called.
**    ABCC_PORT_TIMER_UseCritical()   - Used if any preparation is needed before
**                                      calling "ABCC_PORT_TIMER_EnterCritical()".
**    ABCC_PORT_TIMER_EnterCritical() - Disables timer based interrupts, if they
**                                      are not already disabled.
**    ABCC_PORT_TIMER_ExitCritical()  - Restore interrupts to the state they were
**                                      in when "ABCC_PORT_TIMER_EnterCritical()"
**                                      was called.
**    ABCC_PORT_MemCopy()             - Copy a number of octets, from the source
**                                      pointer to the destination pointer.
**    ABCC_PORT_StrCpyToNative()      - Copy packed string to native string.
**    ABCC_PORT_StrCpyToPacked()      - Copy native string to packed string.
**    ABCC_PORT_CopyOctets()          - Copy octet aligned buffer.
**    ABCC_PORT_Copy8()               - Copy octet aligned 8 bit data.
**    ABCC_PORT_Copy16()              - Copy octet aligned 16 bit data.
**    ABCC_PORT_Copy32()              - Copy octet aligned 32 bit data.
**    ABCC_PORT_Copy64()              - Copy octet aligned 64 bit data.
**
********************************************************************************
********************************************************************************
*/

#include "abcc_sw_port.h"
#include "abcc_drv_cfg.h"

#ifndef ABCC_PORT_H_
#define ABCC_PORT_H_

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
** Macro is used by driver for debug prints such as events or error debug
** information. If not defined the driver will be silent.
** Note! The compiler need to be C99 compliant to support VA_ARGS in in macro.
**------------------------------------------------------------------------------
** Arguments:
**    args - Formatted string
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#ifndef ABCC_PORT_DebugPrint
#define ABCC_PORT_DebugPrint( args )
#endif

/*------------------------------------------------------------------------------
** Copy a number of octets, from the source pointer to the destination pointer.
** This function can be modified to use performance enhancing platform specific
** instructions. The default implementation is memcpy().
** Note that for a 16 bit char platform this function only supports an even
** number of octets.
**------------------------------------------------------------------------------
** Arguments:
**    pbDest         - Pointer to the destination.
**    pbSource       - Pointer to source data.
**    iNbrOfOctets   - The number of octets that shall be copied.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#ifndef ABCC_PORT_MemCpy
#error "ABCC_PORT_MemCpy() must be ported in abcc_sw_port.h"
#endif

/*------------------------------------------------------------------------------
** Critical section implementation guidance.
**
** Critical sections are used when there is a risk of resource conflicts or race
** conditions between ABCC interrupt handler context and the application thread.
**
** Six macros are used to implement the critical sections, three of these are
** specific to the timer functionality of the driver:
** ABCC_PORT_UseCritical()
** ABCC_PORT_EnterCritical()
** ABCC_PORT_ExitCritical()
** ABCC_PORT_TIMER_UseCritical()
** ABCC_PORT_TIMER_EnterCritical()
** ABCC_PORT_TIMER_ExitCritical()
**
** Depending on the configuration of the driver there are different requirements
** on the critical section implementation. Please choose the most suitable
** implementation from the numbered list below. The first statement that is true
** will choose the requirement.
**
** 1. The first three macros need to be implemented if any of the statements
**    below are true.
**       - Any message handling is done within interrupt context.
**
**       Requirements:
**       - The implementation must support that a critical section is entered
**       from interrupt context. ABCC_PORT_UseCritical() should be used for any
**       declarations needed in advance by ABCC_PORT_EnterCritical().
**       - When entering the critical section the required interrupts i.e.
**       any interrupt that may lead to driver access, must be disabled. When
**       leaving the critical section the interrupt configuration must be
**       restored to the previous state.
**
** 2. ABCC_PORT_EnterCritical() and ABCC_PORT_ExitCritical() need to be
**    implemented if any of the statements below are true.
**       - The application is accessing the ABCC driver message interface from
**       different processes or threads without protecting the message
**       interface on a higher level (semaphores or similar).
**
**       Requirement:
**         - When entering the critical section the required interrupts i.e. any
**         interrupt that may lead to driver access, must be disabled. When
**         leaving the critical section the interrupts must be enabled again.
**
** 3. If none of the above is true no implementation is required.
**
** If the application is calling ABCC_RunTimerSystem() from a timer interrupt,
** the last three timer-specific macros will be defined. If left undefined by
** the application in abcc_sw_port.h, these will assume the same definition as
** the corresponding three macros specified above:
**    Requirements:
**    - ABCC_PORT_TIMER_UseCritical() should be used for any declarations needed
**    in advance by ABCC_PORT_TIMER_EnterCritical().
**    - When entering the critical section the required interrupts i.e.
**    any interrupt that may lead to driver access, specifically the timer
**    system, must be disabled. When leaving the critical section the interrupt
**    configuration must be restored to the previous state.
**
**------------------------------------------------------------------------------
*/

/*------------------------------------------------------------------------------
** Please read the general description above about the critical sections
** implementation for implementation guidance.
**
** If any preparation is needed before calling "ABCC_PORT_EnterCritical()" or
** "ABCC_PORT_ExitCritical()" this macro is used to add HW specific necessities.
** This could for example be a local variable to store the current interrupt
** status.
**
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#ifndef ABCC_PORT_UseCritical
#define ABCC_PORT_UseCritical()
#endif

/*------------------------------------------------------------------------------
** Please read the general description above about the critical sections
** implementation for implementation guidance.
**
** If required the macro temporary disables interrupts
** to avoid conflict. Note that all interrupts that could lead to a driver
** access must be disabled.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#ifndef ABCC_PORT_EnterCritical
#define ABCC_PORT_EnterCritical()
#endif

/*------------------------------------------------------------------------------
** Please read the general description above about the critical sections
** implementation for implementation guidance.
**
** Restore interrupts to the state when "ABCC_PORT_EnterCritical()"
** was called.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#ifndef ABCC_PORT_ExitCritical
#define ABCC_PORT_ExitCritical()
#endif

/*------------------------------------------------------------------------------
** Please read the general description above about the critical sections
** implementation for implementation guidance.
**
** If any preparation is needed before calling "ABCC_PORT_TIMER_EnterCritical()"
** or "ABCC_PORT_TIMER_ExitCritical()" this macro is used to add HW specific
** necessities. This could for example be a local variable to store the current
** interrupt status.
**
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#ifndef ABCC_PORT_TIMER_UseCritical
#define ABCC_PORT_TIMER_UseCritical() ABCC_PORT_UseCritical()
#endif

/*------------------------------------------------------------------------------
** Please read the general description above about the critical sections
** implementation for implementation guidance.
**
** If required the macro temporarily disables interrupts
** to avoid conflict. Note that timer interrupts that could lead to a driver
** access must be disabled.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#ifndef ABCC_PORT_TIMER_EnterCritical
#define ABCC_PORT_TIMER_EnterCritical() ABCC_PORT_EnterCritical()
#endif

/*------------------------------------------------------------------------------
** Please read the general description above about the critical sections
** implementation for implementation guidance.
**
** Restore interrupts to the state when "ABCC_PORT_TIMER_EnterCritical()"
** was called.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#ifndef ABCC_PORT_TIMER_ExitCritical
#define ABCC_PORT_TIMER_ExitCritical() ABCC_PORT_ExitCritical()
#endif

/*------------------------------------------------------------------------------
** Copy a native formatted string to a packed string
**------------------------------------------------------------------------------
** Arguments:
**    pxDest            - Pointer to the destination.
**    iDestOctetOffset  - Octet offset to the destination where the copy will
**                        begin.
**    pxSrc             - Pointer to source data.
**    iNbrOfChars       - The number of bytes that shall be copied.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#ifndef ABCC_PORT_StrCpyToPacked
#error "ABCC_PORT_StrCpyToPacked() must be ported in abcc_sw_port.h"
#endif

/*------------------------------------------------------------------------------
** Copy a packed string to a native formatted string
**------------------------------------------------------------------------------
** Arguments:
**    pxDest            - Pointer to the destination.
**    pxSrc             - Pointer to source data.
**    iSrcOctetOffset   - Octet offset to the source where the copy will begin.
**    iNbrOfChars       - The number of bytes that shall be copied.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#ifndef ABCC_PORT_StrCpyToNative
#error "ABCC_PORT_StrCpyToNative() must be ported in abcc_sw_port.h"
#endif

/*------------------------------------------------------------------------------
** Copy a number of octets from a source to a destination.
** For a 16 bit char platform octet alignment support (the octet offset is odd)
** need to be considered when porting this macro.
**------------------------------------------------------------------------------
** Arguments:
**    pxDest            - Base pointer to the destination.
**    iDestOctetOffset  - Octet offset to the destination where the copy will
**                        begin.
**    pxSrc             - Base pointer to source data.
**    iSrcOctetOffset   - Octet offset to the source where the copy will begin.
**    iNumOctets        - Number of octets to copy.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#ifndef ABCC_PORT_CopyOctets
#error "ABCC_PORT_CopyOctets() must be ported in abcc_sw_port.h"
#endif

/*------------------------------------------------------------------------------
** Copy 8 bits from a source to a destination.
** For a 16 bit char platform octet alignment support (the octet offset is odd)
** need to be considered when porting this macro.
**------------------------------------------------------------------------------
** Arguments:
**    pxDest            - Base pointer to the destination.
**    iDestOctetOffset  - Octet offset to the destination where the copy will
**                        begin.
**    pxSrc             - Base pointer to source data.
**    iSrcOctetOffset   - Octet offset to the source where the copy will begin.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#ifndef ABCC_PORT_Copy8
#error "ABCC_PORT_Copy8() must be ported in abcc_sw_port.h"
#endif

/*------------------------------------------------------------------------------
** Copy 16 bits from a source to a destination.
** Octet alignment support (the octet offset is odd) need to be considered
** when porting this macro.
**------------------------------------------------------------------------------
** Arguments:
**    pxDest            - Base pointer to the destination.
**    iDestOctetOffset  - Octet offset to the destination where the copy will
**                        begin.
**    pxSrc             - Base pointer to source data.
**    iSrcOctetOffset   - Octet offset to the source where the copy will begin.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#ifndef ABCC_PORT_Copy16
#error "ABCC_PORT_Copy16() must be ported in abcc_sw_port.h"
#endif

/*------------------------------------------------------------------------------
** Copy 32 bits from a source to a destination.
** Octet alignment support (the octet offset is odd) need to be considered
** when porting this macro.
**------------------------------------------------------------------------------
** Arguments:
**    pxDest            - Base pointer to the destination.
**    iDestOctetOffset  - Octet offset to the destination where the copy will
**                        begin.
**    pxSrc             - Base pointer to source data.
**    iSrcOctetOffset   - Octet offset to the source where the copy will begin.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#ifndef ABCC_PORT_Copy32
#error "ABCC_PORT_Copy32() must be ported in abcc_sw_port.h"
#endif

/*------------------------------------------------------------------------------
** Copy 64 bits from a source to a destination.
** Octet alignment support (the octet offset is odd) need to be considered
** when porting this macro.
**------------------------------------------------------------------------------
** Arguments:
**    pxDest            - Base pointer to the destination.
**    iDestOctetOffset  - Octet offset to the destination where the copy will
**                        begin.
**    pxSrc             - Base pointer to source data.
**    iSrcOctetOffset   - Octet offset to the source where the copy will begin.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#if( ABCC_CFG_64BIT_ADI_SUPPORT || ABCC_CFG_DOUBLE_ADI_SUPPORT )
#ifndef ABCC_PORT_Copy64
#error "ABCC_PORT_Copy64() must be ported in abcc_sw_port.h"
#endif
#endif

#endif  /* inclusion lock */
