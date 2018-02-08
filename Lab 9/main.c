/**
 * @file main.c
 * @Brief Our main file carries the bulk of the cyBot's functionality. Many functions include initializing
 *          IR and Ping Sensor readings, servo movement, and WiFi + uart communication.
 * @author(s) Alex Chanchom
 *          Donald Laracuente
 *          Yi Liu
 *          Richard Smith
 * @date 12/7/2017
 */


#include "lcd.h"
#include "timer.h"
#include "uart.h"
#include "WiFi.h"
#include <stdbool.h>
#include "driverlib/interrupt.h"
#include <math.h>
#include <string.h>
#include <open_interface.h>
#include <movement.h>
#include <music.h>

#define M_PI        3.14159265358979323846

void sonar_timer_init();
void ir_timer_init();
void servo_timer_init();
void move_servo();
int ir_read();
unsigned sonar_read();
unsigned time2dist(unsigned time);
void TIMER3B_Handler(void);
unsigned ping_read();
void init_all();
double toRadians(double angle);
int radialToRealC(double radial_width, int distanceToObject);
void handleObjectFound();
void uart_sendNum(int num);

int linearWidth;
int degree;
int count = 0;
int degrees = 176;
int degree1 = 176;
int objCount = 0;

int smallestLocation = 10000;
int smallestWidth = 10000;
int count_step = 0;
int distance_mm = 5000;
volatile unsigned rising_time; /// start time of the return pulse
volatile unsigned falling_time; /// end time of the return pulse

char press = 0;


/**
 * This is the main, all of our hard work is here
 *   @author(s) Alex Chanchom
 *          Donald Laracuente
 *          Yi Liu
 *          Richard Smith
 *   @date 12/7/2017
 */
int main()
{

    init_all();

    uart_sendString("****************************************************\n\r");
    uart_sendString("Degrees\tIRDistance cm \t Sonar Distance cm\n\r");
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    while (1)
    {
        press = uart_receive();
        ///This key moves the bot forward by 10 cm
        if (press == 'w')
        {
            move_forward(sensor_data, 10);
            degrees = 176;
            move_servo(degrees);
        }

        ///The bot will move CCW at 10 degrees
        if (press == 'a')
        {
            turn_counterClock(sensor_data, 10);
            degrees = 176;
            move_servo(degrees);
        }

        ///The bot will move CCW at 90 degrees
        if (press == 'q')
        {
            turn_counterClock(sensor_data, 75);
            degrees = 176;
            move_servo(degrees);
        }

        ///Moves back by 10 cm
        if (press == 's')
        {
            move_Backward(sensor_data, 10);
            degrees = 176;
            move_servo(degrees);
        }

        ///Rotates CW by 10 degrees
        if (press == 'd')
        {
            turn_clockwise(sensor_data, 10);
            degrees = 176;
            move_servo(degrees);
        }

        ///Rotates CW by 90 degrees
        if (press == 'e')
        {
            turn_clockwise(sensor_data, 80);
            degrees = 176;
            move_servo(degrees);
        }

        ///Triggers the IR reader to scan while stationary
        if (press == 'l')
        {

           degrees = 176;
           move_servo(degrees);
           while (degrees > -10)
           {
               lcd_printf("Object Count: %d", objCount);
               move_servo(degrees);
               timer_waitMillis(100);
               degrees -= 2;

               int IRDist = ir_read();
               int SonarDist = sonar_read();
               SonarDist = SonarDist / 10;

               uart_sendNum(degrees);
               uart_sendString("\t");
               uart_sendNum(IRDist);
               uart_sendString("\t");
               uart_sendString("\t");
               uart_sendNum(SonarDist);
               uart_sendString("\n\r");
               timer_waitMillis(100);
               if (IRDist > 100) //Our IR threshold
               {
                   handleObjectFound();
                   uart_sendString("********Possible Object***********\n\r");
                   objCount++;
               }

           }

           uart_sendString("_______________________________________________________\n\r");
        }

        ///Plays a song!!
        if (press == 'p')
        {
            load_songs(0);
            oi_setLeds(1, 1, 250, 100);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 0, 255);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 100, 100);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 250, 100);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 0, 255);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 100, 100);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 250, 100);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 0, 255);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 100, 100);
            load_songs(1);
            oi_setLeds(1, 1, 250, 100);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 0, 255);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 100, 100);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 250, 100);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 0, 255);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 100, 100);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 250, 100);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 0, 255);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 100, 100);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 250, 100);
            timer_waitMillis(400);
            oi_setLeds(1, 1, 0, 255);
            load_songs(2);
            oi_setLeds(1, 1, 250, 100);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 0, 255);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 100, 100);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 250, 100);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 0, 255);
            timer_waitMillis(500);
            load_songs(0);
            oi_setLeds(1, 1, 250, 100);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 0, 255);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 100, 100);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 250, 100);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 0, 255);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 100, 100);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 250, 100);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 0, 255);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 100, 100);
            load_songs(1);
            oi_setLeds(1, 1, 250, 100);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 0, 255);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 100, 100);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 250, 100);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 0, 255);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 100, 100);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 250, 100);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 0, 255);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 100, 100);
            timer_waitMillis(500);
            oi_setLeds(1, 1, 250, 100);
            timer_waitMillis(400);
            oi_setLeds(1, 1, 0, 255);
            load_songs(2);

        }

        ///Moves forward by 30 cm
        if (press == '3')
        {
            move_forward(sensor_data, 30);
            degrees = 176;
            move_servo(degrees);
        }

        timer_waitMillis(200);

    }
    oi_setWheels(0, 0);
    oi_free(sensor_data);
}

