/* * * * * * * * * * * * * * * * * * * * * * *
 * DYNAMICS CONTROLLER CLASS HEADER
 *
 * Code by: Simon B.
 * Email:   hello@chillibasket.com
 * * * * * * * * * * * * * * * * * * * * * * */

#ifndef DYNAMICS_HPP
#define DYNAMICS_HPP

#include <Arduino.h>

// DYNAMICS CLASS
class Dynamics {

public:
	// Constructors
	Dynamics();
	Dynamics(int _type);
	Dynamics(int _type, float _maxVel, float _setAcc);
	Dynamics(int _type, float _maxVel, float _setAcc, float _setDec);
	Dynamics(int _type, float _maxVel, float _setAcc, float _setDec, float _thresh);

	// Functions
	// Set target position/velocity
	void setTargetPos(float _target);
	void setTargetPos(float _target, float _scale);
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
	void reset();
	void resetPos();

	// Check if the controller has reached the desired position
	bool ready();

	// Update and get the new position value
	// dT = time since last call in milliseconds
	float updateVal(float dT);
	float updateVal();

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
