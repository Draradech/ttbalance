#include "stdinc.h"

void actuate()
{
   static uint16_t standstill;
   int16_t pwmLimit, voltageDeviationOld, targetSpeed;

   OCR1B = parameter.pwmLamp;
   
   targetSpeed = parameter.targetSpeed * 10;
   if(ABS(sensorData.filteredSpeed) / 1024 < 5 && mode == 2)
   {
      standstill++;
      if(standstill > 0x400)
      {
         targetSpeed = (standstill & 0x400) - 0x200;
      }
   }
   else
   {
      standstill = 0;
   }
   
   if(targetSpeed > controlState.targetSpeedGradlim) controlState.targetSpeedGradlim += 2;
   if(targetSpeed < controlState.targetSpeedGradlim) controlState.targetSpeedGradlim -= 2;

   controlState.targetSpeedGradlim = LIMIT(controlState.targetSpeedGradlim, -1500, 1500);
   
   controlState.speedDeviation = sensorData.filteredSpeed / 1024 - controlState.targetSpeedGradlim;
   
   controlState.targetVoltageP = (int32_t)parameter.speedP * controlState.speedDeviation / 64;
   if(mode == 2) controlState.targetVoltageI += (int32_t)parameter.speedI * controlState.speedDeviation / 128;
   controlState.targetVoltageI = LIMIT(controlState.targetVoltageI, (int32_t)-1000 * 1024, (int32_t)1000 * 1024);
   controlState.targetVoltageWP = parameter.speedWP;
   controlState.targetVoltage = controlState.targetVoltageP + controlState.targetVoltageI / 1024 + controlState.targetVoltageWP;
   controlState.targetVoltage = LIMIT(controlState.targetVoltage, parameter.speedWP - 1000, parameter.speedWP + 1000);
   
   voltageDeviationOld = controlState.voltageDeviation;
   controlState.voltageDeviation = controlState.targetVoltage - sensorData.voltage;
   
   controlState.pwmP = (int32_t)parameter.voltageP * controlState.voltageDeviation / 2048;
   controlState.pwmD = (int32_t)parameter.voltageD * (controlState.voltageDeviation - voltageDeviationOld) / 128;
   controlState.pwmFeedback = (int32_t)sensorData.speed / 1024 * parameter.speedFeedback / 1024;
   controlState.pwm = controlState.pwmP + controlState.pwmD + controlState.pwmFeedback;
 
   if (mode == 2 && ABS(controlState.pwm) > 255) mode = 1;
   if (mode == 1 && ABS(controlState.pwm) < 10) mode = 2;
   
   if(mode == 2) pwmLimit = 255;
   else          pwmLimit = 0;

   switch(mode)
   {
      case 0: PORTB = 6; break;
      case 1: PORTB = 4; break;
      case 2: PORTB = 5; break;
   }
   
   if(controlState.pwm < 0)
   {
      PORTC |=  (1 << PC2);
      PORTC &= ~(1 << PC3);
      OCR2B = MIN(-controlState.pwm, pwmLimit);
   }
   else if(controlState.pwm > 0)
   {
      PORTC &= ~(1 << PC2);
      PORTC |=  (1 << PC3);
      OCR2B = MIN(controlState.pwm, pwmLimit);
   }
   else
   {
      PORTC &= ~(1 << PC2);
      PORTC &= ~(1 << PC3);
      OCR2B = 0;
   }
}
