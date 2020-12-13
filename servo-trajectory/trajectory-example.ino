/* * * * * * * * * * * * * * * * * * * * * * *
 * EXAMPLE SKETCH FOR THE SERVO TRAJECTORY CONTROLLER CLASS
 *
 * Code by: Simon Bluett
 * Website: https://wired.chillibasket.com
 * Version: 1.2
 * Date:    19th May 2020
 * Copyright (C) 2020, MIT License
 *
 * In this example, we aim to move a servo motor back and 
 * forth between a position of 20° and 180°. We want 
 * the maximum velocity of the servo to be 60°/second, 
 * the acceleration to be 40°/s^2 and the deceleration 
 * to be 34°/s^2. 
 *
 * An image showing the output servo position, velocity 
 * and acceleration is included with this example. 
 * * * * * * * * * * * * * * * * * * * * * * */


// --- Include the library ---
// Make sure that the file "trajectory.h" is included in 
// the same folder as the Arduino sketch
#include "trajectory.h"
#include <Servo.h>


// --- Instantiate the class ---
/**
 * If you want the acceleration and deceleration to be the same
 * FORMAT: Trajectory(max velocity, acceleration)
 */
//Trajectory servoTrajectory(60, 40);

/**
 * If the acceleration and deceleration are different
 * FORMAT: Trajectory(max velocity, acceleration, deceleration)
 */
Trajectory servoTrajectory(60, 40, 34);

/**
 * By default the dynamics controller turns off when it is within 0.1 units
 * of the target position. This threshold value can be changed in the declaration
 * FORMAT: Dynamics(max velocity, acceleration, deceleration, threshold)
 */
//Trajectory servoTrajectory(60, 40, 34, 0.05);

Servo myservo;


// --- Define global variables ---
// The controller will be updated at a rate of 100Hz
#define UPDATE_FREQUENCY 100
#define UPDATE_TIME (1000 / UPDATE_FREQUENCY)
unsigned long updateTimer = 0;
int moveNumber = 0;


/* * * * * * * * * * * * * * * * * * * * * * *
 * SETUP
 * * * * * * * * * * * * * * * * * * * * * * */
void setup() {

	Serial.begin(115200);
	while(!Serial);
	Serial.println("Starting program");

	// Attaches the servo on pin 9 to the servo object
	myservo.attach(9);

	// We want the servo to start at an angle of 20°
	myservo.write(20);

	// By default the controller starts at 0, so we need to
	// set the starting angle as well
	servoTrajectory.reset(20);

	/**
	 * If we suddenly decide we want to change the maximum velocity to 30°/s,
	 * the acceleration to 15°/s^2 and deceleration to 5.3°/s^2
	 */
	//servoTrajectory.setMaxVel(30);
	//servoTrajectory.setAcc(15);
	//servoTrajectory.setDec(5.3);

	/**
	 * To read what the current velocity and acceleration settings are
	 */
	//float maxVelocity = servoTrajectory.getMaxVel();
	//float acceleration = servoTrajectory.getAcc();
	//float deceleration = servoTrajectory.getDec();

	updateTimer = millis();
}


/* * * * * * * * * * * * * * * * * * * * * * *
 * NEW MOVEMENT COMMANDS
 * * * * * * * * * * * * * * * * * * * * * * */
void nextMove() {
	switch (moveNumber) {
		case 0:
			// First we move to the 180° position as fast as possible
			servoTrajectory.setTargetPos(180);
			break;

		case 1:
			// Then move back to 20° as fast as possible
			servoTrajectory.setTargetPos(20);
			break;

		case 2:
			// Next move to 180°, but over the course of 5 seconds
			servoTrajectory.setTargetPos(180, 5);
			break;

		case 3:
			// Finally back to 20°, taking 8.5 seconds
			servoTrajectory.setTargetPos(20, 8.5);
			break;

		default:
			// If all other moves have completed, stop the program
			Serial.println("All moves completed");
			while(1) {}
	}

	moveNumber++;
}


/* * * * * * * * * * * * * * * * * * * * * * *
 * LOOP
 * * * * * * * * * * * * * * * * * * * * * * */
void loop() {

	// Update the servo position at regular intervals
	if (millis() - updateTimer >= UPDATE_TIME) {
		updateTimer += UPDATE_TIME;

		// Update the controller
		float currentAngle = servoTrajectory.update();

		// Set the new servo position; the function only takes integer numbers
		myservo.write(round(currentAngle));

		/**
		 * For more precise servo control, you could use writeMicroseconds.
		 * The min and max PWM pulse widths which correspond to the 0° and 180°
		 * positions needs to be inserted for MIN_PWM and MAX_PWM.
		 */
		//myservo.writeMicroseconds(map(currentAngle, 0, 180, MIN_PWM, MAX_PWM));

		// Output the target position, along with the current position and velocity
		Serial.print("Target: ");
		Serial.print(servoTrajectory.getTarget());
		Serial.print(", Angle: ");
		Serial.print(servoTrajectory.getPos());
		Serial.print(", Velocity: ");
		Serial.println(servoTrajectory.getVel());

		// Only once the servo has reached the desired position, complete the next move
		if (servoTrajectory.ready()) {
			nextMove();
		}
	}
}
