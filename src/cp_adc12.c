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
*   INCLUDE FILES 
*********************************************************************************************************
*/
#define ADC12_MODULE                /* Define ADC12 module, this is to handle global variables
                                       declaration and externs appropriately                        */

#include "cp_adc12.h"

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

/* Initialize ADC12 unit */
void  ADC12_Init (void)
{
    /* Initialize ADC inputs */
    /* CP_VBAT_VMES - A0, SENSE_BAT_TEMP - A1, CP_VBAT_CMES_2 - A2, CP_VBAT_CMES_1 - A3 */
    P6DIR &= 0xF0;              /* Input                                                            */
    P6SEL |= 0x0F;              /* ADC function                                                     */
    P6REN &= 0xF0;              /* Pullup/Pulldown disabled                                         */
    
    ADC12CTL0 = 0xFF70;         /* SHT1          : 1111 - Sample hold 1 - 1024 ADC12CLK             */
                                /* SHT0          : 1111 - Sample hold 0 - 1024 ADC12CLK             */
                                /* MSC           : 0    - Rising edge of SHI for each conversion    */
                                /* REF2_5V       : 1    - 2.5V reference                            */
                                /* REFON         : 1    - Reference on                              */
                                /* ADC12ON       : 1    - ADC12 on                                  */
                                /* ADC12OVIE     : 0    - Overflow interrupt disable                */
                                /* ADC12TOVIE    : 0    - Timer Overflow interrupt disable          */
                                /* ENC           : 0    - Disable conversion                        */
                                /* ADC12SC       : 0    - Stop conversion                           */
    
    ADC12CTL1 = 0x0200;         /* CSTARTADD     : 0000 - Conversion start address - 0              */
                                /* SHS           : 00   - Sample/Hold source - ADC12SC bit          */
                                /* SHP           : 1    - Sample/Hold pulse mode - SAMPCON from TMR */
                                /* ISSH          : 0    - Sample Hold signal not inverted           */
                                /* ADC12DIV      : 000  - ADC12 Clock divider - Divide by 1         */
                                /* ADC12SSEL     : 00   - ADC12 Clock source - ADC12OSC             */
                                /* CONSEQ        : 00   - Conversion seq - Sing-chan, sing-conv     */
                                /* ADC12BUSY     : 0    - ADC12 Busy (Read only)                    */

    
    /* Battery Status Check */
    ADC12MCTL0 = (0 << 7)      +            /* EOS   : 0    - Not end of sequence                   */
                 VREF_VSS      +            /* VR+ = VREF+ and VR- = AVSS                           */
                 CP_VBAT_VMES;              /* Channel - Battery Voltage                            */
    
    ADC12MCTL1 = (0 << 7)      +            /* EOS   : 0    - Not end of sequence                   */
                 AVCC_VSS      +            /* VR+ = AVCC and VR- = AVSS                            */
                 SENSE_BAT_TEMP;            /* Channel - Battery Temperature                        */

    /* System Current Measurement */
    ADC12MCTL2 = (0 << 7)      +            /* EOS   : 0    - Not end of sequence                   */
                 VREF_VSS      +            /* VR+ = VREF+ and VR- = AVSS                           */
                 CP_VBAT_CMES_2;            /* Channel - System Current measurement, Point 2        */

    ADC12MCTL3 = (0 << 7)      +            /* EOS   : 0    - Not end of sequence                   */
                 VREF_VSS      +            /* VR+ = VREF+ and VR- = AVSS                           */
                 CP_VBAT_CMES_1;            /* Channel - System Current measurement, Point 1        */
    
    /* Resistive Touchpad */
    ADC12MCTL4 = (0 << 7)      +            /* EOS   : 0    - Not end of sequence                   */
                 AVCC_VSS      +            /* VR+ = AVCC and VR- = AVSS                            */
                 CP_TP_XP;                  /* Channel - Resistive touchpad X+                      */

    ADC12MCTL5 = (0 << 7)      +            /* EOS   : 0    - Not end of sequence                   */
                 AVCC_VSS      +            /* VR+ = AVCC and VR- = AVSS                            */
                 CP_TP_XM;                  /* Channel - Resistive touchpad X-                      */

    ADC12MCTL6 = (0 << 7)      +            /* EOS   : 0    - Not end of sequence                   */
                 AVCC_VSS      +            /* VR+ = AVCC and VR- = AVSS                            */
                 CP_TP_YP;                  /* Channel - Resistive touchpad Y+                      */

    ADC12MCTL7 = (0 << 7)      +            /* EOS   : 0    - Not end of sequence                   */
                 AVCC_VSS      +            /* VR+ = AVCC and VR- = AVSS                            */
                 CP_TP_YM;                  /* Channel - Resistive touchpad Y-                      */
    
   
    ADC12IFG   = 0x0000;                    /* Clear interrupt flags                               */
    ADC12IE    = 0x0000;                    /* Disable all interupts                                */
    
    memset(ADC12_Results, 0, sizeof(ADC12_Results));
}

