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
*   INCLUDE FILES 
*********************************************************************************************************
*/
#include "cp_bq24160.h"

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
INT08U  BQ24160_GetStatCtrlReg (void)
{
    INT08U ret    = 0;
    INT08U buf_wr = 0;
    INT08U buf_rd = 0;
    
    do{
        buf_wr = BQ24160_SAT_CNTRL_REG;
        ret = I2C_WriteAndRead(I2C_0_HANDLE, BQ24160_I2C_ADDR, &buf_wr, 1, &buf_rd, 1, TRUE);
        if(ret == I2C_FAULT)
        	return 0;
    }while (ret == I2C_BUSY);
    
    return buf_rd;
}

INT08U  BQ24160_GetBatStatReg (void)
{
    INT08U ret    = 0;
    INT08U buf_wr = 0;
    INT08U buf_rd = 0;

    do{
        buf_wr = BQ24160_BAT_SAT_REG;
        ret = I2C_WriteAndRead(I2C_0_HANDLE, BQ24160_I2C_ADDR, &buf_wr, 1, &buf_rd, 1, TRUE);
        if(ret == I2C_FAULT)
        	return 0;
    }while (ret == I2C_BUSY);

    return buf_rd;
}

INT08U  BQ24160_GetCtrlReg (void)
{
    INT08U ret    = 0;
    INT08U buf_wr = 2;
    INT08U buf_rd = 0;
    
    do{
        buf_wr = BQ24160_CNTRL_REG;
        ret = I2C_WriteAndRead(I2C_0_HANDLE, BQ24160_I2C_ADDR, &buf_wr, 1, &buf_rd, 1, TRUE);
        if(ret == I2C_FAULT)
        	return 0;
    }while (ret == I2C_BUSY);
    
    return buf_rd; 
}

INT08U  BQ24160_GetCtrlBatVltReg (void)
{
    INT08U ret    = 0;
    INT08U buf_wr = 2;
    INT08U buf_rd = 0;
    
    do{
        buf_wr = BQ24160_CNTRL_BAT_VOL_REG;
        ret = I2C_WriteAndRead(I2C_0_HANDLE, BQ24160_I2C_ADDR, &buf_wr, 1, &buf_rd, 1, TRUE);
        if(ret == I2C_FAULT)
        	return 0;
    }while (ret == I2C_BUSY);
    
    return buf_rd; 
}

INT08U  BQ24160_GetCurrentReg (void)
{
    INT08U ret    = 0;
    INT08U buf_wr = 2;
    INT08U buf_rd = 0;
    
    do{
        buf_wr = BQ24160_BAT_TERM_F_CHRG_CUR_REG;
        ret = I2C_WriteAndRead(I2C_0_HANDLE, BQ24160_I2C_ADDR, &buf_wr, 1, &buf_rd, 1, TRUE);
        if(ret == I2C_FAULT)
         return 0;
    }while (ret == I2C_BUSY);
    
    return buf_rd; 
}

INT08U  BQ24160_GetDPPMReg (void)
{
    INT08U ret    = 0;
    INT08U buf_wr = 2;
    INT08U buf_rd = 0;

    do{
        buf_wr = BQ24160_VOL_DPPM_STAT_REG;
        ret = I2C_WriteAndRead(I2C_0_HANDLE, BQ24160_I2C_ADDR, &buf_wr, 1, &buf_rd, 1, TRUE);
        if(ret == I2C_FAULT)
        	return 0;
    }while (ret == I2C_BUSY);

    return buf_rd;
}

INT08U  BQ24160_GetTmrNTCReg (void)
{
    INT08U ret    = 0;
    INT08U buf_wr = 2;
    INT08U buf_rd = 0;

    do{
        buf_wr = BQ24160_TMR_NTC_MNTR_REG;
        ret = I2C_WriteAndRead(I2C_0_HANDLE, BQ24160_I2C_ADDR, &buf_wr, 1, &buf_rd, 1, TRUE);
        if(ret == I2C_FAULT)
        	return 0;
    }while (ret == I2C_BUSY);

    return buf_rd;
}

