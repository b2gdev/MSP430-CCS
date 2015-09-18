/*
*********************************************************************************************************
*   CONFIGURATIONS
*   Source File Configurations
*
*   Copyright © 2010 Zone24x7 Inc. All rights reserved. 
*
*   No part of this publication may be reproduced, transmitted, transcribed, stored in a 
*   retrieval  system,  or  translated  into  any language, in any form or by any means, 
*   electronic, mechanical, photocopying, recording, or otherwise, without prior written 
*   permission  from  Zone24x7  Inc.  All  copyright, confidential information, patents, 
*   design rights and all other intellectual property rights of whatsoever nature in and 
*   to  any  source  code contained herein (including any header files and demonstration 
*   code  that may be included), are and shall remain the sole and exclusive property of 
*   Zone24x7  Inc.  The  information  furnished  herein  is  believed to be accurate and 
*   reliable.  However, no responsibility is assumed by Zone24x7 Inc for its use, or for 
*   any infringements of patents or other rights of third parties resulting from its use.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*   Source File Configurations
*
*   File Name     : cp_conf.h
*   Version       : V1.0.0
*   Programmer(s) : PS
*********************************************************************************************************
*   Note(s)
*   TCBIN OS Release Version - V5.0.0
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*   MODULE
*********************************************************************************************************
*/

#ifndef __CP_CONF_H__
#define __CP_CONF_H__


/*
*********************************************************************************************************
*   INCLUDE FILES
*********************************************************************************************************
*/
#include <msp430.h>
// #include "intrinsics.h"
#include "string.h"

#include "cp_types.h"
#include "cp_circ_buf.h"
#include "cp_power.h"
#include "cp_system.h"
#include "cp_clock.h"
#include "cp_adc12.h"
#include "cp_battery.h"
#include "cp_i2c.h"
#include "cp_bq24150a.h"
#include "cp_bq24160.h"
#include "cp_timer_a.h"
#include "cp_spi.h"
#include "cp_command.h"
#include "cp_br_display.h"
#include "cp_keypad.h"
#include "cp_timer_b.h"

/*
*********************************************************************************************************
*   EXTERNS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*   DEFAULT CONFIGURATION
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*   DEFINES
*********************************************************************************************************
*/
/*  PORT PINS */
#define P0         (1L << 0)
#define P1         (1L << 1)
#define P2         (1L << 2)
#define P3         (1L << 3)
#define P4         (1L << 4)
#define P5         (1L << 5)
#define P6         (1L << 6)
#define P7         (1L << 7)

/*  Firmware Version */
#define     FW_VERSION_MAJOR    0x03        /* V3.0                                               */
#define     FW_VERSION_MINOR    0x00

/*  Supported Features  */
#define     ENABLE_BATTERY
#define     ENABLE_KEYPAD
#define     ENABLE_DISPLAY
#define     ENABLE_HOST_INTERFACE
/*
*********************************************************************************************************
*   DATA TYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*   GLOBAL VARIABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*   MACROS
*********************************************************************************************************
*/
#define     BIT_CLR(SFR,BIT)		SFR &= (~BIT)
#define     BIT_SET(SFR,BIT)		SFR |= (BIT)
/*
*********************************************************************************************************
*   FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*   CONFIGURATION ERRORS
*********************************************************************************************************
*/




























/*
*********************************************************************************************************
*   MODULE END
*********************************************************************************************************
*/

#endif  /* End of __CP_CONF_H__ */
