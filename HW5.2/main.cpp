#include "mbed.h"
#include "bbcar.h"

Ticker servo_ticker;
Ticker servo_feedback_ticker;

PwmIn servo0_f(D9), servo1_f(D10);
PwmOut servo0_c(D11), servo1_c(D12);
BBCar car(servo0_c, servo0_f, servo1_c, servo1_f, servo_ticker, servo_feedback_ticker);

BusInOut qti_pin(D4,D5,D6,D7);

int main() {
    
   parallax_qti qti1(qti_pin);
   int pattern;
   int checkpoint = 0;

   car.goStraight(-50);
   while(1) {
      car.checkDistance(1);

      pattern = (int)qti1;
      printf("%d\n",pattern);
      
      if (pattern == 0b1000){   // 8  right  
            car.turn(-50, 0.1);
      }
      if (pattern == 0b1100){   // 12
            car.turn(-50, 0.5);
      }
      if (pattern == 0b0100){   // 4 right alittle
            car.turn(-50, 0.7); 
      }
      if (pattern == 0b0110){   // 6 forward
            car.goStraight(-50);
      }
      if (pattern == 0b0010){   //2 left  alittle
            car.turn(-50, -0.7);
      }
       if (pattern == 0b0011){   // 3
            car.turn(-50, -0.5);
      }
      if (pattern == 0b0001){   ///1  left alot
            car.turn(-50,-0.1);
      }
      if (pattern == 0b0000){   ///0  go backward
            car.goStraight(50);
      }
      if (pattern == 0b1111){
         car.stop(); 
         ThisThread::sleep_for(4s);

         printf("car have stop \n");
         checkpoint += 1;
         printf("check point = %d\n",checkpoint);

         car.goStraight(-50);
         printf("continue moving\n");

         if (checkpoint == 4){
            car.stop(); 
            printf("distance = %f\n", abs(((car.servo0.angle)*6.5*3.14/360)));
            break;
         }
      }
     ThisThread::sleep_for(10ms);
   }
}
