/*
*********************************************************************************************************
*   BATTERY
*   Battery Related Functions
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
*   Battery Related Functions
*
*   File Name     : cp_battery.c
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

#ifndef __CP_BATTERY_H__
#define __CP_BATTERY_H__


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
#define BAT_LEVEL_SHORT_CCT                         0
#define BAT_LEVEL_DEAD                              1  
#define BAT_LEVEL_WEAK                              2
#define BAT_LEVEL_GOOD                              3
#define BAT_LEVEL_FULL                              4
#define BAT_LEVEL_ERROR                             5

#define BAT_IS_NOT_CHARGING                         0
#define BAT_IS_CHARGING                             1
#define BAT_IS_CHARGING_ERROR                       4

#define BAT_TEMP_BAD                                0
#define BAT_TEMP_GOOD                               1

/* Charge termination current */
#define BAT_CHG_TERM_CUR_50                         50
#define BAT_CHG_TERM_CUR_100                        100
#define BAT_CHG_TERM_CUR_150                        150
#define BAT_CHG_TERM_CUR_200                        200
#define BAT_CHG_TERM_CUR_250                        250
#define BAT_CHG_TERM_CUR_300                        300
#define BAT_CHG_TERM_CUR_500                        500
/* End */

/* BQ27200 I2C address */
#define BQ27200_I2C_ADDR                            0x55            /* 7 bit I2C address            */

/* BQ27200 Register Addresses */
#define     BQ27200_TEMP_LOW_REG                    0x06
#define     BQ27200_TEMP_HIGH_REG                   0x07
#define     BQ27200_VOLT_LOW_REG                    0x08
#define     BQ27200_VOLT_HIGH_REG                   0x09
#define     BQ27200_FLAGS_REG                       0x0A
#define     BQ27200_RSOC_REG                        0x0B
#define     BQ27200_AVG_CUR_LOW_REG                 0x14
#define     BQ27200_AVG_CUR_HIGH_REG                0x15

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
INT08U  Bat_GetInitialBatteryLevel (void);
INT08U  Bat_GetBatteryLevel (void);
INT08U  Bat_GetFuelGaugeBatteryTemp (void);
BOOLEAN  Bat_GetFuelGaugeStatus (PTR_INT08U buf, INT08U size);
INT16U Bat_GetFuelGaugeAvgCurr_mA (void);
INT08U  Bat_IsFuelGaugeBatCharging (void);
INT16U Bat_GetFuelGaugeVoltage (void);

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
