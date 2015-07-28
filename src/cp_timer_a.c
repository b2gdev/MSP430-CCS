/*
*********************************************************************************************************
*   Timer A
*   Timer A Functions
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
#define     TMR_A_BAT_CHRG_CCR0         0x7FFF
#define     TMR_A_BAT_CHRG_DIVIDER      0
                                            /* Battery Charger Timer Period - 8 sec                 */
                                            /* Clock Input - 32768 Hz                               */
                                            /* Input Divider - 8                                    */
                                            /* CCR0 - 0x7FFF (32767)                                */

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
INT16U timer_a_divider = 0;

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

        TACTL   = TASSEL_1 + ID_0 + MC_1;
                                /* Unused       : 000000                                            */
                                /* TASSELx      : 01     - Clock source - ACLK                      */
                                /* IDx          : 00     - Input divider - 1                        */
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
	#ifdef USE_BQ24160
	INT08U statCtrlReg;
	INT08U batSplyStatReg;
	INT08U NTCReg;
	INT08U DPPMReg;
	INT08U chgrStatus;
	INT08U MINSYSMode;
	INT08U VINDPMMode;
	#else
	INT08U bat_level = 0;
	#endif /* USE_BQ24160 */
    INT08U buf_wr[2];
    INT08U ret = 0;
    BOOLEAN bret;

    if (timer_a_divider == TMR_A_BAT_CHRG_DIVIDER){
        timer_a_divider = 0;

#ifdef USE_BQ24160
        /* #####################################################################
		 *  Check for faults
		 * ###################################################################*/
        statCtrlReg = BQ24160_GetStatCtrlReg();
        batSplyStatReg = BQ24160_GetBatStatReg();
        NTCReg = BQ24160_GetTmrNTCReg();
        if(NTCReg == 0){
        	return;
        }

        bret = BQ24160_handle_faults(statCtrlReg,batSplyStatReg,NTCReg);
        if(bret == FALSE){
        	return;
        }


        /* #####################################################################
		 *  Check for status
		 * ###################################################################*/
        chgrStatus = statCtrlReg & 0x70;
        DPPMReg = BQ24160_GetDPPMReg();
		MINSYSMode = DPPMReg & BQ24160_MINSYS_STATUS;
		VINDPMMode = DPPMReg & BQ24160_DPM_STATUS;

		if(chgrStatus == BQ24160_STAT_FAULT){
			#ifdef ENABLE_CHRG_TONE
			Sys_BeepHigh(100);
			#endif

			bret = BQ24160_ChargerInit(BQ24160_IUSB_LIMIT_NA);
			if(bret == FALSE){
				return;
			}

			bret = BQ24160_ChargerEnable(BQ24160_IUSB_LIMIT_NA);
			if(bret == FALSE){
				return;
			}
		}else if(chgrStatus == BQ24160_STAT_CHARGE_DONE){
			Sys_BeepHigh(100);
			Sys_DelayMs(200);
			Sys_BeepHigh(100);
		}else if(MINSYSMode){
			#ifdef ENABLE_CHRG_TONE
			Sys_BeepHigh(100);
			Sys_DelayMs(200);
			Sys_BeepHigh(100);
			Sys_DelayMs(200);
			Sys_BeepHigh(100);
			#endif
		}else if(VINDPMMode){
			#ifdef ENABLE_CHRG_TONE
			Sys_BeepHigh(100);
			Sys_DelayMs(200);
			Sys_BeepHigh(100);
			Sys_DelayMs(200);
			Sys_BeepHigh(100);
			Sys_DelayMs(200);
			Sys_BeepHigh(100);
			#endif
		}else{}

		#ifdef CHK_DEFAULT_PARAMS
        if(BQ24160_is_default_params_det()){
			bret = BQ24160_ChargerInit(BQ24160_IUSB_LIMIT_NA);
			if(bret == FALSE){
				return;
			}

			bret = BQ24160_ChargerEnable(BQ24160_IUSB_LIMIT_NA);
			if(bret == FALSE){
				return;
			}
        }
		#endif


        /* #####################################################################
		 *  Watchdog timer reset
		 * ###################################################################*/
		buf_wr[0] = BQ24160_SAT_CNTRL_REG;
		buf_wr[1] = BQ24160_TMR_RST + BQ24160_SUPPLY_SEL;
   	   							/* B3 (SUPPLY_SEL = 1) - USB has precedence when both supplies are connected    */
		do{
			ret = I2C_Write (I2C_0_HANDLE, BQ24150A_I2C_ADDR, buf_wr, 2);
			if(ret == I2C_FAULT){
				return;
			}
		}while (ret == I2C_BUSY);

#else /* if bq24150a */

        /* #####################################################################
         * Get Charger Status
         * ###################################################################*/
        ret = BQ24150A_GetStatus();

        /*if(ret == 0){   // Commented due to not reading some bits at status reg through I2C
        	return;
        }*/

		/* In any FAULT conditions, re-start charge */
		#ifdef CHK_DEFAULT_PARAMS
		if(((ret & 0x07) != BQ24150A_FAULT_NORMAL) || ((ret & 0x30) == BQ24150A_STAT_FAULT) || (BQ24150A_is_default_params_det()))
		#else
		if(((ret & 0x07) != BQ24150A_FAULT_NORMAL) || ((ret & 0x30) == BQ24150A_STAT_FAULT))
		#endif
		{
			bret =BQ24150A_ChargerInit(BQ24150A_I_IN_LIMIT_NO);
			if(bret == FALSE){
				return;
			}
		}

		switch (ret & 0x30){  /* Bit 5,4 = STAT2, STAT1 */
			case BQ24150A_STAT_READY:
			{
				break;
			}
			case BQ24150A_STAT_CHARGE_IN_PROGRESS:
			{
				#ifdef ENABLE_CHRG_TONE
				Sys_BeepHigh(20);
				Sys_DelayMs(200);
				Sys_BeepHigh(20);
				#endif
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
				Sys_BeepHigh(250);
				Sys_DelayMs(200);
				Sys_BeepHigh(250);
				#endif
				break;
			}
		}

        /* #####################################################################
         *  32sec timer reset
         * ###################################################################*/
        buf_wr[0] = BQ24150A_SAT_CNTRL_REG;                               /* Status Register address         */
        buf_wr[1] = BQ24150A_TMR_RST_OTG + BQ24150A_EN_STAT;			  // Status register bits B6 - EN_STAT, (B5,B4) - Charge status bits are not properly maintained
        do{
            ret = I2C_Write (I2C_0_HANDLE, BQ24150A_I2C_ADDR, buf_wr, 2);
            if(ret == I2C_FAULT){
            	return;
            }
        }while (ret == I2C_BUSY);


       /* ######################################################################
        *   Read battery level
        * ####################################################################*/
       bat_level = Bat_GetBatteryLevel();
       if(bat_level == BAT_LEVEL_ERROR){
    	   return;
       }

       if(BAT_LEVEL_SHORT_CCT == bat_level){
    	   #ifdef ENABLE_CHRG_TONE
    	   /* BAT_SHORT_CCT beeps */
    	   Sys_BeepHigh(50);
    	   Sys_DelayMs(200);
    	   Sys_BeepHigh(50);
    	   Sys_DelayMs(200);
    	   Sys_BeepHigh(50);
    	   Sys_DelayMs(200);
    	   Sys_BeepHigh(50);
		   #endif

           bret =BQ24150A_ChargerEnable();
		   if(bret == FALSE){
			   return;
		   }
       }
       else if(BAT_LEVEL_DEAD == bat_level){
    	   #ifdef ENABLE_CHRG_TONE
    	   /* BAT_DEAD beeps */
    	   Sys_BeepHigh(50);
    	   Sys_DelayMs(200);
    	   Sys_BeepHigh(50);
    	   Sys_DelayMs(200);
    	   Sys_BeepHigh(50);
		   #endif

    	   bret =BQ24150A_ChargerEnable();
		   if(bret == FALSE){
			   return;
		   }
       }
       else if(BAT_LEVEL_WEAK == bat_level){
		   #ifdef ENABLE_CHRG_TONE
    	   /* BAT_WEAK beeps */
    	   Sys_BeepHigh(50);
    	   Sys_DelayMs(200);
    	   Sys_BeepHigh(50);
		   #endif

    	   bret =BQ24150A_ChargerEnable();
		   if(bret == FALSE){
			   return;
		   }
	   }
       else if(BAT_LEVEL_GOOD == bat_level){
    	   bret =BQ24150A_ChargerEnable();
    	   if(bret == FALSE){
    		   return;
    	   }
	   }
       else if(BAT_LEVEL_FULL == bat_level){
			bret = BQ24150A_ChargerDisable();	/* If the battery level is FULL threshold then disable charging */
			if(bret == FALSE){
				return;
			}
       }else{}
#endif /* USE_BQ24160 */
    }
    else{
        timer_a_divider++;
    }
}
