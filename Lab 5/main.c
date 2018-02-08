#include "lcd.h"
#include "timer.h"
#include "button.h"
#include "WiFi.h"

void uart_init(void){
    //enable clock to GPIO, R1 = port B
     SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
     //enable clock to UART1, R1 = UART1. ***Must be done before setting Rx and Tx (See DataSheet)
     SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R1;
     //enable alternate functions on port b pins 0 and 1
     GPIO_PORTB_AFSEL_R |= (BIT0 | BIT1);
     //enable Rx and Tx on port B on pins 0 and 1
     GPIO_PORTB_PCTL_R |= 0x00000011;
     //set pin 0 and 1 to digital
     GPIO_PORTB_DEN_R |= (BIT0 | BIT1);
     //set pin 0 to Rx or input
     GPIO_PORTB_DIR_R &= ~BIT0;
     //set pin 1 to Tx or output
     GPIO_PORTB_DIR_R |= BIT1;

     //calculate baudrate
     uint16_t iBRD = 8;//use equations
     uint16_t fBRD = 44;//use equations

     //turn off uart1 while we set it up
     UART1_CTL_R &= ~(UART_CTL_UARTEN);
     //set baud rate
     UART1_IBRD_R = iBRD;
     UART1_FBRD_R = fBRD;
     //set frame, 8 data bits, 1 stop bit, no parity, no FIFO
     UART1_LCRH_R = UART_LCRH_WLEN_8 ;
     //use system clock as source
     UART1_CC_R = UART_CC_CS_SYSCLK;
     //re-enable enable RX, TX, and uart1
     UART1_CTL_R = (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN);
}


void uart_sendChar(char data)
{
    //wait until there is room to send data
    while(UART1_FR_R & 0x20)
    {

    }

    //send data
    UART1_DR_R = data;
}


char uart_receive(void){
    char data = 0;

    //wait to receive
    while(UART1_FR_R & UART_FR_RXFE)
    {

    }

    //mask the 4 error bits and grab only 8 data bits
    data = (char)(UART1_DR_R & 0xFF);

    return data;
}

void uart_sendString(char data[]){
    int i = 0;
    while(data[i] != '\0'){
        uart_sendChar(data[i]);
        i++;
    }
}


int uart_Receive_non_Blocking(char* rdata){
    if((UART1_FR_R & UART_FR_RXFE) == 0){
        *rdata = (char)(UART1_DR_R & 0xFF);
        return 1;
    }
    return 0;
}


int main(void) {
    lcd_init();
	button_init();
	uart_init();
	//char pass[] = "password123";
	//uint8_t bt;
	//int init1 = 0;

	//uint8_t last_button = 0;
	//init1 = WiFi_start(pass);
	while(1){
	    mainloop();
//	    if(bt != last_button){
//
//	        switch(bt){
//	             case 6:
//	                  uart_sendString("Yes ");
//	                  last_button = bt;
//	                     break;
//	                    case 5:
//	                        uart_sendString("No ");
//	                        last_button = bt;
//	                        break;
//	                    case 4:
//	                        uart_sendString("Blue, no green, Ahhhhh!!! ");
//	                        last_button = bt;
//	                        break;
//	                    case 3:
//	                        uart_sendString("Not so creative ");
//	                        last_button = bt;
//	                        break;
//	                    case 2:
//	                        uart_sendString("Hahahahaa ");
//	                        last_button = bt;
//	                        break;
//	                    case 1:
//	                        uart_sendString("I am the last of the Daleks ");
//	                        last_button = bt;
//	                        break;
//	                    }
//	                }
	}
}

int mainloop(void){
    lcd_init();
   // uart_init();
    button_init();
    uint8_t bt;
    char pass[] = "password123";
    int init1 = 0;
    init1 = WiFi_start(pass);
    uint8_t last_button = 0;
    lcd_printf("waiting for input...");
    uart_sendString("waiting for input...");
    while(1){
        char rec = 0;
        char buffer[21];
        buffer[20] = 0;
        int i = 0;
        int received;
        char current;
        while(i < 20){

            bt = button_getButton();
            if(bt != last_button){
                switch(bt){
                case 6:
                    uart_sendString("Yes ");
                    last_button = bt;
                    break;
                case 5:
                    uart_sendString("No ");
                    last_button = bt;
                    break;
                case 4:
                    uart_sendString("Blue, no green, Ahhhhh!!! ");
                    last_button = bt;
                    break;
                case 3:
                    uart_sendString("Not so creative ");
                    last_button = bt;
                    break;
                case 2:
                    uart_sendString("Hahahahaa ");
                    last_button = bt;
                    break;
                case 1:
                    uart_sendString("I am the last of the Daleks ");
                    last_button = bt;
                    break;
                }
            }
            received = uart_Receive_non_Blocking(&current);
           // current = uart_receive();
            if(received){
                if(current == '\r'  ){
                    i--;
                    lcd_puts('\n');
                    buffer[i] = '\0';
                    break;
                }


                lcd_printf("%c: %d / 20", current, i);
                buffer[i] = current;
                uart_sendChar(current);
                i++;
            }


        }
        lcd_clear();
        lcd_puts(buffer);

    }
    return 0;
}
