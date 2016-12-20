#include "lidar.h"

// Anything you put in setup() is only run once, at the beginning
void setup() {
  // Initialize the pins used to communicate with the RoboRIO
  pinMode(PIN_ENABLE, INPUT);
  pinMode(PIN_MOVE, OUTPUT);
  pinMode(PIN_MOVE_RATE, OUTPUT);
  pinMode(PIN_TURN, OUTPUT);
  pinMode(PIN_FORWARD_CW, OUTPUT);
  pinMode(PIN_FIRE, OUTPUT);
  
  // Always a good idea to stop the robot from moving as soon as possible
  moveRobot(MOVE_STOP, 0);
  
  // Handle the case where you plug the Arduino into the socket, when the robot is still enabled. You
  // definitely don't want it to start moving immediately. Also, if you ever screw-up your software to
  // the point where you can't communicate with the Arduino anymore this gives us a chance to recover it.
  if (digitalRead(PIN_ENABLE) == HIGH)
    delay(10000);

  // Initialize the timer that controls movement
  initializeTimer3();
}


// Anything you put in loop() is run over and over again
void loop() {
  // Don't do anything if not enabled
  if (digitalRead(PIN_ENABLE) == LOW)
    return;

  // Accelerate over 10 seconds (100 * 100 = 10,000ms = 10 seconds)
  for (uint8_t i=0; i< 100; i++) {
    moveRobot(MOVE_FORWARD, i);
    delay(100);
  }
  // Decelerate over 10 seconds (100 * 100 = 10,000ms = 10 seconds)
  for (uint8_t i=100; i> 0; i--) {
    moveRobot(MOVE_FORWARD, i);
    delay(100);
  }

  moveRobot(MOVE_STOP, 0);

  // Wait 3 seconds before repeating the cycle
  delay(3000);  // Wait 3 seconds
}


// Move the robot in the given direction
void moveRobot(uint8_t movement, uint8_t powerPercentage)
{
  // Stop the robot from moving.  It is important to do this first!
  digitalWrite(PIN_MOVE, LOW);
  setPowerLevel(0);
  
  // If the robot is not enabled then ignore this instruction
  if (digitalRead(PIN_ENABLE) == LOW)
    return;
  
  switch(movement) {
    case MOVE_STOP:
      // Do nothing!
      break;
    case  MOVE_FORWARD:
      digitalWrite(PIN_TURN, LOW);
      digitalWrite(PIN_FORWARD_CW, HIGH);
      digitalWrite(PIN_MOVE, HIGH);
      break;
    case  MOVE_BACKWARDS:
      digitalWrite(PIN_TURN, LOW);
      digitalWrite(PIN_FORWARD_CW, LOW);
      digitalWrite(PIN_MOVE, HIGH);
      break;
    case  MOVE_TURN_CLOCKWISE:
      digitalWrite(PIN_TURN, HIGH);
      digitalWrite(PIN_FORWARD_CW, HIGH);
      digitalWrite(PIN_MOVE, HIGH);
      break;
    case  MOVE_TURN_CCW:
      digitalWrite(PIN_TURN, HIGH);
      digitalWrite(PIN_FORWARD_CW, LOW);
      digitalWrite(PIN_MOVE, HIGH);
      break;
  }

  // Set the speed
  setPowerLevel(powerPercentage);
}


