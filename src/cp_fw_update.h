/*
*********************************************************************************************************
*   Firmware update
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
*   Firmware update
*
*   File Name     : cp_fw_update.h
*   Version       : V1.0.0
*   Programmer(s) : AH
*********************************************************************************************************
*   Note(s)
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*   MODULE
*********************************************************************************************************
*/

#ifndef __CP_FW_UPDTE_H__
#define __CP_FW_UPDTE_H__


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
#define SEG_SIZE	 			512
#define SEG_STRT				0

#define ISR_MEM_SEG_START	 	0xFE00
#define COPY_FUNC_START_LOC	 	0x9400
#define COPY_CODE_START_LOC	 	0x16200
#define INFOB_START_LOC	 		0x1080
#define INFOC_START_LOC	 		0x1040

#define COPY_CODE_RAM_LOC	 	0x1100

#define NO_OF_CODE_PRTS			4
#define NO_OF_WRITE_TRIES		5

#define COPY_FUNC_SIZE_MAX		0x0400
#define MAIN_MEM_SIZE_MAX		0x67BE
#define EXT_MEM_SIZE_MAX		0x31FF
#define ISR_MEM_SIZE_MAX		0x40

#define SEGMENT_NO_POS			0
#define SEGMENT_NO_MIN			0
#define SEGMENT_NO_MAX			78

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
extern BOOLEAN meta_data_ok;
extern BOOLEAN meta_data_ack_sent;

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
INT08U get_fw_ver_major (void);
INT08U get_fw_ver_minor (void);
INT08U get_other_code_fw_major (void);
INT08U get_other_code_fw_minor (void);
BOOLEAN get_first_time_run (void);
void set_first_time_run_false(void);
BOOLEAN copy_one_seg_from_spi_data(PTR_INT08U segment_data);
BOOLEAN validate_metadata ();
void copy_metadata (PTR_INT08U meta_data);
void copy_to_ram(void);
void write_code_from_ram(void);

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

#endif  /* End of __CP_FW_UPDTE_H__ */
