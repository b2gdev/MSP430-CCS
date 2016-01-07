/*
*********************************************************************************************************
*   Command Processing
*   Command Processing Functions
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
*   Command Processing Functions
*
*   File Name     : cp_command.h
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
#include    "cp_command.h"

/*
*********************************************************************************************************
*   LOCAL DEFINES
*********************************************************************************************************
*/
#define     DEVICE_COUNT	        7

#define     DEV_CP430               0x01
#define     DEV_KEYPAD		        0x02
#define     DEV_DISPLAY		        0x03
#define     DEV_CHARGER	            0x04
#define     DEV_POWER		        0x05
#define     DEV_MISC		        0x06

#define     CMD_CP430_GET_STATUS    ((DEV_CP430   << 8) + 0x01)
#define     CMD_CP430_GET_SEGMENT   ((DEV_CP430   << 8) + 0x02)
#define     CMD_CP430_GET_METADATA  ((DEV_CP430   << 8) + 0x03)

#define     CMD_KEYPAD_GET_STATUS   ((DEV_KEYPAD  << 8) + 0x01)

#define     CMD_DISPLAY_ON_OFF      ((DEV_DISPLAY << 8) + 0x01)
#define     CMD_DISPLAY_WRITE       ((DEV_DISPLAY << 8) + 0x02)

#define     CMD_CHARGER_GET_STATUS  ((DEV_CHARGER << 8) + 0x01)


#define     CMD_UPDATE_CC_PWR_STATUS  ((DEV_POWER << 8) + 0x01) //KW: CC PWR


#define     STX1                    0x43
#define     STX2                    0x50
#define     ETX                     0x45

#define     MAX_DATA_BUF_SIZE       0x201

#define		RCVD_DATA_LEN_STATUS_REQUEST 	0x00
#define		RCVD_DATA_LEN_DISP_ON_OFF		0x01
#define		RCVD_DATA_LEN_DISP_WRITE		0x18
#define		RCVD_DATA_LEN_PWR_STATUS		0x01
#define		RCVD_DATA_LEN_GET_SEGMENT		0x201
#define		RCVD_DATA_LEN_GET_METADATA		0x24

#define		ACK_CHAR				0x00
#define 	NACK_CHAR				0xFF

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
enum rx_states rx_state;

INT08U  active_rx_device;
INT08U  active_rx_command;
INT16U  active_rx_length;
INT08U  active_rx_checksum;
INT08U  active_rx_data[MAX_DATA_BUF_SIZE];

INT16U  data_length;

INT08U active_tx_checksum;

