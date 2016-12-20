// Timer 1 is used for 2 things:
// 1. Control the speed of the LIDAR motor to keep revs/minute in a good range
// 2. Monitor the "Enable" pin, and disable movement if it goes LOW
//
// Interrupt operation
// ===================
// The timer interrupt is configured to fire every 20ms (0.020 seconds), or 50 times per second.
// When the timer fires the "Enable" pin is checked.  If it is low then robot movement will be
// disabled and the LED's will go into a "heartbeat" sequence to show that the Arduino is still
// alive & kicking!
// The timer is also used to control motor speed.  Power to the motor is PWM'd at a slow rate
// (50 times per second).  You could use analogWrite( ) to send a nice PWM signal, but the PWM 
// frequency is very high so he motor will audibly hum.
// The motor needs approx. 3V to spin at the right rate, but supplying it with a constant voltage
// is a BAD idea because dust bunnies and manufacturing variances mean the rate won't be consistent
// between LIDAR units.
// If the motor receives 5V it will spin at around 600 RPM, and if it receives 0V it will stop.
// At every interrupt, the pin controlling power to the motor is pulled high (so the motor gets 5V
// and will accelerate).  After a certain period of time, the pin will be pulled LOW and the motor
// will start to slow down.  Keep in mind that the motor has momentum, and this HIGH/LOW is happening
// 50 times per second so the acceleration/deceleration won't be noticed!
//
// Technical information
// =====================
// - Timer 1 is set to CTC mode
// - Compare A is set to a value to force a timer interrupt every 20ms
// - Compare B is set to a value that gives the right duration of HIGH to the motor
// Keep in mind that unlike Compare A, Compare B does not reset Timer 1's counter.  The steps look
// something like this:
//   a. Counter = 0: Write motor pin HIGH, and set Compare B to correct duration.
//   b. Counter = Compare B: Write motor pin low
//   c. Counter = Compare A: Counter is set back to 0 (go to a.)
//
// The Arduino Micro Pro has a 16MHz clock.  We set the prescaler is set to 8.  This gives a timer
// speed of 16,000,000 / 8 = 2,000,000. This means the timer counts from 0 to 2,000,000 in one second.
// We'd like the interrupt to fire 50 times per second so we set the compare register OCR1A to
// 2,000,000 / 50 = 40,000.

#include "lidar.h"
#define MOTOR_MIN_RPM      200     // The low range of the desired RPM
#define MOTOR_MAX_RPM      210     // The high range of the desired RPM

#define MOTOR_MIN_DUTY     16000    // Lowest duty cycle.  16000/40000 = 40%
#define MOTOR_MAX_DUTY     30000   // Highest duty cycle.  30000/40000 = 75%
#define MOTOR_START_DUTY   24000   // Duty cycle value that is close to being right (3V/5V * 40000)

boolean isEnableHigh = false;

// Initialize Timer 1
// It should fire 50 times every second (every 20ms)
void initializeTimer1(void) {
  // Set the LIDAR rotation motor pin as output
  pinMode(PIN_LIDAR_MOTOR, OUTPUT);

  cli();                               // Disable global interrupts
  TCCR1A = 0;                          // Timer 0 is independent of the I/O pins, CTC mode
  TCCR1B = _BV(WGM12) + _BV(CS11);     // Timer 0 CTC mode, prescaler is 8
  TCNT1 = 0;                           // Clear the timer count 
  OCR1A = 40000;                       // Set compare match so the interrupt occurs 50 times per second
  OCR1B = MOTOR_START_DUTY;            // Set compare to initial motor PWM value
  TIMSK1 |= _BV(OCIE1A);               // Enable timer compare interrupt
  TIMSK1 |= _BV(OCIE1B);               // Enable the timer compare interrupt for motor motion
  sei();                               // Enable global interrupts
}


// Timer 1 ISR (Iterrupt Service Routine)
// This timer fires 50 times per second (every 20ms)
ISR(TIMER1_COMPA_vect)
{
  // Allow the motor accelerate for a short time
  digitalWrite(PIN_LIDAR_MOTOR, HIGH);
  
  // See if Arduino has control of the robot
  if (digitalRead(PIN_ENABLE) == HIGH) {
    // Arduino has control
    isEnableHigh = true;
  }
  else {
    // Arduino does not have control
    // Did Arduino previously have control?
    if (isEnableHigh == true) {
      // Disable the "Move" pin as a safety measure
      digitalWrite(PIN_MOVE, LOW);
    }
    isEnableHigh = false;
    
    // Show a heatbeat so spectators can see we're alive
    doHeartbeat();
  }
}


// Timer 1 Compare B interrrupt
// This interrupt fires once the desired pulse duration has been sent to the motor
ISR(TIMER1_COMPB_vect)
{
  digitalWrite(PIN_LIDAR_MOTOR, LOW);
}


// Show a "heartbeat" when the Arduino is not in control
// This function is called every 20ms (50 times per second) from the ISR
void doHeartbeat()
{
  // Local variables don't retain their values between calls, but adding "static" means it does!
  static uint16_t counter = 0;
  static boolean isOn = false;
  
  // Is it time to do something with the LED's?  Toggle them every 1/2 second
  if (counter++ >= 25) {
    // Restart the counter
    counter = 0;
    
    // If the LEDs were off then turn them on, or vice versa
    isOn = !isOn;
    
    // With "Move" set to LOW, it is safe to turn the other 2 LEDs on (and off)
    digitalWrite(PIN_TURN, isOn);
    digitalWrite(PIN_FORWARD_CW, !isOn);
  }
}


// The LIDAR gives the current RPM with every reading.  The speed is passed in here and
// the PWM value of power to the motor is adjusted slowly until the LIDAR is rotating in
// the desired range.
void verifyMotorSpeed(uint16_t motorRPM)
{
  // Change the '0' to '1' in the next line to see the motor RPM
  if (0 && (motorRPM < MOTOR_MIN_RPM || motorRPM > MOTOR_MAX_RPM)) {
    Serial.print(motorRPM);
    Serial.print("   ");
    Serial.println(OCR1B);
  }
  // If the LIDAR is spinning too fast then slow it down
  if (motorRPM > MOTOR_MAX_RPM && OCR1B > MOTOR_MIN_DUTY)
    OCR1B -= 5;
  // If the LIDAR is spinning too slow then speed it up
  if (motorRPM < MOTOR_MIN_RPM && OCR1B < MOTOR_MAX_DUTY)
    OCR1B += 5;
}


// Lets the rest of the software know if the Arduino has control of the robot
boolean isEnabled()
{
  return isEnableHigh;
}
