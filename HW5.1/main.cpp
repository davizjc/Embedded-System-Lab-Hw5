#include "mbed.h"
#include "bbcar.h"

#define unitsFC 360                          // Units in a full circle
#define dutyScale 1000                       // Scale duty cycle to 1/000ths
#define dcMin 29                             // Minimum duty cycle
#define dcMax 971                            // Maximum duty cycle
#define q2min unitsFC/4                      // For checking if in 1st uadrant
#define q3max q2min * 3                      // For checking if in 4th uadrant

DigitalOut led1(LED1);
Ticker servo_ticker;
Ticker servo_feedback_ticker;



PwmIn servo0_f(D9), servo1_f(D10);
PwmOut servo0_c(D11), servo1_c(D12);

DigitalInOut pin8(D8);

BBCar car(servo0_c, servo0_f, servo1_c, servo1_f, servo_ticker, servo_feedback_ticker);


  volatile int angle, targetAngle = 3;              // Global shared ariables
  volatile int Kp = 1;                          // Proportional constant
  volatile float tCycle;
  volatile int theta;
  volatile int thetaP;
  volatile int turns = 0;

   
void feedback360(){                           // Position monitoring
     tCycle = servo1_f.period();
     int dc = dutyScale * servo1_f.dutycycle();
     theta = (unitsFC - 1) -                   // Calculate angle
              ((dc - dcMin) * unitsFC)
              / (dcMax - dcMin + 1);
     if(theta < 0)                             // Keep theta valid
        theta = 0;
     else if(theta > (unitsFC - 1))
        theta = unitsFC - 1;

     // If transition from quadrant 4 to
     // quadrant 1, increase turns count.
     if((theta < q2min) && (thetaP > q3max))
        turns++;
     // If transition from quadrant 1 to
     // quadrant 4, decrease turns count.
     else if((thetaP < q2min) && (theta > q3max))
        turns --;

     // Construct the angle measurement from the turns count and
     // current theta value.
     if(turns >= 0)
        angle = (turns * unitsFC) + theta;
     else if(turns <  0)
        angle = ((turns + 1) * unitsFC) - (unitsFC - theta);

     thetaP = theta;                           // Theta previous for next rep
  }

int main() {
   // parallax_ping  ping1(pin8);
   int ans=0;
   int deg=0;
   parallax_laserping  ping1(pin8);
    //servo_feedback_ticker.attach(&feedback360, 1ms);
   //car.goStraight(50);
    car.turn(50, 0.1);  // turn left 
    while(1) {
    //printf("Ping = %f\r\n", (float)ping1);
      if((float)ping1>50) led1 = 1;
      else {
        printf("object on the left has a distance of = %f\n", (float)ping1);
        led1 = 0;
        car.stop();
        break;
      }
      ThisThread::sleep_for(10ms);
   }
    car.turn(-50, 0.1);  // turn right 
    servo_feedback_ticker.attach(&feedback360, 1ms);
    ThisThread::sleep_for(2s);

    while(1) {
       //printf("Ping = %f\r\n", (float)ping1);
      if((float)ping1>50) led1 = 1;
      else {
        printf("object on the right has a distance of = %f\n", (float)ping1);
        printf("%d \n" , car.servo1.angle);
        ans = ((sin(car.servo1.angle/2)*(float)ping1));
        printf("distance between two object is approximately %d \n", abs(ans*2));
        led1 = 0;
        car.stop();
        break;
      }
      ThisThread::sleep_for(10ms);
   }
}