/*
*********************************************************************************************************
*   LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/
INT16U getDataLength (void);
void generateReplyPacket (INT08U ackchar);

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
void  Cmd_Init (void)
{
    rx_state = RX_STATE_STX1;    
}

void  Cmd_RxCommandProcess (void)
{
    INT08U b = 0x00;
    INT08U c = 0x00;
    INT08S i = 0;
    INT08U buf[8];
    BOOLEAN ret;

    while (!Cbuf_IsEmpty(&SPI_1_ReceiveBuffer)){     /* Check for new received data                  */
       Cbuf_Read(&SPI_1_ReceiveBuffer, &c);

        switch (rx_state){
            case RX_STATE_STX1: 
                {
                    if (STX1 == c){
                        rx_state = RX_STATE_STX2;
                    }else{
                        rx_state = RX_STATE_STX1;    /* out of sync                              */
                    }		
                    
                    break;
                }

            case RX_STATE_STX2: 
                {
                    if (STX2 == c){
                        rx_state = RX_STATE_DEVICE;
                    }
                    else{
                        rx_state = RX_STATE_STX1;    /* out of sync                              */
                    }
                    
                    break;
                }

            case RX_STATE_DEVICE: 
                {
                    if (0 < c && c <= DEVICE_COUNT){
                        active_rx_device   = c;
                        active_rx_checksum = c;
                        rx_state           = RX_STATE_COMMAND;
                    }
                    else{
                        rx_state = RX_STATE_STX1;    /* out of sync, invalid device              */
                    }				
                    
                    break;
                }

            case RX_STATE_COMMAND: 
                {
                    active_rx_command   = c;
                    active_rx_checksum += c;
                    rx_state            = RX_STATE_LENGTH_H;
                    
                    break;
                }

            case RX_STATE_LENGTH_H: 
                {
                    active_rx_length    = (c << 8);
                    active_rx_checksum += c;
                    rx_state            = RX_STATE_LENGTH_L;

                    break;
                }

            case RX_STATE_LENGTH_L:
                {
                    active_rx_length   &= 0xFF00;
                    active_rx_length   |= c;
                    active_rx_checksum += c;

                    data_length         = 0;

                    if(active_rx_length != getDataLength()){
                    	rx_state = RX_STATE_STX1;    	/* out of sync, invalid data length              */
                    }
                    else if (active_rx_length > 0){
                        rx_state = RX_STATE_DATA;
                    }
                    else{
                        rx_state = RX_STATE_CHECKSUM;
                    }
                    
                    break;
                }

            case RX_STATE_DATA: 
                {
                    active_rx_data[data_length] = c;
                    active_rx_checksum         += c;
                    data_length++;
                    
                    if (data_length == active_rx_length){
                        rx_state = RX_STATE_CHECKSUM;
                    }

                    break;
                }

            case RX_STATE_CHECKSUM: 
                {
                    if (c == active_rx_checksum){
                        rx_state = RX_STATE_ETX;
                    }
                    else{
                        rx_state = RX_STATE_STX1;    /* out of sync, invalid device              */
                    }
                    
                    break;
                }

            case RX_STATE_ETX: 
                {
                    if (ETX == c){

                        Cbuf_Write(&SPI_1_TransmitBuffer, STX1);
                        Cbuf_Write(&SPI_1_TransmitBuffer, STX2);
                        Cbuf_Write(&SPI_1_TransmitBuffer, active_rx_device);
                        Cbuf_Write(&SPI_1_TransmitBuffer, active_rx_command);
                        
                        active_tx_checksum = (active_rx_device + active_rx_command);
                        
                        switch ((active_rx_device << 8) + active_rx_command){
                            case CMD_CP430_GET_STATUS:
                                {
                                    Cbuf_Write(&SPI_1_TransmitBuffer, 0x00);
                                    Cbuf_Write(&SPI_1_TransmitBuffer, 0x04);
                                    
                                    active_tx_checksum+= 0x00;
                                    active_tx_checksum+= 0x04;
                                    
                                    Cbuf_Write(&SPI_1_TransmitBuffer, get_fw_ver_major());
                                    Cbuf_Write(&SPI_1_TransmitBuffer, get_fw_ver_minor());
                                    Cbuf_Write(&SPI_1_TransmitBuffer, get_other_code_fw_major());
                                    Cbuf_Write(&SPI_1_TransmitBuffer, get_other_code_fw_minor());
                                    
                                    active_tx_checksum+= get_fw_ver_major();
                                    active_tx_checksum+= get_fw_ver_minor();
                                    active_tx_checksum+= get_other_code_fw_major();
                                    active_tx_checksum+= get_other_code_fw_minor();

                                    break;
                                }
                                
                            case CMD_CP430_GET_SEGMENT:
							{
								if(copy_one_seg_from_spi_data(active_rx_data)){
									generateReplyPacket(ACK_CHAR);
								}else{
									generateReplyPacket(NACK_CHAR);
								}

								break;
							}

                            case CMD_CP430_GET_METADATA:
							{
								copy_metadata(active_rx_data);
								if(validate_metadata()){
									generateReplyPacket(ACK_CHAR);
									meta_data_ok = TRUE;
									meta_data_ack_sent = FALSE;
								}else{
									generateReplyPacket(NACK_CHAR);
								}

								break;
							}

                            case CMD_KEYPAD_GET_STATUS:
                                {
                                    Cbuf_Write(&SPI_1_TransmitBuffer, 0x00);
                                    Cbuf_Write(&SPI_1_TransmitBuffer, 0x0C);    /* 12 byets of key status data */
                                    
                                    active_tx_checksum+= 0x00;
                                    active_tx_checksum+= 0x0C;

                                    for (i = 0; i < 4; i++){
                                        b = (INT08U)(KEYPAD_MainKeys >> (i*8));
                                        Cbuf_Write(&SPI_1_TransmitBuffer, b);
                                        active_tx_checksum += b;
                                    }

                                    for (i = 0; i < 4; i++){
                                        b = (INT08U)(BRD_CursorKeysBlock_1 >> (i*8));
                                        Cbuf_Write(&SPI_1_TransmitBuffer, b);
                                        active_tx_checksum += b;
                                    }

                                    for (i = 0; i < 4; i++){
                                        b = (INT08U)(BRD_CursorKeysBlock_2 >> (i*8));
                                        Cbuf_Write(&SPI_1_TransmitBuffer, b);
                                        active_tx_checksum += b;
                                    }
                                    
                                    break;
                                }
                                
                            case CMD_DISPLAY_ON_OFF:
							{
								if((active_rx_data[0] == DISPLAY_ENABLE) || (active_rx_data[0] == DISPLAY_DISABLE))
									Brd_Conrol(active_rx_data[0]);

								generateReplyPacket(ACK_CHAR);

								break;
							}

                            case CMD_DISPLAY_WRITE:
                                {
                                    Brd_WriteDisplay(active_rx_data);                                  
                                    
                                    generateReplyPacket(ACK_CHAR);

                                    break;
                                }
                                
                            case CMD_CHARGER_GET_STATUS:
                                {                                 
                                    memset(buf, 0, sizeof(buf));
                                    
                                    ret = Bat_GetFuelGaugeStatus(buf, 6); 
                                    
                                    if(ret==FALSE)
                                      memset(buf, 0, sizeof(buf));
                                    
                                    Cbuf_Write(&SPI_1_TransmitBuffer, 0x00);
                                    Cbuf_Write(&SPI_1_TransmitBuffer, 0x06);
                                    
                                    active_tx_checksum+= 0x00;
                                    active_tx_checksum+= 0x06;
                                    
                                    for (i = 0; i < 6; i++) {
                                        Cbuf_Write(&SPI_1_TransmitBuffer, buf[i]);
                                        active_tx_checksum+= buf[i];
                                    }

                                    break;
                                }
                                /* {KW}:: CC PWR*/   
                            case CMD_UPDATE_CC_PWR_STATUS:
                                {                                 
                                    if((active_rx_data[0] == CC_PWR_ACTIVE)   ||
                                    (active_rx_data[0] == CC_PWR_MIDDLE)      ||
                                    (active_rx_data[0] == CC_PWR_SUSPEND)     ||
                                    (active_rx_data[0] == CC_PWR_OFF))
                                    {
                                      Pwr_CC_Pwr_Status_Update(active_rx_data[0]);                                     
                                    }
                                    
                                    /* prepare response packet data here (length = 0x0001, data = 0x00)  */
                                    generateReplyPacket(ACK_CHAR);

                                    break;
                                }
                                /*{KW}:: end */
                                
                            default:
                                {
                                    break;
                                }
                        }
                        
                        Cbuf_Write(&SPI_1_TransmitBuffer, active_tx_checksum);
                        Cbuf_Write(&SPI_1_TransmitBuffer, ETX);
                        
                        SPI_1_TX_INT_ENABLE();
                        
                        CP_INT_HIGH();
                        __delay_cycles(100); 
                        CP_INT_LOW();
                    }
                    
                    rx_state = RX_STATE_STX1;
                   
                    break;
                }

            default:
                {
                    break;
                }
        } /* end of switch block  */
        
    } /* end of while loop    */
}

