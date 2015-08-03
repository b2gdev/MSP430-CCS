/*
*********************************************************************************************************
*   Braille Display
*   Braille Display Functions
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
*   Braille Display Functions
*
*   File Name     : cp_br_display.c
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
#define BRD_MODULE                  /* Define Braille Display module, this is to handle global variables
                                       declaration and externs appropriately                        */

#include    "cp_conf.h"

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
static INT08U   brd_disp_buf[BRD_DISP_BUF_SIZE];            /* Local Braille display buffer         */
static INT08U   brd_key_buf[BRD_KEY_BUF_SIZE];              /* Local Key buffer                     */   

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
void  Brd_Init (void)
{
    /* Initialize Braille Display Pins */
    P4DIR |=  (BIT7 + BIT6 + BIT5 + BIT4);              /* Output                                   */
    P4DIR &= ~(BIT3);                                   /* Input                                    */
    P4SEL &= ~(BIT7 + BIT6 + BIT5 + BIT4 + BIT3);       /* I/O function                             */
    P4REN &= ~(BIT7 + BIT6 + BIT5 + BIT4 + BIT3);       /* Pullup/Pulldown disabled                 */
                                                        
                                                        /* Pin 39 - P4.3/TB3 - BRD0                 */
                                                        /* Pin 40 - P4.4/TB4 - BRD1                 */
                                                        /* Pin 41 - P4.5/TB5 - BRD2                 */
                                                        /* Pin 42 - P4.6/TB6 - BRD3                 */
                                                        /* Pin 43 - P4.7/TBCLK - BRD4               */
    
    BIT_CLR(BRD,BRD1_DATA_OUT);
    BIT_SET(BRD,BRD2_STROBE);
    BIT_CLR(BRD,BRD3_CLK);
    
    BRD_DISABLE();                                      /* Shutdown Braille display                 */
    
    /* Initialize variables */
    memset(brd_disp_buf, 255, sizeof(brd_disp_buf));
    memset(brd_key_buf, 0, sizeof(brd_key_buf));
    
    BRD_CursorKeysBlock_1 = 0;
    BRD_CursorKeysBlock_2 = 0;
    BRD_KeyChange = FALSE;
}

void  Brd_DeInit (void)
{
    
}

void  Brd_WriteDisplay (PTR_INT08U buf)
{
    INT08S  i, j;
    INT08U value;
    
    memcpy(brd_disp_buf, buf, sizeof(brd_disp_buf));
    
    BIT_CLR(BRD,BRD3_CLK);
    BIT_SET(BRD,BRD2_STROBE);

    for (i = 0; i < BRD_DISP_BUF_SIZE; i++){
        value = brd_disp_buf[i];

        for (j = 0; j < 8; j++){
            BIT_CLR(BRD,BRD3_CLK);
            
            if (value & (1 << 7)){
                BIT_SET(BRD,BRD1_DATA_OUT);
            }
            else{
                BIT_CLR(BRD,BRD1_DATA_OUT);
            }
            
            value <<= 1;

            __delay_cycles(BRD_CLK_HALF_DELAY);
            BIT_SET(BRD,BRD3_CLK);
            __delay_cycles(BRD_CLK_HALF_DELAY);
        }
    }
    
    BIT_CLR(BRD,BRD3_CLK);

	BIT_CLR(BRD,BRD2_STROBE);
    __delay_cycles(BRD_CLK_HALF_DELAY * 2);
    BIT_SET(BRD,BRD2_STROBE);
}

void  Brd_ReadKeys (PTR_INT08U buf)
{
    INT08S  i, j, k;
    INT08U value;
    INT32U prev_key = 0;

    
    if(is_power_switch_on && !(Pwr_Get_CC_Pwr_Status()==CC_PWR_OFF)){
      BIT_CLR(BRD,BRD3_CLK);
  
      BIT_SET(BRD,BRD2_STROBE);
      __delay_cycles(BRD_CLK_HALF_DELAY);
      BIT_CLR(BRD,BRD2_STROBE);
      __delay_cycles(BRD_CLK_HALF_DELAY);
  
      for (i = (BRD_KEY_BUF_SIZE - 1); i >= 0; i--){
          value = 0;
          
          for (j = 0; j < 8; j++){
  
              __delay_cycles(BRD_CLK_HALF_DELAY);
              BIT_SET(BRD,BRD3_CLK);
              __delay_cycles(BRD_CLK_HALF_DELAY);
  
              if (!BRD0_DATA_IN){
                  value |= (1 << j);
              }
              
              BIT_CLR(BRD,BRD3_CLK);
          }
          
          brd_key_buf[i] = value;
      }
      
      if (buf != NULL){
          memcpy(buf, brd_key_buf, sizeof(brd_key_buf));
      }
              
      BIT_CLR(BRD,BRD3_CLK);
      BIT_SET(BRD,BRD2_STROBE);
      
      prev_key = BRD_CursorKeysBlock_1;
      BRD_CursorKeysBlock_1 = 0;
      k = 0;
     
      for (i = 0; i < 5; i++){
          for (j = 0; j < 8; j+= 2){
              if ((brd_key_buf[i] >> j) & 0x01){
                  BRD_CursorKeysBlock_1 |= (1L << k);
              }
              k++;
          }
      }
      
      if (BRD_CursorKeysBlock_1 != prev_key){
          BRD_KeyChange = TRUE;
      }
    }
}

