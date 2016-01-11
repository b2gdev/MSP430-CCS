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
BOOLEAN hold_at_reset       = 1;

BOOLEAN is_power_switch_on  = 0;

BOOLEAN chgrDnBeep = TRUE;

INT08U CC_Pwr_Status       	= 0;
BOOLEAN is_pwr_status_gpio_stable = FALSE;


/* Used for TPS pwr pin toggle */
INT32U time_stamp_start = 0xffffffff;

void (*new_main_func) (void);

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
*********************************************************************************************************
*/
int  main (void)
{
    INT32U debounce_cnt;                /* CC Pwr stable GPIO debounce */   
    INT08U brl_disp_str[24] = {0,0,0,0,0,1,0,0,0,0,17,69,0,0,0,0,231,205,128,0,0,0,250,90}; //Booting mesg: "Starting"
    INT08U brl_disp_fw_update[24] = {0x00,0x0c,0x00,0x04,0x00,0x01,0x00,0x00,0x49,0x40,0x11,0x07,0x00,0x02,0xcc,0xd4,0xe7,0x24,0x80,0x01,0x5b,0x6c,0xfa,0x7d}; //Firmware updating mesg: "Updating Firmware"
    INT08U battery_level        = 0x00;

    WDTCTL = WDTPW + WDTHOLD;           /* Stop watchdog timer to prevent time out reset            */

    if(health_check()){					/* Check weather metadata is ok								*/
    	new_main_func = (void *) (get_reset_address());
    	new_main_func();				/* Go to new code											*/
    }

    write_int_vec_data();				/*	Write interrupt code space data							*/

    Sys_GpioInit();                     /* Initialize GPIO                                          */

    CP_BATPWR_ENABLE();                 /* Enable Power supply 1 for companian processor            */
    CP_USBPWR_ENABLE();                 /* Enable Power supply 2 for companian processor            */

    Pwr_SystemPowerDisable();           /* Disable system power                                     */

    Pwr_CC_Pwr_Status_Update(CC_PWR_OFF); //Initialize to power off status.
	Sys_Set_System_Status(FALSE);
	is_pwr_status_gpio_stable = FALSE;
    
    CP_USBOTG_S_TPS();                 	/* USB switch connected to TPS65950                         */

    i2c_timeout_count = 0;
    I2C2_ENABLE();						/* I2C level converter on                                   */
    
    Clk_DCOInit();                      /* Initialize DCO                                           */

    ADC12_Init();                       /* Initialize ADC12                                         */
    #ifdef CHK_PRG_RESET
    Clk_ACLK_div(ACLK_DIV_8);			/* ACLK divide                                              */
    Sys_BeepHigh(1000);                 /* High frequency beep,                                     */
    #endif

	TmrA_Init(TMR_A_MODE_BAT_CHARGER);

	Sys_DelayMs(500);
	Sys_DelayMs(500);

	Pwr_SwConf();

	Pwr_ChargerPowerEnable();

	#ifdef  ENABLE_KEYPAD
	Kpd_Init();                 		/* Initialize keypad         	                            */
	TmrB_Init();                		/* Initialize keypad sampling timer    		                */
	#endif

	Sys_DelayMs(250);

	if(get_first_time_run()){			/* Auto start on the first time after a firmwre update		*/
		KEYPAD_MainKeys = (PWR_SW) ?  (PWR_KEY_BIT) : (0x00000000);

		is_power_switch_on = (KEYPAD_MainKeys & PWR_KEY_BIT) ? 1 : 0;

		Brd_Pwr_Update(Pwr_Get_CC_Pwr_Status());

		battery_level = Bat_GetInitialBatteryLevel();

		if((!CP_VBUS_OTG_DET) && ((battery_level == BAT_LEVEL_SHORT_CCT) || (battery_level == BAT_LEVEL_DEAD) || (battery_level == BAT_LEVEL_WEAK))){
			TmrB_IntDisable();			//Do not alllow timerB interrupt to run until the BEEPS are over

			/* Weak battery indication      */
			Sys_BeepHigh(100);
			Sys_DelayMs(200);
			Sys_BeepHigh(100);
			Sys_DelayMs(200);
			Sys_BeepHigh(100);

			TmrB_IntEnable();
		}else{
			Sys_BeepOn(); 			// Startup indication beep
			CC_CP_PWRON_HIGH();
			time_stamp_start = Sys_Get_Heartbeat();
			power_switch_toggle = 1;
			hold_at_reset = 0;
		}
	}

	/* Wait for power key OFF to ON transition after first time run*/
	while(hold_at_reset){
    	if(CP_VBUS_OTG_DET){
    		__low_power_mode_3();		/* Stay at LMP3 to support chrging when charger is plugged  */
    	}
		else{
			__low_power_mode_4();		/* Stay at LMP4 to when charger is not plugged 				*/
		}
    	Sys_DelayMs(250);
    }

	set_first_time_run_false();

	Pwr_SystemPowerEnable();			/* This will power on the main processor.Place this after
										   switch transition										*/

	CP_PWR01_ENABLE();             		/* Enable Braille display                        			*/

	#ifdef ENABLE_DISPLAY
	Brd_Init();                 		/* Initialize Braille display                               */
	BRD_ENABLE();
	#endif

	#ifdef ENABLE_HOST_INTERFACE
	EN_1V8_3V3_LVL_TR();     			/* Enable 1.8V to 3.3V translator 							*/
	Sys_DelayMs(1000);
	#endif
    
    while (1)
    {
    	/********************** Process Commands *************************/
    	if((is_pwr_status_gpio_stable == TRUE) && (OMAP_STATUS_1)){
    		if(Sys_Get_System_Status() == TRUE){
    			#ifdef  ENABLE_HOST_INTERFACE
    			if(Is_Pwr_SystemPowerEnabled()){
    				Cmd_RxCommandProcess();
    				Cmd_TxCommandProcess();

    				if(meta_data_ok && meta_data_ack_sent){
						__bic_SR_register(GIE);

						while(OMAP_STATUS_1){}
						Brd_WriteDisplay(brl_disp_fw_update);
						write_code_from_flash();

						__bis_SR_register(GIE);
    				}
    			}else{}
			  	#endif
    											/* Go LMP at suspend state depending on the power switch    */
    			if(Pwr_Get_CC_Pwr_Status() == CC_PWR_SUSPEND){
    				if(PWR_SW)
    					__low_power_mode_1();
    				else
    					__low_power_mode_3();
    			}else{}
    		}
    		else{
    			/* Init Brd asap */
    			CP_PWR01_ENABLE(); 				/* Re enable Brd power supply 								*/
    			Pwr_CC_Pwr_Status_Update(CC_PWR_ACTIVE);

				Sys_BeepHigh(100);    			/*  System bootup indication Beep2            			    */
				Sys_DelayMs(50);
				Sys_BeepHigh(100);

				Brd_WriteDisplay(brl_disp_str);

				Cmd_Init();            			/* Initialize command protocol         					    */
				while (!MCSPI1_CLK_3V3){
					;                 			/* Wait until SPI master is alive      				        */
				}
				SPI_Init();
				Sys_Set_System_Status(TRUE);
				mp_resetting = 0;
    		}
	  }
      else{
    	  is_pwr_status_gpio_stable = FALSE;
    	  if(Sys_Get_System_Status() == TRUE){
    		  Pwr_CC_Pwr_Status_Update(CC_PWR_OFF);
    		  Sys_Set_System_Status(FALSE);
    		  if(!mp_resetting && !OMAP_STATUS_2){/* De-Init pheriperals before going to LMP at shutdown    */
				  ADC12_DeInit();				  /* to reduce power consumption							*/
				  SPI_DeInit();
				  I2C_DeInit();
				  Sys_ShutDownLPInit();
				  if(CP_VBUS_OTG_DET){
					  ADC12_Init();
					  I2C2_ENABLE();			/* I2C level converter on                                   */
					  __low_power_mode_3();		/* Stay at LMP3 to support chrging when charger is plugged  */
				  }
				  else{
					  __low_power_mode_4();
					  ADC12_Init();
					  I2C2_ENABLE();			/* I2C level converter on                                   */
				  }
				  Sys_ShutDownLPDeInit();		/* Take the pins back to their assigned states. SPI and I2C */
			  }else{}							/* re-inits are done at neceassary places				    */
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

#ifdef I2C_ISSUE_HANDLING_AUTO
     if(i2c_timeout_count >= I2C_ISSUE_FOUND_THRESHOLD){
    	 I2C_Issue_Handle();
     }else if(i2c_timeout_count >= I2C_TIMEOUTS_FOUND_THRESHOLD){
    	 I2C2_DISABLE();						/* I2C level converter off                                  */
    	 if(timer_a_mode != TMR_A_MODE_4_SEC){
			 TmrA_Init(TMR_A_MODE_4_SEC);
		 }
     }else{
    	 I2C2_ENABLE();							/* I2C level converter on                                   */
    	 if(timer_a_mode != TMR_A_MODE_BAT_CHARGER){
    		 TmrA_Init(TMR_A_MODE_BAT_CHARGER);
    	 }
     }
#endif
    }/* end of main while loop */
}
