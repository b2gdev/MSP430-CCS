/*
*********************************************************************************************************
*   Timer A
*   Timer A Functions
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
*   Timer A Functions
*
*   File Name     : cp_timer_a.c
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
#include    "cp_timer_a.h"
#include "cp_i2c.h"

/*
*********************************************************************************************************
*   LOCAL DEFINES
*********************************************************************************************************
*/
//#define     TMR_A_DEBUG
#define     TMR_A_BAT_CHRG_CCR0         0xFFFF
#define     TMR_A_BAT_CHRG_DIVIDER      305 //77 //153(4sec) //305(8 sec) //488
                                            /* Battery Charger Timer Period - 10 sec                */
                                            /* Clock Input - 16 MHz                                 */
                                            /* Input Divider - 8                                    */
                                            /* CCR0 - 0xFFFF (65535)                                */

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
INT08U timer_a_mode;
INT16U timer_a_divider;

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
void  TmrA_Init (INT08U mode)
{
    timer_a_mode    = mode;     /* Set Timer A usage mode (Battery charging or Touchpad)            */
    timer_a_divider = 0;        /* Reset Timer A divider                                            */

    if (timer_a_mode == TMR_A_MODE_BAT_CHARGER){    /* Battery charger mode                         */
        TACCTL0 = CCIE;         /* CMx          : 00   - Capture mode - No capture                  */
                                /* CCISx        : 00   - Capture/Compare input - CCIxA              */
                                /* SCS          : 0    - Asynchronous capture                       */
                                /* SCCI         : 0    - Synchronized capture/compare input         */
                                /* Unused       : 0                                                 */
                                /* CAP          : 0    - Capture mode - Compare mode                */
                                /* OUTMODx      : 000  - OUT bit value                              */
                                /* CCIE         : 1    - Capture/Compare interrupt - Enabled        */
                                /* CCI          : 0    - (Read only)                                */
                                /* OUT          : 0    - Output mode 0, Output - Low                */
                                /* COV          : 0    - Capture overflow - No capture overflow     */
                                /* CCIFG        : 0    - Capture/Compare interrupt flag (Read only) */

        TACCR0  = TMR_A_BAT_CHRG_CCR0;  /* TACCRx       : Compare mode Timer value                  */

        TACTL   = TASSEL_2 + ID_3 + MC_1;
                                /* Unused       : 000000                                            */
                                /* TASSELx      : 10     - Clock source - SMCLK                     */
                                /* IDx          : 11     - Input divider - 8                        */
                                /* MCx          : 01     - Mode control - Up mode                   */
                                /* Unused       : 0                                                 */
                                /* TACLR        : 0      - Timer Clear - Not cleared                */
                                /* TAIE         : 0      - Timer Interrupt - Interrupt disabled     */
                                /* TAIFG        : 0      - Timer Interrupt flag                     */

        __bis_SR_register(GIE); /* Enable Global Interrupts                                         */
    }
    else{                                       /* Touchpad mode                                    */
        ;
    }
}

inline void TmrA_IntEnable()
{
  TACCTL0 = CCIE;
}

