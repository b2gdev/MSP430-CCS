/*
*********************************************************************************************************
*   ADC12
*   ADC12 Functions
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
*   ADC12 Functions
*
*   File Name     : cp_adc12.c
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

#ifndef __CP_ADC12_H__
#define __CP_ADC12_H__


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
#ifdef ADC12_MODULE
    #define CP_ADC12_EXT
#else
    #define CP_ADC12_EXT extern
#endif

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

/* ADC Channel Count */
#define ADC12_CHANNEL_COUNT     8

/* ADC Channels */
#define CP_VBAT_VMES            INCH_0              /* Battery voltage                              */
#define SENSE_BAT_TEMP          INCH_1              /* Battery temperature                          */
#define CP_VBAT_CMES_2          INCH_2              /* System current measurement, Point 2          */
#define CP_VBAT_CMES_1          INCH_3              /* System current measurement, Point 1          */
#define CP_TP_XP                INCH_4              /* Resistive touchpad, X+                       */
#define CP_TP_XM                INCH_5              /* Resistive touchpad, X-                       */
#define CP_TP_YP                INCH_6              /* Resistive touchapd, Y+                       */
#define CP_TP_YM                INCH_7              /* Resistive touchapd, Y-                       */

/* ADC12 Voltage References */
#define AVCC_VSS                SREF_0              /* VR+ = VREF+ and VR- = AVSS                   */
#define VREF_VSS                SREF_1              /* VR+ = AVCC and VR- = AVSS                    */

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
CP_ADC12_EXT INT16U ADC12_Results[ADC12_CHANNEL_COUNT];  /* Global array to hold last results of each 
                                                           ADC12 channel                            */

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
void  ADC12_Init (void);
void  ADC12_DeInit (void);
void  ADC12_SingleConv (INT32U channel, BOOLEAN poll);
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

#endif  /* End of __CP_ADC12_H__ */
