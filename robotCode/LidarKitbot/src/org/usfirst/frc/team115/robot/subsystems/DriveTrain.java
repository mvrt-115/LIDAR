package org.usfirst.frc.team115.robot.subsystems;

import org.usfirst.frc.team115.robot.RobotMap;
import org.usfirst.frc.team115.robot.commands.Drive;

import edu.wpi.first.wpilibj.CANTalon;
import edu.wpi.first.wpilibj.Counter;
import edu.wpi.first.wpilibj.DigitalInput;
import edu.wpi.first.wpilibj.DigitalOutput;
import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.RobotDrive;
import edu.wpi.first.wpilibj.command.Subsystem;

/**
 * Drive train subsystem code specifying the different components of the drive train,
 * @author Heather Baker
 *
 */
public class DriveTrain extends Subsystem {
	
	// Create the four motor controllers for the robot
	private CANTalon frontLeft, frontRight, backLeft, backRight;
	
	// RobotDrive holds the four motor controllers above and makes it easier to control the drive train as a complete subsystem
	private RobotDrive driveTrain;
	
	// Create the input for moving, turning, and the direction
	private DigitalInput move, turn, direction, shoot;
	// Create the output for enabling the Arduino
	private DigitalOutput enable, flashlight;
	
	private boolean arduinoEnabled = false;
	

	private Counter count;
	private double ARDUINO_PULSE_CYCLE = 100;
	

	
	/**
	 * Constructor for the DriveTrain. Instantiates all the objects associated with the drive train.
	 */
	public DriveTrain() {
		// Create motor controllers. RobotMap.<location> is the ID of the CANTalon for the motor
		// If you go to RobotMap.java, you can see the ID's
		frontLeft = new CANTalon(RobotMap.FRONT_LEFT);
		frontRight = new CANTalon(RobotMap.FRONT_RIGHT);
		backLeft = new CANTalon(RobotMap.BACK_LEFT);
		backRight = new CANTalon(RobotMap.BACK_RIGHT);
		
		// Collects all the motor controllers together so drive commands are simpler for us
		// Order of the motor controllers does matter
		driveTrain = new RobotDrive(frontLeft, backLeft, frontRight, backRight);
		
		// create counter to detect speed
		count = new Counter();
		count.setUpSource(move);
		count.setDistancePerPulse(1);
		
		// Create the inputs and outputs. RobotMap.<name> is the RoboRIO pin number the output/input is attached to
		// If you go to RobotMap.java, you can see the pin numbers
		enable = new DigitalOutput(RobotMap.ENABLE);
		move = new DigitalInput(RobotMap.MOVE);
		turn = new DigitalInput(RobotMap.TURN);
		direction = new DigitalInput(RobotMap.DIRECTION);
		shoot = new DigitalInput(5);
		flashlight = new DigitalOutput(9);
	}
	
	/**
	 * Enables and disables the Arduino. Enable = true, disable = false.
	 * @param enable					Enable or disable the Arduino.
	 */
	public void enableArduino(boolean enable) {
		arduinoEnabled = enable; // used to determine if driving with joystick or with Arduino.
		this.enable.set(enable); // output to the Arduino whether enabling or disabling.
	}
	
	public boolean isArduinoEnabled() {
		return arduinoEnabled;
	}
	
	// this should be in a different subsystem, but it's 5am and I need to study for stats
	// should work fine
	public boolean getShoot() {
		return shoot.get();
	}

	public void shootLight(boolean enable) {
		flashlight.set(enable);
	}
	
	public double getSpeedScalar() {
		double speed = count.getRate() * 1000 / ARDUINO_PULSE_CYCLE; // this may need to change
		if (speed > 1) {
			speed = 1;
		}
		if (speed < 0.35/0.85) {
			speed = 0.35/0.85;
		}
		return speed;
	}
	
	public void resetCount() {
		count.reset();
	}



	
	/**
	 * @return							Return the value of move (high or low)
	 */
	public boolean getMove() {
		return move.get();
	}
	
	/**
	 * @return							Return the value of turn (high or low)
	 */
	public boolean getTurn() {
		return turn.get();
	}
	
	/**
	 * @return							Return the value of direction (high or low)
	 */
	public boolean getDirection() {
		return direction.get();
	}
	
	/**
	 * @param joystick					Joystick to drive the robot with
	 */
	public void drive(Joystick joystick) {
		driveTrain.arcadeDrive(joystick);
	}
	
	/**
	 * @param move						Speed to move the robot forward or backwards
	 * @param rotate					Speed to rotate the robot
	 */
	public void drive(double move, double rotate) { // Note there is already a drive method, Java distinguished between these two methods because they have different parameters (search Overloading).
		driveTrain.arcadeDrive(move, rotate);
	}
	
	/**
	 * Stop the robot from driving using the drive(double move, double rotate command).
	 */
	public void stop() {
		drive(0,0);
	}
	

	/**
	 * Default command to drive the robot with
	 */
	@Override
	protected void initDefaultCommand() {
		setDefaultCommand(new Drive());
	}

}



