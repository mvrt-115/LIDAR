#define PIN_ENABLE           9  // Input from RoboRIO
#define PIN_MOVE             8  // Output to RoboRIO
#define PIN_TURN             7  // Output to RoboRIO
#define PIN_FORWARD_CW       6  // Output to RoboRIO
#define PIN_FIRE             5  // Output to RoboRIO
#define PIN_DEBUG            4  // Spare pin that can be used for debugging
#define PIN_LIDAR_MOTOR      A0 // The I/O pin used to control motor speed

#define MOVE_STOP            0
#define MOVE_FORWARD         1
#define MOVE_BACKWARDS       2
#define MOVE_TURN_CLOCKWISE  3
#define MOVE_TURN_CCW        4


// Anything you put in setup() is only run once, at the beginning
void setup() {
  // Initialize the pins used to communicate with the RoboRIO
  pinMode(PIN_ENABLE, INPUT);
  pinMode(PIN_MOVE, OUTPUT);
  pinMode(PIN_TURN, OUTPUT);
  pinMode(PIN_FORWARD_CW, OUTPUT);
  pinMode(PIN_FIRE, OUTPUT);
  
  // Always a good idea to stop the robot from moving as soon as possible
  moveRobot(MOVE_STOP);
  
  // Handle the case where you plug the Arduino into the socket, when the robot is still enabled. You
  // definitely don't want it to start moving immediately. Also, if you ever screw-up your software to
  // the point where you can't communicate with the Arduino anymore this gives us a chance to recover it.
  if (digitalRead(PIN_ENABLE) == HIGH)
    delay(10000);
}


// Anything you put in loop() is run over and over again
void loop() {
  // Don't do anything if not enabled
  if (digitalRead(PIN_ENABLE) == LOW)
    return;
    
  moveRobot(MOVE_FORWARD);
  delay(3000);  // Wait 3 seconds
  moveRobot(MOVE_STOP);
  delay(3000);  // Wait 3 seconds
  moveRobot(MOVE_BACKWARDS);
  delay(3000);  // Wait 3 seconds
  moveRobot(MOVE_STOP);
  delay(3000);  // Wait 3 seconds
  moveRobot(MOVE_TURN_CLOCKWISE);
  delay(3000);  // Wait 3 seconds
  moveRobot(MOVE_STOP);
  delay(3000);  // Wait 3 seconds
  moveRobot(MOVE_TURN_CCW);
  delay(3000);  // Wait 3 seconds
  moveRobot(MOVE_STOP);
  delay(10000);  // Wait 10 seconds
}


// Move the robot in the given direction
void moveRobot(uint8_t movement)
{
  // Stop the robot from moving.  It is important to do this first!
  digitalWrite(PIN_MOVE, LOW);
  
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
}


