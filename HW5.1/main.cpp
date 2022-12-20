#include "mbed.h"
#include "bbcar.h"

DigitalOut led1(LED1);
Ticker servo_ticker;
Ticker servo_feedback_ticker;

PwmIn servo0_f(D9), servo1_f(D10);
PwmOut servo0_c(D11), servo1_c(D12);

DigitalInOut pin8(D8);

BBCar car(servo0_c, servo0_f, servo1_c, servo1_f, servo_ticker, servo_feedback_ticker);

int main() {
   // parallax_ping  ping1(pin8);
   parallax_laserping  ping1(pin8);
   //car.goStraight(50);
    car.turn(50, 0.1);  // turn left 
    while(1) {
    printf("Ping = %f\r\n", (float)ping1);
      if((float)ping1>50) led1 = 1;
      else {
        printf(" object on the left has a distance of = %f\r\n", (float)ping1);
        led1 = 0;
        car.stop();
        break;
      }
      ThisThread::sleep_for(10ms);
   }
    printf("go right \n");
    car.turn(-50, 0.1);  // turn right 
    ThisThread::sleep_for(3s);

    while(1) {
       printf("Ping = %f\r\n", (float)ping1);
      if((float)ping1>50) led1 = 1;
      else {
        printf(" object on the right has a distance of = %f\r\n", (float)ping1);
        led1 = 0;
        car.stop();
        break;
      }
      ThisThread::sleep_for(10ms);
   }
}
