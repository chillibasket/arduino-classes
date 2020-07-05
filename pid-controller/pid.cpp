/* * * * * * * * * * * * * * * * * * * * * * *
 * PID CONTROLLER CLASS
 *
 * Code by: Simon Bluett
 * Email:   hello@chillibasket.com
 * Version: 1.1
 * Date:    3rd July 2020
 * Copyright (C) 2020, MIT License
 * * * * * * * * * * * * * * * * * * * * * * */

#include "pid.hpp"


/**
 * PID Default Constructor
 * 
 * @param  (nP) Proportional gain term, default = 0.1
 * @param  (nI) Integral gain term, default = 0
 * @param  (nD) Derivative gain term, default = 0
 * @param  (maxVal) Maximum PID output value, default = -1 (disabled)
 * @param  (thresh) Error cut-off threshold, default = -1 (disabled)
 */
PID::PID(float nP, float nI, float nD, float maxVal, float thresh) {
	Kp = nP;
	Ki = nI;
	Kd = nD;
	maxPID = maxVal;
	threshold = thresh;
	oldCurrent = 0;
	iTerm = 0;
	oldTime = millis();
}


/**
 * Default destructer
 */
PID::~PID(){

}


/**
 * Set new PID gain terms
 *
 * @param  (nP) New proportional gain
 * @param  (nI) New integral gain
 * @param  (nD) New derivative gain
 */
void PID::setGain(float nP, float nI, float nD) {
	Kp = nP;
	Ki = nI;
	Kd = nD;
}


/**
 * Reset the PID controller accumulators
 */
void PID::reset() {
	iTerm = 0;
	oldTime = millis();
}


/**
 * Calculate new PID controller output
 *
 * @param  (target) The target position of the system
 * @param  (current) The current position of the system
 * @param  (dT) Time since PID value was last updated
 * @return The new PID output value
 */
float PID::update(float target, float current, float dT) {

	// Calculate Error
	float error = target - current;
	float pid = 0;

	// If error is below a deadzone threshold
	if (threshold > 0 && abs(error) < threshold) {
		oldCurrent = current;
		iTerm = 0;

	// Otherwise calculate the PID output
	} else {
		// Calculate our PID terms
		iTerm += error * dT; 
		float dTerm = (current - oldCurrent) / dT;

		// Set old variable to equal new ones
		oldCurrent = current;

		// Obtain PID output value
		pid = (error * Kp) + (iTerm * Ki) - (dTerm * Kd);

		// Limit PID value to maximum values
		if (maxPID > 0) {
			if (pid > maxPID) pid = maxPID;
			else if (pid < -maxPID) pid = -maxPID;
		}
	}

	return pid;
}

// Overloaded "update" function, were the time change has not been specified
float PID::update(float target, float current) {

	// Calculate Time Change
	unsigned long newTime = millis();
	float dT = float(newTime - oldTime);
	oldTime = newTime;
	
	return updateVal(target, current, dT);
}
