/*
*********************************************************************************************************
*   SYSTEM
*   System Specific Functions
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
*   System Specific Functions
*
*   File Name     : system.c
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
#include "cp_system.h"

/*
*********************************************************************************************************
*   LOCAL DEFINES
*********************************************************************************************************
*/
#define     BEEP_LOW_FREQ       3000L            /* 1 kHz */
#define     BEEP_HIGH_FREQ      4000L            /* 8 kHz */

#define     BEEP_LOW_CYCLES     (SYS_CLK_FREQ_HZ/BEEP_LOW_FREQ/2)
#define     BEEP_LOW_CYCLES     (SYS_CLK_FREQ_HZ/BEEP_LOW_FREQ/2)

#define     BEEP_HIGH_CYCLES    (SYS_CLK_FREQ_HZ/BEEP_HIGH_FREQ/2)
#define     BEEP_HIGH_CYCLES    (SYS_CLK_FREQ_HZ/BEEP_HIGH_FREQ/2)

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
BOOLEAN system_status = FALSE;
extern INT32U TmrB_Heartbeat;
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

/* Initialize GPIOs */
void  Sys_GpioInit (void)
{
    /* CP_PWR_nEN */
    BIT_SET(P7DIR,P7);      /* Output                                                              */
    BIT_CLR(P7SEL,P7);      /* I/O function                                                        */
    BIT_CLR(P7REN,P7);      /* Pullup/Pulldown disabled                                            */
    BIT_SET(P7OUT,P7);      /* HIGH - Switch off system power by turning off Power Switches        */   
    
    /* CP_INT */
    BIT_SET(P2DIR,P7);      /* Output                                                              */
    BIT_CLR(P2SEL,P7);      /* I/O function                                                        */
    BIT_CLR(P2REN,P7);      /* Pullup/Pulldown disabled                                            */
    BIT_CLR(P2OUT,P7);      /* LOW - CP Interrupt - Disabled                                       */
    
    /* MCSPI1_CLK_3V3 */     
    BIT_CLR(P5DIR,P0);      /* Input                                                               */
    BIT_CLR(P5SEL,P0);      /* I/O function                                                        */
    //{RD}
    BIT_SET(P5REN,P0);      /* Pullup/Pulldown enabled                                            */
    BIT_CLR(P5OUT,P0);      /* LOW -                  */
    
    /* CP_VBUS_OTG_DET */
    BIT_CLR(P2DIR,P6);      /* Input                                                               */
    BIT_CLR(P2SEL,P6);      /* I/O function                                                        */
    BIT_CLR(P2REN,P6);      /* Pullup/Pulldown disabled                                            */
    
    /* CP_USBOTG_DP_OUT */   
    BIT_CLR(P7DIR,P0);      /* Input                                                               */
    BIT_CLR(P7SEL,P0);      /* I/O function                                                        */
    BIT_CLR(P7REN,P0);      /* Pullup/Pulldown disabled                                            */
    
    /* CP_USBOTG_DP */       
    BIT_CLR(P7DIR,P1);      /* Input                                                               */
    BIT_CLR(P7SEL,P1);      /* I/O function                                                        */
    BIT_CLR(P7REN,P1);      /* Pullup/Pulldown disabled                                            */
    
    /* CP_USBOTG_DN */       
    BIT_CLR(P7DIR,P2);      /* Input                                                               */
    BIT_CLR(P7SEL,P2);      /* I/O function                                                        */
    BIT_CLR(P7REN,P2);      /* Pullup/Pulldown disabled                                            */
    
    /* CP_USBOTG_S */
    BIT_SET(P7DIR,P3);      /* Output                                                              */
    BIT_CLR(P7SEL,P3);      /* I/O function                                                        */
    BIT_CLR(P7REN,P3);      /* Pullup/Pulldown disabled                                            */
    BIT_CLR(P7OUT,P3);      /* LOW - Switch USB to TPS65950                                        */
    
    /* CP_BATPWR_nEN */
    BIT_SET(P7DIR,P4);      /* Output                                                              */
    BIT_CLR(P7SEL,P4);      /* I/O function                                                        */
    BIT_CLR(P7REN,P4);      /* Pullup/Pulldown disabled                                            */
    BIT_CLR(P7OUT,P4);      /* LOW - Switch on Power supply 1 for companian processor              */
    
    /* CP_CHRG_PWRON */
    BIT_SET(P7DIR,P5);      /* Output                                                              */
    BIT_CLR(P7SEL,P5);      /* I/O function                                                        */
    BIT_CLR(P7REN,P5);      /* Pullup/Pulldown disabled                                            */
    BIT_CLR(P7OUT,P5);      /* LOW - Switch off Power supply for Battery charger                   */
    
    /* CP_USBPWR_nEN */
    BIT_SET(P7DIR,P6);      /* Output                                                              */
    BIT_CLR(P7SEL,P6);      /* I/O function                                                        */
    BIT_CLR(P7REN,P6);      /* Pullup/Pulldown disabled                                            */
    BIT_CLR(P7OUT,P6);      /* LOW - Switch on Power supply 2 for companian processor              */
    
    /* CP_PWR01_EN */        /* {PS} : P8.7 is an input only pin, but this is used to control 
                                CP_PWR01_EN using internal pull up/down resistors                   */
    BIT_CLR(P8DIR,P7);      /* Input                                                               */
    BIT_CLR(P8SEL,P7);      /* I/O function                                                        */
    BIT_SET(P8REN,P7);      /* Pullup/Pulldown enabled                                             */
    BIT_CLR(P8OUT,P7);      /* LOW - Switch off Power supply for Display and USB                   */
    
    /* CC_CP_PWRON */
    BIT_SET(P5DIR,P4);      /* Output                                                              */
    BIT_CLR(P5SEL,P4);      /* I/O function                                                        */
    BIT_CLR(P5REN,P4);      /* Pullup/Pulldown disabled                                            */
    BIT_CLR(P5OUT,P4);      /* LOW - Switch off system power by controlling PWRON of TPS65950      */
    
    /* BUZZER_EN */
    BIT_SET(P5DIR,P6);      /* Output                                                              */
    BIT_CLR(P5SEL,P6);      /* I/O function                                                        */
    BIT_CLR(P5REN,P6);      /* Pullup/Pulldown disabled                                            */
    BIT_CLR(P5OUT,P6);      /* LOW - Beeper disabled                                               */    
    
    /* nEN_1V8_3V3_LVL_TR */
    BIT_SET(P8DIR,P5);      /* Output                                                              */
    BIT_CLR(P8SEL,P5);      /* I/O function                                                        */
    BIT_CLR(P8REN,P5);      /* Pullup/Pulldown disabled                                            */
    BIT_SET(P8OUT,P5);      /* HIGH - Disable 1.8V to 3.3V Level translator                        */    
    
    /* I2C2_3V3_SDA */  
    BIT_CLR(P3DIR,P1);      /* Input                                                               */
    BIT_CLR(P3SEL,P1);      /* I/O function                                                        */
    BIT_CLR(P3REN,P1);      /* Pullup/Pulldown disabled                                            */
    
    /* I2C2_3V3_SCL */  
    BIT_CLR(P3DIR,P2);      /* Input                                                               */
    BIT_CLR(P3SEL,P2);      /* I/O function                                                        */
    BIT_CLR(P3REN,P2);      /* Pullup/Pulldown disabled                                            */
    
    /* I2C2_EN */
    BIT_SET(P5DIR,P5);      /* Output                                                              */
    BIT_CLR(P5SEL,P5);      /* I/O function                                                        */
    BIT_CLR(P5REN,P5);      /* Pullup/Pulldown disabled                                            */
    BIT_SET(P5OUT,P5);      /* HIGH - I2C2 enabled                                                 */
    
    /* CP_I2C1_SDA */   
    BIT_CLR(P5DIR,P1);      /* Input                                                               */
    BIT_CLR(P5SEL,P1);      /* I/O function                                                        */
    BIT_CLR(P5REN,P1);      /* Pullup/Pulldown disabled                                            */
    
    /* CP_I2C1_SCL */   
    BIT_CLR(P5DIR,P2);     /* Input                                                                */
    BIT_CLR(P5SEL,P2);     /* I/O function                                                         */
    BIT_CLR(P5REN,P2);     /* Pullup/Pulldown disabled                                             */

    /* CAM_CP_I2C1_EN */
    BIT_SET(P5DIR,P7);      /* Output                                                              */
    BIT_CLR(P5SEL,P7);      /* I/O function                                                        */
    BIT_CLR(P5REN,P7);      /* Pullup/Pulldown disabled                                            */
    BIT_CLR(P5OUT,P7);      /* LOW - I2C1 disabled                                                 */
    
    /* BOARD_ID_0 */
    BIT_CLR(P8DIR,P0);     /* Input                                                                */
    BIT_CLR(P8SEL,P0);     /* I/O function                                                         */
    BIT_CLR(P8REN,P0);     /* Pullup/Pulldown disabled                                             */
    
    /* BOARD_ID_1 */
    BIT_CLR(P8DIR,P1);     /* Input                                                                */
    BIT_CLR(P8SEL,P1);     /* I/O function                                                         */
    BIT_CLR(P8REN,P1);     /* Pullup/Pulldown disabled                                             */    

    /* BOARD_ID_2 */
    BIT_CLR(P8DIR,P2);     /* Input                                                                */
    BIT_CLR(P8SEL,P2);     /* I/O function                                                         */
    BIT_CLR(P8REN,P2);     /* Pullup/Pulldown disabled                                             */    

    /* BOARD_ID_3 */
    BIT_CLR(P8DIR,P3);     /* Input                                                                */
    BIT_CLR(P8SEL,P3);     /* I/O function                                                         */
    BIT_CLR(P8REN,P3);     /* Pullup/Pulldown disabled                                             */    

    /* BOARD_ID_4 */
    BIT_CLR(P8DIR,P4);     /* Input                                                                */
    BIT_CLR(P8SEL,P4);     /* I/O function                                                         */
    BIT_CLR(P8REN,P4);     /* Pullup/Pulldown disabled                                             */
    
    /* ACC_INT */
    BIT_CLR(P2DIR,P2);     /* Input                                                                */
    BIT_CLR(P2SEL,P2);     /* I/O function                                                         */
    BIT_SET(P2REN,P2);     /* Pullup/Pulldown enabled                                              */
    BIT_CLR(P2OUT,P2);     /* LOW - Active high interrupt                                          */
        
    /* BUZZER_EN */
    BIT_SET(P5DIR,P3);      /* Output                                                              */
    BIT_CLR(P5SEL,P3);      /* I/O function                                                        */
    BIT_CLR(P5REN,P3);      /* Pullup/Pulldown disabled                                            */
    BIT_CLR(P5OUT,P3);      /* LOW - Buzzer disabled                                               */    
    
    /* CP_PWRSW_nFAULT */
    BIT_SET(P8DIR,P6);      /* Output                                                              */
    BIT_CLR(P8SEL,P6);      /* I/O function                                                        */
    BIT_CLR(P8REN,P6);      /* Pullup/Pulldown disabled                                            */
    BIT_SET(P8OUT,P6);      /* HIGH - {PS} : Safe level                                            */
    
    /* DSS_CP_SPI_CS */      
    BIT_SET(P3DIR,P3);      /* Output                                                              */
    BIT_CLR(P3SEL,P3);      /* I/O function                                                        */
    BIT_CLR(P3REN,P3);      /* Pullup/Pulldown disabled                                            */
    BIT_CLR(P3OUT,P3);      /* LOW                                                                 */        
    
    /* {RD} testing CP_SPI_SOMI_3V3 */
    BIT_CLR(P3DIR,P5);      /* Input                                                               */
    BIT_CLR(P3SEL,P5);      /* I/O function                                                        */
    BIT_SET(P3REN,P5);      /* Pullup/Pulldown disabled                                            */
    BIT_CLR(P3OUT,P5);      /* LOW -                  */
    
    /* OMAP_STATUS_1 */      
    BIT_CLR(P6DIR,P5);      /* Input                                                              */
    BIT_CLR(P6SEL,P5);      /* I/O function                                                        */
    BIT_SET(P6REN,P5);      /* Pullup/Pulldown disabled                                            */
    BIT_CLR(P6OUT,P5);      /* LOW                                                                 */
    
    /* {RD} testing CP_SPI_SIMO_3V3 */
    BIT_SET(P3DIR,P4);      /* Output                                                               */
    BIT_CLR(P3SEL,P4);      /* I/O function                                                        */
    BIT_CLR(P3REN,P4);      /* Pullup/Pulldown disabled                                            */
    BIT_CLR(P3OUT,P4);      /* LOW -                  */
    
    /* {RD} PWR_SW */
    BIT_CLR(P2DIR,P1);     /* Input                                                                */
    BIT_CLR(P2SEL,P1);     /* I/O function                                                         */
    BIT_SET(P2REN,P1);     /* Pullup/Pulldown enabled                                              */
    BIT_CLR(P2OUT,P1);     /* LOW - Active high interrupt                                          */
}

