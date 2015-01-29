#include "stdinc.h"

#define ABS(x) (((x) > 0) ? (x) : -(x))

void actuate()
{
   int16_t pwmLimit, voltageDeviationOld;

   OCR1B = parameter.pwmLamp;
   
   if(parameter.targetSpeed * 10 > controlState.targetSpeedGradlim) controlState.targetSpeedGradlim++;
   if(parameter.targetSpeed * 10 < controlState.targetSpeedGradlim) controlState.targetSpeedGradlim--;
   
   controlState.speedDeviation = sensorData.filteredSpeed / 1024 - controlState.targetSpeedGradlim;
   
   controlState.targetVoltageP = (int32_t)parameter.speedP * controlState.speedDeviation / 64;
   controlState.targetVoltageI += (int32_t)parameter.speedI * controlState.speedDeviation / 128;
   controlState.targetVoltageWP = parameter.speedWP;
   controlState.targetVoltage = controlState.targetVoltageP + controlState.targetVoltageI / 1024 + controlState.targetVoltageWP;
   
   voltageDeviationOld = controlState.voltageDeviation;
   controlState.voltageDeviation = controlState.targetVoltage - sensorData.voltage;
   
   controlState.pwmP = (int32_t)parameter.voltageP * controlState.voltageDeviation / 2048;
   controlState.pwmD = (int32_t)parameter.voltageD * (voltageDeviationOld - controlState.voltageDeviation) / 512;
   controlState.pwmFeedback = (int32_t)sensorData.speed * parameter.speedFeedback / 1024;
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
   
   if (controlState.pwm < 0)
   {
      PORTC |=  (1 << PC2);
      PORTC &= ~(1 << PC3);
      OCR2B = MIN(-controlState.pwm, pwmLimit);
   }
   else if (controlState.pwm > 0)
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