/* De-initialize ADC12 unit */
void  ADC12_DeInit (void)
{
    ADC12CTL0 = 0x0000;         /* SHT1          : 0000 - Sample hold 1 - 4 ADC12CLK                */
                                /* SHT0          : 0000 - Sample hold 0 - 4 ADC12CLK                */
                                /* MSC           : 0    - Rising edge of SHI for each conversion    */
                                /* REF2_5V       : 0    - 1.5V reference                            */
                                /* REFON         : 0    - Reference off                             */
                                /* ADC12ON       : 0    - ADC12 off                                 */
                                /* ADC12OVIE     : 0    - Overflow interrupt disable                */
                                /* ADC12TOVIE    : 0    - Timer Overflow interrupt disable          */
                                /* ENC           : 0    - Disable Conversion                        */
                                /* ADC12SC       : 0    - Stop Conversion                           */

    ADC12CTL1 = 0x0000;         /* CSTARTADD     : 0000 - Conversion start address - 0              */
                                /* SHS           : 00   - Sample/Hold source - ADC12SC bit          */
                                /* SHP           : 0    - Sample/Hold pulse mode - SAMPCON from SHI */
                                /* ISSH          : 0    - Sample Hold signal not inverted           */
                                /* ADC12DIV      : 000  - ADC12 Clock divider - Divide by 1         */
                                /* ADC12SSEL     : 00   - ADC12 Clock source - ADC12OSC             */
                                /* CONSEQ        : 00   - Conversion seq - Sing-chan, sing-conv     */
                                /* ADC12BUSY     : 0    - ADC12 Busy (Read only)                    */
    
    ADC12MCTL0 = 0x00;          /* EOS   : 0    - Not end of sequence                               */
                                /* VR+ = AVCC and VR- = AVSS                                        */
                                /* Channel - A0                                                     */

    ADC12MCTL1 = 0x00;          /* EOS   : 0    - Not end of sequence                               */
                                /* VR+ = AVCC and VR- = AVSS                                        */
                                /* Channel - A0                                                     */
    
    ADC12MCTL2 = 0x00;          /* EOS   : 0    - Not end of sequence                               */
                                /* VR+ = AVCC and VR- = AVSS                                        */
                                /* Channel - A0                                                     */
    
    ADC12MCTL3 = 0x00;          /* EOS   : 0    - Not end of sequence                               */
                                /* VR+ = AVCC and VR- = AVSS                                        */
                                /* Channel - A0                                                     */
    
    ADC12MCTL4 = 0x00;          /* EOS   : 0    - Not end of sequence                               */
                                /* VR+ = AVCC and VR- = AVSS                                        */
                                /* Channel - A0                                                     */
    
    ADC12MCTL5 = 0x00;          /* EOS   : 0    - Not end of sequence                               */
                                /* VR+ = AVCC and VR- = AVSS                                        */
                                /* Channel - A0                                                     */
    
    ADC12MCTL6 = 0x00;          /* EOS   : 0    - Not end of sequence                               */
                                /* VR+ = AVCC and VR- = AVSS                                        */
                                /* Channel - A0                                                     */
    
    ADC12MCTL7 = 0x00;          /* EOS   : 0    - Not end of sequence                               */
                                /* VR+ = AVCC and VR- = AVSS                                        */
                                /* Channel - A0                                                     */

    
    ADC12IFG   = 0x0000;        /* Clear interrupt flags                                            */
    ADC12IE    = 0x0000;        /* Disable all interupts                                            */
}

void  ADC12_SingleConv (INT32U channel, BOOLEAN poll)
{
    ADC12CTL1 &= 0x0FFF;
    ADC12CTL1 |= (channel * 0x1000u);
    
    if (!poll)
    {
        ADC12IE   |= (1 << channel);
        ADC12CTL0 |= (ENC + ADC12SC);
    }
    else
    {
        ADC12CTL0 |= (ENC + ADC12SC);

        while (ADC12CTL1 & ADC12BUSY)
        {
            /* Wait for conversion to complete */
        }
        
        if (ADC12IFG & (1 << 0)){
            ADC12_Results[0] = ADC12MEM0;           /* Move results, IFG is cleared                     */
        }
    
        if (ADC12IFG & (1 << 1)){
            ADC12_Results[1] = ADC12MEM1;           
        }
    
        if (ADC12IFG & (1 << 2)){
            ADC12_Results[2] = ADC12MEM2;           
        }
    
        if (ADC12IFG & (1 << 3)){
            ADC12_Results[3] = ADC12MEM3;           
        }
    
        if (ADC12IFG & (1 << 4)){
            ADC12_Results[4] = ADC12MEM4;           
        }
    
        if (ADC12IFG & (1 << 5)){
            ADC12_Results[5] = ADC12MEM5;           
        }
    
        if (ADC12IFG & (1 << 6)){
            ADC12_Results[6] = ADC12MEM6;           
        }
    
        if (ADC12IFG & (1 << 7)){
            ADC12_Results[7] = ADC12MEM7;           
        }
        
        ADC12CTL0 &= ~(ENC + ADC12SC);
    }
}
/*
*********************************************************************************************************
*********************************************************************************************************
*   LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/* ADC12 Interrupt Service Routine */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = ADC12_VECTOR
__interrupt void  ADC12_Isr (void)
#else
void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12_Isr (void)
#endif
{
    if (ADC12IFG & (1 << 0)){
        ADC12_Results[0] = ADC12MEM0;           /* Move results, IFG is cleared                     */
    }

    if (ADC12IFG & (1 << 1)){
        ADC12_Results[1] = ADC12MEM1;           
    }

    if (ADC12IFG & (1 << 2)){
        ADC12_Results[2] = ADC12MEM2;           
    }

    if (ADC12IFG & (1 << 3)){
        ADC12_Results[3] = ADC12MEM3;           
    }

    if (ADC12IFG & (1 << 4)){
        ADC12_Results[4] = ADC12MEM4;           
    }

    if (ADC12IFG & (1 << 5)){
        ADC12_Results[5] = ADC12MEM5;           
    }

    if (ADC12IFG & (1 << 6)){
        ADC12_Results[6] = ADC12MEM6;           
    }

    if (ADC12IFG & (1 << 7)){
        ADC12_Results[7] = ADC12MEM7;           
    }
    
    ADC12CTL0 &= ~(ENC + ADC12SC);
}
