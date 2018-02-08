/**
 * @file movement.c
 * @Brief This file implements the cyBot's maneuverability and object avoidance requirements
 * @author(s) Alex Chanchom
 *          Donald Laracuente
 *          Yi Liu
 *          Richard Smith
 * @date 12/7/2017
 */
#include "lcd.h"
#include "open_interface.h"
#include "timer.h"
#include <stdlib.h>

/**
 * Moves the bot backwards by a given distance
 *   @author(s) Alex Chanchom
 *          Donald Laracuente
 *          Yi Liu
 *          Richard Smith
 *   @param self   The allocated memory of the bot for the open interface
 *   @param centimeters   The distance given in which the bot will travel
 *   @date 12/7/2017
 */
void move_Backward(oi_t *self, int centimeters)
{
    int distance_mm = centimeters * 10;
    oi_setWheels(-150, -150);
    int sum = distance_mm;
    while (sum > 0)
    {



        oi_update(self);
        sum += self->distance;
    }
    oi_setWheels(0, 0);
    return;
}

/**
 * Rotates the bot CW direction
 *   @author(s) Alex Chanchom
 *          Donald Laracuente
 *          Yi Liu
 *          Richard Smith
 *   @param sensor   The allocated memory of the bot for the open interface
 *   @param degrees    The given angle that the bot will rotate to
 *   @date 12/7/2017
 */
void turn_clockwise(oi_t *sensor, int degrees)

{
    degrees = degrees * -1;
    oi_setWheels(-150, 150);
    int sum = 0;
    while (sum > degrees)
    {
        oi_update(sensor);
        sum += sensor->angle;
    }
    oi_setWheels(0, 0);
    return;
}

/**
 * Rotates the bot CCW direction
 *   @author(s) Alex Chanchom
 *          Donald Laracuente
 *          Yi Liu
 *          Richard Smith
 *   @param sensor   The allocated memory of the bot for the open interface
 *   @param degrees    The given angle that the bot will rotate to
 *   @date 12/7/2017
 */
void turn_counterClock(oi_t *sensor, int degrees)
{
    oi_setWheels(150, -150);
    int sum = 0;
    while (sum < degrees)
    {
        oi_update(sensor);
        sum += sensor->angle;
    }
    oi_setWheels(0, 0);
    return;
}

/**
 * Moves the bot forward. This function also includes object avoidance and
 *   @author(s) Alex Chanchom
 *          Donald Laracuente
 *          Yi Liu
 *          Richard Smith
 *   @param sensor   The allocated memory of the bot for the open interface
 *   @param degrees    The given angle that the bot will rotate to
 *   @date 12/7/2017
 */
void move_forward(oi_t *self, int centimeters)
{
    int distance_mm = centimeters * 10;
    // int r = rand() % 90;
    oi_setWheels(150, 150);
    int sum = 0;
    while (sum < distance_mm)
    {
        if (self->cliffLeftSignal > 2700)
        {
            uart_sendString("Tape on left side \n\r");
            move_Backward(self, 10);
            //turn_clockwise(self, 91);
            //oi_setWheels(150, 150);
            break;
        }

        else if (self->cliffFrontLeftSignal > 2700)
        {
            //move_Backward(self, 15);
//            turn_clockwise(self, 180);
//            oi_setWheels(150, 150);
           uart_sendString("Tape on front left\n\r");
            move_Backward(self, 10);
            break;
        }

        else if (self->cliffFrontRightSignal > 2700)
        {
//            move_Backward(self, 10);
//            turn_clockwise(self, 180);
//            oi_setWheels(150, 150);
           uart_sendString("Tape on front right \n\r");
            move_Backward(self, 10);
            break;
        }

        else if (self->cliffRightSignal > 2700)
        {
//            turn_counterClock(self, 91);
//            oi_setWheels(150, 150);
           uart_sendString("Tape on right side \n\r");
            move_Backward(self, 10);
            break;
        }

        if (self->bumpLeft)
        {
//            move_Backward(self, 10);
//            turn_clockwise(self, 91);
//            oi_setWheels(150, 150);
            uart_sendString("Bump left side \n\r");
            move_Backward(self, 10);
            break;
        }

        if (self->bumpRight)
        {
//            move_Backward(self, 10);
//            turn_counterClock(self, 91);
//            oi_setWheels(150, 150);
            uart_sendString("Bump right side \n\r");
            move_Backward(self, 10);
            break;
        }
        if (self->cliffLeft)
        {
//            turn_clockwise(self, 91);
//            oi_setWheels(150, 150);
            uart_sendString("cliff on left \n\r");
            move_Backward(self, 10);
            break;
        }
        if (self->cliffFrontLeft)
        {
//            move_Backward(self, 10);
//            turn_clockwise(self, 180);
//            oi_setWheels(150, 150);
            uart_sendString("cliff on front left\n\r");
            move_Backward(self, 10);
            break;
        }
        if (self->cliffFrontRight)
        {
//            move_Backward(self, 10);
//            turn_counterClock(self, 180);
//            oi_setWheels(150, 150);
            uart_sendString("cliff on front right\n\r");
            move_Backward(self, 10);
            break;
        }
        if (self->cliffRight)
        {
//            turn_counterClock(self, 91);
//            oi_setWheels(150, 150);
            uart_sendString("cliff on right\n\r");
            move_Backward(self, 10);
            break;
        }
        oi_update(self);
        sum += self->distance;
    }
    oi_setWheels(0, 0);
    return;
}

//void move_Backward(oi_t *self, int centimeters){
//    int distance_mm = centimeters*10;
//    oi_setWheels(-250, -250);
//    int sum = distance_mm;
//    while(sum > 0){
//        oi_update(self);
//        sum += self->distance;
//    }
//    oi_setWheels(0,0);
//    return;
//}
//
//
//void turn_clockwise(oi_t *sensor, int degrees){
//    degrees = degrees * -1;
//    oi_setWheels(-250,250);
//    int sum = 0;
//    while(sum > degrees){
//        oi_update(sensor);
//        sum += sensor -> angle;
//    }
//    oi_setWheels(0,0);
//    return;
//}
//
//void turn_counterClock(oi_t *sensor, int degrees){
//    oi_setWheels(250,-250);
//    int sum = 0;
//    while(sum < degrees){
//             oi_update(sensor);
//             sum += sensor -> angle;
//    }
//    oi_setWheels(0,0);
//    return;
//}
