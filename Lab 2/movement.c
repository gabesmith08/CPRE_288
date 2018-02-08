/*
 * movement.c
 *
 *  Created on: Sep 7, 2017
 *      Author: Donaldl
 */
#include "lcd.h"
#include "open_interface.h"
#include "timer.h"

void move_forward(oi_t *self, int centimeters){
    int distance_mm = centimeters*10;
    oi_setWheels(250,250);
    int sum = 0;
    while(sum < distance_mm){
        oi_update(self);
        sum +=self->distance;
    }
    oi_setWheels(0,0);
    return sum;
}

void move_Backward(oi_t *self, int centimeters){
    int distance_mm = centimeters*10;
    oi_setWheels(-250, -250);
    int sum = distance_mm;
    while(sum > 0){
        oi_update(self);
        sum += self->distance;
    }
    oi_setWheels(0,0);
    return;
}


void turn_clockwise(oi_t *sensor, int degrees){
    degrees = degrees * -1;
    oi_setWheels(-250,250);
    int sum = 0;
    while(sum > degrees){
        oi_update(sensor);
        sum += sensor -> angle;
    }
    oi_setWheels(0,0);
    return;
}

void turn_counterClock(oi_t *sensor, int degrees){
    oi_setWheels(250,-250);
    int sum = 0;
    while(sum < degrees){
             oi_update(sensor);
             sum += sensor -> angle;
    }
    oi_setWheels(0,0);
    return;
}
