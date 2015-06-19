/*
*********************************************************************************************************
*   BQ24160
*   BQ24160 Fully Integrated Switch-Mode One-Cell Li-Ion Charger Functions
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
*   BQ24160 Fully Integrated Switch-Mode One-Cell Li-Ion Charger Functions
*
*   File Name     : cp_BQ24160.h
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

#ifndef __CP_BQ24160_H__
#define __CP_BQ24160_H__


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
/* BQ24160 I2C address */
#define     BQ24160_I2C_ADDR                       0x6B            /* 7 bit I2C address            */

/* BQ24160 Register Addresses */
#define     BQ24160_SAT_CNTRL_REG                  0x00
#define     BQ24160_BAT_SAT_REG               	   0x01
#define     BQ24160_CNTRL_REG                      0x02
#define     BQ24160_CNTRL_BAT_VOL_REG              0x03
#define     BQ24160_VEN_PRT_REV_REG                0x04
#define     BQ24160_BAT_TERM_F_CHRG_CUR_REG        0x05
#define     BQ24160_VOL_DPPM_STAT_REG              0x06
#define     BQ24160_TMR_NTC_MNTR_REG       		   0x07

/* Status/Control Register (Address - 0x00) Bits */
#define     BQ24160_TMR_RST	                       (0x01 << 7)

#define     BQ24160_STAT_NO_SOURCE                 (0x00 << 4)
#define     BQ24160_STAT_IN_READY		           (0x01 << 4)
#define     BQ24160_STAT_USB_READY	               (0x02 << 4)
#define     BQ24160_STAT_CHARGE_FROM_IN	           (0x03 << 4)
#define     BQ24160_STAT_CHARGE_FROM_USB           (0x04 << 4)
#define     BQ24160_STAT_CHARGE_DONE               (0x05 << 4)
/*          Value (0x06 << 4) is N/A                        */
#define     BQ24160_STAT_FAULT                     (0x07 << 4)

#define     BQ24160_SUPPLY_SEL                     (0x01 << 3)

#define     BQ24160_FAULT_NORMAL                   (0x00 << 0)
#define     BQ24160_FAULT_THERMAL_SHUT             (0x01 << 0)
#define     BQ24160_FAULT_BAT_TEMP	               (0x02 << 0)
#define     BQ24160_FAULT_WATCHDOG_EXP	           (0x03 << 0)
#define     BQ24160_FAULT_SFTY_TMR_EXP             (0x04 << 0)
#define     BQ24160_FAULT_IN_SUPPLY                (0x05 << 0)
#define     BQ24160_FAULT_USB_SUPPLY               (0x06 << 0)
#define     BQ24160_FAULT_BATTERY	               (0x07 << 0)

/* Battery/ Supply Status Register (Address - 0x01) Bits */
#define     BQ24160_IN_NORMAL                      (0x00 << 6)
#define     BQ24160_IN_OVP                 		   (0x01 << 6)
#define     BQ24160_IN_WEAK                		   (0x02 << 6)
#define     BQ24160_IN_VOL_LOW              	   (0x03 << 6)

#define     BQ24160_USB_NORMAL                     (0x00 << 4)
#define     BQ24160_USB_OVP                 	   (0x01 << 4)
#define     BQ24160_USB_WEAK                	   (0x02 << 4)
#define     BQ24160_USB_VOL_LOW              	   (0x03 << 4)

#define     BQ24160_OTG_LOCK	                   (0x01 << 3)

#define     BQ24160_BAT_NORMAL                     (0x00 << 1)
#define     BQ24160_BAT_OVP                 	   (0x01 << 1)
#define     BQ24160_BAT_NOT_PRESENT            	   (0x02 << 1)
/*          Value (0x03 << 1) is N/A                        */

#define     BQ24160_EN_NOBATOP			           (0x01 << 0)

/* Control Register (Address - 0x02) Bits */
#define     BQ24160_RESET			               (0x01 << 7)

#define     BQ24160_IUSB2_LIMIT_100MA              (0x00 << 4)
#define     BQ24160_IUSB3_LIMIT_150MA              (0x01 << 4)
#define     BQ24160_IUSB2_LIMIT_500MA              (0x02 << 4)
#define     BQ24160_IUSB_LIMIT_800MA               (0x03 << 4)
#define     BQ24160_IUSB3_LIMIT_900MA              (0x04 << 4)
#define     BQ24160_IUSB_LIMIT_1500MA              (0x05 << 4)
#define     BQ24160_IUSB_LIMIT_NA                  (0x06 << 4)
/*          Value (0x07 << 4) is also same                  */