void Brd_Pwr_Update(INT08U pwr_status)
{
  if(pwr_status == CC_PWR_OFF){
    Brd_ClearDisplay();
    BRD_DISABLE();
    CP_PWR01_DISABLE(); /*{KW}: Brd power supply disable */
  }
  else if((is_power_switch_on || (pwr_status == CC_PWR_ACTIVE)) && (pwr_status != CC_PWR_OFF)){
    BRD_ENABLE();  
  }
  else{
    Brd_ClearDisplay();
    BRD_DISABLE();  
  }
}


void  Brd_ClearDisplay(void)
{
  INT08U brl_disp_clear_buf[BRD_DISP_BUF_SIZE];
  memset(brl_disp_clear_buf, 0, sizeof(brd_disp_buf));
  Brd_WriteDisplay(brl_disp_clear_buf);
}

void  Brd_Conrol (INT08U control){
	if(control == DISPLAY_ENABLE){
		BRD_ENABLE();
	}else
		BRD_DISABLE();
}


/* {PS} - Uncomment if Write and Read functionality is required */
/*
void  Brd_WriteAndRead (PTR_INT08U wr_buf, PTR_INT08U rd_buf)
{
    INT08S  i, j;
    INT08U value;
    INT32U prev_key = 0x0000;
    
    memcpy(brd_disp_buf, wr_buf, sizeof(brd_disp_buf));
    
    BIT_CLR(BRD,BRD3_CLK);
    BIT_SET(BRD,BRD2_STROBE);

    for (i = 0; i < BRD_DISP_BUF_SIZE; i++){
        value = brd_disp_buf[i];

        for (j = 0; j < 8; j++){
            BIT_CLR(BRD,BRD3_CLK);
            
            if (value & (1 << 7)){
                BIT_SET(BRD,BRD1_DATA_OUT);
            }
            else{
                BIT_CLR(BRD,BRD1_DATA_OUT);
            }
            
            value <<= 1;

            __delay_cycles(BRD_CLK_HALF_DELAY);
            BIT_SET(BRD,BRD3_CLK);
            __delay_cycles(BRD_CLK_HALF_DELAY);
        }
    }
    
    BIT_CLR(BRD,BRD3_CLK);
    BIT_CLR(BRD,BRD2_STROBE);
    __delay_cycles(BRD_CLK_HALF_DELAY);

    for (i = (BRD_KEY_BUF_SIZE - 1); i >= 0; i--){
        value = 0;
        
        for (j = 0; j < 8; j++){

            __delay_cycles(BRD_CLK_HALF_DELAY);
            BIT_SET(BRD,BRD3_CLK);
            __delay_cycles(BRD_CLK_HALF_DELAY);

            if (!BRD0_DATA_IN){
                value |= (1 << j);
            }
            
            BIT_CLR(BRD,BRD3_CLK);
        }
        
        brd_key_buf[i] = value;
    }
    
    if (rd_buf != NULL){
        memcpy(rd_buf, brd_key_buf, sizeof(brd_key_buf));
    }
            
    BIT_CLR(BRD,BRD3_CLK);
    BIT_SET(BRD,BRD2_STROBE);
    
    prev_key = BRD_CursorKeysBlock_1;
    
    for (i = 0; i < 4; i++){
        BRD_CursorKeysBlock_1 |= (brd_key_buf[i] << 0);
        BRD_CursorKeysBlock_1 |= (brd_key_buf[i] << 2);
        BRD_CursorKeysBlock_1 |= (brd_key_buf[i] << 4);
        BRD_CursorKeysBlock_1 |= (brd_key_buf[i] << 6);
    }
    
    if (BRD_CursorKeysBlock_1 != prev_key){
        BRD_KeyChange = TRUE;
    }    
}
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*   LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/