/**
 * This function simply initializes all necessary embedded systems
 *   @author(s) Alex Chanchom
 *          Donald Laracuente
 *          Yi Liu
 *          Richard Smith
 *   @date 12/7/2017
 */

void init_all()
{
    lcd_init();
    sonar_timer_init();
    ir_timer_init();
    servo_timer_init();
    uart_init();

    ///Above devices are initialized,
    char pass[] = "password123";
    int init1 = 0;
    init1 = WiFi_start(pass);
}

/**
 * This function will gather readings and check if objects are detected
 *   @author(s) Alex Chanchom
 *          Donald Laracuente
 *          Yi Liu
 *          Richard Smith
 *   @date 12/7/2017
 */
void handleObjectFound()
{
    unsigned int start = degrees;
    int IRDist = ir_read();
    int distance = sonar_read() / 10;

    ///Our readings scanned upwards to 100 cm
    while (IRDist > 100)
    {
        degrees -= 2;
        move_servo(degrees);
        timer_waitMillis(250);
        IRDist = ir_read();
    }
    unsigned int end = degrees;

    ///Our code for converting the degrees that an object is located at into a calculated linear width
    int radial_width = start - end;
    int real_width = radialToRealC(radial_width, distance);
    linearWidth = sqrt(
            pow(start, 2) + pow(end, 2)
                    - 2 * start * end * cos(toRadians(radial_width)));

    if (smallestWidth > real_width && real_width > 0)
    {
        smallestWidth = real_width;
        smallestLocation = start;
        uart_sendString("WIDTH: ");
        uart_sendNum(real_width);
        uart_sendString("\n\r");
    }
}

/**
 * This function inserts the given int parameter into the array
 *   @author(s) Alex Chanchom
 *          Donald Laracuente
 *          Yi Liu
 *          Richard Smith
 *   @param num   The number to append onto the array
 *   @date 12/7/2017
 */
void uart_sendNum(int num)
{
    char str[15];
    sprintf(str, "%d", num);
    uart_sendString(str);
}

