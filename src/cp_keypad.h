/*
*********************************************************************************************************
*   KEYPAD
*   KEYPAD Functions
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
*   KEYPAD Functions
*
*   File Name     : cp_keypad.h
*   Version       : V1.0.0
*   Programmer(s) : PS
*********************************************************************************************************
*   Note(s)
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*   MODULE
*********************************************************************************************************
*/

#ifndef __CP_KEYPAD_H__
#define __CP_KEYPAD_H__


/*
*********************************************************************************************************
*   INCLUDE FILES
*********************************************************************************************************
*/
#include "cp_conf.h"

/*
*********************************************************************************************************
*   EXTERNS
*********************************************************************************************************
*/
#ifdef KEYPAD_MODULE
    #define CP_KEYPAD_EXT
#else
    #define CP_KEYPAD_EXT extern
#endif

/*{KW}:: */
extern INT32U time_stamp_start;
/*{KW}:: end */
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
#define PWR_OFF_KEY_COMBINATION_DEFINED
#define PWR_RESET_KEY_COMBINATION_DEFINED
#define RECOVERY_MODE_ENTERING_DEFINED

#define PWR_SW          (P2IN & P1)             /* P2.1         */

#define BR_KEY1         (P1IN & P0)				/* P1.0         */
#define BR_KEY2         (P1IN & P1)             /* P1.1         */
#define BR_KEY3         (P1IN & P2)             /* P1.2         */
#define BR_KEY4         (P1IN & P3)             /* P1.3         */
#define BR_KEY5         (P1IN & P4)             /* P1.4         */
#define BR_KEY6         (P1IN & P5)             /* P1.5         */
#define BR_KEY7         (P1IN & P6)             /* P1.6         */
#define BR_KEY8         (P1IN & P7)             /* P1.7         */
#define BR_KEY9         (P2IN & P0)            	/* P2.0         */  /* Space Bar */

#define KEYMAT_COL0     (P2IN & P3)             /* P2.3         */
#define KEYMAT_COL1     (P2IN & P4)             /* P2.4         */
#define KEYMAT_COL2     (P2IN & P5)             /* P2.5         */

#define KEYMAT_ROWS     P4OUT	            	/* Keymat rows connected to PORT4         */
#define KEYMAT_ROW0     P0            			/* Pin0         */
#define KEYMAT_ROW1     P1            			/* Pin1         */
#define KEYMAT_ROW2     P2            			/* Pin2         */

/* Key Bit Positions */

#define BR_KEY1_BIT         (1L << 0)
#define BR_KEY2_BIT         (1L << 1)
#define BR_KEY3_BIT         (1L << 2)
#define BR_KEY4_BIT         (1L << 3)
#define BR_KEY5_BIT         (1L << 4)
#define BR_KEY6_BIT         (1L << 5)
#define BR_KEY7_BIT         (1L << 6)
#define BR_KEY8_BIT         (1L << 7)

#define BR_KEY9_BIT         (1L << 8)
#define NAV_LEFT_KEY_BIT    (1L << 9)
#define NAV_RIGHT_KEY_BIT   (1L << 10)
#define NAV_UP_KEY_BIT      (1L << 11)
#define NAV_DOWN_KEY_BIT    (1L << 12)
#define NAV_OK_KEY_BIT      (1L << 13)
#define BWD_KEY_BIT         (1L << 14)
#define FWD_KEY_BIT         (1L << 15)

#define VOL_UP_KEY_BIT      (1L << 16)
#define VOL_DOWN_KEY_BIT    (1L << 17)
#define PWR_KEY_BIT         (1L << 18)

#define KEY_SAMPLE_COUNT    1
#define KEY_VALID_THRESHOLD (KEY_SAMPLE_COUNT * 0.75)

/*{KW}: Test shutdown key combinaiton: to be decided */
#define PWR_SHUTDOWN_COMB   (BWD_KEY_BIT | VOL_DOWN_KEY_BIT)
#define PWR_RESET_COMB      (FWD_KEY_BIT | VOL_DOWN_KEY_BIT)

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
CP_KEYPAD_EXT INT32U KEYPAD_MainKeys;
CP_KEYPAD_EXT BOOLEAN KEYPAD_KeyChange;
CP_KEYPAD_EXT INT32U KEYPAD_MainKeys_Buf[KEY_SAMPLE_COUNT];
CP_KEYPAD_EXT INT08U KEYPAD_SampleIndex;

/*
*********************************************************************************************************
*   MACROS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*   FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void  Kpd_Init (void);
void  Kpd_DeInit (void);
void  Kpd_Read (void);
void  Kpd_ReadEx (void);
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

#endif  /* End of __MODULE_H__ */
