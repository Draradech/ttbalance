#ifndef _RAM_H_
#define _RAM_H_

#include "stdinc.h"

#ifdef _DEFINE_RAM_VARS_
#define global
#else
#define global extern
#endif

typedef struct SRawSensorData
{
   int16_t   voltage;
   int16_t   pulses;
} SRawSensorData;

typedef struct
{
   int16_t   voltage;        // LSB: 0.001 V
   int16_t   voltageOld;
   int16_t   speed;
   int32_t   speedslow;
} SAttitude;

typedef struct
{
   int16_t   targetSub;
   int8_t	ready;
   int16_t   standstill;
} SState;

typedef struct
{
   int16_t   ppart;
   int16_t   dpart;
   int16_t   ipart;
   int16_t   target;
   int16_t   targetSpeed;
   int16_t   targetOffset;
   int16_t   speedgain;
   int16_t   lamp;
} SControlPara;

typedef struct
{
   int16_t   power;
   int16_t   p;
   int16_t   d;
   int16_t   target;
} SOutput;


global SState state;
global SOutput out;
global SRawSensorData rawSensorData;
global SAttitude attitude;
global SControlPara controlPara;

global uint8_t lastLoopTicks;
global bool measurementOut;

#endif // _RAM_H_
