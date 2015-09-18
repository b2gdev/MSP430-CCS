/*
*********************************************************************************************************
*   KEYPAD
*   KEYPAD Functions
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
*   KEYPAD Functions
*
*   File Name     : cp_keypad.h
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
#define KEYPAD_MODULE                /* Define Keypad module, this is to handle global variables
                                       declaration and externs appropriately                        */

#include "cp_keypad.h"

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
BOOLEAN power_switch_toggle = 0;
BOOLEAN power_off_pressed = 0;
BOOLEAN power_reset_pressed = 0;
BOOLEAN mp_resetting = 0;

extern INT08U updateFirmware();

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
void  Kpd_Init (void)
{
    /* Initialize Keypad inputs */

    /* BR_KEY1 - P1.0, BR_KEY2 - P1.1, BR_KEY3 - P1.2, BR_KEY4 - P1.3, BR_KEY5 - P1.4, BR_KEY6 - P1.5, 
       BR_KEY7 - P1.6, BR_KEY8 - P1.7                                                               */
    P1DIR &= 0x00;              /* Input                                                            */
    P1SEL &= 0x00;              /* I/O function                                                     */
    P1REN |= 0xFF;              /* Pullup/Pulldown enabled                                          */
    P1OUT &= 0x00;              /* Pulldown selected                                                */
    
    /* BR_KEY9 - P2.0, PWR_SW - P2.1, KEYMAT_COL0 - P2.3, KEYMAT_COL1 - P2.4, KEYMAT_COL2 - P2.5    */
    P2DIR &= 0xC4;              /* Input                                                            */
    P2SEL &= 0xC4;              /* I/O function                                                     */
    P2REN |= 0x3B;              /* Pullup/Pulldown enabled                                          */
    P2OUT &= 0xC4;              /* Pulldown selected                                                */
    
    /* KEYMAT_ROW0 - P4.0, KEYMAT_ROW1 - P4.1, KEYMAT_ROW2 - P4.2                                   */
    P4DIR |= 0x07;              /* Output                                                           */
    P4SEL &= 0xF8;              /* I/O function                                                     */
    P4REN &= 0xF8;              /* Pullup/Pulldown disabled                                         */
    
    /* Initialize variables */
    KEYPAD_MainKeys          = 0;
    KEYPAD_KeyChange         = FALSE;
    memset(KEYPAD_MainKeys_Buf, 0, sizeof(KEYPAD_MainKeys_Buf));
    KEYPAD_SampleIndex = 0;    
}

