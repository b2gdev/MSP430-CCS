/*
*********************************************************************************************************
*   Circular Buffer
*   Circular Buffer Functions
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
*   Circular Buffer Functions
*
*   File Name     : cp_circ_buf.c
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
#include    "cp_circ_buf.h"

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
/* Init Ciruclar Buffer */
void  Cbuf_Init (CIRC_BUFFER *c_buf, INT08U *data, INT16U size)
{
    c_buf->wr_ptr = 0;
    c_buf->rd_ptr = 0;
    c_buf->size   = size;
    c_buf->data   = data;
}
 
inline BOOLEAN  Cbuf_IsFull (CIRC_BUFFER* c_buf)
{
    if (c_buf->rd_ptr == 0){    /* return (((c_buf->wr_ptr + 1) % c_buf->size) == c_buf->rd_ptr);   */
        return ((c_buf->wr_ptr + 1) == c_buf->size);
    }
    else{
        return ((c_buf->wr_ptr + 1) == c_buf->rd_ptr);
    }
}
 
BOOLEAN  Cbuf_IsEmpty (CIRC_BUFFER* c_buf)
{
    return (c_buf->rd_ptr == c_buf->wr_ptr);
}
 
inline BOOLEAN  Cbuf_Write (CIRC_BUFFER* c_buf, INT08U data)
{
    BOOLEAN is_full = FALSE;

    if(!is_full){
        c_buf->data[c_buf->wr_ptr] = data;
        c_buf->wr_ptr++;
        
        if (c_buf->wr_ptr == c_buf->size){      /* c_buf->wr_ptr %= c_buf->size; */
            c_buf->wr_ptr = 0;
        }
    }
    
    return is_full;
}
 
inline BOOLEAN  Cbuf_Read (CIRC_BUFFER* c_buf, INT08U* data)
{
    BOOLEAN is_empty = Cbuf_IsEmpty(c_buf);
    
    if(!is_empty){
        *data = c_buf->data[c_buf->rd_ptr];
        c_buf->rd_ptr++;
        
        if (c_buf->rd_ptr == c_buf->size){      /* c_buf->rd_ptr %= c_buf->size; */
            c_buf->rd_ptr = 0;
        }
    }
    
    return is_empty;
}


/*
*********************************************************************************************************
*********************************************************************************************************
*   LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/
