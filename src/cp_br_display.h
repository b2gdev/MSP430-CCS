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
*   File Name     : cp_br_display.h
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

#ifndef __CP_BR_DISPLAY_H__
#define __CP_BR_DISPLAY_H__


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
#ifdef BRD_MODULE
    #define CP_BRD_EXT
#else
    #define CP_BRD_EXT extern
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
#define BRD0_DATA_IN        (P4IN & (1<<3))                  /* P4.3                             */
#define BRD                 P4OUT                             /* PORT4                            */
#define BRD1_DATA_OUT       P4                                /* Pin4                             */
#define BRD2_STROBE         P5                                /* Pin5                             */
#define BRD3_CLK            P6                                /* Pin6                             */
#define BRD4_SHUTDOWN       P7                                /* Pin7                             */

#define BRD_CLK_HALF_DELAY  7

#define DISPLAY_ENABLE		1
#define DISPLAY_DISABLE		0

/* Buffer sizes */
#define BRD_DISP_BUF_SIZE   24              /* Size of local Braille display buffer                 */
#define BRD_KEY_BUF_SIZE    5              /* Size of local Key buffer                              */

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
CP_BRD_EXT INT32U BRD_CursorKeysBlock_1;
CP_BRD_EXT INT32U BRD_CursorKeysBlock_2;
CP_BRD_EXT BOOLEAN BRD_KeyChange;

/*
*********************************************************************************************************
*   MACROS
*********************************************************************************************************
*/
#define     BRD_ENABLE()        BIT_CLR(BRD,BRD4_SHUTDOWN)
#define     BRD_DISABLE()       BIT_SET(BRD,BRD4_SHUTDOWN)

/*
*********************************************************************************************************
*   FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void  Brd_Init (void);
void  Brd_DeInit (void);
void  Brd_WriteDisplay (PTR_INT08U buf);
void  Brd_ReadKeys (PTR_INT08U buf);
void Brd_Pwr_Update(INT08U pwr_status);
void  Brd_ClearDisplay(void);
void  Brd_Conrol (INT08U control);
/* {PS} - Uncomment when Write and Read functionality is required */
/* void  Brd_WriteAndRead (PTR_INT08U wr_buf, PTR_INT08U rd_buf); */

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
