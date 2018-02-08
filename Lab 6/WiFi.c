/*
 * WiFi.c
 *
 *  Created on: Sep 25, 2016
 *      Author: ericm
 *	Updates: dmlarson
 *
 * To use WiFi.c
 * -change the uart_sendchar to your code that sends a single char
 * -change the uart_sendBuffer to your code that sends an entire string
 * -change the uart_init to your code that initializes the uart module
 * -change the include statement in the WiFi.h file to your own uart header file
 * -in your main function call WiFi_start with a null terminated string for your password
 * -read the WiFi bridge documentation to understand how this is working
 *
 */

#include "WiFi.h"
#include "lcd.h"
#include "timer.h"
#include <string.h>

#define COMMAND_PIN	0x04

#define COMMAND_START	0
#define COMMAND_STOP	1

#define RETVAL_SUCCESS	0


/**
 * starts WiFi bridge
 * @param password a null terminated string to be used as wireless password
 * @return 0 on success, -1 failure
 */
int WiFi_start(char *password) {
	//Initialize UART
	//#error DELETE ME - change uart_init() to your uart initialize function
	uart_init();

	//Initialize command pin for WiFi connection
	//SYSCTL_RCGCGPIO_R |= BIT1; //Turn on GPIO Port B
	GPIO_PORTB_DEN_R |= COMMAND_PIN; //Enable PB2
	GPIO_PORTB_DIR_R |= COMMAND_PIN; //Set PB2 to output
	GPIO_PORTB_DATA_R &= ~COMMAND_PIN; //Clear command pin

	//Wait for pin to settle
	timer_waitMillis(1);

	//send the start command with password to the wifi
	uint8_t response = _sendCommand(COMMAND_START, password, strlen(password) + 1);

	if(response != RETVAL_SUCCESS) {
		return -1;
	}
	else {
		return 0;
	}
}

int WiFi_stop() {
	return _sendCommand(COMMAND_STOP, NULL, 0);
}

uint8_t _sendCommand(uint8_t command, uint8_t* param, uint8_t len) {
	//Raise command Pin High
	GPIO_PORTB_DATA_R |= COMMAND_PIN;

	//Send WiFi command - 0x00
	//#error DELETE ME - change uart_sendChar(command) to your send character function
	uart_sendChar(command);

	//Send parameter (if applicable)
	if(len > 0) {
		//send password
		while(*param != 0){
			//#error DELETE ME - change uart_sendChar(*param) to your send character function
		    uart_sendChar(*param);
			param++;
		}
		//send null
		uart_sendChar(0);
	}

	//Wait for response from WiFi microcontroller
	//#error DELETE ME - change uart_receive to your receive character function
	char response = uart_receive();

	//Lower the command Pin
	GPIO_PORTB_DATA_R &= ~COMMAND_PIN;

	return response;
}
