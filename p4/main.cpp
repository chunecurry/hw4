#include"mbed.h"
#include "bbcar.h"
Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BBCar car(pin5, pin6, servo_ticker);
BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx



int main(){
    double pwm_table0[] = {-150, -120, -90, -60, -30, 0, 30, 60, 90, 120, 150};
    double speed_table0[] = {-8.445, -7.812, -6.647, -6.408, -3.900, 0.000, 3.900, 7.843, 8.880, 8.901, 9.199};
    double pwm_table1[] = {-100, -80, -60, -40, -20, 0, 20, 40, 60, 80, 100};
    double speed_table1[] = {-10.445, -9.812, -9.647, -9.408, -5.900, 0.000, 5.900, 10.843, 11.880, 11.401, 12.199};

   // first and fourth argument : length of table
    car.setCalibTable(11, pwm_table0, speed_table0, 11, pwm_table1, speed_table1);

    uart.set_baud(9600);
    while(1){
      if(uart.readable()){

            char recv[1];
            uart.read(recv, sizeof(recv));
            pc.write(recv, sizeof(recv));
            if(recv[0]=='1'){
                //car.turn(20, 0.3);
                car.goStraight(10);
                //ThisThread::sleep_for(1ms);
            }
            else if(recv[0]=='0'){
                car.goStraightCalib(-10);
                //ThisThread::sleep_for(1ms);
            }
            else if(recv[0]=='5'){
                car.stop();
                break;
                //ThisThread::sleep_for(1ms);
            }
            else car.stop();
      }
      //ThisThread::sleep_for(100ms);
   }
}