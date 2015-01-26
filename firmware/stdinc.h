#ifndef _STDINC_H_
#define _STDINC_H_

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//   I N C L U D E S                                                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include <util/atomic.h>
#include <util/delay.h>

#include "ram.h"

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//   C O N S T A N T S                                                       //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#define TICKS_PER_LOOP      100
#define TICKS_PER_SECOND    50000
#define LOOPS_PER_SECOND    500
#define LOOP_TIME_MS        2

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//   M A C R O S                                                             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#define force_inline        __attribute((always_inline))
#define noinline            __attribute((noinline))
#define noreturn            __attribute((noreturn))

#define HIGH(x)             ((uint8_t)((x) >> 8))
#define LOW(x)              ((uint8_t)(x))

#define MAX(a, b)           (((a) > (b)) ? (a) : (b))
#define MIN(a, b)           (((a) < (b)) ? (a) : (b))

#define MS_TO_LOOP(ms)      ((ms) / LOOP_TIME_MS)

#define LIMIT(x, min, max)  (((x) < (min)) ? (min) : (((x) < (max)) ? (x) : (max)))

#define PT1(new_val, old_val, time_constant_ms) \
   ((old_val) + ((new_val) - (old_val)) / MS_TO_LOOP(time_constant_ms))

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//   P R O T O T Y P E S                                                     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void init(void);

bool recvPacket(uint8_t *command, uint8_t *size, uint8_t **data);
void sendPacketBegin(uint8_t command);
void sendData(uint8_t byte);
void sendData16(uint16_t word);
void sendPacketEnd(void);

void input(void);
void output(void);

void initSensors(void);
void readSensors(void);

void attitudeCalculation(void);

void actuate(void);

#endif // _STDINC_H_