void  Sys_GpioDeInit (void)
{

}

void  Sys_DelayMs (INT16U count)
{
    INT16U i = 0;
    
    for (i = 0; i < count; i++) {
        __delay_cycles(1000*SYS_CLK_FREQ_MHZ);
    }
}

/*{KW}: New beep functions ~4kHz */
void  Sys_BeepOn(void) 
{
  BIT_SET(P5SEL,P6);      /* I/O function  :ACLK */
}
void  Sys_BeepOff(void) 
{
  BIT_CLR(P5SEL,P6);      /* I/O function  :GPIO */
  BIT_CLR(P5OUT,P6);      /* LOW - Beeper disabled */  
}

void  Sys_BeepHigh (INT16U duration)    /* duration is in milliseconds */
{
  Sys_BeepOn();
  Sys_DelayMs(duration);
  Sys_BeepOff();
}



//void  Sys_BeepLow (INT16U duration)     /* duration is in milliseconds */
//{
//    INT32U count = 0;
//    INT32U i     = 0;
//    
//    count = (duration * BEEP_LOW_FREQ)/1000;
//    
//    for (i = 0; i < count; i++) {
//        BUZZER_EN = 1;
//        __delay_cycles(BEEP_LOW_CYCLES);
//        BUZZER_EN = 0;
//        __delay_cycles(BEEP_LOW_CYCLES);
//    }
//}
//
//void  Sys_BeepHigh (INT16U duration)    /* duration is in milliseconds */
//{
//    INT32U count = 0;
//    INT32U i     = 0;
//    
//    count = (duration * BEEP_HIGH_FREQ)/1000;
//    
//    for (i = 0; i < count; i++) {
//        BUZZER_EN = 1;
//        __delay_cycles(BEEP_HIGH_CYCLES);
//        BUZZER_EN = 0;
//        __delay_cycles(BEEP_HIGH_CYCLES);
//    }  
//}

