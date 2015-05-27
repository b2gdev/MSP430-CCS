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
*   INCLUDE FILES 
*********************************************************************************************************
*/
#include "cp_i2c.h"

/*
*********************************************************************************************************
*   LOCAL DEFINES
*********************************************************************************************************
*/
//{RD}
#define I2C_TRIES 0x1000
#define OWNADD 0x48
//#define ENABLE_I2C_RECOVERY
//#define ENABLE_I2C_ERROR_BEEPS

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


INT08S byteCtr;

PTR_INT08U  i2c_0_tx_buf_ptr;
PTR_INT08U  i2c_0_rx_buf_ptr;

INT08U      i2c_tx_count;
INT08U      i2c_rx_count;
INT08U Transaction_failed = 0;
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
void  I2C_Init (void)
{
    /* I2C Module 0 *********************************************************************************/
    
    /* Initialize I2C pins */
    P3SEL |= 0x06;              /* I2C function                                                     */
    P3REN &= 0xF9;              /* Pullup/Pulldown disabled                                         */

    UCB0CTL1 |= UCSWRST;                        /* Enable SW reset                                  */
    UCB0CTL0  = (UCMST + UCMODE_3 + UCSYNC);    /* I2C Master, synchronous mode                     */
    UCB0CTL1  = (UCSSEL_2 + UCSWRST);           /* Use SMCLK, keep SW reset                         */
    UCB0BR0   = 160;                            /* fSCL = SMCLK/160 = 16MHz/160 = ~100kHz           */
    UCB0BR1   = 0;                              /*                                                  */   
}

void  I2C_DeInit (void)
{
    
}

INT08U  I2C_Write (INT08U handle, INT08U slave_addr, PTR_INT08U data, INT08U count)
{
   if(I2C_wait_till_ready()) {    
     //CP_SPI_SIMO_3V3_HIGH;                // {RD} I2C test
     return I2C_FAULT;
   }

   TmrA_IntDisable();
   if(count) {
     TI_USCI_I2C_transmitinit(slave_addr);            // initialize USCI
          //CP_SPI_SIMO_3V3_HIGH;
     if(I2C_wait_till_ready())
       goto i2c_recovery;                             // wait for bus to be free
          //CP_SPI_SIMO_3V3_LOW;
     TI_USCI_I2C_transmit(count, data);               // transmit 
   }
   CP_SPI_SIMO_3V3_HIGH();
   I2C_wait_till_ready();                             // wait for bus to be free
   CP_SPI_SIMO_3V3_LOW();
i2c_recovery:   
   Sys_DelayMs(1);
   TmrA_IntEnable();
   
   if(BQ24150A_No_Response_Reset_Cmd) {
     return I2C_FAULT;
   }
   
   return I2C_SUCCESS;
}

INT08U  I2C_Read (INT08U handle, INT08U slave_addr, PTR_INT08U data, INT08U count, BOOLEAN sync)
{
   if(I2C_wait_till_ready()) {     
     //CP_SPI_SIMO_3V3_HIGH;                // {RD} I2C test
     return I2C_FAULT;
   }
   
   TmrA_IntDisable();
   if(count) {
     TI_USCI_I2C_receiveinit(slave_addr);            // initialize USCI
          //CP_SPI_SIMO_3V3_HIGH;
     if(I2C_wait_till_ready())
       goto i2c_recovery;                             // wait for bus to be free
          //CP_SPI_SIMO_3V3_LOW;
     TI_USCI_I2C_receive(count, data);               // receive 
   }
   CP_SPI_SIMO_3V3_HIGH();
   I2C_wait_till_ready();                             // wait for bus to be free
   CP_SPI_SIMO_3V3_LOW();
i2c_recovery:                             
   Sys_DelayMs(1);
   TmrA_IntEnable();
   
   if(BQ24150A_No_Response_Reset_Cmd) {
     return I2C_FAULT;
   }
   
   return I2C_SUCCESS;
}

