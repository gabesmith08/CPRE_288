/*
 * main.c
 */
#include "timer.h"
#include "lcd.h"
#include "button.h"
#include <stdbool.h>
#include "driverlib/interrupt.h"
void timer1_init();
void move_servo();
void button_timer_init();

#define TIMER4A_PRESCALER  250
unsigned int degree;
int direction;
int lastPressed;

int main(void) {
    lcd_init();
    button_init();
    timer1_init();
    direction = 1;
    degree = 72;
    move_servo();
    timer_waitMillis(500);
    degree = 170;
    move_servo();
    timer_waitMillis(500);
    degree =-25;
    move_servo();
    timer_waitMillis(500);
    button_timer_init();
    while(1){}
}

void TIMER4A_Handler(void){
    TIMER4_ICR_R = TIMER_ICR_TBTOCINT; //clear flag
        int pressed = button_getButton();

        if (pressed == 1) {
            degree += (1 * direction);
            move_servo();
        }
        else if (pressed == 2) {
            degree += (2.5 * direction);
            move_servo();
        }
        else if (pressed == 3) {
            degree += (5 * direction);
            move_servo();
        }
        else if (pressed == 4 && lastPressed != 4) {
            direction *= -1;
        }
        timer_waitMillis(250);
        lastPressed = pressed;




}

void button_timer_init(){
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R4; // Turn on clock to TIMER4

    //Configure the timer for input capture mode
    TIMER4_CTL_R &= ~(TIMER_CTL_TAEN ); //disable timerB to allow us to change settings
    TIMER4_CFG_R |= TIMER_CFG_16_BIT; //set to 16 bit timer

    TIMER4_TAMR_R = TIMER_TAMR_TAMR_PERIOD; //set for periodic mode, down count

    TIMER4_TAPR_R = TIMER4A_PRESCALER - 1; // 1 sec clock


    TIMER4_TAILR_R = (int)(16000000/TIMER4A_PRESCALER); // set the load value for the 1 second clock (with your prescaler, how many clock cycles happen in 1 second?)


    //clear TIMER3B interrupt flags
    TIMER4_ICR_R = (TIMER_ICR_TATOCINT ); //clears TIMER4 time-out interrupt flags
    TIMER4_IMR_R |= (TIMER_IMR_TATOIM ); //enable TIMER4(A&B) time-out interrupts

    //initialize local interrupts
    NVIC_EN2_R = 0x00000C0; //#warning "enable interrupts for TIMER4A and TIMER4B" n = 0, 1, 2, 3, or 4

    IntRegister(INT_TIMER4A, TIMER4A_Handler); //register TIMER4A interrupt handler

    IntMasterEnable(); //intialize global interrupts

    TIMER4_CTL_R |= (TIMER_CTL_TAEN ); //Enable TIMER4A & TIMER4B

}
void timer1_init() {
    SYSCTL_RCGCGPIO_R |= BIT1; // Turn on clock to GPIOportB5
    GPIO_PORTB_DIR_R |= BIT5; //PB5 to output

    GPIO_PORTB_AFSEL_R |= BIT5;
    GPIO_PORTB_PCTL_R = 0x700000; //T1CCP1

    GPIO_PORTB_DEN_R |= 0b00100000;

    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1; //turn on clk for timer1

    TIMER1_CTL_R &= 0xFEFF; //disable timer to config

    TIMER1_CFG_R = TIMER_CFG_16_BIT; //set size of timer to 16
    TIMER1_TBMR_R |= 0b00001010; //periodic and PWM enable

    int pulse_period = 320000;

    TIMER1_TBILR_R = pulse_period & 0xFFFF; //lower 16 bits of the interval
    TIMER1_TBPR_R = pulse_period >> 16; //set the upper 8 bits of the interval

    TIMER1_CTL_R |= 0x0100; //enable timer
}

void move_servo()
{
    unsigned int out = degree*(133) + 10000;
    if(direction == 1){
        lcd_printf("%d inc,\n%d degrees",out, degree);

    }
    else{
        lcd_printf("%d dec",out);
    }
    TIMER1_TBMATCHR_R = (320000 - out) & 0xFFFF;
    TIMER1_TBPMR_R = (320000 - out) >> 16;

}