INT16U  Sys_GetSystemCurrent_mA (void)
{
    INT16U sys_cur_mes_v1;
    INT16U sys_cur_mes_v2;  
    INT16U sys_cur_ma = 0;
    
    ADC12_SingleConv(CP_VBAT_CMES_1, TRUE);    
    sys_cur_mes_v1 = ADC12_Results[CP_VBAT_CMES_1];
    
    ADC12_SingleConv(CP_VBAT_CMES_2, TRUE);    
    sys_cur_mes_v2 = ADC12_Results[CP_VBAT_CMES_2];
    
    sys_cur_ma = (INT16U)(((((sys_cur_mes_v1 - sys_cur_mes_v2)*2.5)/4096) *3 / .05) * 1000);
    
    return (INT16U)sys_cur_ma;
}
/*{KW}:: system heartbeat kept in Timer B*/
INT32U Sys_Get_Heartbeat(void)
{
  return TmrB_Heartbeat;
}

/*{KW}:: Set System_status Variable, indicates SPI ready status */
void Sys_Set_System_Status(BOOLEAN status)
{
  system_status = status;
}

/*{KW}:: Set System_status Variable, indicates SPI ready status */
BOOLEAN Sys_Get_System_Status(void)
{
  return system_status;
}

/*
*********************************************************************************************************
*********************************************************************************************************
*   LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/
