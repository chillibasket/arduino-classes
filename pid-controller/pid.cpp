/* * * * * * * * * * * * * * * * * * * * * * *
 * PID CONTROLLER CLASS
 *
 * Code by: Simon B.
 * Email:   hello@chillibasket.com
 * * * * * * * * * * * * * * * * * * * * * * */

#include "pid.hpp"

/*
 * \Func 	PID::PID()
 * \Desc 	Default constructor
 */
PID::PID(){
	PID(0, 0, 0, -1, -1);
}

// Constructor with gains defined
PID::PID(float nP, float nI, float nD){
	PID(nP, nI, nD, -1, -1);
}

// Constructor with gains and max PID value defined
PID::PID(float nP, float nI, float nD, float maxVal) {
	PID(nP, nI, nD, maxVal, -1);
}

// Constructor with gains, max PID value, and cut-off threshold defined
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


/*
 * \Func 	~PID::PID()
 * \Desc 	Default destructer
 */
PID::~PID(){

}


/*
 * \Func 	void PID::setConst(float nP, float nI, float nD)
 * \Desc 	Update the PID constant gain terms
 * \Para 	(nP) New proportional gain
 * \Para 	(nI) New integral gain
 * \Para 	(nD) New derivative gain
 */
void PID::setConst(float nP, float nI, float nD) {
	Kp = nP;
	Ki = nI;
	Kd = nD;
}


/*
 * \Func 	void PID::reset()
 * \Desc 	Reset the PID controller accumulators
 */
void PID::reset() {
	oldCurrent = 0;
	iTerm = 0;
	oldTime = millis();
}


/*
 * \Func 	void PID::update(float target, float current, float dT)
 * \Desc 	Calculate a new PID output value
 * \Para 	(target) The target position of the system
 * \Para 	(current) The current position of the system
 * \Para 	(dT) Time since PID value was last updated
 * \Retu 	The new PID output value
 */
float PID::updateVal(float target, float current, float dT) {
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
		if (maxPID > 0 && pid > maxPID) pid = maxPID;
		else if (maxPID > 0 && pid < -maxPID) pid = -maxPID;
	}

	return pid;
}

// Overloaded "update" function, were the time change has not been specified
float PID::updateVal(float target, float current) {
	// Calculate Time Change
	// Using float to avoid integer division
	unsigned long newTime = millis();
	float dT = float(newTime - oldTime);
	oldTime = newTime;
	
	return updateVal(target, current, dT);
}
