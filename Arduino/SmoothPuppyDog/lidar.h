#define PIN_ENABLE           9  // Input from RoboRIO
#define PIN_MOVE             8  // Output to RoboRIO
#define PIN_TURN             7  // Output to RoboRIO
#define PIN_FORWARD_CW       6  // Output to RoboRIO
#define PIN_FIRE             5  // Output to RoboRIO
#define PIN_MOVE_RATE        4  // Output to RoboRIO
#define PIN_LIDAR_MOTOR      A0 // The I/O pin used to control motor speed

#define MOVE_STOP            0
#define MOVE_FORWARD         1
#define MOVE_BACKWARDS       2
#define MOVE_TURN_CLOCKWISE  3
#define MOVE_TURN_CCW        4

#define ERR_WAY_TOO_SLOW     82  // LIDAR rotation speed is extremely low
#define ERR_TOO_SLOW         80  // LIDAR rotation speed is low
#define ERR_NO_READING       53  // Unable to get reading
#define ERR_TOO_CLOSE        3   // Obstacle is very close to LIDAR

#define DEGREE_OFFSET        8   // Offset to accomodate LIDAR not being mounted straight (multiple of 4)