INT08U  I2C_WriteAndRead (INT08U handle, INT08U slave_addr, PTR_INT08U write_data, INT08U write_count, 
                          PTR_INT08U read_data, INT08U read_count, BOOLEAN sync)
{
   if(I2C_wait_till_ready()) {     
     //CP_SPI_SIMO_3V3_HIGH;                // {RD} I2C test
     return I2C_FAULT;
   }

   TmrA_IntDisable();
   if(write_count) {
     TI_USCI_I2C_transmitinit(slave_addr);            // initialize USCI
     //CP_SPI_SIMO_3V3_HIGH;
    if(I2C_wait_till_ready())
       goto i2c_recovery;                             // wait for bus to be free
         //CP_SPI_SIMO_3V3_LOW;
     TI_USCI_I2C_transmit(write_count, write_data);   // transmit      
   }
   CP_SPI_SIMO_3V3_HIGH();
   if(I2C_wait_till_ready())
       goto i2c_recovery;                             // wait for bus to be free
   CP_SPI_SIMO_3V3_LOW();
   Sys_DelayMs(1);
    
   if(read_count) {
     TI_USCI_I2C_receiveinit(slave_addr);             // initialize USCI
          //CP_SPI_SIMO_3V3_HIGH;
     if(I2C_wait_till_ready())
       goto i2c_recovery;                             // wait for bus to be free
          //CP_SPI_SIMO_3V3_LOW;
     TI_USCI_I2C_receive(read_count, read_data);      // receive 
   }
   CP_SPI_SIMO_3V3_HIGH();
   I2C_wait_till_ready();                             // wait for bus to be free
   CP_SPI_SIMO_3V3_LOW();
i2c_recovery:   
   Sys_DelayMs(1);
   TmrA_IntEnable();
   
   if(BQ24150A_No_Response_Reset_Cmd) {
     return I2C_FAULT;
   }
   
   return I2C_SUCCESS;
}

/*
*********************************************************************************************************
*********************************************************************************************************
*   LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

//------------------------------------------------------------------------------
// void TI_USCI_I2C_slave_receiveinit()
//
// This function initializes the USCI module for master-receive operation. 
//
// IN:   unsigned char slave_address   =>  Slave Address
//-----------------------------------------------------------------------------
void TI_USCI_I2C_slave_receiveinit()
{
    P3SEL |= 0x06;              /* I2C function                                                     */
    P3REN &= 0xF9;              /* Pullup/Pulldown disabled                                         */

    UCB0CTL1 = UCSWRST;                         /* Enable SW reset                                  */
    UCB0CTL0  = (UCMODE_3 + UCSYNC);    /* I2C slave, synchronous mode                     */
    UCB0I2COA   = OWNADD;          // Own Address & Respond to general call                           
    UCB0CTL1 &= ~UCSWRST;                       // Clear SW reset, resume operation
    UCB0I2CIE = UCSTPIE|UCSTTIE; // Enable STP interrupts
    UC0IE = UCB0RXIE|UCB0TXIE; 
	byteCtr = 0 ;
    __bis_SR_register(GIE);     /* Enable Global Interrupts                         */
}

//------------------------------------------------------------------------------
// void TI_USCI_I2C_receiveinit(unsigned char slave_address)
//
// This function initializes the USCI module for master-receive operation. 
//
// IN:   unsigned char slave_address   =>  Slave Address
//-----------------------------------------------------------------------------
void TI_USCI_I2C_receiveinit(unsigned char slave_address)
{
    P3SEL |= 0x06;              /* I2C function                                                     */
    P3REN &= 0xF9;              /* Pullup/Pulldown disabled                                         */

    UCB0CTL1 = UCSWRST;                         /* Enable SW reset                                  */
    // UCB0CTL0  = (UCMST + UCMODE_3 + UCSYNC);    /* I2C Master, synchronous mode                     */
    UCB0CTL0  = (UCMST + UCMODE_3 + UCSYNC + UCMM);    /* I2C Master, multimaster, synchronous mode                     */
    UCB0CTL1  = (UCSSEL_2 + UCSWRST);           /* Use SMCLK, keep SW reset                         */
    UCB0BR0   = 160;                            /* fSCL = SMCLK/160 = 16MHz/160 = ~100kHz           */
    UCB0BR1   = 0;  
                           
    UCB0I2CSA = slave_address;                  // Set slave address
    UCB0I2COA   = UCGCEN + OWNADD;          // Own Address & Respond to general call
    UCB0CTL1 &= ~UCSWRST;                       // Clear SW reset, resume operation
    UCB0I2CIE = UCNACKIE|UCALIE|UCSTPIE; // Enable AL & NACK interrupts
    UC0IE = UCB0RXIE; 
    __bis_SR_register(GIE);     /* Enable Global Interrupts                         */
}



