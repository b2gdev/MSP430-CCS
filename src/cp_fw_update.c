/*
*********************************************************************************************************
*   Firmware update
*
*   Copyright � 2010 Zone24x7 Inc. All rights reserved.
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
*   File Name     : cp_fw_update.c
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

/*
*********************************************************************************************************
*   INCLUDE FILES
*********************************************************************************************************
*/
#include    "cp_fw_update.h"

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
BOOLEAN meta_data_ok = FALSE;
BOOLEAN meta_data_ack_sent = FALSE;

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma DATA_SECTION(code_data_at_infoB,".infoB")
CODE_DATA_STR code_data_at_infoB;
#else
CODE_DATA_STR code_data_at_infoB __attribute__((section(".infoB")));
#endif

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma DATA_SECTION(first_time_run,".infoC")
volatile BOOLEAN first_time_run;
#else
volatile BOOLEAN first_time_run __attribute__((section(".infoC")));
#endif

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma DATA_SECTION(base_code_fw_ver,".infoD")
const volatile BASE_CODE_FW_VER_STR base_code_fw_ver = {.fw_ver_major = FW_VERSION_MAJOR, .fw_ver_minor = FW_VERSION_MINOR};
#else
BASE_CODE_FW_VER_STR base_code_fw_ver __attribute__((section(".infoD"))) = {.fw_ver_major = FW_VERSION_MAJOR, .fw_ver_minor = FW_VERSION_MINOR};
#endif

#warning "Please verify weather int_vec_data matches with data in ISR space"

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
INT08U int_vec_data[INT_VEC_SZ_TI] = {
#ifdef USE_BQ24160
		0x18, 0x36, 0x18, 0x36, 0x18, 0x36, 0x18, 0x36, 0x18, 0x36, 0x18, 0x36, 0x18, 0x36, 0x18, 0x36,
		0x18, 0x36, 0x18, 0x36, 0x18, 0x36, 0x18, 0x36, 0x18, 0x36, 0x18, 0x36, 0x18, 0x36, 0x18, 0x36,
		0xA2, 0x35, 0xE0, 0x35, 0x18, 0x36, 0x00, 0x31, 0x18, 0x36, 0xF2, 0x34, 0x6A, 0x34, 0x62, 0x35,
		0x18, 0x36, 0x86, 0x33, 0x18, 0x36, 0x18, 0x36, 0x18, 0x36, 0x84, 0x32, 0x18, 0x36, 0xFE, 0x35 };
#else
		0xDC, 0x35, 0xDC, 0x35, 0xDC, 0x35, 0xDC, 0x35, 0xDC, 0x35, 0xDC, 0x35, 0xDC, 0x35, 0xDC, 0x35,
		0xDC, 0x35, 0xDC, 0x35, 0xDC, 0x35, 0xDC, 0x35, 0xDC, 0x35, 0xDC, 0x35, 0xDC, 0x35, 0xDC, 0x35,
		0x66, 0x35, 0xA4, 0x35, 0xDC, 0x35, 0x00, 0x31, 0xDC, 0x35, 0xB6, 0x34, 0x2E, 0x34, 0x26, 0x35,
		0xDC, 0x35, 0x86, 0x33, 0xDC, 0x35, 0xDC, 0x35, 0xDC, 0x35, 0x84, 0x32, 0xDC, 0x35, 0xC2, 0x35 };
#endif
#else
INT08U int_vec_data[INT_VEC_SZ_GNU] = {
#ifdef USE_BQ24160
		0xEE, 0x34, 0xCC, 0x34, 0xFF, 0xFF, 0x40, 0x35, 0xFF, 0xFF, 0x1C, 0x33, 0x08, 0x34, 0xAE, 0x33,
		0xFF, 0xFF, 0x08, 0x37, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x12, 0x38, 0xFF, 0xFF, 0xA4, 0x32 };