/**
 * This function will gather readings and check if objects are detected
 *   @author(s) Alex Chanchom
 *          Donald Laracuente
 *          Yi Liu
 *          Richard Smith
 *   @param radial_width    This is the value used to calculate radians
 *   @param distanceToObject   Measured distance from detected object
 *   @date 12/7/2017
 */
int radialToRealC(double radial_width, int distanceToObject)
{
    double radians = toRadians(radial_width);
    return (int) (2 * distanceToObject * sin(radians));
}


/**
 * This function converts degrees to radians
 *   @author(s) Alex Chanchom
 *          Donald Laracuente
 *          Yi Liu
 *          Richard Smith
 *   @param double angle    The angle to be converted
 *   @date 12/7/2017
 */
double toRadians(double angle)
{
    return angle * ((double) M_PI / 180);
}


/**
 * This function initializes the timers for ADC
 *   @author(s) Alex Chanchom
 *          Donald Laracuente
 *          Yi Liu
 *          Richard Smith
 *   @date 12/7/2017
 */
void ir_timer_init()
{
    //enable ADC 0 module on port D
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    //enable clock for ADC
    SYSCTL_RCGCADC_R |= 0x1;
    //enable port D pin 0 to work as alternate functions
    GPIO_PORTB_AFSEL_R |= 0x01;
    //set pin to input - 0
    GPIO_PORTB_DEN_R &= 0b11111110;
    //disable analog isolation for the pin
    GPIO_PORTB_AMSEL_R |= 0x01;
    //initialize the port trigger source as processor (default)
    GPIO_PORTB_ADCCTL_R = 0x00;

    //disable SS0 sample sequencer to configure it
    ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN0;
    //initialize the ADC trigger source as processor (default)
    ADC0_EMUX_R = ADC_EMUX_EM0_PROCESSOR;
    //set 1st sample to use the AIN10 ADC pin
    ADC0_SSMUX0_R |= 0x000A;
    //enable raw interrupt
    ADC0_SSCTL0_R |= (ADC_SSCTL0_IE0 | ADC_SSCTL0_END0);
    //enable oversampling to average
    ADC0_SAC_R |= ADC_SAC_AVG_64X;
    //re-enable ADC0 SS0
    ADC0_ACTSS_R |= ADC_ACTSS_ASEN0;
}


/**
 * This function initializes the timers for ADC
 *   @author(s) Alex Chanchom
 *          Donald Laracuente
 *          Yi Liu
 *          Richard Smith
 *   @date 12/7/2017
 */
