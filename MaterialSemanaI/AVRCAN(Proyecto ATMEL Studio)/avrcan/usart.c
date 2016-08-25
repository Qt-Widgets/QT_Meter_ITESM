/****************************************************************** 
* usart.c The Universal Synchronous and Asynchronous serial Receiver
* 		 and Transmitter (USART)
* Created on: Jul 9, 2015
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
*© 2015
*****************************************************************/ 
#ifndef usart_C_
 #define usart_C_
#endif

/**
 * \addtogroup usart_module
 * @{
 */

/******************************************************************/
/*    I N C L U D E   F I L E S                                   */
/******************************************************************/
#include <usart.h>
#include <cpu.h>
#include <avr/interrupt.h>
#include <system_callbacks.h>

/*******************************************************************************************/
/*    M A C R O S                                                                          */
/*******************************************************************************************/

typedef enum{
	GPIO_IN=0,
	GPIO_OUT
}GPIO_TYPES;

typedef struct
{
	uint16_t ubrr;
	uint8_t  u2x;
}USART_CONFIG;

#define GPIO_TX_USART0			(1<<1)
#define BAUDREG  				((OSCSPEED / (16 * 9600)) - 1)

#define cbi(port, bit) 			(port) &= ~(1 << (bit))
#define sbi(port, bit) 			(port) |= (1 << (bit))

#define RX_DATA_COMPLETE		(1<<RXC0)
#define TX_DATA_COMPLETE		(1<<5)

#define SET_GPIO_INPUT(ddr_port, bit) (ddr_port) &= ~(1 << (bit))
#define SET_GPIO_OUTPUT(ddr_port, bit) (ddr_port) |= (1 << (bit))


//---------------------------------------
//   16MHZ Matrix
//---------------------------------------
#ifdef UBRR_16MHZ_MATRIX
#if DOUBLE_SPEED_OPERATION == 0

const USART_CONFIG usart_table[MAX_SUPPORTED_BAUDRATE]={
      {416	    ,0},  	//	error -0.10%
      {207	    ,0},  	//	error  0.20%
      {103	    ,0},  	//	error  0.20%
      {68	    ,0},  	//	error  0.60%
      {51	    ,0},  	//	error  0.20%
      {34	    ,0},  	//	error -0.80%
      {25	    ,0},  	//	error  0.20%
      {16	    ,0},  	//	error  2.10%
      {12	    ,0},  	//	error  0.20%
      {8	    ,0},  	//	error -3.50%
      {3	    ,0},  	//	error  8.50%
      {3	    ,0},  	//	error  0.00%
      {1	    ,0},  	//	error  0.00%
      {0	    ,0}  	//	error  0.00%
    };

#else

const USART_CONFIG usart_table[MAX_SUPPORTED_BAUDRATE]={
      {832	    ,1},  	//	error  0.00%
      {416	    ,1},  	//	error -0.10%
      {207	    ,1},  	//	error  0.20%
      {138	    ,1},  	//	error -0.10%
      {103	    ,1},  	//	error  0.20%
      {68	    ,1},  	//	error  0.60%
      {51	    ,1},  	//	error  0.20%
      {34	    ,1},  	//	error -0.80%
      {25	    ,1},  	//	error  0.20%
      {16	    ,1},  	//	error  2.10%
      {8	    ,1},  	//	error -3.50%
      {7	    ,1},  	//	error  0.00%
      {3	    ,1},  	//	error  0.00%
      {1	    ,1} 	//	error 0.00%
    };

#endif
#endif

static uint8_t* uart_tx_buffer = NULL;
static uint8_t* uart_rx_buffer = NULL;
static uint32_t uart_tx_len = 0;
static uint32_t uart_rx_len = 0;
static uint32_t uart_tx_idx = 0;
static uint32_t uart_rx_idx = 0;
static uint8_t  uart_in_progress = 0;

/*******************************************************************************************/
/*    F U N C T I O N   P R O T O T Y P E S                                                */
/*******************************************************************************************/



/**********************************************************************************//**
 *  \param[in] param Description of the  parameter of the function.
 *  \param[out] param Description of the  parameter of the function.
 *  \return 0
 *  \brief Module Description
 *  \see See more information Here !!
 *  \note First Implementation 5:34:44 PM Jul 9, 2015
 *  \warning Warning.
 *  \author Author
 ****************************************************************************************/
void usart_init(uint8_t Baud)
{
	uint16_t BaudRate =0;
	uint8_t  u2x_double_speed=0;

	//GPIO ENABLE
	SET_GPIO_OUTPUT(USART0_DDR_PORT,TX_USART0_PIN);
	SET_GPIO_INPUT(USART0_DDR_PORT,RX_USART0_PIN);

	BaudRate=(uint16_t) usart_table[Baud].ubrr;
	u2x_double_speed=usart_table[Baud].u2x;

	//set BaudRate into registers
	UBRR0H = (unsigned char) (BaudRate>>8);
	UBRR0L = (unsigned char) BaudRate;

	// Bit 7 – RXCIEn: RX Complete Interrupt Enable
	// Bit 6 – TXCIEn: TX Complete Interrupt Enable
	// Bit 5 – UDRIEn: USARTn Data Register Empty Interrupt Enable
	// Bit 4 – RXENn: Receiver Enable
	// Bit 3 – TXENn: Transmitter Enable
	// Bit 1 – RXB8n: Receive Data Bit 8
	// Bit 0 – TXB8n: Transmit Data Bit 8

	UCSR0B= (1<<RXEN0) | // RX USART ENABLE
			(1<<TXEN0);	 // TX USART ENABLE

	UCSR0C = (USART_Char_Size_8_bit <<UCSZ00);
	//UART_Transmit((uint8_t)0x00);
	/*Enable Interrupts*/
	cbSetTXBuffer( uart_tx_buffer, &uart_tx_len);
	cbSetRXBuffer( uart_rx_buffer, &uart_rx_len);
	uart_tx_idx = 0;
	uart_rx_idx = 0;
	UART_setBusy( FALSE );
	usart_enable_interrupts();
}