void  Cmd_TxCommandProcess (void)
{
    INT08U checksum = 0x00;
    INT08S i        = 0x00;
    INT08U b        = 0x00;
    
    if (rx_state == RX_STATE_STX1){
        if (Cbuf_IsEmpty(&SPI_1_TransmitBuffer)){
            if (KEYPAD_KeyChange == TRUE || BRD_KeyChange == TRUE){
                KEYPAD_KeyChange = FALSE;
                BRD_KeyChange    = FALSE;

                Cbuf_Write(&SPI_1_TransmitBuffer, STX1);
                Cbuf_Write(&SPI_1_TransmitBuffer, STX2);

                Cbuf_Write(&SPI_1_TransmitBuffer, (INT08U)DEV_KEYPAD);
                Cbuf_Write(&SPI_1_TransmitBuffer, (INT08U)CMD_KEYPAD_GET_STATUS);
                checksum = (INT08U)(DEV_KEYPAD + CMD_KEYPAD_GET_STATUS);
                
                Cbuf_Write(&SPI_1_TransmitBuffer, 0x00);
                Cbuf_Write(&SPI_1_TransmitBuffer, 0x0C);    /* 12 byets of key status data */
                
                checksum += 0x00;
                checksum += 0x0C;
    
                for (i = 0; i < 4; i++){
                    b = (INT08U)(KEYPAD_MainKeys >> (i*8));
                    Cbuf_Write(&SPI_1_TransmitBuffer, b);
                    checksum += b;
                }
    
                for (i = 0; i < 4; i++){
                    b = (INT08U)(BRD_CursorKeysBlock_1 >> (i*8));
                    Cbuf_Write(&SPI_1_TransmitBuffer, b);
                    checksum += b;
                }
    
                for (i = 0; i < 4; i++){
                    b = (INT08U)(BRD_CursorKeysBlock_2 >> (i*8));
                    Cbuf_Write(&SPI_1_TransmitBuffer, b);
                    checksum += b;
                }
                
                Cbuf_Write(&SPI_1_TransmitBuffer, checksum);
                Cbuf_Write(&SPI_1_TransmitBuffer, ETX);
                
                SPI_1_TX_INT_ENABLE();
                
                CP_INT_HIGH();
                __delay_cycles(100);
                CP_INT_LOW();
            }
        }
    }
}