//------------------------------------------------------------------------------
// void TI_USCI_I2C_transmitinit(unsigned char slave_address)
//
// This function initializes the USCI module for master-transmit operation. 
//
// IN:   unsigned char slave_address   =>  Slave Address
//------------------------------------------------------------------------------
void TI_USCI_I2C_transmitinit(unsigned char slave_address)
{
    P3SEL |= 0x06;              /* I2C function                                                     */
    P3REN &= 0xF9;              /* Pullup/Pulldown disabled                                         */

    UCB0CTL1 = UCSWRST;                         /* Enable SW reset                                  */
    // UCB0CTL0  = (UCMST + UCMODE_3 + UCSYNC);    /* I2C Master, synchronous mode                     */
    UCB0CTL0  = (UCMST + UCMODE_3 + UCSYNC + UCMM);    /* I2C Master, multimaster, synchronous mode                     */
    UCB0CTL1  = (UCSSEL_2 + UCSWRST);           /* Use SMCLK, keep SW reset                         */
    UCB0BR0   = 160;                            /* fSCL = SMCLK/160 = 16MHz/160 = ~100kHz           */
    UCB0BR1   = 0;  
                           
    UCB0I2CSA = slave_address;                  // Set slave address
    UCB0I2COA   = UCGCEN + OWNADD;          // Own Address & Respond to general call
    UCB0CTL1 &= ~UCSWRST;                       // Clear SW reset, resume operation
    UCB0I2CIE = UCNACKIE|UCALIE|UCSTPIE; // Enable AL & NACK interrupts
    UC0IE = UCB0TXIE;   
    __bis_SR_register(GIE);                     /* Enable Global Interrupts                         */
}



//------------------------------------------------------------------------------
// void TI_USCI_I2C_receive(unsigned char byteCount, unsigned char *field)
//
// This function is used to start an I2C commuincation in master-receiver mode. 
//
// IN:   unsigned char byteCount  =>  number of bytes that should be read
//       unsigned char *field     =>  array variable used to store received data
//------------------------------------------------------------------------------
void TI_USCI_I2C_receive(unsigned char byteCount, unsigned char *field){
  i2c_0_rx_buf_ptr = field;
  if ( byteCount == 1 ){
    byteCtr = 0 ;
    __bic_SR_register(GIE);                   //__disable_interrupt();
    //CP_SPI_SIMO_3V3_HIGH;                // {RD} I2C test
    UCB0CTL1 |= UCTXSTT;                      // I2C start condition
    while (UCB0CTL1 & UCTXSTT);               // Start condition sent?
    UCB0CTL1 |= UCTXSTP;                      // I2C stop condition
    //CP_SPI_SIMO_3V3_LOW;                // {RD} I2C test
   __bis_SR_register(GIE);                    // __enable_interrupt();
  } 
  else if ( byteCount > 1 ) {
    byteCtr = byteCount - 2 ;
    UCB0CTL1 |= UCTXSTT;                      // I2C start condition
  } 
  else {
    return;                                   // This condition is never met
  }
}



//------------------------------------------------------------------------------
// void TI_USCI_I2C_transmit(unsigned char byteCount, unsigned char *field)
//
// This function is used to start an I2C commuincation in master-transmit mode. 
//
// IN:   unsigned char byteCount  =>  number of bytes that should be transmitted
//       unsigned char *field     =>  array variable. Its content will be sent.
//------------------------------------------------------------------------------
void TI_USCI_I2C_transmit(unsigned char byteCount, unsigned char *field){
  i2c_0_tx_buf_ptr = field;
  byteCtr = byteCount;
  UCB0CTL1 |= UCTR + UCTXSTT;                 // I2C TX, start condition
}


//------------------------------------------------------------------------------
// unsigned char TI_USCI_I2C_notready()
//
// This function is used to check if there is commuincation in progress. 
//
// OUT:  unsigned char  =>  0: I2C bus is idle, 
//                          1: communication is in progress
//------------------------------------------------------------------------------
unsigned char TI_USCI_I2C_notready(){
  return (UCB0STAT & UCBBUSY);
}

