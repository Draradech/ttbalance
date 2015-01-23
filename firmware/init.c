#include "stdinc.h"

void init(void)
{
   // PA0 - Battery

   // PA1 - Switch
   PORTA |= (1 << PA1);

   // PA2

   // PA3

   // PA4 - MaxSonar Trigger
   DDRA |= (1 << PA4);

   // PA5

   // PA6

   // PA7

   // PB0 - RGB 1
   DDRB  |= (1 << PB0);

   // PB1 - RGB 2
   DDRB  |= (1 << PB1);

   // PB2 - RBG 3
   DDRB  |= (1 << PB2);

   // PB3

   // PB4

   // PB5

   // PB6

   // PB7

   // PC0 - Motor Enc A

   // PC1 - Motor Enc B

   // PC2 - Motor B
   DDRC |= (1 << PC2);

   // PC3 - Motor A
   DDRC |= (1 << PC3);

   // PC4

   // PC5

   // PC6

   // PC7

   // PD0 - Bluetooth RX

   // PD1 - Bluetooth TX

   // PD2 - MaxSonar RX

   // PD3

   // PD4 - Lamp
   DDRD |= (1 << PD4);

   // PD5

   // PD6 - Motor PWM
   DDRD |= (1 << PD6);

   // ADC
   ADMUX  =  (1 << REFS0);     // AVCC with external cap
   ADCSRA =  (1 << ADEN)       // ADC Enabled
          |  (1 << ADATE)      // ADC Auto Trigger
          |  (1 << ADPS0)
          |  (1 << ADPS1)
          |  (1 << ADPS2);     // prescalar 128
   ADCSRA |= (1 << ADSC);      // ADC start conversion

   // PCINT
   PCICR  = (1 << PCIE2);      // pin change interrupts for port C
   PCMSK2 = (1 << PCINT16)     // pin change interrupt for PC0
          | (1 << PCINT17);    // pin change interrupt for PC1

   // UART (already initialized by bootloader)
   UCSR0B |= (1 << RXCIE0);    // RX Complete Interrupt

   // Timer 0 - Mainloop
   TCCR0A = (1 << WGM01);      // CTC mode,
   TCCR0B = (1 << CS01);
   OCR0A  = 50 - 1;            // Prescalar 8, TOP 50 -> 50000 Hz

   TIMSK0 = (1 << OCIE0A);     // Output compare interrupt enable

   // Timer 1 - PWM on OC1B (Lamp)
   TCCR1A = (1 << COM1B1)      // clear on compare match
          | (1 << WGM10);
   TCCR1B = (1 << WGM12)       // fast pwm
          | (1 << CS10)
          | (1 << CS11);       // prescalar 64 -> 1200Hz PWM

   // Timer 2 - 2x PWM
   TCCR2A = (1 << COM2B1)
          | (1 << WGM20);      // non-inverting phase-correct PWM mode
   TCCR2B = (1 << CS20);       // Prescalar 1 -> ~80khz PWM

   // Ram Vars
   attitude.voltage = 2500;
   
   controlPara.dpart = 80;
   controlPara.ppart = 15;
   controlPara.ipart = -10;
   controlPara.speedgain = 75;
   controlPara.target = 300;
   controlPara.targetOffset = 15;
   controlPara.lamp = 100;
   controlPara.targetSpeed = 100;
   
   // Enable Interrupts
   sei();
}