BOOLEAN  BQ24160_ChargerInit (INT08U i_in_limit)
{
    INT08U ret = 0;
    INT08U buf_wr[2];
    
    					/* ---------- Update Status/Control Register Register --------------------- */
       buf_wr[0] = BQ24160_SAT_CNTRL_REG;
       buf_wr[1] = BQ24160_SUPPLY_SEL;
               	   	/* B3 (SUPPLY_SEL = 1) - USB has precedence when both supplies are connected    */
       do{
		   ret = I2C_Write (I2C_0_HANDLE, BQ24160_I2C_ADDR, buf_wr, 2);
		   if(ret == I2C_FAULT)
			   goto i2c_error;
	   } while(ret == I2C_BUSY);


       	   	   	/* ---------- Update Battery/ Supply Status Register Register --------------------- */
	  buf_wr[0] = BQ24160_BAT_SAT_REG;
	  buf_wr[1] = 0x00;
												/* USB Lockout and No Battery Operation disabled    */
	  do{
		   ret = I2C_Write (I2C_0_HANDLE, BQ24160_I2C_ADDR, buf_wr, 2);
		   if(ret == I2C_FAULT)
			   goto i2c_error;
	   } while(ret == I2C_BUSY);


                                        /* ---------- Update Control Register --------------------- */
    buf_wr[0] = BQ24160_CNTRL_REG;                          /* Control Register address             */
    buf_wr[1] = i_in_limit + BQ24160_EN_STAT + BQ24160_TE + BQ24160_nCE;
                                        /* B0 (HZ_MODE = 0)  - Set Not High Impedance Mode          */
                                        /* B1 (nCE = 1)      - Disable Charger                      */
                                        /* B2 (TE = 1)       - Enable Charge Current Termination    */
                                        /* B3 (EN_STAT = 1)  - Enable STAT output 					*/
    do{
        ret = I2C_Write (I2C_0_HANDLE, BQ24160_I2C_ADDR, buf_wr, 2);
        if(ret == I2C_FAULT)
        	goto i2c_error;
    } while(ret == I2C_BUSY);
    

                                        /* ---------- Update Control/Battery Voltage Register ----- */
    buf_wr[0] = BQ24160_CNTRL_BAT_VOL_REG;                  /* Control/Bat Voltage Register address */
    buf_wr[1] = BQ24160_V_BREG_5 + BQ24160_V_BREG_1 + BQ24160_V_BREG_0;
                       /* B7,B6,B5,B4,B3,B2 (1,0,0,0,1,1) - Battery Regulation Voltage = 4.20V      */
    do{
        ret = I2C_Write (I2C_0_HANDLE, BQ24160_I2C_ADDR, buf_wr, 2);
        if(ret == I2C_FAULT)
        	goto i2c_error;
    } while(ret == I2C_BUSY);


                                        /* ------ Update Bat Term/Fast Charge Current Register ---- */
    buf_wr[0] = BQ24160_BAT_TERM_F_CHRG_CUR_REG;
    buf_wr[1] = BQ24160_I_CHRG_4 + BQ24160_I_CHRG_3 + BQ24160_I_CHRG_0 + BQ24160_I_TERM_2 + BQ24160_I_TERM_0;
                                        /* B2,B1,B0 (1,0,1) - Term Current Sense Voltage  - 300mA   */
                                        /* B7,B6,B5,B4,B3 (1,1,0,0,1) -  Charge Current   - 2425mA  */
    do{
        ret = I2C_Write (I2C_0_HANDLE, BQ24160_I2C_ADDR, buf_wr, 2);
        if(ret == I2C_FAULT)
        	goto i2c_error;
    } while(ret == I2C_BUSY);
    

										/* ------ Update VIN-DPM Voltage/ DPPM Status Register ---- */
	buf_wr[0] = BQ24160_VOL_DPPM_STAT_REG;
	buf_wr[1] = 0x00;
																	/* VIN-DPM threshold is 4.20V   */
	do{
		ret = I2C_Write (I2C_0_HANDLE, BQ24160_I2C_ADDR, buf_wr, 2);
		if(ret == I2C_FAULT)
			goto i2c_error;
	} while(ret == I2C_BUSY);


										   /* ------ Update Safety Timer/ NTC Monitor Register ---- */
	buf_wr[0] = BQ24160_TMR_NTC_MNTR_REG;
	buf_wr[1] = BQ24160_SFT_TMR_6HUR + BQ24160_TS_EN;
												   /* B7 (2XTMR_EN = 0) - No timer slowdown         */
												   /* B6,B5 (0,1)       - 6 hour fast charge        */
	                                    		   /* B3 (TS_EN = 1)    - TS function enabled       */
												   /* B0 (LOW_CHG = 0)  - Charge current not halfed */
	do{
		ret = I2C_Write (I2C_0_HANDLE, BQ24160_I2C_ADDR, buf_wr, 2);
		if(ret == I2C_FAULT)
			goto i2c_error;
	} while(ret == I2C_BUSY);

    return TRUE;
i2c_error:
    return FALSE;
}

BOOLEAN  BQ24160_ChargerEnable (INT08U i_in_limit)
{
    INT08U ret = 0;
    INT08U buf_wr[2];
    
										/* ---------- Update Control Register --------------------- */
	buf_wr[0] = BQ24160_CNTRL_REG;                          /* Control Register address             */
	buf_wr[1] = i_in_limit + BQ24160_EN_STAT + BQ24160_TE;
										/* B0 (HZ_MODE = 0)  - Set Not High Impedance Mode          */
										/* B1 (nCE = 0)      - Enable Charger                       */
										/* B2 (TE = 1)       - Enable Charge Current Termination    */
										/* B3 (EN_STAT = 1)  - Enable STAT output 					*/
	do{
		ret = I2C_Write (I2C_0_HANDLE, BQ24160_I2C_ADDR, buf_wr, 2);
		if(ret == I2C_FAULT)
			goto i2c_error;
	} while(ret == I2C_BUSY);
    
    return TRUE;
i2c_error:
    return FALSE;  
}

