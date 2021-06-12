#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BufferedSerial xbee(D1, D0);

BufferedSerial pc(USBTX, USBRX);

BBCar car(pin5, pin6, servo_ticker);

void RP(Arguments *in, Reply *out);
RPCFunction reverp(&RP, "RP");

Thread t;
EventQueue queue(32 * EVENTS_EVENT_SIZE);

double d1, d2;
char P;


int main() {
   char buf[256], outbuf[256];
   FILE *devin = fdopen(&xbee, "r");
   FILE *devout = fdopen(&xbee, "w");
   while (1) {
      memset(buf, 0, 256);
      for( int i = 0; ; i++ ) {
         char recv = fgetc(devin);
         if(recv == '\n') {
            printf("\r\n");
            break;
         }
         buf[i] = fputc(recv, devout);
      }
   RPC::call(buf, outbuf);
   }
}

void RP (Arguments *in, Reply *out)   {
    bool success = true;

    double velocity;
    double time1;
    double time2;
    // In this scenario, when using RPC delimit the two arguments with a space.
    d1 = in->getArg<double>();
    d2 = in->getArg<double>();
    P = in->getArg<char>();
    // Have code here to call another RPC function to wake up specific led or close it.
   velocity = 15.4;
   time1 = 1000*(d1+12.5)/velocity;
   time2 = 1000*(d2+5)/velocity;
   car.goStraight(-100);
   ThisThread::sleep_for(time1);
   car.stop();
   ThisThread::sleep_for(500ms);
   car.turn(100, 0.3);
   ThisThread::sleep_for(600ms);
   car.stop();
   ThisThread::sleep_for(500ms);
   car.goStraight(-100);
   ThisThread::sleep_for(time2);
   car.stop();

}