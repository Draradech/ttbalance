#include "stdinc.h"

static volatile int16_t pulses;

#define X 0

static int8_t steps[4][4] = {{0, -1, 1, X}, {1, 0, X, -1}, {-1, X, 0, 1}, {X, 1, -1, 0}};

static void readSolar(void)
{
   rawSensorData.voltage = ADC;
}

static void readPulses(void)
{
   ATOMIC_BLOCK(ATOMIC_FORCEON)
   {
      rawSensorData.pulses = pulses;
      pulses = 0;
   }
}

static int8_t pinold = 1;
void readSensors(void)
{
   readSolar();
   readPulses();
   if(pinold != !!(PINA & (1 << PA1)))
   {
	   if(pinold == 0)
	   {
		   state.ready = !state.ready;
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