/*
*********************************************************************************************************
*********************************************************************************************************
*   LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/
INT16U getDataLength (void)
{
	INT16U packetType = ((active_rx_device << 8) + active_rx_command);

	if ((packetType == CMD_CP430_GET_STATUS) || (packetType == CMD_KEYPAD_GET_STATUS) || (packetType == CMD_CHARGER_GET_STATUS)){
		return RCVD_DATA_LEN_STATUS_REQUEST;
	}else if (packetType ==  CMD_DISPLAY_ON_OFF){
		return RCVD_DATA_LEN_DISP_ON_OFF;
	}else if (packetType ==  CMD_DISPLAY_WRITE){
		return RCVD_DATA_LEN_DISP_WRITE;
	}else if (packetType == CMD_UPDATE_CC_PWR_STATUS){
		return RCVD_DATA_LEN_PWR_STATUS;
	}else if (packetType == CMD_CP430_GET_SEGMENT){
		return RCVD_DATA_LEN_GET_SEGMENT;
	}else if (packetType == CMD_CP430_GET_METADATA){
		return RCVD_DATA_LEN_GET_METADATA;
	}else{
		return 0;				//Undefined packet type
	}
}

void generateReplyPacket (INT08U ackchar)
{
	Cbuf_Write(&SPI_1_TransmitBuffer, 0x00);
	active_tx_checksum+= 0x00;

	Cbuf_Write(&SPI_1_TransmitBuffer, 0x01);				// Length =	1
	active_tx_checksum+= 0x01;

	Cbuf_Write(&SPI_1_TransmitBuffer, ackchar);				// ACK or NACK
	active_tx_checksum+= ackchar;
}
