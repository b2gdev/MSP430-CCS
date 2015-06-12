/*
*********************************************************************************************************
*   I2C
*   I2C Functions
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
*   I2C Functions
*
*   File Name     : cp_i2c.c
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

#ifndef __CP_I2C_H__
#define __CP_I2C_H__


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

/* I2C Module Handle */
#define     I2C_0_HANDLE                0

/* I2C Error Codes */
#define     I2C_SUCCESS                 0
#define     I2C_BUSY                    1
#define     I2C_FAULT                   2
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
void  I2C_Init (void);
void  I2C_DeInit (void);

INT08U  I2C_Write (INT08U handle, INT08U slave_addr, PTR_INT08U data, INT08U count);
INT08U  I2C_Read (INT08U handle, INT08U slave_addr, PTR_INT08U data, INT08U count, BOOLEAN sync);
INT08U  I2C_WriteAndRead (INT08U handle, INT08U slave_addr, PTR_INT08U write_data, INT08U write_count, 
                          PTR_INT08U read_data, INT08U read_count, BOOLEAN sync);
/* TI methods */
void TI_USCI_I2C_receiveinit(unsigned char slave_address);
void TI_USCI_I2C_transmitinit(unsigned char slave_address);
void TI_USCI_I2C_slave_receiveinit();

void TI_USCI_I2C_receive(unsigned char byteCount, unsigned char *field);
void TI_USCI_I2C_transmit(unsigned char byteCount, unsigned char *field);

unsigned char TI_USCI_I2C_slave_present(unsigned char slave_address);
unsigned char TI_USCI_I2C_notready();
unsigned char I2C_wait_till_ready();

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