void servo_timer_init()
{
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

/**
 * This function moves the servo at a degree
 *   @author(s) Alex Chanchom
 *          Donald Laracuente
 *          Yi Liu
 *          Richard Smith
 *   @param degree    The degree in which the servo will point at
 *   @date 12/7/2017
 */
void move_servo(degree)
{
    unsigned int out = degree * (120) + 10000;
    TIMER1_TBMATCHR_R = (320000 - out) & 0xFFFF;
    TIMER1_TBPMR_R = (320000 - out) >> 16;
}

/**
 * This function works as a flag to indicate that the digital signal is ready
 *   @author(s) Alex Chanchom
 *          Donald Laracuente
 *          Yi Liu
 *          Richard Smith
 *   @date 12/7/2017
 */
int ir_read()
{
    ///initiate SS1 conversion
    ADC0_PSSI_R = ADC_PSSI_SS0;
    ///wait for ADC conversion to be complete
    while ((ADC0_RIS_R & ADC_RIS_INR0) == 0)
    {
        ///wait
    }
    ///grab result
    return ADC0_SSFIFO0_R;
}

/**
 * This function gathers distance readings from the sonar
 *   @author(s) Alex Chanchom
 *          Donald Laracuente
 *          Yi Liu
 *          Richard Smith
 *   @date 12/7/2017
 */
unsigned sonar_read()
{
    send_pulse();
    while (count != 2)
        ;
    count = 0;
    unsigned distance = ping_read();
    return distance;
}

/**
 * This function calculates the distance based on the sonar signal
 *   @author(s) Alex Chanchom
 *          Donald Laracuente
 *          Yi Liu
 *          Richard Smith
 *   @date 12/7/2017
 */
unsigned ping_read()
{
    unsigned elapsed_time = falling_time - rising_time;
    int distance = time2dist(elapsed_time);
    return distance;
}

/**
 * This function converts the time that the ping travels to the distance
 *   @author(s) Alex Chanchom
 *          Donald Laracuente
 *          Yi Liu
 *          Richard Smith
 *   @param time
 *   @date 12/7/2017
 */
unsigned time2dist(unsigned time)
{
    double interval = time * 17;
    int out = interval / 1600;
    return out;
}

/**
 * This function will send the pulse to the cyBot's sonar sensor
 *   @author(s) Alex Chanchom
 *          Donald Laracuente
 *          Yi Liu
 *          Richard Smith
 *   @date 12/7/2017
 */
void send_pulse()
{
    ///disable alternate function of pin afsel & pctl
    GPIO_PORTB_AFSEL_R &= 0xF7;
    GPIO_PORTB_PCTL_R |= 0x7000;

    GPIO_PORTB_DIR_R |= 0x08; /// set PB3 as output

    GPIO_PORTB_DATA_R |= 0x08; /// set PB3 to high

    /// wait at least 5 microseconds based on data sheet
    timer_waitMicros(5);

    GPIO_PORTB_DATA_R &= 0xF7; /// set PB3 to low

    GPIO_PORTB_DIR_R &= 0xF7; /// set PB3 as input

    ///enable alternate function
    GPIO_PORTB_AFSEL_R |= 0x08;
    GPIO_PORTB_PCTL_R |= 0x7000;

    TIMER3_CTL_R |= TIMER_CTL_TBEN; ///Enable TIMER3B
}

/**
 * This function initializes and enables the timers for the sonar sensor
 *   @author(s) Alex Chanchom
 *          Donald Laracuente
 *          Yi Liu
 *          Richard Smith
 *   @date 12/7/2017
 */
void sonar_timer_init()
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1; /// Turn on clock to GPIOportB
    GPIO_PORTB_DEN_R |= 0x08;

    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3; /// Turn on clock to TIMER3

    ///Configure the timer for input capture mode
    TIMER3_CTL_R &= ~(TIMER_CTL_TBEN); ///disable timerB to allow us to change settings
    TIMER3_CFG_R |= TIMER_CFG_16_BIT; ///set to 16 bit timer

    TIMER3_CTL_R |= TIMER_CTL_TBEVENT_BOTH;
    TIMER3_TBMR_R = 0x03 | 0x04 | TIMER_TBMR_TBCDIR; ///set for capture mode & edge time mode pp735

    ///clear TIMER3B interrupt flags
    TIMER3_ICR_R = (TIMER_ICR_TBTOCINT); ///clears TIMER4 time-out interrupt flags
    TIMER3_IMR_R |= (TIMER_IMR_CBEIM); ///enable TIMER4(A&B) time-out interrupts

    ///initialize local interrupts
    NVIC_EN1_R |= 0x10;

    IntRegister(INT_TIMER3B, TIMER3B_Handler); ///register TIMER4B interrupt handler
    IntMasterEnable(); ///initialize global interrupts
}


/**
 * This function tracks the time that the sonar will be sending pulses along with the time that it is received
 *   @author(s) Alex Chanchom
 *          Donald Laracuente
 *          Yi Liu
 *          Richard Smith
 *   @date 12/7/2017
 */
void TIMER3B_Handler(void)
{
    int event_time = TIMER3_TBR_R;

    if (count == 0)
    {
        count = 1;
        rising_time = event_time;
    }
    else if (count == 1)
    {
        count = 2;
        falling_time = event_time;
    }

    TIMER3_ICR_R |= TIMER_ICR_CBECINT;
}
