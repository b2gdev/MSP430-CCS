/*
*********************************************************************************************************
*   SPI
*   SPI Functions
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
*   SPI Functions
*
*   File Name     : cp_spi.c
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

/*
*********************************************************************************************************
*   INCLUDE FILES 
*********************************************************************************************************
*/
#define SPI_MODULE                  /* Define SPI module, this is to handle global variables
                                       declaration and externs appropriately                        */

#include    "cp_spi.h"

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
void  SPI_Init (void)
{
    /* SPI Module 0 *********************************************************************************/

            
    /* SPI Module 1 *********************************************************************************/

    /* Initialize SPI pins */
    P3SEL |= (BIT7 + BIT6);     /* SPI function                                                     */
                                /* Pin 35 - P3.7/UCA1RXD/UCA1SOMI                                   */
                                /* Pin 34 - P3.6/UCA1TXD/UCA1SIMO                                   */
    P3REN &= ~(BIT7 + BIT6);    /* Pullup/Pulldown disabled                                         */
    
    P5SEL |= (BIT0 + BIT3);     /* SPI function                                                     */
                                /* Pin 44 - P5.0/UCB1STE/UCA1CLK                                    */
                                /* Pin 47 - P5.3/UCB1CLK/UCA1STE                                    */
    P5REN &= ~(BIT0);           /* Pullup/Pulldown disabled                                         */
    
    P5OUT &=  ~(BIT3);          /* Pulldown selected                                                  */
    P5REN |=  (BIT3);           /* Pullup/Pulldown enabled                                          */

    UCA1CTL1  = UCSWRST;                    /* Put state machine in reset                           */
    
    UCA1CTL0  = UCSYNC + UCMODE_1 + UCMSB + UCCKPL;    
                                            /* 4-pin, active high cs, 8-bit SPI slave                */
    UCA1CTL1 &= ~UCSWRST;                   /* Initialize USCI state machine                        */
    UC1IE    |= UCA1RXIE;                   /* Enable USCI_A1 RX interrupt                          */
    __bis_SR_register(GIE);                 /* Enable Global Interrupts                             */    
    
    /* Initialize buffers */
    Cbuf_Init(&SPI_1_ReceiveBuffer, spi_1_rx_buf, SPI_1_RX_BUF_SIZE);
    Cbuf_Init(&SPI_1_TransmitBuffer, spi_1_tx_buf, SPI_1_TX_BUF_SIZE);    
}

// SPI De-Init put the some SPI pins back to normal GPIO. Other SPI pins are handled by Sys_ShutDownLPInit
void SPI_DeInit(void){
	/* MCSPI1_SOMI_3V3 */
	BIT_SET(P3DIR,P7);      /* Output                                                              */
	BIT_CLR(P3SEL,P7);      /* I/O function                                                        */
	BIT_CLR(P3REN,P7);      /* Pullup/Pulldown disabled                                            */
	BIT_CLR(P3OUT,P7);      /* LOW                                                                 */
}

/*
*********************************************************************************************************
*********************************************************************************************************
*   LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB1RX_VECTOR
__interrupt void  SPI_RxIsr (void)
#else
void __attribute__ ((interrupt(USCIAB1RX_VECTOR))) SPI_RxIsr (void)
#endif
{
    if (UC1IFG & UCA1RXIFG){
        Cbuf_Write(&SPI_1_ReceiveBuffer, UCA1RXBUF);
        __low_power_mode_off_on_exit();
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB1TX_VECTOR
__interrupt void  SPI_TxIsr (void)
#else
void __attribute__ ((interrupt(USCIAB1TX_VECTOR))) SPI_TxIsr (void)
#endif
{
    INT08U c = 0x00;
    
    if (UC1IFG & UCA1TXIFG){
        if (!Cbuf_IsEmpty(&SPI_1_TransmitBuffer)){
            Cbuf_Read(&SPI_1_TransmitBuffer, &c);
            UCA1TXBUF = c;
        }
        else{
            SPI_1_TX_INT_DISABLE();
        }
        __low_power_mode_off_on_exit();
    }
}
