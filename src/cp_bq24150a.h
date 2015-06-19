/*
*********************************************************************************************************
*   BQ24150A
*   BQ24150A Fully Integrated Switch-Mode One-Cell Li-Ion Charger Functions
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
*   BQ24150A Fully Integrated Switch-Mode One-Cell Li-Ion Charger Functions
*
*   File Name     : cp_bq24150a.h
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

#ifndef __CP_BQ24150A_H__
#define __CP_BQ24150A_H__


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
/* BQ24150A I2C address */
#define     BQ24150A_I2C_ADDR                       0x6B            /* 7 bit I2C address            */

/* BQ24150A Register Addresses */
#define     BQ24150A_SAT_CNTRL_REG                  0x00
#define     BQ24150A_CNTRL_REG                      0x01
#define     BQ24150A_CNTRL_BAT_VOL_REG              0x02
#define     BQ24150A_VEN_PRT_REV_REG                0x03
#define     BQ24150A_BAT_TERM_F_CHRG_CUR_REG        0x04

/* Status/Control Register (Address - 0x00) Bits */
#define     BQ24150A_TMR_RST_OTG                    (0x01 << 7)
#define     BQ24150A_EN_STAT                        (0x01 << 6)

#define     BQ24150A_STAT_READY                     (0x00 << 4)
#define     BQ24150A_STAT_CHARGE_IN_PROGRESS        (0x01 << 4)
#define     BQ24150A_STAT_CHARGE_DONE               (0x02 << 4)
#define     BQ24150A_STAT_FAULT                     (0x03 << 4)

#define     BQ24150A_BOOST_MODE                     (0x01 << 3)

#define     BQ24150A_FAULT_NORMAL                   (0x00 << 0)
#define     BQ24150A_FAULT_VBUS_OVP                 (0x01 << 0)
#define     BQ24150A_FAULT_SLEEP                    (0x02 << 0)
#define     BQ24150A_FAULT_POOR_INPUT_SRC           (0x03 << 0)
#define     BQ24150A_FAULT_OUTPUT_OVP               (0x04 << 0)
#define     BQ24150A_FAULT_THERM_SHUTDOWN           (0x05 << 0)
#define     BQ24150A_FAULT_TMR_FAULT                (0x06 << 0)
#define     BQ24150A_FAULT_NO_BATTERY               (0x07 << 0)

//#define     BQ24150A_FAULT_NORMAL                   (0x00 << 0)
//#define     BQ24150A_FAULT_VBUS_OVP                 (0x01 << 0)
#define     BQ24150A_FAULT_OVER_LOAD                (0x02 << 0)
#define     BQ24150A_FAULT_BAT_VOL_LOW              (0x03 << 0)
#define     BQ24150A_FAULT_BAT_OVP                  (0x04 << 0)
//#define     BQ24150A_FAULT_THERM_SHUTDOWN           (0x05 << 0)
//#define     BQ24150A_FAULT_TMR_FAULT                (0x06 << 0)
/*          Value 111 is N/A                                 */

/* Control Register (Address - 0x01) Bits */
#define     BQ24150A_I_IN_LIMIT_100MA               (0x00 << 6)
#define     BQ24150A_I_IN_LIMIT_500MA               (0x01 << 6)
#define     BQ24150A_I_IN_LIMIT_800MA               (0x02 << 6)
#define     BQ24150A_I_IN_LIMIT_NO                  (0x03 << 6)

#define     BQ24150A_V_LOW_3V4                      (0x00 << 4)
#define     BQ24150A_V_LOW_3V5                      (0x01 << 4)
#define     BQ24150A_V_LOW_3V6                      (0x02 << 4)
#define     BQ24150A_V_LOW_3V7                      (0x03 << 4)

#define     BQ24150A_TE                             (0x01 << 3)
#define     BQ24150A_nCE                            (0x01 << 2)
#define     BQ24150A_HZ_MODE                        (0x01 << 1)
#define     BQ24150A_OPA_MODE                       (0x01 << 0)

/* Control/Battery Voltage Register (Address - 0x02) Bits */
#define     BQ24150A_VO_REG_5                       (0x01 << 7)
#define     BQ24150A_VO_REG_4                       (0x01 << 6)
#define     BQ24150A_VO_REG_3                       (0x01 << 5)
#define     BQ24150A_VO_REG_2                       (0x01 << 4)
#define     BQ24150A_VO_REG_1                       (0x01 << 3)
#define     BQ24150A_VO_REG_0                       (0x01 << 2)
#define     BQ24150A_OTG_PL                         (0x01 << 1)
#define     BQ24150A_OTG_EN                         (0x01 << 0)

/* Battery Termination/Fast Charge Current Register (Address - 0x04) Bits */
#define     BQ24150A_RESET                          (0x01 << 7)
#define     BQ24150A_VI_CHRG_2                      (0x01 << 6)
#define     BQ24150A_VI_CHRG_1                      (0x01 << 5)
#define     BQ24150A_VI_CHRG_0                      (0x01 << 4)
/*          Bit 3 is N/A                                     */
#define     BQ24150A_VI_TERM_2                      (0x01 << 2)
#define     BQ24150A_VI_TERM_1                      (0x01 << 1)
#define     BQ24150A_VI_TERM_0                      (0x01 << 0)


/*{KW}: charger registers default values */
#define VOLT_REG_DEFAULT  0x0A
#define CTRL_REG_DEFAULT  0x30
#define CURR_REG_DEFAULT  0x89
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

/*
*********************************************************************************************************
*   FUNCTION PROTOTYPES
*********************************************************************************************************
*/
INT08U  BQ24150A_GetStatus (void);
BOOLEAN  BQ24150A_ChargerDeInit (void);
BOOLEAN  BQ24150A_ChargerInit (INT08U i_in_limit);
BOOLEAN  BQ24150A_ChargerEnable ();
BOOLEAN  BQ24150A_ChargerDisable ();
INT08U BQ24150A_GetVoltageReg(void); //test
INT08U  BQ24150A_GetCtrlReg (void) ;//test
INT08U  BQ24150A_GetCurrentReg (void) ;//test
BOOLEAN BQ24150A_is_default_params_det(void);//test

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
