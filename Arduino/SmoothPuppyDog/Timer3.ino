// See Timer1 for complete description of timer operation.  Timer1 and Timer3 are both
// 16-bit timers
//
// Timer 3 is used for one thing:
// 1. Pulse the MOVE_RATE pin to indicate the desired speed
//
// Since the RoboRio polls for data every 20ms, this is used as the unit of time.  The
// pulse rate can vary between 1 (10% power) and 10 (100 % power).
//   - 10 pulses every 20ms = 500 pulses per second = 500Hz
//   - 1 pulse every 20ms = 50 pulses per second = 50Hz
//
// The MOVE pin could be used for the rate too.  This would save an I/O pin with just
// a few minor software changes

#include "lidar.h"

uint8_t powerLevel = 0;

// Initialize Timer 3
void initializeTimer3(void) {
  // Set the LIDAR rotation motor pin as output
  pinMode(PIN_LIDAR_MOTOR, OUTPUT);

  cli();                               // Disable global interrupts
  TCCR3A = 0;                          // Timer 3 is independent of the I/O pins, CTC mode
  TCCR3B = _BV(WGM32) + _BV(CS31);     // Timer 3 CTC mode, prescaler is 8
  TCNT3 = 0;                           // Clear the timer count 
  OCR3A = 40000;                       // Set compare match so the interrupt occurs 50 times per second
  OCR3B = 200;                         // The pulse width is always 200/40000*20ms = 0.1ms
  TIMSK3 |= _BV(OCIE3A);               // Enable timer compare interrupt
  TIMSK3 |= _BV(OCIE3B);               // Enable the timer compare interrupt for motor motion
  sei();                               // Enable global interrupts
}


// Timer 3 ISR (Iterrupt Service Routine)
// This timer fires every time a pulse is needed
ISR(TIMER3_COMPA_vect)
{
  // Time to issue a pulse
  if (powerLevel != 0)
    digitalWrite(PIN_MOVE_RATE, HIGH);
}


// Timer 3 Compare B interrrupt
// This interrupt fires once the desired pulse duration has been sent
ISR(TIMER3_COMPB_vect)
{
  digitalWrite(PIN_MOVE_RATE, LOW);
}


// Set the motor power level
// Power level is between 0 (no power) and 100 (full power)
void setPowerLevel(uint8_t powerPercentage)
{
  // Make sure the power level is acceptable
  powerPercentage = constrain(powerPercentage, 0, 100);
  
  // Save the power level
  powerLevel = powerPercentage;

  // Disable global interrupts
  cli();
                              
  // Set the high interrupt mark
  // 40,000 = 20ms (which would give 1 pulse per second)
  if (powerPercentage == 0)
    OCR3A = 40000;
  else
    OCR3A = 40000 / powerPercentage; 

  // Restart the interrupt counter
  TCNT1 = 0;

  // Enable global interrupts
  sei();
}


