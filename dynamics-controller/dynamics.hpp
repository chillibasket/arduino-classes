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
	void setTargetPos(float _target);
	void setTargetPos(float _target, float _scale);
	void setTargetVel(float _target);
	void setMaxVel(float _maxVel) { maxVel = _maxVel; };
	void setAcc(float _acc) { acc = _acc; };
	void setDec(float _dec) { dec = _dec; };
	//void setType(int _type);
	float getTargetPos() { return targetPos; };
	float getTargetVel() { return targetVel; };
	float getMaxVel() { return maxVel; };
	float getAcc() { return acc; };
	float getDec() { return dec; };
	float getScale() { return scale; };
	void reset();
	void resetCnts();
	bool ready();

	float updateVal(float dT);
	float updateVal();

	// Default destructor
	~Dynamics(); 

private:
	int type;          // Dynamics system type
	// type = 0: "position" control (Straight line with parabolic blends)
	// type = 1: "velocity" control (Straight line with parabolic blends)

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
