/* * * * * * * * * * * * * * * * * * * * * * *
 * EXAMPLE SKETCH FOR THE DYNAMICS CONTROLLER CLASS
 *
 * Code by: Simon Bluett
 * Email:   hello@chillibasket.com
 * Version: 1.0
 * Date:    26th April 2020
 *
 * In this example, we aim to move a servo motor from a 
 * position of 10°, to a final position of 180°. We want 
 * the maximum velocity of the servo to be 20°/second, 
 * the acceleration to be 10°/sec^2 and the deceleration 
 * to be 8°/sec. 
 *
 * An image showing the output servo position, velocity 
 * and acceleration is included with this example. 
 *
 * This example uses only the position control mode of the 
 * dynamics controller, so in the class instantiation, the 
 * variable "type" = 0.
 * * * * * * * * * * * * * * * * * * * * * * */



// --- Include the library ---
/* Make sure that the files "dynamics.cpp" and "dynamic.h"
 * are included in the same folder as the Arduino sketch */
#include "dynamics.hpp"
#include <Servo.h>


// --- Instantiate the class ---
/* If you want the acceleration and deceleration to be the same
 * FORMAT: Dynamics(type, max velocity, acceleration) */
// Dynamics dynoController(0, 20, 10);

/* If the acceleration and deceleration are different
 * FORMAT: Dynamics(type, max velocity, acceleration, deceleration) */
Dynamics dynoController(0, 20, 10, 8);

/* By default the dynamics controller turns off when it is within 0.5 units
 * of the target position. This threshold value can be changed in the declaration
 * FORMAT: Dynamics(type, max velocity, acceleration, deceleration, threshold) */
// Dynamics dynoController(0, 20, 10, 8, 0.1);

Servo myservo;


// --- Define global variables ---
#define UPDATE_FREQUENCY 100 		// The controller will be updated at a rate of 100Hz
unsigned long updateTimer = 0;



/* * * * * * * * * * * * * * * * * * * * * * *
 * SETUP
 * * * * * * * * * * * * * * * * * * * * * * */
void setup() {

	Serial.begin(9600);
	Serial.println("Starting program");

	myservo.attach(9);  // attaches the servo on pin 9 to the servo object

	// We want the servo to start at an angle of 10°
	myservo.write(10);

	// By default the controller starts at 0, so we need to
	// set the starting angle as well
	dynoController.reset(10);

	// Ok, no we want to move the servo to the 180° position
	dynoController.setTargetPos(180);

	/* If we suddenly decide we want to change the maximum velocity to 30°/s */
	//dynoController.setMaxVel(30);

	/* To change the acceleration to 15°/s^2 and deceleration to 5°/s^2 */
	//dynoController.setAcc(15);
	//dynoController.setDec(5);

	/* To read what the current velocity and acceleration settings are */
	//float currentVelocity = dynoController.getMaxVel();
	//float currentAcceleration = dynoController.getAcc();
	//float currentDeceleration = dynoController.getDec();
}


/* * * * * * * * * * * * * * * * * * * * * * *
 * LOOP
 * * * * * * * * * * * * * * * * * * * * * * */
void loop() {

	// Update the servo position at regular intervals
	if (updateTimer < millis()) {
		updateTimer = millis() + (1000 / UPDATE_FREQUENCY);

		// Update the controller
		float currentAngle = dynoController.update();

		// Set the new servo position
		myservo.write(int(currentAngle));

		Serial.print("Current servo angle: ");
		Serial.println(currentAngle);

		// If the servo has reached the desired position, stop the program
		if (dynoController.ready()) {
			Serial.println("Target position reached");
			while(1) {}
		}
	}
}