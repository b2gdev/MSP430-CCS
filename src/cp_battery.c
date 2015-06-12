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
*   INCLUDE FILES 
*********************************************************************************************************
*/
#include "cp_battery.h"

/*
*********************************************************************************************************
*   LOCAL DEFINES
*********************************************************************************************************
*/
#define BAT_SHORT_CCT_THRESHOLD         (((2.7/2)/2.5)      * 4096)     /* Threshold    - 2.7V      */
#define BAT_DEAD_THRESHOLD              (((3.1/2)/2.5)      * 4096)     /* Threshold    - 3.1V      */
#define BAT_WEAK_THRESHOLD              (((3.3/2)/2.5)      * 4096)     /* Threshold    - 3.3V      */ 
#define BAT_GOOD_THRESHOLD              (((4.15/2)/2.5)     * 4096)     /* Threshold    - 4.15V     */ 
#define BAT_HISTERESIS                  (((0.06/2)/2.5)     * 4096)     /* Threshold    - 0.06V     */ 
#define BAT_TEMP_THRESHOLD              (((3.3*50/150)/3.3) * 4096)     /* Threshold    - 1.1V      */
#define BAT_ERROR_THRESHOLD             0                               /* Threshold    - ERROR     */

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
 INT16U bat_cur    = 0;
 INT16U bat_vlt    = 0;
 
 INT16U vol_mv = 0;

 double bat_good_threshold = BAT_GOOD_THRESHOLD;
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
INT08U  Bat_GetInitialBatteryLevel (void)		// No i2c communication in this
{
    ADC12_SingleConv(CP_VBAT_VMES, TRUE);  
    
    bat_vlt = ADC12_Results[CP_VBAT_VMES];
    
    if (bat_vlt <= BAT_SHORT_CCT_THRESHOLD){
        return BAT_LEVEL_SHORT_CCT;
    }
    else if (bat_vlt <= BAT_DEAD_THRESHOLD){ 
        return BAT_LEVEL_DEAD;
    } 
    else if (bat_vlt <= BAT_WEAK_THRESHOLD){ 
        return BAT_LEVEL_WEAK;
    } 
    else if (bat_vlt <= BAT_GOOD_THRESHOLD){
        return BAT_LEVEL_GOOD;
    }    
    else{
        return BAT_LEVEL_GOOD;       
    }
}

INT08U  Bat_GetBatteryLevel (void)
{   
    INT08U is_charging = 0;
    
    if(bat_good_threshold == BAT_GOOD_THRESHOLD - BAT_HISTERESIS)
    {
    	bat_vlt = Bat_GetFuelGaugeVoltage();		// Read battery voltage from fuel gauge once battery went full until determining the charge restart point
    	if(bat_vlt == 0)
    		return BAT_LEVEL_ERROR;
    }
    else
    {
    	ADC12_SingleConv(CP_VBAT_VMES, TRUE);		// Read battery voltage from voltage divider under normal conditions
    	bat_vlt = ADC12_Results[CP_VBAT_VMES];
    }
    
    if (bat_vlt <= BAT_SHORT_CCT_THRESHOLD){
        return BAT_LEVEL_SHORT_CCT;
    }
    else if (bat_vlt <= BAT_DEAD_THRESHOLD){ 
        return BAT_LEVEL_DEAD;
    } 
    else if (bat_vlt <= BAT_WEAK_THRESHOLD){ 
        return BAT_LEVEL_WEAK;
    } 
    else if (bat_vlt <= bat_good_threshold){
    	bat_good_threshold = BAT_GOOD_THRESHOLD;
        return BAT_LEVEL_GOOD;
    }    
    else{
        is_charging = Bat_IsFuelGaugeBatCharging();
        if(is_charging == BAT_IS_CHARGING){
        	bat_cur = Bat_GetFuelGaugeAvgCurr_mA();
        	if(bat_cur == 0)
        		return BAT_LEVEL_ERROR;

        	if(bat_cur <= BAT_CHG_TERM_CUR_300) {			// Charge Termination current 300mA
        		bat_good_threshold = BAT_GOOD_THRESHOLD - BAT_HISTERESIS;
        		return BAT_LEVEL_FULL;
        	}
        	else
        	{
        		return BAT_LEVEL_GOOD;
        	}
        }else if(is_charging == BAT_IS_CHARGING_ERROR){
        	return BAT_LEVEL_ERROR;
        }
        return BAT_LEVEL_FULL;       
    }
}


INT08U  Bat_GetFuelGaugeBatteryTemp (void)
{
    ADC12_SingleConv(SENSE_BAT_TEMP, TRUE);
    
    if (ADC12_Results[SENSE_BAT_TEMP] >= BAT_TEMP_THRESHOLD)
    {
        return BAT_TEMP_GOOD;
    }
    else
    {
        return BAT_TEMP_BAD;
    }
}

