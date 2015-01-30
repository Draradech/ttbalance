#ifndef _RAM_H_
#define _RAM_H_

#include "stdinc.h"

#ifdef _DEFINE_RAM_VARS_
#define global
#else
#define global extern
#endif

typedef struct
{
   int16_t   voltageRaw;
   int16_t   voltage;        // LSB: 0.001 V
   int16_t   pulses;
   int32_t   speed;
   int32_t   filteredSpeed;
} SSensorData;

typedef struct
{
   int16_t   targetSpeed;
   int16_t   speedP;
   int16_t   speedI;
   int16_t   speedWP;
   int16_t   voltageP;
   int16_t   voltageD;
   int16_t   speedFeedback;
   int16_t   pwmLamp;
} SParameter;

typedef struct
{
   int16_t   targetSpeedGradlim;
   int16_t   speedDeviation;
   int16_t   targetVoltageP;
   int32_t   targetVoltageI;
   int16_t   targetVoltageWP;
   int16_t   targetVoltage;
   int16_t   voltageDeviation;
   int16_t   pwmP;
   int16_t   pwmD;
   int16_t   pwmFeedback;
   int16_t   pwm;
} SControlState;

global SSensorData sensorData;
global SParameter parameter;
global SControlState controlState;

global uint8_t lastLoopTicks;
global uint8_t measurementOut;
global uint8_t mode;

#endif // _RAM_H_