void  Kpd_Read (void)
{
    INT32U prev_main_keys = 0;
    INT08U prev_brl_keys  = 0;
    INT08U brl_keys       = 0;
    
    prev_main_keys = KEYPAD_MainKeys;
    
    KEYPAD_MainKeys = (PWR_SW) ? (KEYPAD_MainKeys | PWR_KEY_BIT) : (KEYPAD_MainKeys & (~(PWR_KEY_BIT)));
    
    KEYPAD_MainKeys = (BR_KEY1) ? (KEYPAD_MainKeys | BR_KEY1_BIT) : (KEYPAD_MainKeys & (~(BR_KEY1_BIT)));
    KEYPAD_MainKeys = (BR_KEY2) ? (KEYPAD_MainKeys | BR_KEY2_BIT) : (KEYPAD_MainKeys & (~(BR_KEY2_BIT)));
    KEYPAD_MainKeys = (BR_KEY3) ? (KEYPAD_MainKeys | BR_KEY3_BIT) : (KEYPAD_MainKeys & (~(BR_KEY3_BIT)));
    KEYPAD_MainKeys = (BR_KEY4) ? (KEYPAD_MainKeys | BR_KEY4_BIT) : (KEYPAD_MainKeys & (~(BR_KEY4_BIT)));
    KEYPAD_MainKeys = (BR_KEY5) ? (KEYPAD_MainKeys | BR_KEY5_BIT) : (KEYPAD_MainKeys & (~(BR_KEY5_BIT)));
    KEYPAD_MainKeys = (BR_KEY6) ? (KEYPAD_MainKeys | BR_KEY6_BIT) : (KEYPAD_MainKeys & (~(BR_KEY6_BIT)));
    KEYPAD_MainKeys = (BR_KEY7) ? (KEYPAD_MainKeys | BR_KEY7_BIT) : (KEYPAD_MainKeys & (~(BR_KEY7_BIT)));
    KEYPAD_MainKeys = (BR_KEY8) ? (KEYPAD_MainKeys | BR_KEY8_BIT) : (KEYPAD_MainKeys & (~(BR_KEY8_BIT)));
    KEYPAD_MainKeys = (BR_KEY9) ? (KEYPAD_MainKeys | BR_KEY9_BIT) : (KEYPAD_MainKeys & (~(BR_KEY9_BIT)));
    
    BIT_SET(KEYMAT_ROWS,KEYMAT_ROW0);
    KEYPAD_MainKeys = (KEYMAT_COL0) ? (KEYPAD_MainKeys | VOL_UP_KEY_BIT)    : (KEYPAD_MainKeys & (~(VOL_UP_KEY_BIT)));
    KEYPAD_MainKeys = (KEYMAT_COL1) ? (KEYPAD_MainKeys | NAV_RIGHT_KEY_BIT) : (KEYPAD_MainKeys & (~(NAV_RIGHT_KEY_BIT)));
    KEYPAD_MainKeys = (KEYMAT_COL2) ? (KEYPAD_MainKeys | VOL_DOWN_KEY_BIT)  : (KEYPAD_MainKeys & (~(VOL_DOWN_KEY_BIT)));
    BIT_CLR(KEYMAT_ROWS,KEYMAT_ROW0);

    BIT_SET(KEYMAT_ROWS,KEYMAT_ROW1);
    KEYPAD_MainKeys = (KEYMAT_COL0) ? (KEYPAD_MainKeys | NAV_LEFT_KEY_BIT) : (KEYPAD_MainKeys & (~(NAV_LEFT_KEY_BIT)));
    KEYPAD_MainKeys = (KEYMAT_COL1) ? (KEYPAD_MainKeys | NAV_OK_KEY_BIT)   : (KEYPAD_MainKeys & (~(NAV_OK_KEY_BIT)));
    KEYPAD_MainKeys = (KEYMAT_COL2) ? (KEYPAD_MainKeys | NAV_DOWN_KEY_BIT) : (KEYPAD_MainKeys & (~(NAV_DOWN_KEY_BIT)));
    BIT_CLR(KEYMAT_ROWS,KEYMAT_ROW1);

    BIT_SET(KEYMAT_ROWS,KEYMAT_ROW2);
    KEYPAD_MainKeys = (KEYMAT_COL0) ? (KEYPAD_MainKeys | FWD_KEY_BIT)      : (KEYPAD_MainKeys & (~(FWD_KEY_BIT)));
    KEYPAD_MainKeys = (KEYMAT_COL1) ? (KEYPAD_MainKeys | NAV_UP_KEY_BIT)   : (KEYPAD_MainKeys & (~(NAV_UP_KEY_BIT)));
    KEYPAD_MainKeys = (KEYMAT_COL2) ? (KEYPAD_MainKeys | BWD_KEY_BIT)      : (KEYPAD_MainKeys & (~(BWD_KEY_BIT)));
    BIT_CLR(KEYMAT_ROWS,KEYMAT_ROW2);
    
    prev_brl_keys = (INT08U)(prev_main_keys  & 0x000000FF);
    brl_keys      = (INT08U)(KEYPAD_MainKeys & 0x000000FF);
    
    if (prev_brl_keys != 0x00){                 /* previously any braille key is pressed            */
        if (brl_keys == 0x00){                  /* all braille keys are up                          */
            KEYPAD_KeyChange = TRUE;
        }
    }
    else {                                      /* previously no braille key is pressed             */
        if (brl_keys != 0) {                    /* any brailley key is pressed                      */
            if (prev_brl_keys != brl_keys) {
                KEYPAD_KeyChange = TRUE;
            }
        }
    }
            
    if ((KEYPAD_MainKeys & 0xFFFFFF00) != (prev_main_keys & 0xFFFFFF00)) {
        KEYPAD_KeyChange = TRUE;
    }
}

