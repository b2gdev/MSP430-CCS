/*
*********************************************************************************************************
*   BQ24150A
*   BQ24150A Fully Integrated Switch-Mode One-Cell Li-Ion Charger Functions
*
*   Copyright � 2010 Zone24x7 Inc. All rights reserved. 
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
*   INCLUDE FILES 
*********************************************************************************************************
*/
#include "cp_bq24150a.h"

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
//Status register bits EN_STAT, STAT2, STAT1 (B5,B6,B7) are not properly read through I2C
INT08U  BQ24150A_GetStatus (void)
{
    INT08U ret    = 0;
    INT08U buf_wr = 0;
    INT08U buf_rd = 0;
    
    do{
        buf_wr = BQ24150A_SAT_CNTRL_REG;
        ret = I2C_WriteAndRead(I2C_0_HANDLE, BQ24150A_I2C_ADDR, &buf_wr, 1, 
                                &buf_rd, 1, TRUE);
        if(ret == I2C_FAULT)
         return 0;
    }while (ret == I2C_BUSY);
    
    return buf_rd; // (buf_rd & 0x30);
}

//Voltage register bits OTG_PL, VO(REG2) (B1,B4) are not properly read through I2C
INT08U  BQ24150A_GetVoltageReg (void)   //test
{
    INT08U ret    = 0;
    INT08U buf_wr = 2;
    INT08U buf_rd = 0;
    
    do{
        buf_wr = BQ24150A_CNTRL_BAT_VOL_REG;
        ret = I2C_WriteAndRead(I2C_0_HANDLE, BQ24150A_I2C_ADDR, &buf_wr, 1, 
                                &buf_rd, 1, TRUE);
        if(ret == I2C_FAULT)
         return 0;
    }while (ret == I2C_BUSY);
    
    return buf_rd; 
}

//Control register bit nCE, TE (B2,B3) is not properly read through I2C
INT08U  BQ24150A_GetCtrlReg (void)   //test
{
    INT08U ret    = 0;
    INT08U buf_wr = 2;
    INT08U buf_rd = 0;
    
    do{
        buf_wr = BQ24150A_CNTRL_REG;
        ret = I2C_WriteAndRead(I2C_0_HANDLE, BQ24150A_I2C_ADDR, &buf_wr, 1, 
                                &buf_rd, 1, TRUE);
        if(ret == I2C_FAULT)
         return 0;
    }while (ret == I2C_BUSY);
    
    return buf_rd; 
}

//Current register bits VI(CHRG0), VI(CHRG1), Reset (B4,B5,B7) are not properly read through I2C
INT08U  BQ24150A_GetCurrentReg (void)   //test
{
    INT08U ret    = 0;
    INT08U buf_wr = 2;
    INT08U buf_rd = 0;
    
    do{
        buf_wr = BQ24150A_BAT_TERM_F_CHRG_CUR_REG;
        ret = I2C_WriteAndRead(I2C_0_HANDLE, BQ24150A_I2C_ADDR, &buf_wr, 1, 
                                &buf_rd, 1, TRUE);
        if(ret == I2C_FAULT)
         return 0;
    }while (ret == I2C_BUSY);
    
    return buf_rd; 
}

// Charger De-Init function put the charger IC to Hi-Z mode. I2C not active after that
BOOLEAN  BQ24150A_ChargerDeInit (void)
{
    INT08U ret = 0;
    INT08U buf_wr[2];

                                        /* ---------- Update Control Register --------------------- */
    buf_wr[0] = BQ24150A_CNTRL_REG;                         /* Control Register address             */
    buf_wr[1] = BQ24150A_HZ_MODE + BQ24150A_nCE + BQ24150A_TE;
                                        /* B0 (OPA_MODE = 0) - Set Mode to Charger                  */
                                        /* B1 (HZ_MODE = 1)  - High Impedance Mode                  */
                                        /* B2 (nCE = 1)      - Disable Charger                      */
                                        /* B3 (TE = 1)       - Enable Charge Current Termination    */

    do{
        ret = I2C_Write (I2C_0_HANDLE, BQ24150A_I2C_ADDR, buf_wr, 2);
        if(ret == I2C_FAULT)
          goto i2c_error;
    } while(ret == I2C_BUSY);

    return TRUE;
i2c_error:
    return FALSE;
}

