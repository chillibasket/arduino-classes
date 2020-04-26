/* * * * * * * * * * * * * * * * * * * * * * *
 * PID CONTROLLER CLASS HEADER
 *
 * Code by: Simon Bluett
 * Email:   hello@chillibasket.com
 * * * * * * * * * * * * * * * * * * * * * * */

#ifndef PID_HPP
#define PID_HPP

#include <Arduino.h>

// PID CONTROLLER CLASS DEFINITION
class PID {

public:
	// Constructors
	PID();
	PID(float nP, float nI, float nD);
	PID(float nP, float nI, float nD, float maxVal);
	PID(float nP, float nI, float nD, float maxVal, float thresh);

	// Functions
	void setMax(float maxVal) { maxPID = maxVal; };
	void setThresh(float thresh) { threshold = thresh; };
	void setConst(float nP, float nI, float nD);
	void setKp(float nP) { Kp = nP; };
	void setKi(float nI) { Ki = nI; };
	void setKd(float nD) { Kd = nD; };
	float getKp() { return Kp; };
	float getKi() { return Ki; };
	float getKd() { return Kd; };
	float getMax() { return maxPID; };
	float getThresh() { return threshold; };
	void  reset();
	float update(float target, float current, float dT);
	float update(float target, float current);

	// Default destructor
	~PID(); 

private:
	float Kp, Ki, Kd, maxPID, threshold;
	float oldCurrent, iTerm;
	unsigned long oldTime;
};

#endif /* PID_HPP */
