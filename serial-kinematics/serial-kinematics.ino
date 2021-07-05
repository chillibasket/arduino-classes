 /* * * * * * * * * * * * * * * * * * * * * * *
 * Example Code for Forward Kinematics Library
 *
 * @file     serial-kinamtics.ino
 * @brief    Demonstrate the use of the Kinematics library
 * @author   Simon Bluett
 * @website  https://wired.chillibasket.com/
 *
 * @copyright Copyright (C) 2021 - MIT License
 * @date      5th July 2021
 * @version   0.1
 *
 * This sketch is still a work in progress! I will
 * publish a detailed tutorial regarding this library
 * once I have finished developing and testing it.
 *
 * @note      Requires the following library to be installed:
 *            "BasicLinearAlgebra" by Tom Stewart
 *            <https://github.com/tomstewart89/BasicLinearAlgebra>
 * * * * * * * * * * * * * * * * * * * * * * */

#include "SerialKinematics.hpp"


// Define the robot manipulator arm
// Denavit Hartenberg Parameters    a,   alpha,    d,   omega
struct DhRow dhParameters[5] = {{   0, -PI/2.0,  4.5, -PI/2.0},
                                { 9.5,       0,    0, -PI/2.0},
                                {  11,       0,    0,  PI/2.0},
                                {   0, -PI/2.0,    0,       0},
                                {   0,       0, 10.5,       0}};

// Supply the DH parameter list to the kinematics object
SerialKinematics kinematics(dhParameters, 5);


int currentTest = 0;


/**
 * Setup Function
 */
void setup() {
	Serial.begin(115200);
	while(!Serial);

	testScript();
}


/**
 * Perform Tests
 */
void testScript() {

	// Set the joint positions for each test
	switch(currentTest) {
		case 0:
			break;
		case 1:
			kinematics.setParameter(  -PI/4.0, 0, 3);
			kinematics.setParameter(  -PI/5.0, 1, 3);
			kinematics.setParameter(   PI/2.0, 2, 3);
			kinematics.setParameter(-4*PI/5.0, 3, 3);
			kinematics.setParameter(        0, 4, 3);
			break;

		case 2:
			kinematics.setParameter(  -PI/4.0, 0, 3);
			kinematics.setParameter(  -1.1694, 1, 3);
			kinematics.setParameter(   1.4661, 2, 3);
			kinematics.setParameter(  -1.8675, 3, 3);
			kinematics.setParameter(        0, 4, 3);
			break;

		case 3:
			kinematics.setParameter(   1.2217, 0, 3);
			kinematics.setParameter(  -2.5307, 1, 3);
			kinematics.setParameter(   2.5307, 2, 3);
			kinematics.setParameter(  -PI/2.0, 3, 3);
			kinematics.setParameter(        0, 4, 3);
			break;

		case 4:
			kinematics.setParameter(   1.2217, 0, 3);
			kinematics.setParameter(  -2.0944, 1, 3);
			kinematics.setParameter(   2.0944, 2, 3);
			kinematics.setParameter(  -0.8203, 3, 3);
			kinematics.setParameter(  -PI/2.0, 4, 3);
			break;

		default:
			while(1);
	}

	// Initialise data arrays
	BLA::Matrix<4,4> tmatrix;
	float coordinates[3] { 0 };
	float eulerangles[3] { 0 };

	// Start timer
	unsigned long stopwatch = micros();

	// Calculate overall transformation matrix using forward kinematics
	kinematics.forwardKinematics(tmatrix);

	// Convert the matrix to the coordinates and orientation of the end effector
	kinematics.tmatrix2coordinates(coordinates, tmatrix);
	kinematics.tmatrix2euler(eulerangles, tmatrix);

	// End the timer
	stopwatch = micros() - stopwatch;

	// Output the result
	Serial.print("Test-");
	Serial.print(currentTest); Serial.print(": ");
	Serial.print(coordinates[0]); Serial.print(" x, \t");
	Serial.print(coordinates[1]); Serial.print(" y, \t");
	Serial.print(coordinates[2]); Serial.print(" z, \t");
	Serial.print((eulerangles[0] * 4068) / 71.0); Serial.print(" y, \t");
	Serial.print((eulerangles[1] * 4068) / 71.0); Serial.print(" p, \t");
	Serial.print((eulerangles[2] * 4068) / 71.0); Serial.print(" r, \t");
	Serial.println(stopwatch);
}


/**
 * Main Program Loop
 */
void loop() {
	testScript();
	currentTest++;
}