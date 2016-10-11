package org.usfirst.frc.team115.robot.commands;

import org.usfirst.frc.team115.robot.Robot;

import edu.wpi.first.wpilibj.command.Command;

public class Drive extends Command {
	
	private final double SPEED = 0.5; // Automatic speed for driving using the Arduino
	
	public Drive() {
		requires(Robot.driveTrain); // always requires requires() in the constructor for the command. Tells the command what subsystem it is using.
	}
	
	@Override
	protected void initialize() {
		
	}

	@Override
	protected void execute() {
		if (Robot.driveTrain.isArduinoEnabled()) { // if the Arduino is enabled
			if (Robot.driveTrain.getMove()) { // if Move is true
				if (Robot.driveTrain.getTurn()) { // it turn is true
					if (Robot.driveTrain.getDirection()) { // if the direction is high (true)
						Robot.driveTrain.drive(0, SPEED); // turn clockwise
					} else { // if the direction is low (false)
						Robot.driveTrain.drive(0, -SPEED); // turn counterclockwise
					}
				} else { // if turn is false
					if (Robot.driveTrain.getDirection()) { // if the direction is high (true)
						Robot.driveTrain.drive(SPEED, 0); // go forward
					} else { // if the direction is low (false)
						Robot.driveTrain.drive(-SPEED, 0); // go backwards
					}
				}
			}
		} else { // if the Arduino is disabled
			Robot.driveTrain.drive(Robot.oi.getJoystick()); // drive using the joystick
		}
		
	}

	@Override
	protected boolean isFinished() { // default command so never finished
		return false;
	}

	@Override
	protected void end() { // when the command ends then the drive train stops
		Robot.driveTrain.stop();
	}

	@Override
	protected void interrupted() { // if the command is interrupted, then run what is in end() (above)
		end();
		
	}

}

