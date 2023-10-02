/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Starter Kit version 285f43c (2023-07-06)                              **
**                                                                            **
** Delivered with:                                                            **
**    ABP            8.03.01 (2022-12-12)                                     **
**    ABCC Driver    5.08.01 (2021-12-09)                                     **
**                                                                            */
========================================================================
    CONSOLE APPLICATION : Raspberry Pi Project Overview
========================================================================

External dependencies:

- This project depends on the bcm2835 library for Raspberry Pi, which
  can be downloaded from here: http://www.airspayce.com/mikem/bcm2835/

- This library must be built and installed for this project to run.

Building:

- It is assumed that GCC and GNU Make is available.

- The Makefile in the project directory defines two targets, 'release'
  and 'debug'.

- If the GPIO library refered to above is available "make release" in
  the project directory should be enough to get a working binary.

Plaform-specific requirements:

- The compiled binary must be started with 'root' priviliges, or
  equivalent, if SPI is to be used. The GPIO library works directly on
  the memory-mapped registers for the SPI hardware, and those are not
  accessible to a normal user.

- The UART port in the GPIO connector is normally set up as the system
  console, this has to be changed via the "Raspberry Pi Configuration"
  tool if the UART is to be used for the ABCC.