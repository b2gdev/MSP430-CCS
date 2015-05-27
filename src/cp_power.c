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
*   File Name     : power.c
*   Version       : V1.0.0
*   Programmer(s) : PS
*********************************************************************************************************
*   Note(s)
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*   INCLUDE FILES 
*********************************************************************************************************
*/
#include "cp_power.h"
#include "cp_conf.h"

/*
*********************************************************************************************************
*   LOCAL DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*   LOCAL CONSTANTS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*   LOCAL DATA TYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*   LOCAL TABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*   LOCAL VARIABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*   LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*   LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*   GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/


/* Enable system power via Main Power Switches */
void  Pwr_SystemPowerEnable (void)
{
    CP_PWR_ENABLE();                     /* Active low enable                    */
}

/* Disable system power by turning off Main Power Switches */
void  Pwr_SystemPowerDisable (void)
{
    CP_PWR_DISABLE();                     /* Active high disable                  */
}

/* Is system power enabled ? */  
BOOLEAN Is_Pwr_SystemPowerEnabled(void)
{
  return (P7OUT & P7) ? 0 : 1;
}
/* Enable power to battery charger */
void  Pwr_ChargerPowerEnable (void)
{
    CP_CHRG_PWRON();
}

/* Disable power to battery charger */
void  Pwr_ChargerPowerDisable (void)
{
    CP_CHRG_PWROFF();
}

/* Update CC power status */
void Pwr_CC_Pwr_Status_Update(INT08U new_status)
{
  CC_Pwr_Status = new_status; 
  Brd_Pwr_Update(CC_Pwr_Status);//{KW}:: maintain braille display power status too, in here
  #ifdef CC_PWR_DEBUD_ENABLED
  Sys_BeepHigh(20);
  #endif
}

/* Get CC power status */
INT08U Pwr_Get_CC_Pwr_Status(void)
{
  return CC_Pwr_Status;
}
/*
*********************************************************************************************************
*********************************************************************************************************
*   LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/