/**********************************************************************************//**
 *  \param[in] param Description of the  parameter of the function.
 *  \param[out] param Description of the  parameter of the function.
 *  \return 0
 *  \brief Module Description
 *  \see See more information Here !!
 *  \note First Implementation 9:04:43 AM Jul 13, 2015
 *  \warning Warning.
 *  \author Author
 ****************************************************************************************/
void usart_enable_interrupts(void){
	  sbi(UCSR0B,RXCIE0);
}

/**********************************************************************************//**
 *  \param[in] param Description of the  parameter of the function.
 *  \param[out] param Description of the  parameter of the function.
 *  \return 0
 *  \brief Module Description
 *  \see See more information Here !!
 *  \note First Implementation 9:05:05 AM Jul 13, 2015
 *  \warning Warning.
 *  \author Author
 ****************************************************************************************/
void usart_disable_interrupts(void){
	cbi(UCSR0B,RXCIE0);
	cli();
}

/**********************************************************************************//**
 *  \param[in] param Description of the  parameter of the function.
 *  \param[out] param Description of the  parameter of the function.
 *  \return 0
 *  \brief Module Description
 *  \see See more information Here !!
 *  \note First Implementation 9:05:09 AM Jul 13, 2015
 *  \warning Warning.
 *  \author Author
 ****************************************************************************************/
uint8_t UART_Receive()
{
	if (UCSR0A & RX_DATA_COMPLETE)  //  if(UCSRA & RX_DATA_COMPLETE)
		return UDR0;
	else
		return 0;
}

/**********************************************************************************//**
 *  \param[in] param Description of the  parameter of the function.
 *  \param[out] param Description of the  parameter of the function.
 *  \return 0
 *  \brief Module Description
 *  \see See more information Here !!
 *  \note First Implementation 9:05:16 AM Jul 13, 2015
 *  \warning Warning.
 *  \author Author
 ****************************************************************************************/
void UART_Transmit(uint8_t Data)
{
	while (!(UCSR0A & TX_DATA_COMPLETE )){

	}
	UDR0 = (uint8_t)Data;
}

/**********************************************************************************//**
 *  \param[in] param Description of the  parameter of the function.
 *  \param[out] param Description of the  parameter of the function.
 *  \return 0
 *  \brief Module Description
 *  \see See more information Here !!
 *  \note First Implementation 9:05:16 AM Jul 13, 2015
 *  \warning Warning.
 *  \author Author
 ****************************************************************************************/
void UART_StreamTransmit( uint8_t *data, uint32_t len )
{
    uint32_t i = 0;

    for ( ; i < len; i++ )
    {
        UART_Transmit( data[i] );
    }
    return;
}

/**********************************************************************************//**
 *  \param[in] param Description of the  parameter of the function.
 *  \param[out] param Description of the  parameter of the function.
 *  \return 0
 *  \brief Module Description
 *  \see See more information Here !!
 *  \note First Implementation 9:05:16 AM Jul 13, 2015
 *  \warning Warning.
 *  \author Author
 ****************************************************************************************/
uint8_t UART_IsBusy( void )
{
    return uart_in_progress;
}

/**********************************************************************************//**
 *  \param[in] param Description of the  parameter of the function.
 *  \param[out] param Description of the  parameter of the function.
 *  \return 0
 *  \brief Module Description
 *  \see See more information Here !!
 *  \note First Implementation 9:05:16 AM Jul 13, 2015
 *  \warning Warning.
 *  \author Author
 ****************************************************************************************/
void UART_setBusy( uint8_t val )
{
    uart_in_progress = val;

    return;
}
/**********************************************************************************//**
 *  \param[in] param Description of the  parameter of the function.
 *  \param[out] param Description of the  parameter of the function.
 *  \return 0
 *  \brief Module Description
 *  \see See more information Here !!
 *  \note First Implementation 9:05:16 AM Jul 13, 2015
 *  \warning Warning.
 *  \author Author
 ****************************************************************************************/
ISR( USART0_RX_vect )
{
    uint8_t data = 0;
    uint8_t valid_buff = 0;

    data = UART_Receive( );
    if ( !UART_IsBusy( ) )
    {
        cbSetTXBuffer( &uart_tx_buffer, &uart_tx_len);
        cbSetRXBuffer( &uart_rx_buffer, &uart_rx_len);
        valid_buff = ( ( uart_rx_buffer ) && ( uart_rx_len ) );
        if ( valid_buff && cbStartOfFrame( data ) )
        {
            UART_setBusy( TRUE );
            uart_rx_idx = 0;
            uart_rx_buffer[ uart_rx_idx++ ] = data;
        }
    }
    else
    {
        valid_buff = ( ( uart_rx_buffer ) && ( uart_rx_len ) );
        if ( ( valid_buff ) && ( uart_rx_len > uart_rx_idx ) )
        {
            uart_rx_buffer[ uart_rx_idx++ ] = data;
        }
        else if ( ( valid_buff ) && ( uart_rx_len <= uart_rx_idx ) )
        {
            cbCommitData( uart_rx_buffer, uart_rx_idx );
            UART_setBusy( FALSE );
            cbSetupReply( uart_rx_buffer, uart_tx_buffer, &uart_tx_idx);
            UART_StreamTransmit( uart_tx_buffer, uart_tx_idx );
        }
        else
        {
            UART_setBusy( FALSE );
        }
    }
}

/**
 * @}
 */
