/*
 * uart.c
 * implementation of basic USART functions.
 * Created: 7/1/2013 3:31:57 PM
 *  Author: newagain
 */ 

#include <avr/io.h>
#include "uart.h"
/**
*	used to initialize USART 
*	@param baudrate the baudrate of the USART
*/
void USART_Init(void)
{
	//int baud = 51;//uncomment for RS232 cable
	int baud = 34; //for bluetooth
	UBRR0H = (unsigned char)(baud>>8);
	UBRR0L = (unsigned char)baud;
	
	UCSR0A = 0b00000010;
	UCSR0B = 0b00011000;
	UCSR0C = 0b00000110;
}

/**
*	Used to transmit a piece of data
*	@param data the data sent by the USART
**/

char USART_Recieve(void)
{
	while (1)
	{
		if((UCSR0A & 0b10000000))
		{
			return UDR0;
			
		}
		if((UCSR0A & 0b00100000))
		{
			return 0;
		}
	}
}

/**
*	Used to receive a piece of data
*	@return the data from serial keyboard.
**/

void USART_Transmit(char data)
{
	
	/* Wait for empty transmit buffer by checking the UDRE bit */
	while((UCSR0A & 0b00100000) == 0)
	;
	/* Put data into transmit buffer; sends the data */
	UDR0 = data;
}
