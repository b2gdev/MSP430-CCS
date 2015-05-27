/*
*********************************************************************************************************
*   Timer B
*   Timer B Functions
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
*   Timer B Functions
*
*   File Name     : cp_timer_b.c
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
#include    "cp_timer_b.h"

/*
*********************************************************************************************************
*   LOCAL DEFINES
*********************************************************************************************************
*/
#define     TMR_B_KEYPAD_CCR0         0xFFFF
#define     TMR_B_KEYPAD_DIVIDER      1
                                            /* Keypad Timer Period - 150 msec                        */
                                            /* Clock Input - 16 MHz                                 */
                                            /* Input Divider - 1                                    */
                                            /* CCR0 - 0xFFFF (65535)                                */
#define TIMER_B_CNT_100MS            25    /*{KW}:: 4ms*25   = 100ms  */
#define TIMER_B_CNT_1000MS           250   /*{KW}:: 4ms*250  = 1000ms */
#define TIMER_B_CNT_8000MS           2000  /*{KW}:: 4ms*2000 = 8000ms */

#define HEARTBEAT_RST_THRES          65536  /*{KW}:: half of teh max value of u32 */

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
INT16U timer_b_divider;
INT32U TmrB_Heartbeat = 0;

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
void  TmrB_Init ()
{
    timer_b_divider = 0;    /* Reset Timer B divider                                            */
    
    TBCCTL0 = CCIE;         /* CMx          : 00   - Capture mode - No capture                      */
                            /* CCISx        : 00   - Capture/Compare input - CCIxA                  */
                            /* SCS          : 0    - Asynchronous capture                           */
                            /* CCLDx        : 00   - TBCLx loads on write to TBCCRx                 */
                            /* CAP          : 0    - Capture mode - Compare mode                    */
                            /* OUTMODx      : 000  - OUT bit value                                  */
                            /* CCIE         : 1    - Capture/Compare interrupt - Enabled            */
                            /* CCI          : 0    - (Read only)                                    */
                            /* OUT          : 0    - Output mode 0, Output - Low                    */
                            /* COV          : 0    - Capture overflow - No capture overflow         */
                            /* CCIFG        : 0    - Capture/Compare interrupt flag (Read only)     */
    
    TBCCR0  = TMR_B_KEYPAD_CCR0;  /* TBCCRx : Compare mode Timer value                              */

    TBCTL   = TBCLGRP_0 + CNTL_0 + TBSSEL_2 + ID_0 + MC_1;
                            /* Unused       : 0                                                     */
                            /* TBCLGRP      : 00     - Each TBCLx latch loads independently         */
                            /* CNTLx        : 00     - Counter length - 16 bit                      */
                            /* Unused       : 0                                                     */
                            /* TBSSELx      : 10     - Clock source - SMCLK                         */
                            /* IDx          : 00     - Input divider - 1                            */
                            /* MCx          : 01     - Mode control - Up mode                       */
                            /* Unused       : 0                                                     */
                            /* TBCLR        : 0      - Timer Clear - Not cleared                    */
                            /* TBIE         : 0      - Timer Interrupt - Interrupt disabled         */
                            /* TBIFG        : 0      - Timer Interrupt flag                         */
    
    __bis_SR_register(GIE); /* Enable Global Interrupts                                             */
}

/*
*********************************************************************************************************
*********************************************************************************************************
*   LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)		/* Timer B0 interrupt service routine               */
#pragma vector=TIMERB0_VECTOR
__interrupt void  TmrB_Isr (void)
#else
void __attribute__ ((interrupt(TIMERB0_VECTOR))) TmrB_Isr (void)
#endif
{
    TmrB_Heartbeat++; /*{KW}: used for TPS pwr pin toggling */
    
    if (timer_b_divider == TMR_B_KEYPAD_DIVIDER){
        timer_b_divider = 0;
        
        DSS_CP_SPI_CS_HIGH();
        Kpd_ReadEx();
        DSS_CP_SPI_CS_LOW();
        
        Brd_ReadKeys(NULL);
        
        __low_power_mode_off_on_exit();     /* Exit LPM0                                            */
    }
    else{
        timer_b_divider++;
    }
    
    /*{KW}: toggle TPS power pin here, if required */
    if (power_switch_toggle){
      if (Sys_Get_Heartbeat() >= time_stamp_start + TIMER_B_CNT_100MS)
      {
        Sys_BeepOff(); //{RD} Power switch toggle indicator beep OFF
        CC_CP_PWRON_LOW();
        power_switch_toggle = 0;                
      }
    }
    else if (power_off_pressed)
    {
      if (Sys_Get_Heartbeat() >= time_stamp_start + TIMER_B_CNT_1000MS)
      {
        CC_CP_PWRON_LOW();
        power_off_pressed = 0;
      }
    }
    else if (power_reset_pressed)
    {
      /*{KW}: just a beep */
      if(((Sys_Get_Heartbeat() - time_stamp_start) % 256) > 32)
      {        
        Sys_BeepOff();
      }
      else
      {
        Sys_BeepOn();
      } /*{KW}: just a beep END*/
      
      if (Sys_Get_Heartbeat() >= time_stamp_start + TIMER_B_CNT_8000MS)
      {
        CC_CP_PWRON_LOW();
        power_reset_pressed = 0;
      }
    }
    else {
      if (TmrB_Heartbeat >= HEARTBEAT_RST_THRES){
        TmrB_Heartbeat = 0;
      }
    }
}
