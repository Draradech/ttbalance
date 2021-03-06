#include "stdinc.h"

static volatile int16_t pulses;

#define X 0

static int8_t steps[4][4] = {{0, -1, 1, X}, {1, 0, X, -1}, {-1, X, 0, 1}, {X, 1, -1, 0}};

static void readSolar(void)
{
   sensorData.voltageRaw = ADC;
}

static void readPulses(void)
{
   ATOMIC_BLOCK(ATOMIC_FORCEON)
   {
      sensorData.pulses = pulses;
      pulses = 0;
   }
}

void readSensors(void)
{
   static uint8_t pinold = 1;
   int16_t tmp16;
   int32_t tmp32;

   readSolar();
   readPulses();

   tmp16 = (int32_t) sensorData.voltageRaw * 1250 >> 8;        // * 5000 (5,0 V) / 1024
   sensorData.voltage = PT1(tmp16, sensorData.voltage, parameter.tcVoltage);
   
   tmp32 = (int32_t) sensorData.pulses * 125 * 1024;
   sensorData.speed = PT1(tmp32, sensorData.speed, parameter.tcSpeed1);
   
   tmp32 = (int32_t) sensorData.speed;
   sensorData.filteredSpeed = PT1(tmp32, sensorData.filteredSpeed, parameter.tcSpeed2);
   
   if(pinold != ((PINA & (1 << PA1)) >> PA1))
   {
      if(pinold == 0)
      {
         mode = !mode;
      }
      pinold = !pinold;
   }
}

volatile static uint8_t pcll;
ISR(PCINT2_vect)
{
   uint8_t pc = PINC & 0x03;
   pulses += steps[pcll][pc];
   pcll = pc;
}