#else
		0xEE, 0x34, 0xCC, 0x34, 0xFF, 0xFF, 0x40, 0x35, 0xFF, 0xFF, 0x1C, 0x33, 0x08, 0x34, 0xAE, 0x33,
		0xFF, 0xFF, 0x08, 0x37, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC4, 0x37, 0xFF, 0xFF, 0xA4, 0x32 };
#endif
#endif

/*
*********************************************************************************************************
*   LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void flash_write(PTR_INT08U src, PTR_INT08U dest, INT16U len);

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
INT08U get_fw_ver_major (void){
	return base_code_fw_ver.fw_ver_major;
}

INT08U get_fw_ver_minor (void){
	return base_code_fw_ver.fw_ver_minor;
}

INT08U get_other_code_fw_major (void){
	return code_data_at_infoB.fw_ver_major;
}

INT08U get_other_code_fw_minor (void){
	return code_data_at_infoB.fw_ver_minor;
}

INT16U get_reset_address (void){
	return code_data_at_infoB.reset_address;
}

BOOLEAN health_check (void){												// Verfies the checksums in metadata structure
	PTR_INT08U actual_loc;
	PTR_INT08U locations [] = {(PTR_INT08U) code_data_at_infoB.copy_func_start_loc, (PTR_INT08U)  code_data_at_infoB.main_mem_start_loc, (PTR_INT08U)  code_data_at_infoB.isr_mem_start_loc, (PTR_INT08U)  code_data_at_infoB.ext_mem_start_loc};
	INT16U lengths [] = {code_data_at_infoB.copy_func_code_size, code_data_at_infoB.main_mem_code_size, code_data_at_infoB.isr_mem_code_size, code_data_at_infoB.ext_mem_code_size};
	INT08U chksms [] = {code_data_at_infoB.copy_func_code_checksum, code_data_at_infoB.main_mem_code_checksum, code_data_at_infoB.isr_mem_code_checksum, code_data_at_infoB.ext_mem_code_checksum};
	INT08U cur_chksm = 0;
	INT16U i,j;

	for (i = 0; i < NO_OF_CODE_PRTS; i++){
		if(locations[i] == ((PTR_INT08U) INFOB_UNINIT_CHAR)){
			return FALSE;
		}

		actual_loc = locations[i];
		cur_chksm = 0;

		for(j = 0; j < lengths[i]; j++){
			cur_chksm += *actual_loc;
			actual_loc++;
		}

		if(cur_chksm != chksms[i]){
			return FALSE;
		}
	}

	if(!(code_data_at_infoB.copy_func_code_size && code_data_at_infoB.main_mem_code_size && code_data_at_infoB.isr_mem_code_size)){
		return FALSE;
	}

	return TRUE;
}
																		// If health_check fails ISR space data will be loaded with base code addresses
void write_int_vec_data(void){
	INT08U i;

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
	INT08U len = INT_VEC_SZ_TI;
	PTR_INT08U isr_data_ptr = (PTR_INT08U) (ISR_DATA_START_TI);
#else
	INT08U len = INT_VEC_SZ_GNU;
	PTR_INT08U isr_data_ptr = (PTR_INT08U) (ISR_DATA_START_GNU);
#endif

	for(i=0; i<len; i++){
		if(((*isr_data_ptr) != int_vec_data[i]) && (int_vec_data[i] != 0xFF)){
			break;
		}
		isr_data_ptr++;
	}

	if(i<len){															// If current ISR data is different from base code ISR data
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
		flash_write(int_vec_data,(PTR_INT08U) (ISR_DATA_START_TI),len);
#else
		flash_write(int_vec_data,(PTR_INT08U) (ISR_DATA_START_GNU),len);
#endif
	}
}

BOOLEAN get_first_time_run (void){										// Weather it is necessary to auto start
	return first_time_run;
}

void set_first_time_run_false(void){
	INT08U src = FALSE;
	if(first_time_run){
		flash_write(&src,(PTR_INT08U) (&first_time_run), 1);
	}
}

BOOLEAN copy_one_seg_from_spi_data(PTR_INT08U segment_data)
{
	if(segment_data[SEGMENT_NO_POS] <= SEGMENT_NO_MAX){
		flash_write(&(segment_data[SEGMENT_NO_POS + 1]), (PTR_INT08U) (COPY_CODE_START_LOC  + (segment_data[SEGMENT_NO_POS] * (INT32U) (SEG_SIZE))), SEG_SIZE);
		return TRUE;
	} else {
		return FALSE;
	}
}

BOOLEAN validate_metadata ()
{
	INT16U i,j;
	PTR_INT08U copy_loc = (PTR_INT08U) COPY_CODE_START_LOC;
	INT16U lengths [] = {code_data_at_infoB.copy_func_code_size, code_data_at_infoB.main_mem_code_size, code_data_at_infoB.isr_mem_code_size, code_data_at_infoB.ext_mem_code_size};
	INT08U chksms [] =  {code_data_at_infoB.copy_func_code_checksum, code_data_at_infoB.main_mem_code_checksum, code_data_at_infoB.isr_mem_code_checksum, code_data_at_infoB.ext_mem_code_checksum};
	INT08U checksum = 0;

	if((code_data_at_infoB.copy_func_code_size > ((INT16U) COPY_FUNC_SIZE_MAX)) || (code_data_at_infoB.main_mem_code_size > ((INT16U) MAIN_MEM_SIZE_MAX)) || (code_data_at_infoB.ext_mem_code_size > ((INT16U) EXT_MEM_SIZE_MAX)) || (code_data_at_infoB.isr_mem_code_size > ((INT16U) ISR_MEM_SIZE_MAX))){
		return FALSE;													// Weather it is possible to accommadate code size
	}

	for (i = 0; i < NO_OF_CODE_PRTS; i++)
	{
		checksum = 0;
		for (j = 0; j < lengths[i]; j++)
		{
		  checksum += *copy_loc;
		  copy_loc++;
		}

		if(lengths[i] && (chksms[i] != checksum)){						// Validate checksums for all available code regions
			return FALSE;
		}
	}

	return TRUE;
}

void copy_metadata (PTR_INT08U meta_data)
{
	flash_write(meta_data, (PTR_INT08U) INFOB_START_LOC, sizeof(code_data_at_infoB));
}


void write_code_from_flash(void)
{
  PTR_INT08U actual_loc, copied_loc = (PTR_INT08U) COPY_CODE_START_LOC;
  INT16U i,j,tried=1;
  INT32U temp;
  PTR_INT08U ptr_first_time_run = (PTR_INT08U) INFOC_START_LOC;
  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  // Keep the order of locations, lengths, chksms in the ascending order of start locations
  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  // -> Copying Code, Main Mem, ISR Mem, Ext Mem
  PTR_INT08U locations [] = {(PTR_INT08U) code_data_at_infoB.copy_func_start_loc, (PTR_INT08U)  code_data_at_infoB.main_mem_start_loc, (PTR_INT08U)  code_data_at_infoB.isr_mem_start_loc, (PTR_INT08U)  code_data_at_infoB.ext_mem_start_loc};
  INT16U lengths [] = {code_data_at_infoB.copy_func_code_size, code_data_at_infoB.main_mem_code_size, code_data_at_infoB.isr_mem_code_size, code_data_at_infoB.ext_mem_code_size};
  INT08U chksms [] = {code_data_at_infoB.copy_func_code_checksum, code_data_at_infoB.main_mem_code_checksum, code_data_at_infoB.isr_mem_code_checksum, code_data_at_infoB.ext_mem_code_checksum};
  INT08U cur_chksm = 0;
  PTR_INT08U main_mem_final_addr = ((PTR_INT08U) code_data_at_infoB.main_mem_start_loc) + code_data_at_infoB.main_mem_code_size;
  PTR_INT08U isr_mem_start_addr = (PTR_INT08U) code_data_at_infoB.isr_mem_start_loc;

  FCTL2 = FWKEY + FSSEL1 + FN5 + FN3 + FN2 + FN1 + FN0;             // SMCLK/48 for Flash Timing Generator
  while(FCTL3&BUSY);
  FCTL3 = FWKEY;		                     						// Clear LOCK

  for (i = 0; i < NO_OF_CODE_PRTS; )
  {
	  actual_loc = locations[i];
	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	// Copy function, main memory and extension memory writing should start form a beginning of a segment
	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	// ISR space is in the middle of a segment
	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	// If the segment that contains the ISR space was not erased from main memory writing
	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	// erase it at the start of isr mem writing
	  if((actual_loc == isr_mem_start_addr) && (main_mem_final_addr < (PTR_INT08U) ISR_MEM_SEG_START))
	  {
		  FCTL1 = FWKEY + ERASE;
		  *actual_loc = 0;
		  while(FCTL3&BUSY);
		  FCTL1 = FWKEY + WRT;
	  }

  	  for (j = 0; j < lengths[i]; j++)
  	  {
		  temp = (INT32U) actual_loc;								// Erase segment at the beginning of a segment
		  if((temp % SEG_SIZE) == SEG_STRT){
			  FCTL1 = FWKEY + ERASE;
			  *actual_loc = 0;
			  while(FCTL3&BUSY);

			  FCTL1 = FWKEY + WRT;
		  }

		  *actual_loc = *copied_loc;								// Copy to code location
		  while(FCTL3&BUSY);

		  actual_loc++;
		  copied_loc++;
  	  }

  	  FCTL1 = FWKEY;												// Generate checksum
  	  cur_chksm =0;
  	  actual_loc = locations[i];
  	  for(j = 0; j < lengths[i]; j++){
  		cur_chksm += *actual_loc;
  		actual_loc++;
  	  }
  	  FCTL1 = FWKEY + WRT;

  	  if(cur_chksm == chksms[i]){									// Chcecksum OK
  		  i++;
  		  tried = 1;
  		  if(i >= NO_OF_CODE_PRTS){
  			FCTL1 = FWKEY + ERASE;
			*ptr_first_time_run = 0;
			while(FCTL3&BUSY);

			FCTL1 = FWKEY + WRT;
			*ptr_first_time_run = TRUE;
			while(FCTL3&BUSY);
  		  }
  	  }else if(tried < NO_OF_WRITE_TRIES){							// Checksum wrong but no of tries remaining
  		  tried++;
  		  copied_loc -= lengths[i];
  	  }else{
  		  i=NO_OF_CODE_PRTS+1;										// Unsuccessful - Write
  	  }
  }

  FCTL1 = FWKEY;
  FCTL3 = FWKEY + LOCK;                     						// LOCK flash

  WDTCTL = 0;														// PUC
}

/*
*********************************************************************************************************
*********************************************************************************************************
*   LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/
void flash_write(PTR_INT08U src, PTR_INT08U dest, INT16U len){
	INT16U i;

	__bic_SR_register(GIE);
	FCTL2 = FWKEY + FSSEL1 + FN5 + FN3 + FN2 + FN1 + FN0;           	// SMCLK/48 for Flash Timing Generator

	FCTL3 = FWKEY;                     									// Clear Lock bit
	FCTL1 = FWKEY + ERASE;
	*dest = 0;						                           			// Dummy write to erase the segment

	FCTL1 = FWKEY + WRT;

	for (i = 0; i < len; i++)
	{
		*dest = *src;													// Copy contents
		dest++;
		src++;

	}

	FCTL1 = FWKEY;                            							// Clear WRT bit
	FCTL3 = FWKEY + LOCK;                     							// Set LOCK bit
	__bis_SR_register(GIE);
}