inline void TmrA_IntDisable()
{
  TACCTL0 = 0;
}
void  TmrA_Stop (void)
{
    TACCTL0 &= ~(1 << 5);        /* Clear MCx (bit 5)                                                */
    TACCTL0 &= ~(1 << 4);        /* Clear MCx (bit 4)                                                */
}
/*
*********************************************************************************************************
*********************************************************************************************************
*   LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)		/* Timer A0 interrupt service routine               */
#pragma vector=TIMERA0_VECTOR
__interrupt void  TmrA_Isr (void)
#else
void __attribute__ ((interrupt(TIMERA0_VECTOR))) TmrA_Isr (void)
#endif
{
    INT08U buf_wr[2];
    INT08U bat_level = 0;
    INT08U ret = 0;
    BOOLEAN bret;

    if (timer_a_divider == TMR_A_BAT_CHRG_DIVIDER){
        timer_a_divider = 0;
        #ifdef TMR_A_DEBUG
        CP_SPI_SIMO_3V3 = 1;                // {RD} I2C test
        #endif
        // KW: testing, multimaster collision detection
        //if((UCB0CTL0 & 0x08) == 0)
        //{
        //  Sys_BeepHigh(100);
        //  Sys_DelayMs(100);
        //}//testing end

        /* #####################################################################
         * {KW}: Reset charger chip on no respond 
         * ###################################################################*/
        if(BQ24150A_No_Response_Reset_Cmd)
          BQ24150A_No_Response_Reset_Cmd = 0;
        if(0) {
          BQ24150A_No_Response_Reset_Cmd = 0 ;           
          
          Pwr_ChargerPowerDisable();
          Sys_DelayMs(1000);

          Pwr_ChargerPowerEnable();
          Sys_DelayMs(10);

          bret=BQ24150A_ChargerDisable(CHGR_OFF);
          if(bret == FALSE){
            #ifdef TMR_A_DEBUG
            Sys_DelayMs(200);
            CP_SPI_SIMO_3V3 = 0;                // {RD} I2C test
            #endif
            #ifdef ENABLE_I2C_RECOVERY
            I2C2_EN     = 1;
            #endif
            return;
          }
          bret=BQ24150A_ChargerInit(BQ24150A_I_IN_LIMIT_NO);
          if(bret == FALSE){
            #ifdef TMR_A_DEBUG
            Sys_DelayMs(200);
            CP_SPI_SIMO_3V3 = 0;                // {RD} I2C test
            #endif
            #ifdef ENABLE_I2C_RECOVERY
            I2C2_EN     = 1;
            #endif
            return;
          }
          bret=BQ24150A_ChargerEnable(CHGR_ON);
          if(bret == FALSE){
            #ifdef TMR_A_DEBUG
            Sys_DelayMs(200);
            CP_SPI_SIMO_3V3 = 0;                // {RD} I2C test
            #endif
            #ifdef ENABLE_I2C_RECOVERY
            I2C2_EN     = 1;
            #endif
            return;
          }
          #ifdef ENABLE_CHRG_TONE
          Sys_BeepHigh(1000);      //KW: Indicate recovery from I2C fail
          #endif
          #ifdef ENABLE_I2C_RECOVERY
          I2C2_EN     = 1;
          #endif
        }

        /* #####################################################################
         * {KW}: 32sec timer reset
         * ###################################################################*/        
        buf_wr[0] = BQ24150A_SAT_CNTRL_REG;                               /* Control Register address         */
        buf_wr[1] = BQ24150A_TMR_RST_OTG + BQ24150A_EN_STAT;
        do{  
            ret = I2C_Write (I2C_0_HANDLE, BQ24150A_I2C_ADDR, buf_wr, 2); /* Return value is not checked      */
            if(ret == I2C_FAULT){
            #ifdef TMR_A_DEBUG
            Sys_DelayMs(200);
            CP_SPI_SIMO_3V3 = 0;                // {RD} I2C test
            #endif
            return;
          }
        }while (ret == I2C_BUSY);
        
       /* ######################################################################
        * {KW}: Read battery level 
        * ####################################################################*/
        bat_level = Bat_GetBatteryLevel(); 
       if(bat_level == BAT_LEVEL_ERROR){
            #ifdef TMR_A_DEBUG
            Sys_DelayMs(200);
            CP_SPI_SIMO_3V3 = 0;                // {RD} I2C test
            #endif
            return;
          }
       /************************************************************************ 
        * (1) {KW}: Battery level Short Circuit 
        ***********************************************************************/
        if(BAT_LEVEL_SHORT_CCT == bat_level){
          /* {KW}: if we didnt go through bat_dead level, do the following also */
          if(charger_activity != CHGR_WENT_DEAD){  
            /*{KW}: BAT_DEAD beep */
            Sys_BeepHigh(50);
            Sys_DelayMs(200);
            Sys_BeepHigh(50);
            Sys_DelayMs(200);
            Sys_BeepHigh(50);
            
            Pwr_Bat_Good = 0;        

            charger_activity = CHGR_WENT_DEAD; 
          }
        }
        
        /***********************************************************************
         * (2) {KW}: Battery level detected below DEAD for the first time
         * Implies that the battery isn't charging from Short Circuit Status 
         * It is discharging from GOOD level 
         **********************************************************************/
        if((charger_activity != CHGR_WENT_DEAD) && (BAT_LEVEL_DEAD == bat_level)){
          /*{KW}: BAT_DEAD beep */
          Sys_BeepHigh(50);
          Sys_DelayMs(200);
          Sys_BeepHigh(50);
          Sys_DelayMs(200);
          Sys_BeepHigh(50);

          Pwr_Bat_Good = 0;         

          charger_activity = CHGR_WENT_DEAD; 
        }
        
        /***********************************************************************
         * (3) {KW}: Battery level is beyond FULL threshold 
         **********************************************************************/
        else if(BAT_LEVEL_FULL == bat_level){
          /* {KW}: If the battery level rising beyong the FULL threshold for the first time,
           * then disable charging
           */
          if(charger_activity != CHGR_WENT_FULL){
            bret = BQ24150A_ChargerDisable(CHGR_WENT_FULL);
            if(bret == FALSE){
            #ifdef TMR_A_DEBUG
            Sys_DelayMs(200);
            CP_SPI_SIMO_3V3 = 0;                // {RD} I2C test
            #endif
            return;
          }
          }          
        }
        
        /***********************************************************************
        * (4) {KW}: Battery level is GOOD 
        **********************************************************************/
        else if(BAT_LEVEL_GOOD == bat_level){
          /* {KW}: If the battery level dropped from FULL to GOOD
           * re-start charging 
           */
          if(charger_activity == CHGR_WENT_FULL){ 
            bret =BQ24150A_ChargerInit(BQ24150A_I_IN_LIMIT_NO);  
            if(bret == FALSE){
            #ifdef TMR_A_DEBUG
            Sys_DelayMs(200);
            CP_SPI_SIMO_3V3 = 0;                // {RD} I2C test
            #endif
            return;
          }
            bret =BQ24150A_ChargerEnable(CHGR_ON);
            if(bret == FALSE){
            #ifdef TMR_A_DEBUG
            Sys_DelayMs(200);
            CP_SPI_SIMO_3V3 = 0;                // {RD} I2C test
            #endif
            return;
          }
          }  
          
          charger_activity = CHGR_ON; 
          Pwr_Bat_Good = 1; 
        } 
        
        /* #####################################################################
         * {KW}: Get Charger Status
         * ###################################################################*/
        ret = BQ24150A_GetStatus();
        
        if(ret == 0){
            #ifdef TMR_A_DEBUG
            Sys_DelayMs(200);
            CP_SPI_SIMO_3V3 = 0;                // {RD} I2C test
            #endif
            return;
          }
          
        /* {KW}: If any FAULT conditions, re-start charge */
        #ifdef CHK_DEFAULT_PARAMS
        if(((ret & 0x07) != BQ24150A_FAULT_NORMAL) || ((ret & 0x30) == BQ24150A_STAT_FAULT) || (BQ24150A_is_default_params_det()))
        #else
        if(((ret & 0x07) != BQ24150A_FAULT_NORMAL) || ((ret & 0x30) == BQ24150A_STAT_FAULT))
        #endif
        { 
          #ifdef CHK_DEFAULT_PARAMS
          if(BQ24150A_No_Response_Reset_Cmd){
            #ifdef TMR_A_DEBUG
            Sys_DelayMs(200);
            CP_SPI_SIMO_3V3 = 0;                // {RD} I2C test
            #endif
            return;
          }
          #endif
          
          bret =BQ24150A_ChargerDisable(charger_activity); // do not change charger_activity value here
          if(bret == FALSE){
            #ifdef TMR_A_DEBUG
            Sys_DelayMs(200);
            CP_SPI_SIMO_3V3 = 0;                // {RD} I2C test
            #endif
            return;
          }
          bret =BQ24150A_ChargerInit(BQ24150A_I_IN_LIMIT_NO);
          if(bret == FALSE){
            #ifdef TMR_A_DEBUG
            Sys_DelayMs(200);
            CP_SPI_SIMO_3V3 = 0;                // {RD} I2C test
            #endif
            return;
          }
          bret =BQ24150A_ChargerEnable(charger_activity);  // do not change charger_activity value here         
          if(bret == FALSE){
            #ifdef TMR_A_DEBUG
            Sys_DelayMs(200);
            CP_SPI_SIMO_3V3 = 0;                // {RD} I2C test
            #endif
            return;
          }
        }
        
        /* {KW}: Indicate charger status via beeps. (For debug purposes) */
        switch (ret & 0x30){  /* bt 5,4 = STAT2, STAT1 */
            case BQ24150A_STAT_READY:
                {
                    #ifdef ENABLE_CHRG_TONE   
                  //Sys_BeepHigh(20);  // {RD} DEMO
                    #endif
                    break;
                }

            case BQ24150A_STAT_CHARGE_IN_PROGRESS:
                {
                    #ifdef ENABLE_CHRG_TONE                   
                    Sys_BeepHigh(20); 
                    Sys_DelayMs(200);
                    Sys_BeepHigh(20); 
                    #endif
                    // __low_power_mode_off_on_exit();     /* Exit LPM0                                */
                    break;
                }

            case BQ24150A_STAT_CHARGE_DONE:
                {
                    #ifdef ENABLE_CHRG_TONE
                    Sys_BeepHigh(20); 
                    Sys_DelayMs(200);
                    Sys_BeepHigh(20); 
                    Sys_DelayMs(200);
                    Sys_BeepHigh(20); 
                    #endif
                    break;
                }

            case BQ24150A_STAT_FAULT:
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
                    break;
                }

            default:
                {                  
                  #ifdef ENABLE_CHRG_TONE
//                    Sys_BeepHigh(750);
                    Sys_BeepHigh(250);
                    Sys_DelayMs(200); 
                    Sys_BeepHigh(250);
                  #endif
                    break;
                }
        }
      #ifdef TMR_A_DEBUG
      Sys_DelayMs(200);
      CP_SPI_SIMO_3V3 = 0;                // {RD} I2C test
      #endif
    }
    else{
        timer_a_divider++;
    }
}