BOOLEAN  Bat_GetFuelGaugeStatus (PTR_INT08U buf, INT08U size)
{
    INT08U ret    = 0;
    INT08U buf_wr = 0;
    INT08U i      = 0;
    
    if (size < 6) {     /* check buffer size */
        return FALSE;
    }
    
    /* BQ27200_TEMP_LOW_REG */
    do{
        buf_wr = BQ27200_TEMP_LOW_REG;
        ret = I2C_WriteAndRead(I2C_0_HANDLE, BQ27200_I2C_ADDR, &buf_wr, 1, &buf[i++], 1, TRUE);
        if(ret == I2C_FAULT)
        	goto i2c_error;
    }while (ret == I2C_BUSY);
    
    /* BQ27200_TEMP_HIGH_REG */
    do{
        buf_wr = BQ27200_TEMP_HIGH_REG;
        ret = I2C_WriteAndRead(I2C_0_HANDLE, BQ27200_I2C_ADDR, &buf_wr, 1, &buf[i++], 1, TRUE);
        if(ret == I2C_FAULT)
        	goto i2c_error;
    }while (ret == I2C_BUSY);

    /* BQ27200_VOLT_LOW_REG */
    do{
        buf_wr = BQ27200_VOLT_LOW_REG;
        ret = I2C_WriteAndRead(I2C_0_HANDLE, BQ27200_I2C_ADDR, &buf_wr, 1, &buf[i++], 1, TRUE);
        if(ret == I2C_FAULT)
        	goto i2c_error;
    }while (ret == I2C_BUSY);

    /* BQ27200_VOLT_HIGH_REG */
    do{
        buf_wr = BQ27200_VOLT_HIGH_REG;
        ret = I2C_WriteAndRead(I2C_0_HANDLE, BQ27200_I2C_ADDR, &buf_wr, 1, &buf[i++], 1, TRUE);
        if(ret == I2C_FAULT)
        	goto i2c_error;
    }while (ret == I2C_BUSY);

    /* BQ27200_FLAGS_REG */
    do{
        buf_wr = BQ27200_FLAGS_REG;
        ret = I2C_WriteAndRead(I2C_0_HANDLE, BQ27200_I2C_ADDR, &buf_wr, 1, &buf[i++], 1, TRUE);
        if(ret == I2C_FAULT)
        	goto i2c_error;
    }while (ret == I2C_BUSY);

    /* BQ27200_RSOC_REG */
    do{
        buf_wr = BQ27200_RSOC_REG;
        ret = I2C_WriteAndRead(I2C_0_HANDLE, BQ27200_I2C_ADDR, &buf_wr, 1, &buf[i++], 1, TRUE);
        if(ret == I2C_FAULT)
        	goto i2c_error;
    }while (ret == I2C_BUSY);   
    
    return TRUE;
i2c_error:
    return FALSE;
}

INT16U Bat_GetFuelGaugeAvgCurr_mA (void)
{
    INT08U ret = 0;
    INT08U buf_wr = 0;
    
    INT08U avg_cur_low      = 0;
    INT08U avg_cur_high     = 0;
    INT16U avg_cur_ma = 0;
    
    Sys_DelayMs(1);    
 
    /* BQ27200_AVG_CUR_LOW_REG */
    do{
        buf_wr = BQ27200_AVG_CUR_LOW_REG;
        ret = I2C_WriteAndRead(I2C_0_HANDLE, BQ27200_I2C_ADDR, &buf_wr, 1, &avg_cur_low, 1, TRUE);
        if(ret == I2C_FAULT)
        	goto i2c_error;
    }while (ret == I2C_BUSY);

    /* BQ27200_AVG_CUR_HIGH_REG */
    do{
        buf_wr = BQ27200_AVG_CUR_HIGH_REG;
        ret = I2C_WriteAndRead(I2C_0_HANDLE, BQ27200_I2C_ADDR, &buf_wr, 1, &avg_cur_high, 1, TRUE);
        if(ret == I2C_FAULT)
        	goto i2c_error;
    }while (ret == I2C_BUSY);    
    
    avg_cur_ma = (INT16U) (((((double)(avg_cur_high << 8) + (double)(avg_cur_low)))*3.57) / 20);    
    
    return avg_cur_ma;
i2c_error:
    return 0;
}

/* This returns the voltage read from fuel gauge in units of ADC readings */
INT16U Bat_GetFuelGaugeVoltage (void)
{  
    INT08U ret = 0;
    INT08U buf_wr = 0;
    
    INT08U vol_low      = 0;
    INT08U vol_high     = 0;
    INT16U vol_adc_units = 0;
    
    Sys_DelayMs(1);    
 
    /* BQ27200_VOLT_LOW_REG */
    do{
        buf_wr = BQ27200_VOLT_LOW_REG;
        ret = I2C_WriteAndRead(I2C_0_HANDLE, BQ27200_I2C_ADDR, &buf_wr, 1, &vol_low, 1, TRUE);
        if(ret == I2C_FAULT)
        	goto i2c_error;
    }while (ret == I2C_BUSY);

    /* BQ27200_VOLT_HIGH_REG */
    do{
        buf_wr = BQ27200_VOLT_HIGH_REG;
        ret = I2C_WriteAndRead(I2C_0_HANDLE, BQ27200_I2C_ADDR, &buf_wr, 1, &vol_high, 1, TRUE);
        if(ret == I2C_FAULT)
        	goto i2c_error;
    }while (ret == I2C_BUSY);    
    
    vol_mv = (INT16U) ((double)(vol_high << 8) + (double)(vol_low));   
    
    vol_adc_units = (INT16U) (((((double)vol_mv)/2)/2.5/1000)* 4096);
    
    return vol_adc_units;
i2c_error:
    return 0;
}

INT08U  Bat_IsFuelGaugeBatCharging (void)
{
    INT08U ret = 0;
    INT08U buf_wr = 0;
    INT08U flags      = 0;
    
    Sys_DelayMs(1);
    
   /* BQ27200_FLAGS_REG */
    do{
        buf_wr = BQ27200_FLAGS_REG;
        ret = I2C_WriteAndRead(I2C_0_HANDLE, BQ27200_I2C_ADDR, &buf_wr, 1, &flags, 1, TRUE);
        if(ret == I2C_FAULT)
        	return BAT_IS_CHARGING_ERROR;
    }while (ret == I2C_BUSY);
    
    if((flags>>7) == 1)
    	return BAT_IS_CHARGING;
    return BAT_IS_NOT_CHARGING;    
}
/*
*********************************************************************************************************
*********************************************************************************************************
*   LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/
