/*
*********************************************************************************************************
*   MAIN
*   Main Function
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
*   Main Function
*
*   File Name     : cp_main.c
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
#include "cp_conf.h"

/*
*********************************************************************************************************
*   LOCAL DEFINES
*********************************************************************************************************
*/
#define DEBOUNCE_TIME_100MS         1600000
#define DEBOUNCE_TIME_500MS         8000000
#define DEBOUNCE_TIME_1000MS       16000000

#define CHK_PRG_RESET

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
INT08U battery_level        = 0x00;
INT08U charger_status       = BQ24150A_STAT_READY;
INT08U charger_status_prev  = BQ24150A_STAT_READY;

BOOLEAN Pwr_Sw_On           = 0;
BOOLEAN Pwr_Bat_Good        = 0;

INT08U CC_Pwr_Status       = 0; 
BOOLEAN is_pwr_status_gpio_stable = FALSE; 

BOOLEAN BQ24150A_No_Response_Reset_Cmd = 0; 

/*{KW}: latest charger activity */
INT08U charger_activity = CHGR_OFF;

/*{KW}: used for TPS pwr pin toggle */
INT32U time_stamp_start = 0xffffffff;

extern BOOLEAN is_power_switch_on;

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

/*
*********************************************************************************************************
*********************************************************************************************************
*   LOCAL FUNCTIONS
*********************************************************************************************************
**************************************************************
*******************************************
*/
int  main (void)
{
    INT32U debounce_cnt;                /* CC Pwr stable GPIO debounce */   
    INT08U brl_disp_str[24] = {0,0,0,0,0,1,0,0,0,0,
                                17,69,0,0,0,0,231,205,128,0,
                                0,0,250,90}; // {RD} Booting mesg: "Starting"

    WDTCTL = WDTPW + WDTHOLD;           /* Stop watchdog timer to prevent time out reset            */
    
    Sys_GpioInit();                     /* Initialize GPIO                                          */
    
    CP_SPI_SIMO_3V3_LOW();                // {RD} I2C test
    
    CP_BATPWR_ENABLE();                  /* Enable Power supply 1 for companian processor            */
    CP_USBPWR_ENABLE();                  /* Enable Power supply 2 for companian processor            */

    Pwr_SystemPowerDisable();           /* Disable system power                                     */    
    
    CP_USBOTG_S_TPS();                    /* USB switch connected to TPS65950                         */
    I2C2_ENABLE();
    
    Clk_DCOInit();                      /* Initialize DCO                                           */
    ADC12_Init();                       /* Initialize ADC12                                         */    
    #ifdef CHK_PRG_RESET
    Clk_ACLK_div(ACLK_DIV_4); 
    Sys_BeepHigh(1000);                 /* High frequency beep, for debug purposes                  */
    #endif
    Clk_ACLK_div(ACLK_DIV_8);           /* ACLK divide                                              */ 
    
    #ifdef ENABLE_BATTERY
        Sys_DelayMs(500);
        Sys_DelayMs(500);
        battery_level = Bat_GetInitialBatteryLevel(); /*{KW}: no i2c communications is this */    // Bat_GetBatteryLevel();
    #else
        battery_level = BAT_LEVEL_GOOD;
    #endif
    
    switch (battery_level){
      case BAT_LEVEL_SHORT_CCT:    
      case BAT_LEVEL_DEAD:    
      case BAT_LEVEL_WEAK:    
            {
                /* Weak battery indication      */
                Sys_BeepHigh(50);      
                Sys_DelayMs(200);
                Sys_BeepHigh(50);
                Sys_DelayMs(200);
                Sys_BeepHigh(50);
                
                /* Wait until chrger detected */
                while (!CP_VBUS_OTG_DET){
                //  __low_power_mode_0();   /* Stay in LPM0   */
                  Sys_DelayMs(250);
                }

                /* Disable CP_BATPWR_nEN if battery is DEAD or SHORT_CCT */
                if(Bat_GetInitialBatteryLevel() != BAT_LEVEL_WEAK)
                {
                  CP_USBPWR_ENABLE();      /* Enable Power supply 2 for companian processor            */
                  CP_BATPWR_DISABLE();      /* Disable Power supply 1 for companian processor           */
                }
                
                Pwr_ChargerPowerEnable();
                Sys_DelayMs(1); 
                
                BQ24150A_ChargerDisable(CHGR_WENT_DEAD);
 //               BQ24150A_ChargerInit(BQ24150A_I_IN_LIMIT_500MA);
                BQ24150A_ChargerInit(BQ24150A_I_IN_LIMIT_NO);  
                BQ24150A_ChargerEnable(CHGR_WENT_DEAD);

                TmrA_Init(TMR_A_MODE_BAT_CHARGER);
                
                while (Pwr_Bat_Good != 1){ 
                    /* wait until battery level is good */
                   // __low_power_mode_0();   /* Stay in LPM0                                         
                }  
                
                /* {PS} BEGIN: */
                CP_USBPWR_ENABLE();      /* Enable Power supply 2 for companian processor            */
                CP_BATPWR_ENABLE();      /* Enable Power supply 1 for companian processor            */
                /* {PS} END: */
                
                /* battery is good now, fall through to next case */
            }
        
       case BAT_LEVEL_GOOD:
       case BAT_LEVEL_FULL:  
            {   
                /* check if the charger is already on       */
                if ((BAT_LEVEL_SHORT_CCT != battery_level) || 
                    (BAT_LEVEL_DEAD != battery_level) || 
                    (BAT_LEVEL_WEAK != battery_level)) 
                {  
                    Pwr_ChargerPowerEnable();
                    Sys_DelayMs(1); 

                    if(BAT_LEVEL_GOOD == battery_level){
                      BQ24150A_ChargerDisable(CHGR_OFF);
   //                   BQ24150A_ChargerInit(BQ24150A_I_IN_LIMIT_500MA);
                      BQ24150A_ChargerInit(BQ24150A_I_IN_LIMIT_NO); 
                      BQ24150A_ChargerEnable(CHGR_ON);                    
                    }
                    else if(BAT_LEVEL_FULL == battery_level){
                      BQ24150A_ChargerDisable(CHGR_WENT_FULL);   
                    }
                    TmrA_Init(TMR_A_MODE_BAT_CHARGER);                    
                }                
                
                    
                /* {RD}: wait for power key OFF to ON transition */                
                while(PWR_SW)
                  Sys_DelayMs(10);                
                while(!PWR_SW)
                  Sys_DelayMs(50);                                              
                
                Sys_BeepHigh(125);    /* {RD} System bootup indication Beep1                */
                
                is_power_switch_on = PWR_SW; /*{KW}: Initial status of the power switch              */
                
                CP_PWR01_ENABLE();             /* {PS} : Enable Braille display                        */
                
                #ifdef  ENABLE_KEYPAD
                Kpd_Init();                 /* Initialize keypad                                    */
                TmrB_Init();                /* Initialize keypad sampling timer                     */
                #endif
                
                #ifdef ENABLE_DISPLAY
                Brd_Init();                 /* Initialize Braille display                           */                
                BRD_ENABLE();
                #endif
                
                #ifdef ENABLE_HOST_INTERFACE
                EN_1V8_3V3_LVL_TR();     		/* Enable 1.8V to 3.3V translator */
                Sys_DelayMs(1000);               
                Pwr_SystemPowerEnable();  
                Pwr_CC_Pwr_Status_Update(CC_PWR_OFF); /*{KW}: Initialize to power offf status. This will be updated when teh CC pwr GPIO is stable */
                Sys_Set_System_Status(FALSE);  
                is_pwr_status_gpio_stable = FALSE;                
                #endif
                break;
            }
            
        default:
            {
                break;
            }
    }/* end of switch case */    
    
    while (1)
    {         
      /********************** Process Commands *************************/
      if((is_pwr_status_gpio_stable == TRUE) && (OMAP_STATUS_1)){
        if(Sys_Get_System_Status() == TRUE){
          #ifdef  ENABLE_HOST_INTERFACE
          if(Is_Pwr_SystemPowerEnabled()){ 
            Cmd_RxCommandProcess();
            Cmd_TxCommandProcess();
          }  
          #endif
        }  
        else{
          /* {RD} Init Brd asap */
          CP_PWR01_ENABLE(); /* {KW}: re enable Brd power supply */
          Pwr_CC_Pwr_Status_Update(CC_PWR_ACTIVE);
          //Brd_ClearDisplay();

          /* {RD} System bootup indication Beep2                */
          Sys_BeepHigh(100);    
          Sys_DelayMs(50);
          Sys_BeepHigh(100);
          
          //Brd_ClearDisplay();                   
          Brd_WriteDisplay(brl_disp_str);
  
          Cmd_Init();            /* Initialize command protocol             */ 
          while (!MCSPI1_CLK_3V3){
              ;                 /* Wait until SPI master is alive           */ 
          } 
          SPI_Init();
          Sys_Set_System_Status(TRUE);
        }
      }
      else{
        is_pwr_status_gpio_stable = FALSE;
        if(Sys_Get_System_Status() == TRUE){
          Pwr_CC_Pwr_Status_Update(CC_PWR_OFF); 
          Sys_Set_System_Status(FALSE);
        }
        else{
          debounce_cnt = 0;
          while((debounce_cnt < DEBOUNCE_TIME_500MS) && OMAP_STATUS_1){
            debounce_cnt++;
          }          
          if(debounce_cnt >= DEBOUNCE_TIME_500MS){
            is_pwr_status_gpio_stable = TRUE;
          }          
        }
      }
    }/* end of main while loop */
}