//------------------------------------------------------------------------------
// unsigned char I2C_wait_till_ready()
//
// This function is used to check if there is commuincation in progress for N number of times. 
//
// OUT:  unsigned char  =>  0: I2C bus is idle, 
//                          1: Timeout Error
//------------------------------------------------------------------------------
#ifdef ENABLE_I2C_RECOVERY
unsigned char I2C_wait_till_ready(){
  INT16U n;
  for(n=0; n<I2C_TRIES; n++){
    if(!TI_USCI_I2C_notready())
      return 0;
  }
  BQ24150A_No_Response_Reset_Cmd = 1;
  //CP_SPI_SIMO_3V3_HIGH;                // {RD} I2C test
  I2C2_EN     = 0;
    
  /* I2C2_3V3_SDA */  
    P3DIR_bit.P1   = 0;      /* Input                                                               */
    P3SEL_bit.P1   = 0;      /* I/O function                                                        */
    P3REN_bit.P1   = 0;      /* Pullup/Pulldown disabled                                            */
    
    /* I2C2_3V3_SCL */  
    P3DIR_bit.P2   = 0;      /* Input                                                               */
    P3SEL_bit.P2   = 0;      /* I/O function                                                        */
    P3REN_bit.P2   = 0;      /* Pullup/Pulldown disabled                                            */

    Sys_DelayMs(10);  
  I2C_Init();
  
  return 1;
}
#else
unsigned char I2C_wait_till_ready(){
  while(TI_USCI_I2C_notready()){
    if(Transaction_failed){
        Transaction_failed = 0;
        BQ24150A_No_Response_Reset_Cmd = 1;
        return 1;
    }
  }
    return 0;
}
#endif

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void  USCIAB0RX_ISR (void)
#else
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCIAB0RX_ISR (void)
#endif
{
  if (UCB0STAT & UCNACKIFG){            // send STOP if slave sends NACK
    UCB0CTL1 |= UCTXSTP;
    UCB0STAT &= ~UCNACKIFG;
    /*
    Sys_BeepHigh(250);
    Sys_DelayMs(200); 
    Sys_BeepHigh(250);
    */
    //BQ24150A_No_Response_Reset_Cmd = 1;    
  }else if (UCB0STAT & UCALIFG){     
    //BQ24150A_No_Response_Reset_Cmd = 1;    
     UCB0STAT &= ~UCALIFG;                                                              // Arbitration Lost 
     TI_USCI_I2C_slave_receiveinit(); // Enable Stop interrupt
     //UCB0CTL1 = UCSWRST;
     //CP_SPI_SIMO_3V3_HIGH;                // {RD} I2C test
     Transaction_failed = 1;       // Device now in slave mode
  }else if (UCB0STAT & UCSTPIFG){    
#ifdef ENABLE_I2C_ERROR_BEEPS    
    Sys_BeepHigh(20); 
    Sys_DelayMs(200);
    Sys_BeepHigh(20);
    Sys_DelayMs(200);
    Sys_BeepHigh(20);
    Sys_DelayMs(200); 
    Sys_BeepHigh(20);    
    Sys_DelayMs(200); 
    Sys_BeepHigh(20);
#endif    
    UCB0STAT &= ~UCSTPIFG;           // Reset Flag 
    UCB0I2CIE  &= ~UCSTPIE;            // Disable Stop interrupt
    //CP_SPI_SIMO_3V3_LOW;
    //Transaction_failed = 0;       // Device now in slave mode
  }else
	UCB0STAT &= ~(UCSTPIFG + UCSTTIFG);
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0TX_VECTOR
__interrupt void  USCIAB0TX_ISR (void)
#else
void __attribute__ ((interrupt(USCIAB0TX_VECTOR))) USCIAB0TX_ISR (void)
#endif
{
  if (UCB0STAT & UCSTPIFG){
#ifdef ENABLE_I2C_ERROR_BEEPS    
    Sys_BeepHigh(20); 
    Sys_DelayMs(200);
    Sys_BeepHigh(20);
    Sys_DelayMs(200);
    Sys_BeepHigh(20);
#endif
    //CP_SPI_SIMO_3V3_LOW;
    //Transaction_failed = 0;       // Device now in slave mode
    UCB0STAT &= ~UCSTPIFG;           // Reset Flag 
    UCB0I2CIE  &= ~UCSTPIE;            // Disable Stop interrupt
  }
  if (IFG2 & UCB0RXIFG){
	if ( UCB0CTL0 & UCMST) {
		if ( byteCtr == 0 ){
		  UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
		  *i2c_0_rx_buf_ptr = UCB0RXBUF;
		  i2c_0_rx_buf_ptr++;
		}
		else {
		  *i2c_0_rx_buf_ptr = UCB0RXBUF;
		  i2c_0_rx_buf_ptr++;
		  byteCtr--;
		}
	}else{ // Slave Receive operations
		*i2c_0_rx_buf_ptr = UCB0RXBUF;
		i2c_0_rx_buf_ptr++;
		//slvByteCtr++;
	}
  }
  else {
    if ( UCB0CTL0 & UCMST) {
      if (byteCtr == 0){
        UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
        UC0IFG &= ~UCB0TXIFG;                   // Clear USCI_B0 TX int flag
      }
      else {
        UCB0TXBUF = *i2c_0_tx_buf_ptr;
        i2c_0_tx_buf_ptr++;
        byteCtr--;
        }
    }else{
      // Slave Transmit operations
      UCB0TXBUF = *i2c_0_tx_buf_ptr;
      i2c_0_tx_buf_ptr++;
      //slvByteCtr++;
    }
  }
}
