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
*   File Name     : cp_spi.h
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

#ifndef __CP_SPI_H__
#define __CP_SPI_H__


/*
*********************************************************************************************************
*   INCLUDE FILES
*********************************************************************************************************
*/
#include    "cp_conf.h"

/*
*********************************************************************************************************
*   EXTERNS
*********************************************************************************************************
*/
#ifdef SPI_MODULE
    #define CP_SPI_EXT
#else
    #define CP_SPI_EXT extern
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
/* SPI Buffer sizes */
#define     SPI_1_RX_BUF_SIZE               256
#define     SPI_1_TX_BUF_SIZE               256

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
CP_SPI_EXT CIRC_BUFFER SPI_1_ReceiveBuffer;             /* Global buffer to hold SPI receive data   */
CP_SPI_EXT CIRC_BUFFER SPI_1_TransmitBuffer;            /* Global buffer to hold SPI transmit data  */

     
CP_SPI_EXT INT08U spi_1_rx_buf [SPI_1_RX_BUF_SIZE];
CP_SPI_EXT INT08U spi_1_tx_buf [SPI_1_TX_BUF_SIZE];

/*
*********************************************************************************************************
*   MACROS
*********************************************************************************************************
*/
#define SPI_1_TX_INT_ENABLE()   (UC1IE |= UCA1TXIE)
#define SPI_1_TX_INT_DISABLE()  (UC1IE &= ~(UCA1TXIE))

/*
*********************************************************************************************************
*   FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void  SPI_Init (void);
void  SPI_DeInit (void);

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

#endif  /* End of __MODULE_H__ */
