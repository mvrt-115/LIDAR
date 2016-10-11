package org.usfirst.frc.team115.robot.commands;

import org.usfirst.frc.team115.robot.Robot;

import edu.wpi.first.wpilibj.command.Command;

public class EnableArduino extends Command {
	
	private boolean enable = false;
	
	/**
	 * @param enable						True = drive with the Arduino, false = drive with the joystick
	 */
	public EnableArduino(boolean enable) {
		requires(Robot.driveTrain);
		this.enable = enable;
	}

	@Override
	protected void initialize() {
		Robot.driveTrain.enableArduino(enable); // set arduinoEnabled boolean in driveTrain (accessed through the enableArduino method)
	}

	@Override
	protected void execute() {
		// nothing to do here only need to enable the arduino in this command
	}

	@Override
	protected boolean isFinished() {
		return true; // finished after one run because only need to set a boolean
	}

	@Override
	protected void end() {
		// nothing to do here, no end sequence required
	}

	@Override
	protected void interrupted() {
		// nothing to do here, no interrupted sequence required
	}

}

