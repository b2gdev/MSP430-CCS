/*
*********************************************************************************************************
*   SYSTEM
*   System Specific Functions
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
*   System Specific Functions
*
*   File Name     : cp_system.h
*   Version       : V1.0.0
*   Programmer(s) : PS
*********************************************************************************************************
*   Note(s)
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*   MODULE
*********************************************************************************************************
*/

#ifndef __CP_SYSTEM_H__
#define __CP_SYSTEM_H__


/*
*********************************************************************************************************
*   INCLUDE FILES
*********************************************************************************************************
*/
#include "cp_conf.h"

/*
*********************************************************************************************************
*   EXTERNS
*********************************************************************************************************
*/
extern INT32U TmrB_heartbeat;

/*
*********************************************************************************************************
*   DEFAULT CONFIGURATION
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*   DEFINES
*********************************************************************************************************
*/
#define CP_PWR_ENABLE()         BIT_CLR(P7OUT,P7)       /* P7.7 = 0     */
#define CP_PWR_DISABLE()        BIT_SET(P7OUT,P7)       /* P7.7 = 1     */
#define CP_INT_HIGH()           BIT_SET(P2OUT,P7)       /* P2.7 = 1     */
#define CP_INT_LOW()            BIT_CLR(P2OUT,P7)       /* P2.7 = 0     */
#define MCSPI1_CLK_3V3          (P5IN & P0)             /* P5.0         */
#define CP_VBUS_OTG_DET         (P2IN & P6)             /* P2.6         */
#define CP_USBOTG_DP_OUT        (P7IN & P0)             /* P7.0         */
#define CP_USBOTG_DP            (P7IN & P1)             /* P7.1         */
#define CP_USBOTG_DN            (P7IN & P2)             /* P7.2         */
#define CP_USBOTG_S_TPS()       BIT_CLR(P7OUT,P3)       /* P7.3 = 0     */
#define CP_BATPWR_ENABLE()      BIT_CLR(P7OUT,P4)       /* P7.4 = 0     */
#define CP_BATPWR_DISABLE()     BIT_SET(P7OUT,P4)       /* P7.4 = 1     */
#define CP_CHRG_PWRON()         BIT_SET(P7OUT,P5)       /* P7.5 = 1     */
#define CP_CHRG_PWROFF()        BIT_CLR(P7OUT,P5)       /* P7.5 = 0     */
#define CP_USBPWR_ENABLE()      BIT_CLR(P7OUT,P6)       /* P7.6 = 0     */
#define CP_USBPWR_DISABLE()     BIT_SET(P7OUT,P6)       /* P7.6 = 1     */
#define CP_PWR01_ENABLE()       BIT_SET(P8OUT,P7)       /* P8.7 = 1 (Input only pin) */
#define CP_PWR01_DISABLE()      BIT_CLR(P8OUT,P7)       /* P8.7 = 0 (Input only pin) */
#define CC_CP_PWRON_HIGH()      BIT_SET(P5OUT,P4)       /* P5.4 = 1     */
#define CC_CP_PWRON_LOW()       BIT_CLR(P5OUT,P4)       /* P5.4 = 0     */

//#define BUZZER_EN             P5OUT_bit.P6            /* P5.6         */
// #define CP_SOFT_RST          P5OUT_bit.P6            /* P5.6         */

#define EN_1V8_3V3_LVL_TR()     BIT_CLR(P8OUT,P5)       /* P8.5 = 0     */
#define DIS_1V8_3V3_LVL_TR()    BIT_SET(P8OUT,P5)       /* P8.5 = 1     */
#define I2C2_3V3_SDA            (P3IN & P1)             /* P3.1         */
#define I2C2_3V3_SCL            (P3IN & P2)             /* P3.2         */
#define I2C2_ENABLE()           BIT_SET(P5OUT,P5)       /* P5.5 = 1     */
#define I2C2_DISABLE()          BIT_CLR(P5OUT,P5)       /* P5.5 = 0     */
#define CP_I2C1_SDA             (P5IN & P1)             /* P5.1         */
#define CP_I2C1_SCL             (P5IN & P2)             /* P5.2         */
#define CAM_CP_I2C1_ENABLE()    BIT_SET(P5OUT,P7)       /* P5.7 = 1     */
#define CAM_CP_I2C1_DISABLE()   BIT_CLR(P5OUT,P7)       /* P5.7 = 0     */
#define BOARD_ID_0              (P8IN & P0)             /* P8.0         */
#define BOARD_ID_1              (P8IN & P1)             /* P8.1         */
#define BOARD_ID_2              (P8IN & P2)             /* P8.2         */
#define BOARD_ID_3              (P8IN & P3)             /* P8.3         */
#define BOARD_ID_4              (P8IN & P4)             /* P8.4         */
#define ACC_INT                 (P2IN & P2)             /* P2.2         */
// #define BUZZER_EN            P5OUT_bit.P3            /* P5.3         */
//#define CP_PWRSW_nFAULT       P8OUT_bit.P6            /* P8.6 (Output only pin */

#define DSS_CP_SPI_CS_HIGH()    BIT_SET(P3OUT,P3)      /* P3.3 = 1     */
#define DSS_CP_SPI_CS_LOW()     BIT_CLR(P3OUT,P3)      /* P3.3 = 0     */

// {RD}
#define CP_SPI_SOMI_3V3         (P3IN & P5)            /* P3.5         */
#define CP_SPI_SIMO_3V3_HIGH()  BIT_SET(P3OUT,P4)      /* P3.4 = 1     */
#define CP_SPI_SIMO_3V3_LOW()   BIT_CLR(P3OUT,P4)      /* P3.4 = 0     */
#define OMAP_STATUS_1           (P6IN & P5)            /* P6.5         */

/*
*********************************************************************************************************
*   DATA TYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*   GLOBAL VARIABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*   MACROS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*   FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void  Sys_GpioInit (void);
void  Sys_GpioDeInit (void);
void Sys_ShutDownLPInit(void);
void Sys_ShutDownLPDeInit(void);
INT16U  Sys_GetSystemCurrent_mA (void);

void  Sys_DelayMs (INT16U count);

void  Sys_BeepOn(void);
void  Sys_BeepOff(void);

//void  Sys_BeepLow (INT16U duration);
void  Sys_BeepHigh (INT16U duration);

/*{KW}::*/
INT32U Sys_Get_Heartbeat(void);
/*{KW}:: end*/

/* {KW}:: System_status Variable */
void Sys_Set_System_Status(BOOLEAN status);
BOOLEAN Sys_Get_System_Status(void);

/*
*********************************************************************************************************
*   CONFIGURATION ERRORS
*********************************************************************************************************
*/




























/*
*********************************************************************************************************
*   MODULE END
*********************************************************************************************************
*/

#endif  /* End of __CP_SYSTEM_H__ */