void  Kpd_ReadEx (void)
{
    INT32U MainKeys = 0;
    INT08U i = 0;
    INT08U j = 0;
    INT08U match_count = 0;
    BOOLEAN enter_recovery = 0;
    
    INT32U prev_main_keys = 0;

    if (is_power_switch_on && !(Pwr_Get_CC_Pwr_Status()==CC_PWR_OFF)){
      MainKeys = (BR_KEY1) ? (MainKeys | BR_KEY1_BIT) : (MainKeys & (~(BR_KEY1_BIT)));
      MainKeys = (BR_KEY2) ? (MainKeys | BR_KEY2_BIT) : (MainKeys & (~(BR_KEY2_BIT)));
      MainKeys = (BR_KEY3) ? (MainKeys | BR_KEY3_BIT) : (MainKeys & (~(BR_KEY3_BIT)));
      MainKeys = (BR_KEY4) ? (MainKeys | BR_KEY4_BIT) : (MainKeys & (~(BR_KEY4_BIT)));
      MainKeys = (BR_KEY5) ? (MainKeys | BR_KEY5_BIT) : (MainKeys & (~(BR_KEY5_BIT)));
      MainKeys = (BR_KEY6) ? (MainKeys | BR_KEY6_BIT) : (MainKeys & (~(BR_KEY6_BIT)));
      MainKeys = (BR_KEY7) ? (MainKeys | BR_KEY7_BIT) : (MainKeys & (~(BR_KEY7_BIT)));
      MainKeys = (BR_KEY8) ? (MainKeys | BR_KEY8_BIT) : (MainKeys & (~(BR_KEY8_BIT)));
      MainKeys = (BR_KEY9) ? (MainKeys | BR_KEY9_BIT) : (MainKeys & (~(BR_KEY9_BIT)));
    }
    
    BIT_SET(KEYMAT_ROWS,KEYMAT_ROW0);
    if (!(Pwr_Get_CC_Pwr_Status()==CC_PWR_OFF) && (is_power_switch_on || !(Pwr_Get_CC_Pwr_Status()==CC_PWR_SUSPEND))){
        MainKeys = (KEYMAT_COL0) ? (MainKeys | VOL_UP_KEY_BIT)    : (MainKeys & (~(VOL_UP_KEY_BIT)));
    }
    if (is_power_switch_on && !(Pwr_Get_CC_Pwr_Status()==CC_PWR_OFF)){
       MainKeys = (KEYMAT_COL1) ? (MainKeys | NAV_RIGHT_KEY_BIT) : (MainKeys & (~(NAV_RIGHT_KEY_BIT)));
    }
    if (!(Pwr_Get_CC_Pwr_Status()==CC_PWR_OFF) && (is_power_switch_on || !(Pwr_Get_CC_Pwr_Status()==CC_PWR_SUSPEND))){
       MainKeys = (KEYMAT_COL2) ? (MainKeys | VOL_DOWN_KEY_BIT)  : (MainKeys & (~(VOL_DOWN_KEY_BIT)));
    }
    
    enter_recovery = (KEYMAT_COL2);
    /*{AH}:: CP_STATUS_1 drives MP to recovery mode during boot up */
	#ifdef RECOVERY_MODE_ENTERING_DEFINED
	if(enter_recovery){
		CP_STATUS_1_HIGH();
	}else{
		CP_STATUS_1_LOW();
	}
	#endif

    BIT_CLR(KEYMAT_ROWS,KEYMAT_ROW0);
    __delay_cycles(1000);
    BIT_SET(KEYMAT_ROWS,KEYMAT_ROW1);
    
    if (is_power_switch_on && !(Pwr_Get_CC_Pwr_Status()==CC_PWR_OFF)){
      MainKeys = (KEYMAT_COL0) ? (MainKeys | NAV_LEFT_KEY_BIT) : (MainKeys & (~(NAV_LEFT_KEY_BIT)));
      MainKeys = (KEYMAT_COL1) ? (MainKeys | NAV_OK_KEY_BIT)   : (MainKeys & (~(NAV_OK_KEY_BIT)));
      MainKeys = (KEYMAT_COL2) ? (MainKeys | NAV_DOWN_KEY_BIT) : (MainKeys & (~(NAV_DOWN_KEY_BIT)));
    }
    
    BIT_CLR(KEYMAT_ROWS,KEYMAT_ROW1);
    __delay_cycles(1000);
    BIT_SET(KEYMAT_ROWS,KEYMAT_ROW2);
      
    if (is_power_switch_on && !(Pwr_Get_CC_Pwr_Status()==CC_PWR_OFF)){
      MainKeys = (KEYMAT_COL0) ? (MainKeys | FWD_KEY_BIT)      : (MainKeys & (~(FWD_KEY_BIT)));
      MainKeys = (KEYMAT_COL1) ? (MainKeys | NAV_UP_KEY_BIT)   : (MainKeys & (~(NAV_UP_KEY_BIT)));
      MainKeys = (KEYMAT_COL2) ? (MainKeys | BWD_KEY_BIT)      : (MainKeys & (~(BWD_KEY_BIT)));
    }
    
    BIT_CLR(KEYMAT_ROWS,KEYMAT_ROW2);
    
    if (KEYPAD_SampleIndex == KEY_SAMPLE_COUNT) {
        KEYPAD_SampleIndex = 0;
        
        for (i = 0; i < KEY_SAMPLE_COUNT; i++){
            match_count = 0;
            
            for (j = 0; j < KEY_SAMPLE_COUNT; j++){
                if (KEYPAD_MainKeys_Buf[i] == KEYPAD_MainKeys_Buf[j]){
                    match_count++;
                }
            }
            
            if (match_count > KEY_VALID_THRESHOLD){
                prev_main_keys  = KEYPAD_MainKeys;
                KEYPAD_MainKeys = KEYPAD_MainKeys_Buf[i];                  
                
                if ((KEYPAD_MainKeys & 0xFFFFFFFF) != (prev_main_keys & 0xFFFFFFFF)) {

                	/*{KW}:: test long power button press */
					#ifdef PWR_OFF_KEY_COMBINATION_DEFINED /*{KW}: define a proper pwr off combination */
					if (((KEYPAD_MainKeys & PWR_SHUTDOWN_COMB) == PWR_SHUTDOWN_COMB) && ((KEYPAD_MainKeys & PWR_SHUTDOWN_COMB) != (prev_main_keys & PWR_SHUTDOWN_COMB))) {
					   if((power_off_pressed == 0) && (Pwr_Get_CC_Pwr_Status() != CC_PWR_OFF))
					   {
						 CC_CP_PWRON_HIGH();
						 time_stamp_start = Sys_Get_Heartbeat();
						 power_off_pressed = 1;
					   }
					}
					#endif
                    
                    /*{KW}:: test 8s reset power button press */
                    #ifdef PWR_RESET_KEY_COMBINATION_DEFINED /*{KW}: define a proper pwr off combination */
                    if (((KEYPAD_MainKeys & PWR_RESET_COMB) == PWR_RESET_COMB) && ((KEYPAD_MainKeys & PWR_RESET_COMB) != (prev_main_keys & PWR_RESET_COMB))) {
                       if((power_reset_pressed == 0) && (Pwr_Get_CC_Pwr_Status() != CC_PWR_OFF))
                       {
                         CC_CP_PWRON_HIGH();
                         time_stamp_start = Sys_Get_Heartbeat();
                         power_reset_pressed = 1;               
                       }
                    } 
                    #endif
                    
                    #ifdef PWR_OFF_KEY_COMBINATION_DEFINED
                    if (((KEYPAD_MainKeys & PWR_KEY_BIT) != (prev_main_keys & PWR_KEY_BIT)) ||
                    	(((KEYPAD_MainKeys & PWR_SHUTDOWN_COMB) != (prev_main_keys & PWR_SHUTDOWN_COMB)) && ((KEYPAD_MainKeys & PWR_SHUTDOWN_COMB) == PWR_SHUTDOWN_COMB)) ||
                        (((KEYPAD_MainKeys & PWR_RESET_COMB) != (prev_main_keys & PWR_RESET_COMB)) && ((KEYPAD_MainKeys & PWR_RESET_COMB) == PWR_RESET_COMB))  ){/*{KW}: do not send keypad change interrupt for power key and pwr off combination */
                    	KEYPAD_KeyChange = FALSE;
                    }else{
                    	KEYPAD_KeyChange = TRUE;
                    }
					#endif
		    break;
                }               
            } 
        }
    }
    else{
        KEYPAD_MainKeys_Buf[KEYPAD_SampleIndex] = MainKeys;
        KEYPAD_SampleIndex++;
    }
}

/*
*********************************************************************************************************
*********************************************************************************************************
*   LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/
