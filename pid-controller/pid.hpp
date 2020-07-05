/* * * * * * * * * * * * * * * * * * * * * * *
 * PID CONTROLLER CLASS HEADER
 *
 * Code by: Simon Bluett
 * Email:   hello@chillibasket.com
 * Version: 1.1
 * Date:    3rd July 2020
 * Copyright (C) 2020, MIT License
 * * * * * * * * * * * * * * * * * * * * * * */

#ifndef PID_HPP
#define PID_HPP

#include <Arduino.h>

// PID CONTROLLER CLASS DEFINITION
class PID {

public:
	// Constructor
	PID(float nP = 0.1, float nI = 0, float nD = 0, float maxVal = -1, float thresh = -1);

	// Update thresholds
	void setMax(float maxVal) { maxPID = maxVal; };
	void setThresh(float thresh) { threshold = thresh; };

	// Update gain terms
	void setGain(float nP, float nI, float nD);
	void setKp(float nP) { Kp = nP; };
	void setKi(float nI) { Ki = nI; };
	void setKd(float nD) { Kd = nD; };

	// Get current gain terms
	float getKp() { return Kp; };
	float getKi() { return Ki; };
	float getKd() { return Kd; };

	// Get current thresholds
	float getMax() { return maxPID; };
	float getThresh() { return threshold; };

	// Reset and update functions
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
