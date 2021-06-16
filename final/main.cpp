#include "mbed.h"
#include "bbcar.h"

BufferedSerial pc(USBTX, USBRX);
Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BufferedSerial uart(D1, D0);
BufferedSerial xbee(A1,A0); //tx,rx
BBCar car(pin5, pin6, servo_ticker);

int main() {

    double pwm_table0[] = {-150, -120, -90, -60, -30, 0, 30, 60, 90, 120, 150};
    double speed_table0[] = {-8.445, -7.812, -6.647, -6.408, -3.900, 0.000, 3.900, 7.843, 8.880, 8.901, 9.199};
    double pwm_table1[] = {-100, -80, -60, -40, -20, 0, 20, 40, 60, 80, 100};
    double speed_table1[] = {-10.445, -9.812, -9.647, -9.408, -5.900, 0.000, 5.900, 10.843, 11.880, 11.401, 12.199};
    car.setCalibTable(11, pwm_table0, speed_table0, 11, pwm_table1, speed_table1);  // use above table to make car go straight

    char buf[256], outbuf[256];
    FILE *devin = fdopen(&xbee, "r");
    FILE *devout = fdopen(&xbee, "w");

    int a=1;

    while (a==0) {
        int i=0;
        char recv = fgetc(devin);
        if(recv == '1') {       // if xbee is availible, text 1 to start the following functions
            a=1;
        }
        buf[i] = fputc(recv, devout);
    }
    uart.set_baud(9600);
    while(a==1){
        if(uart.readable()){    // read data from openMV
            car.stop();
            char rec[1];
            uart.read(rec, sizeof(rec));
            pc.write(rec, sizeof(rec));
            if(rec[0]=='0'){
                car.goStraightCalib(-10);   // go straight if detecting line
                //ThisThread::sleep_for(100ms);
            }           
            else if(rec[0]=='1'){                
                car.turn(5, 0.3); // if apriltag is in left, turn the car to make it toward apriltag
            }
            else if(rec[0]=='2'){                
                car.turn(5, -0.3); // if apriltag is in right, turn the car to make it toward apriltag
            }
            else if(rec[0]=='5'){  // once it is toward apriltag, stop the car and end function
                car.stop();            
                a=0;
            }
            else car.stop();       // if not detecting anything, car holds still
        }
        ThisThread::sleep_for(100ms);
    }
}

