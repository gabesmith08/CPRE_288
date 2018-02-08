#include "lcd.h"
#include "open_interface.h"
#include "timer.h"
#include "movement.h"

/// Simple 'Hello, world' program
/**
 * This program prints "Hello, world" to the LCD screen
 * @author Chad Nelson
 * @date 06/26/2012
 */


int main (void) {
   oi_t *sensor_data = oi_alloc();
   oi_init(sensor_data);

   int sum = 0;
   int distance_mm = 100;
//   oi_setWheels(250, 250);
   while (sum < distance_mm) {
       oi_update(sensor_data);
       sum += sensor_data -> distance;
       if(sensor_data -> bumpLeft){
           //move_Backward(sensor_data, 15);
          // sum -= 150;
           turn_clockwise(sensor_data, 91);
          // move_forward(sensor_data, 25);
          // turn_counterClock(sensor_data,91);
          // oi_setWheels(250,250);
       } else if(sensor_data -> bumpRight){
          // move_Backward(sensor_data, 15);
          // sum -= 150;
           turn_counterClock(sensor_data, 91);
          // move_forward(sensor_data, 25);
          // turn_clockwise(sensor_data,91);
          // oi_setWheels(250,250);
       }
   }

   oi_setWheels(0,0);
   oi_free(sensor_data);
   return;
}
