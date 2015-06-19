/*
*********************************************************************************************************
*   POWER
*   Power Management Functions
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
*   Power Management Functions
*
*   File Name     : cp_power.h
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

#ifndef __CP_POWER_H__
#define __CP_POWER_H__


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

#define CC_PWR_ACTIVE     0x01
#define CC_PWR_MIDDLE     0x02
#define CC_PWR_SUSPEND    0x03
#define CC_PWR_OFF        0x04

//#define CC_PWR_DEBUD_ENABLED

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
extern BOOLEAN is_power_switch_on;
extern INT08U CC_Pwr_Status; 

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

void  Pwr_SystemPowerEnable (void);
void  Pwr_SystemPowerDisable (void);
BOOLEAN Is_Pwr_SystemPowerEnabled(void);

void  Pwr_ChargerPowerEnable (void);
void  Pwr_ChargerPowerDisable (void);

void Pwr_CC_Pwr_Status_Update(INT08U new_status);
INT08U Pwr_Get_CC_Pwr_Status(void);


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

#endif  /* End of __CP_POWER_H__ */
