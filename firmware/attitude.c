#include "stdinc.h"

void attitudeCalculation(void)
{
   int16_t tmp16;
   int32_t tmp32;

   tmp16 = (int32_t) rawSensorData.voltage * 1250 >> 8;        // * 5000 (5,0 V) / 1024
   attitude.voltageOld = attitude.voltage;
   attitude.voltage = PT1(tmp16, attitude.voltage, 8);
   
   tmp16 = rawSensorData.pulses * 128;
   attitude.speed = PT1(tmp16, attitude.speed, 16);
   
   tmp32 = (int32_t) attitude.speed * 1024;
   attitude.speedslow = PT1(tmp32, attitude.speedslow, 512);
}