BOOLEAN  BQ24160_ChargerDisable (INT08U i_in_limit)
{
    INT08U ret = 0;
    INT08U buf_wr[2];
    
										/* ---------- Update Control Register --------------------- */
	buf_wr[0] = BQ24160_CNTRL_REG;                          /* Control Register address             */
	buf_wr[1] = i_in_limit + BQ24160_EN_STAT + BQ24160_TE + BQ24160_nCE;
										/* B0 (HZ_MODE = 0)  - Set Not High Impedance Mode          */
										/* B1 (nCE = 1)      - Disable Charger                      */
										/* B2 (TE = 1)       - Enable Charge Current Termination    */
										/* B3 (EN_STAT = 1)  - Enable STAT output 					*/
	do{
		ret = I2C_Write (I2C_0_HANDLE, BQ24160_I2C_ADDR, buf_wr, 2);
		if(ret == I2C_FAULT)
			goto i2c_error;
	} while(ret == I2C_BUSY);
    
    return TRUE;
i2c_error:
    return FALSE;
}

BOOLEAN BQ24160_is_default_params_det(void)
{
   INT08U test_buf[3];
   BOOLEAN ret = 0;
   test_buf[0] = BQ24160_GetCtrlReg();
   if(test_buf[0] == 0)
	   return 1;
   test_buf[1] = BQ24160_GetCtrlBatVltReg();
   if(test_buf[1] == 0)
	   return 1;
   test_buf[2] = BQ24160_GetCurrentReg();
   if(test_buf[2] == 0)
	   return 1;
   
   if(test_buf[0] == NEW_CTRL_REG_DEFAULT || test_buf[1] == NEW_CTRL_BAT_VLT_REG_DEFAULT || test_buf[2] == NEW_CURR_REG_DEFAULT)
   {
	   ret = 1;
   }

   return ret;
}

BOOLEAN BQ24160_handle_faults(INT08U StatCtrlReg, INT08U BatSplyStatReg, INT08U NTCReg)
{
   INT08U StatRegFaults = StatCtrlReg & 0x07;
   INT08U USBVltFaults 	= BatSplyStatReg & 0x30;
   INT08U BatVltFaults 	= BatSplyStatReg & 0x06;
   INT08U NTCFaults 	= BatSplyStatReg & 0x06;

   BOOLEAN ret;

   	   	   	   	   	   	   	   	   	   	   /* If a timer fault occurs re-init charger and give two small beeps */
   	   	   	   	   	   	   	   	   	   	   /* Do not re-init charger for safety timer expiration. Avoid continuous charging of defective battery */
   if(StatRegFaults == BQ24160_FAULT_WATCHDOG_EXP)
   {
	   #ifdef ENABLE_CHRG_TONE
	   Sys_BeepHigh(20);
	   Sys_DelayMs(200);
	   Sys_BeepHigh(20);
	   #endif

	   ret =BQ24160_ChargerInit(BQ24160_IUSB_LIMIT_NA);
	   if(ret == FALSE){
			return FALSE;
	   }

	   ret = BQ24160_ChargerEnable(BQ24160_IUSB_LIMIT_NA);
	   if(ret == FALSE){
		   return FALSE;
	   }
   }											  	/* If a battery voltage fault occurs give three small beeps */
   else if(StatRegFaults == BQ24160_FAULT_BATTERY || BatVltFaults == BQ24160_BAT_OVP || BatVltFaults == BQ24160_BAT_NOT_PRESENT)
   {
	   #ifdef ENABLE_CHRG_TONE
	   Sys_BeepHigh(20);
	   Sys_DelayMs(200);
	   Sys_BeepHigh(20);
	   Sys_DelayMs(200);
	   Sys_BeepHigh(20);
	   #endif
   }											  				/* If a USB fault occurs give four small beeps */
   else if(StatRegFaults == BQ24160_FAULT_USB_SUPPLY || USBVltFaults != BQ24160_USB_NORMAL)
   {
	   #ifdef ENABLE_CHRG_TONE
	   Sys_BeepHigh(20);
	   Sys_DelayMs(200);
	   Sys_BeepHigh(20);
	   Sys_DelayMs(200);
	   Sys_BeepHigh(20);
	   Sys_DelayMs(200);
	   Sys_BeepHigh(20);
	   #endif
   }											  			/* If a temperature fault occurs give five small beeps */
   else if(StatRegFaults == BQ24160_FAULT_THERMAL_SHUT || StatRegFaults == BQ24160_FAULT_BAT_TEMP || NTCFaults != BQ24160_TS_FAULT_NO)
   {
	   #ifdef ENABLE_CHRG_TONE
	   Sys_BeepHigh(20);
	   Sys_DelayMs(200);
	   Sys_BeepHigh(20);
	   Sys_DelayMs(200);
	   Sys_BeepHigh(20);
	   Sys_DelayMs(200);
	   Sys_BeepHigh(20);
	   Sys_DelayMs(200);
	   Sys_BeepHigh(20);
	   #endif
   }

   return TRUE;
}

/*
*********************************************************************************************************
*********************************************************************************************************
*   LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/
