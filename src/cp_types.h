/*
*********************************************************************************************************
*   TYPES
*   Data Types
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
*   Portable Data Types
*
*   File Name     : cp_types.h
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

#ifndef __CP_TYPES_H__
#define __CP_TYPES_H__


/*
*********************************************************************************************************
*   INCLUDE FILES
*********************************************************************************************************
*/

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
/* Boolean Types */
#define TRUE            1
#define FALSE           0

/*
*********************************************************************************************************
*   DATA TYPES
*********************************************************************************************************
*/
typedef unsigned char           CHAR;       /* 8-bit character                  */
typedef unsigned char           BOOLEAN;    /* 8-bit boolean or logical         */
typedef unsigned char           INT08U;     /* 8-bit unsigned integer           */
typedef   signed char           INT08S;     /* 8-bit signed integer             */
typedef unsigned short          INT16U;     /* 16-bit unsigned integer          */
typedef   signed short          INT16S;     /* 16-bit signed integer            */
typedef unsigned long           INT32U;     /* 32-bit unsigned integer          */
typedef   signed long           INT32S;     /* 32-bit signed integer            */
typedef unsigned long long      INT64U;     /* 64-bit unsigned integer          */
typedef   signed long long      INT64S;     /* 64-bit signed integer            */
typedef          float          FP32;       /* 32-bit floating point            */
typedef          double         FP64;       /* 64-bit floating point            */

typedef unsigned char*          PTR_CHAR;   /* 8-bit character pointer          */
typedef unsigned char*          PTR_INT08U; /* 8-bit unsigned integer pointer   */
typedef   signed char*          PTR_INT08S; /* 8-bit signed integer pointer     */
typedef unsigned short*         PTR_INT16U; /* 16-bit unsigned integer pointer  */
typedef   signed short*         PTR_INT16S; /* 16-bit signed integer pointer    */
typedef unsigned long*          PTR_INT32U; /* 32-bit unsigned integer pointer  */
typedef   signed long*          PTR_INT32S; /* 32-bit signed integer pointer    */
typedef unsigned long long*     PTR_INT64U; /* 64-bit unsigned integer pointer  */
typedef   signed long long*     PTR_INT64S; /* 64-bit signed integer pointer    */
typedef          float*         PTR_FP32;   /* 32-bit floating point pointer    */
typedef          double*        PTR_FP64;   /* 64-bit floating point pointer    */

/* Circular Buffer Types */
typedef struct
{
    INT16U wr_ptr;          /* write pointer                                    */
    INT16U rd_ptr;          /* read pointer                                     */
    INT16U size;            /* size of circular buffer                          */
    INT08U *data;           /* data                                             */
} CIRC_BUFFER;

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

#endif  /* End of __CP_TYPES_H__ */
