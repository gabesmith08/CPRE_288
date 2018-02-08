/*
 * main.c
 */

#include "lcd.h"
#include "timer.h"
#include <stdbool.h>
#include "stdint.h"
#include <inc/tm4c123gh6pm.h>

#include "driverlib/interrupt.h"

volatile unsigned current_time = 0;
volatile unsigned last_time = 0;
volatile int update_flag =0;


void TIMER3B_Handler(void){
    TIMER3_ICR_R |= TIMER_ICR_CBECINT;

    if(update_flag == 0){
        last_time = current_time;
        current_time = TIMER3_TBR_R;
        update_flag = 3;
    }
    else{
        last_time = current_time;
        current_time = TIMER3_TBR_R;
        update_flag = 0;
    }
}

void send_pulse(){
    TIMER3_CTL_R &= ~(TIMER_CTL_TBEN);
    GPIO_PORTB_AFSEL_R &= ~BIT3;

    GPIO_PORTB_PCTL_R &= 0x0000;

    GPIO_PORTB_DIR_R |= 0x08;

    GPIO_PORTB_DATA_R |= 0x08;

    timer_waitMillis(5);

    GPIO_PORTB_DATA_R &= 0xF7;

    GPIO_PORTB_DIR_R &= 0xF7;
    GPIO_PORTB_AFSEL_R |= BIT3;

    GPIO_PORTB_PCTL_R |= 0x00007000;

    TIMER3_CTL_R |= TIMER_CTL_TBEN;
}


void clock_timer_init(void){
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;

    GPIO_PORTB_DEN_R |= 0x08;
    GPIO_PORTB_PCTL_R |= 0x07000;

    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3;

    TIMER3_CTL_R &= ~(TIMER_CTL_TBEN);

    TIMER3_CFG_R |= TIMER_CFG_16_BIT;

    TIMER3_TBMR_R |= TIMER_TBMR_TBCMR | TIMER_TBMR_TBMR_CAP | TIMER_TBMR_TBCDIR;
    TIMER3_TBILR_R |= 0xFFFF;

    TIMER3_ICR_R = (TIMER_ICR_CBECINT);
    TIMER3_IMR_R |= (TIMER_IMR_CBEIM);

    NVIC_EN1_R |= 0b10000;

    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    IntMasterEnable();

    TIMER3_CTL_R |= (TIMER_CTL_TBEN | TIMER_CTL_TBEVENT_BOTH);

}

int main(void) {

    float distance_m = 0;
    float distance = 0;
    float width = 0;
    float width_error = 0;

    clock_timer_init();
    lcd_init();

    while(1)
    {

        send_pulse();

        while(update_flag != 3);

        if(update_flag == 3)
        {

            if(current_time < last_time)
            {

                width = current_time + (1<<24) - last_time;
                distance_m = (current_time + (1<<24) - last_time)*(1/16000000.0);
                distance = distance_m / 2 * 340 * 100;
                lcd_printf("D %0.2f \n PW %0.0f \n Diff %d \n", distance, width);
            }
            else{
                width = current_time - last_time;
                distance_m = (current_time - last_time)*(1/16000000.0);
                distance = distance_m / 2 * 340 * 100;
                lcd_printf("D %0.2f \n PW %0.0f \n Diff %d \n", distance, width);
            }
            update_flag = 0;
        }
        timer_waitMillis(50);
    }
    return 0;
}
