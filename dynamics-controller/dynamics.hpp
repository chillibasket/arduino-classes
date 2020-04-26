/* * * * * * * * * * * * * * * * * * * * * * *
 * DYNAMICS CONTROLLER CLASS HEADER
 *
 * Code by: Simon Bluett
 * Email:   hello@chillibasket.com
 * Version: 1.1
 * Date: 26th April 2020
 * * * * * * * * * * * * * * * * * * * * * * */

#ifndef DYNAMICS_HPP
#define DYNAMICS_HPP

#include <Arduino.h>

// DYNAMICS CLASS
class Dynamics {

public:
	// Constructors
	Dynamics(int _type = 0, float _maxVel = 100, float _setAcc = 50, float _setDec = -1, float _thresh = 0.5);

	// Functions
	// Set target position/velocity
	void setTargetPos(float _target, float _scale = 1);
	void setTargetVel(float _target);

	// Get the current target position or velocity values
	float getTargetPos() { return targetPos; };
	float getTargetVel() { return targetVel; };

	// Set max velocity, acceleration or decelleration
	void setMaxVel(float _maxVel) { maxVel = _maxVel; };
	void setAcc(float _acc) { acc = _acc; };
	void setDec(float _dec) { dec = _dec; };

	// Get the current max velocity, accel, decel or scale values 
	float getMaxVel() { return maxVel; };
	float getAcc() { return acc; };
	float getDec() { return dec; };
	float getScale() { return scale; };

	// Reset the controller
	void reset(float newPos = 0);
	void resetPos(float newPos = 0);

	// Check if the controller has reached the desired position
	bool ready();

	// Update and get the new position value
	// dT = time since last call in milliseconds
	float update(float dT);
	float update();

	// Default destructor
	~Dynamics(); 

private:
	// type = 0: "position" control (Straight line with parabolic blends)
	// type = 1: "velocity" control (Straight line with parabolic blends)
	int type;          // Dynamics system type

	float threshold;   // Position error threshold below which no motion occurs 
	float targetPos;   // Target Position
	float targetVel;   // Target Velocity
	float curPos;      // Current Position
	float curVel;      // Current Velocity
	float maxVel;      // Maximum Velocity
	float acc;         // Constant Acceleraction
	float dec;         // Constant Decelleration
	float scale;       // Scale factor - used to synchonise motors around curves
	unsigned long oldTime;
	bool noTasks;
};

#endif /* DYNAMICS_HPP */