#include "stdinc.h"

#define SIGN(x) ((x > 0) ? 1 : ((x < 0) ? -1 : 0))
#define ABS(x) (((x) > 0) ? (x) : -(x))

void actuate()
{
   int16_t power, powerLimit, target;

   OCR1B = controlPara.lamp;

   if(ABS(attitude.speed) > 32)
   {
      state.targetSub -= controlPara.ipartout * (attitude.speedslow / 1024 - controlPara.targetSpeed * 10) / 128;
   }
   while(state.targetSub >= 1000) controlPara.target++, state.targetSub -= 1000;
   while(state.targetSub <= 1000) controlPara.target--, state.targetSub += 1000;
   
   target = controlPara.target;
   if(ABS(attitude.speedslow) > 20480)
   {
      target += controlPara.targetOffset * SIGN(attitude.speedslow);
   }
   target += controlPara.ppartout * (attitude.speedslow / 1024 - controlPara.targetSpeed * 10) / 64;
   out.target = target;
   
   out.p = -(int32_t) controlPara.ppart * (rawSensorData.voltage - target) / 64;
   power = out.p;
   
   out.d = -(int32_t) controlPara.dpart * (attitude.voltage - attitude.voltageOld) / 128;
   power += out.d;
   
   power += (int32_t) attitude.speed * controlPara.speedgain / 1024;
   
   if (  (state.ready == 2)
      && (ABS(power) > 255)
      )
   {
      state.ready = 1;
   }
   
   if (  (state.ready == 1)
      && (ABS(power) < 10)
      )
   {
      state.ready = 2;
   }
   
   powerLimit = 255;
   if(state.ready != 2)
   {
      powerLimit = 0;
   }
   else if(ABS(attitude.speed) > 32)
   {
      state.standstill = 0;
   }
   else
   {
      state.standstill++;
   }
   
   if(state.standstill > 2500)
   {
      power += SIGN(power) * (state.standstill - 2500);
   }
   out.power = power;

   switch(state.ready)
   {
      case 0: PORTB = 6; break;
      case 1: PORTB = 4; break;
      case 2: PORTB = 5; break;
   }
   
   if (power < 0)
   {
      PORTC |=  (1 << PC2);
      PORTC &= ~(1 << PC3);
      OCR2B = MIN(-power, powerLimit);
   }
   else if (power > 0)
   {
      PORTC &= ~(1 << PC2);
      PORTC |=  (1 << PC3);
      OCR2B = MIN(power, powerLimit);
   }
   else
   {
      PORTC &= ~(1 << PC2);
      PORTC &= ~(1 << PC3);
      OCR2B = 0;
   }
}