#define     BQ24160_EN_STAT                        (0x01 << 3)
#define     BQ24160_TE                             (0x01 << 2)
#define     BQ24160_nCE                            (0x01 << 1)
#define     BQ24160_HZ_MODE                        (0x01 << 0)

/* Control/Battery Voltage Register (Address - 0x03) Bits */
#define     BQ24160_V_BREG_5                       (0x01 << 7)
#define     BQ24160_V_BREG_4                       (0x01 << 6)
#define     BQ24160_V_BREG_3                       (0x01 << 5)
#define     BQ24160_V_BREG_2                       (0x01 << 4)
#define     BQ24160_V_BREG_1                       (0x01 << 3)
#define     BQ24160_V_BREG_0                       (0x01 << 2)

#define     BQ24160_I_IN_LIMIT                     (0x01 << 1)
#define     BQ24160_DATA_EN                        (0x01 << 0)

/* Battery Termination/Fast Charge Current Register (Address - 0x05) Bits */
#define     BQ24160_I_CHRG_4                       (0x01 << 7)
#define     BQ24160_I_CHRG_3                       (0x01 << 6)
#define     BQ24160_I_CHRG_2                       (0x01 << 5)
#define     BQ24160_I_CHRG_1                       (0x01 << 4)
#define     BQ24160_I_CHRG_0                       (0x01 << 3)

#define     BQ24160_I_TERM_2                       (0x01 << 2)
#define     BQ24160_I_TERM_1                       (0x01 << 1)
#define     BQ24160_I_TERM_0                       (0x01 << 0)

/* VIN-DPM Voltage / DPPM Status Register (Address - 0x06) Bits */
#define     BQ24160_MINSYS_STATUS                  (0x01 << 7)
#define     BQ24160_DPM_STATUS                     (0x01 << 6)

#define     BQ24160_VIN_USB_DPM_2                  (0x01 << 5)
#define     BQ24160_VIN_USB_DPM_1                  (0x01 << 4)
#define     BQ24160_VIN_USB_DPM_0                  (0x01 << 3)

#define     BQ24160_VIN_IN_DPM_2                   (0x01 << 2)
#define     BQ24160_VIN_IN_DPM_1                   (0x01 << 1)
#define     BQ24160_VIN_IN_DPM_0                   (0x01 << 0)

/* Safety Timer/ NTC Monitor Register (Address - 0x07) Bits */
#define     BQ24160_2XTMR_EN                       (0x01 << 7)

#define     BQ24160_SFT_TMR_27MIN                  (0x00 << 5)
#define     BQ24160_SFT_TMR_6HUR                   (0x01 << 5)
#define     BQ24160_SFT_TMR_9HUR                   (0x02 << 5)
#define     BQ24160_SFT_TMR_DISABLE                (0x03 << 5)
/*                     B4 is N/A                            */
#define     BQ24160_TS_EN                          (0x01 << 3)

#define     BQ24160_TS_FAULT_NO                    (0x00 << 1)
#define     BQ24160_TS_FAULT_01                    (0x01 << 1)
#define     BQ24160_TS_FAULT_10                    (0x10 << 1)
#define     BQ24160_TS_FAULT_11                    (0x11 << 1)

#define     BQ24160_LOW_CHG                        (0x01 << 0)

/* Charger activities */
#define CHGR_OFF                            0
#define CHGR_WENT_DEAD                      1
#define CHGR_WENT_FULL                      2
#define CHGR_ON                             3


/* Charger registers default values */
#define NEW_CTRL_REG_DEFAULT				0x8C
#define NEW_CTRL_BAT_VLT_REG_DEFAULT  		0x14
#define NEW_CURR_REG_DEFAULT  				0x32
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
INT08U   BQ24160_GetStatCtrlReg (void);
INT08U   BQ24160_GetBatStatReg (void);
INT08U   BQ24160_GetCtrlReg (void);
INT08U   BQ24160_GetCtrlBatVltReg (void);
INT08U   BQ24160_GetCurrentReg (void);
INT08U   BQ24160_GetDPPMReg (void);
INT08U   BQ24160_GetTmrNTCReg (void);
BOOLEAN  BQ24160_ChargerInit (INT08U i_in_limit);
BOOLEAN  BQ24160_ChargerDeInit (void);
BOOLEAN  BQ24160_ChargerEnable (INT08U i_in_limit);
BOOLEAN  BQ24160_ChargerDisable (INT08U i_in_limit);
BOOLEAN  BQ24160_is_default_params_det(void);
BOOLEAN  BQ24160_handle_faults(INT08U StatCtrlReg, INT08U BatSplyStatReg, INT08U NTCReg);

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