BOOLEAN  BQ24150A_ChargerInit (INT08U i_in_limit)
{
    INT08U ret = 0;
    INT08U buf_wr[2];
    
                                        /* ---------- Update Control Register --------------------- */
    buf_wr[0] = BQ24150A_CNTRL_REG;                         /* Control Register address             */
    buf_wr[1] = BQ24150A_nCE + BQ24150A_TE + BQ24150A_V_LOW_3V7 + i_in_limit;
                                        /* B0 (OPA_MODE = 0) - Set Mode to Charger                  */
                                        /* B1 (HZ_MODE = 0)  - Set Not High Impedance Mode          */
                                        /* B2 (nCE = 1)      - Disable Charger                      */
                                        /* B3 (TE = 1)       - Enable Charge Current Termination    */
                                        /* B4 (VLOWV_1 = 1) - Set Weak Battery Threshold to 3.7V    */
                                        /* B5 (VLOWV_2 = 1) - Set Weak Battery Threshold to 3.7V    */

    do{
        ret = I2C_Write (I2C_0_HANDLE, BQ24150A_I2C_ADDR, 
                         buf_wr, 2);                        /* Write Control Register               */
        if(ret == I2C_FAULT)
          goto i2c_error;
    } while(ret == I2C_BUSY);
    
                                        /* ---------- Update Control/Battery Voltage Register ----- */
    buf_wr[0] = BQ24150A_CNTRL_BAT_VOL_REG;                 /* Control/Bat Voltage Register address */
    buf_wr[1] = BQ24150A_OTG_PL + BQ24150A_VO_REG_5 + BQ24150A_VO_REG_2; 
                                        /* B0 (OTG_EN = 0) - Disable OTG Pin                        */
                                        /* B1 (OTG_PL = 1) - OTG Polarity, Active Hight             */
                                        /* B2,B3,B4,B5,B6,B7 (Vo = 0,0,1,0,0,1) 
                                                          - Battery Regulation Voltage = 4.22V      */
    
    do{
        ret = I2C_Write (I2C_0_HANDLE, BQ24150A_I2C_ADDR, 
                         buf_wr, 2);                        /* Write Control Register               */
        if(ret == I2C_FAULT)
          goto i2c_error;
    } while(ret == I2C_BUSY);
                                          
                                        /* ------ Update Bat Term/Fast Charge Current Register ---- */
    buf_wr[0] = BQ24150A_BAT_TERM_F_CHRG_CUR_REG;
    buf_wr[1] = BQ24150A_VI_TERM_0 + BQ24150A_VI_TERM_2 + BQ24150A_VI_CHRG_0 + BQ24150A_VI_CHRG_1 +BQ24150A_VI_CHRG_2; 
                                        /* B0,B1,B2 (1,0,1) - Term Current Sense Voltage  - 300mA    */
                                        /* B4, B5, B6 (1, 1, 1) - Charge Cur Sense Voltage - 1200mA  */

    do{
        ret = I2C_Write (I2C_0_HANDLE, BQ24150A_I2C_ADDR, 
                         buf_wr, 2);                        /* Write Control Register               */
        if(ret == I2C_FAULT)
          goto i2c_error;
    } while(ret == I2C_BUSY);
    
    return TRUE;
i2c_error:
    return FALSE;
}

//Control register bit TE (B3) is not properly read through I2C
BOOLEAN  BQ24150A_ChargerEnable ()
{
    INT08U ret       = 0;
    INT08U buf_wr[2];
    INT08U buf_rd    = 0;
    
                                        /* ---------- Update Control Register --------------------- */
    buf_wr[0] = BQ24150A_CNTRL_REG;                         /* Control Register address             */

    do{
        ret = I2C_WriteAndRead(I2C_0_HANDLE, BQ24150A_I2C_ADDR, buf_wr, 1, 
                               &buf_rd, 1, TRUE);           /* Read Control Register                */
        if(ret == I2C_FAULT)
          goto i2c_error;
    } while (ret == I2C_BUSY);

    
    buf_wr[1] = buf_rd & (~BQ24150A_nCE);                   /* Enable charger                       */

    do{
         ret = I2C_Write (I2C_0_HANDLE, BQ24150A_I2C_ADDR, 
                         buf_wr, 2);                        /* Write Control Register               */
         if(ret == I2C_FAULT)
          goto i2c_error;
    } while(ret == I2C_BUSY);
    
    return TRUE;
i2c_error:
    return FALSE;  
}

//Control register bit TE (B3) is not properly read through I2C
BOOLEAN  BQ24150A_ChargerDisable ()
{
    INT08U ret       = 0;
    INT08U buf_wr[2];
    INT08U buf_rd    = 0;
                                        /* ---------- Update Control Register --------------------- */
    buf_wr[0] = BQ24150A_CNTRL_REG;                         /* Control Register address             */

    do{
        ret = I2C_WriteAndRead(I2C_0_HANDLE, BQ24150A_I2C_ADDR, buf_wr, 1, 
                               &buf_rd, 1, TRUE);           /* Read Control Register                */
        if(ret == I2C_FAULT)
          goto i2c_error;
    } while (ret == I2C_BUSY);

    
    buf_wr[1] = buf_rd | BQ24150A_nCE;                      /* Disable charger                      */

    do{
        ret = I2C_Write (I2C_0_HANDLE, BQ24150A_I2C_ADDR, 
                         buf_wr, 2);                        /* Write Control Register               */
        if(ret == I2C_FAULT)
          goto i2c_error;
    } while(ret == I2C_BUSY);
    
    return TRUE;
i2c_error:
    return FALSE;    
}

BOOLEAN BQ24150A_is_default_params_det(void) //test
{
   INT08U test_buf[3];
   BOOLEAN ret = 0;
   test_buf[0] = BQ24150A_GetVoltageReg();
   if(test_buf[0] == 0)
     return 1;
   test_buf[1] = BQ24150A_GetCtrlReg();
   if(test_buf[1] == 0)
     return 1;
   test_buf[2] = BQ24150A_GetCurrentReg();
   if(test_buf[2] == 0)
     return 1;
   
   if(test_buf[0] == VOLT_REG_DEFAULT || test_buf[1] == CTRL_REG_DEFAULT || test_buf[2] == CURR_REG_DEFAULT)
   {
     ret = 1;
   }

   return ret;
}

/*
*********************************************************************************************************
*********************************************************************************************************
*   LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/
