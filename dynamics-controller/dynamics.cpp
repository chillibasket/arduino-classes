/* * * * * * * * * * * * * * * * * * * * * * *
 * DYNAMICS CONTROLLER CLASS
 *
 * Code by: Simon B.
 * Email: 	hello@chillibasket.com
 * * * * * * * * * * * * * * * * * * * * * * */

#include "dynamics.hpp"

/*
 * \Func 	Dynamics()
 * \Desc 	Default constructor
 */
Dynamics::Dynamics(){
	Dynamics(0, 0, 0, 0, 0.5);
}

// System type specified
Dynamics::Dynamics(int _type) {
	Dynamics(_type, 0, 0, 0, 0.5);
}

// System type, max velocity and acceleration specified
Dynamics::Dynamics(int _type, float _maxVel, float _acc) {
	Dynamics(_type, _maxVel, _acc, _acc, 0.5);
}

// System type, max velocity, acceleration and deceleration specified
Dynamics::Dynamics(int _type, float _maxVel, float _acc, float _dec) {
	Dynamics(_type, _maxVel, _acc, _dec, 0.5);
}

// System type, max velocity, acceleration, deceleration and threshold specified
Dynamics::Dynamics(int _type, float _maxVel, float _acc, float _dec, float _thresh) {
	type = _type;
	targetPos = targetVel = curPos = curVel = 0;
	maxVel = _maxVel;
	acc = _acc;
	dec = _dec;
	scale = 1;
	oldTime = millis();
	threshold = _thresh;
	noTasks = true;
}


/*
 * \Func 	~Dynamics()
 * \Desc 	Default destructer
 */
Dynamics::~Dynamics() {

}


/*
 * \Func 	setTargetPos(float _targetPos, float _scale)
 * \Para 	(_targetPos) New target position value
 * \Para 	(_scale) New factor by which to scale the output position 
 */
void Dynamics::setTargetPos(float _targetPos, float _scale) {
	targetPos = _targetPos;
	scale = _scale;
	type = 0;
	curPos = 0;
	noTasks = false;
}

// Overload function; assume scale=1
void Dynamics::setTargetPos(float _target) {
	setTargetPos(_target, 1);
}


/*
 * \Func 	setTargetVel(float _targetVel)
 * \Para 	(_targetVel) New target velocity value
 */
void Dynamics::setTargetVel(float _targetVel) {
	// curPos remains unchanged
	targetVel = _targetVel;
	if (targetVel >  maxVel) targetVel =  maxVel;
	if (targetVel < -maxVel) targetVel = -maxVel;
	type = 1;
	scale = 1;
	noTasks = false;
}

/*
void Dynamics::setType(int _type) { 
	type = _type; 
	// To avoid sudden changes, set target to equal current parameters
	if (type == 0) target = curPos;
	else if (type == 1) target = curVel;
}
*/

/*
 * \Func 	update(double dT)
 * \Desc 	Update the current position according to the kinematic formulae
 * \Para 	(dT) The time change since function was last called
 * \Retu 	THe new position of the system
 */
float Dynamics::updateVal(float dT) {

	// Position Control (Linear velocity with parabolic blends)
	if (type == 0) {

		float posError = targetPos - curPos;

		// If position error is above the threshold
		if (abs(posError) > threshold) {

			// Determine motion direction
			bool dir = true;
			if (posError < 0) dir = false;

			// Determine whether to accelerate or decelerate
			float acceleration = acc;
			if ((0.5 * curVel * curVel / dec) > abs(posError)) acceleration = -dec;

			// Update the current velocity
			if (dir) curVel += acceleration * dT / 1000.0;
			else curVel -= acceleration * dT / 1000.0;

			// Limit Velocity
			if (curVel > maxVel) curVel = maxVel;
			if (curVel < -maxVel) curVel = -maxVel;
			
			float dP = curVel * dT / 1000.0;

			if (abs(dP) < abs(posError)) curPos += dP;
			else curPos = targetPos;

		} else {
			curVel = 0;
			noTasks = true;
		}

	// Velocity Control (Linear velocity with parabolic blends)
	} else if (type == 1) {

		float velError = targetVel - curVel;
		//Serial.print(targetVel); Serial.print("tar, ");
		//Serial.print(curVel); Serial.print("vel, ");
		//Serial.print(velError); Serial.print("err, ");

		// If velocity error is above the threshold
		if (abs(velError) > threshold) {

			// Determine whether to accelerate or decelerate
			float acceleration = acc;
			if (targetVel < curVel && curVel >= 0) acceleration = -dec;
			else if (targetVel < curVel && curVel < 0) acceleration = -acc; 
			else if (targetVel > curVel && curVel < 0) acceleration = dec;

			// Update the current velocity
			float dV = acceleration * dT / 1000.0;
			if (abs(dV) < abs(velError)) curVel += dV;
			else curVel = targetVel;
			//curVel = targetVel;
			//Serial.print(curVel); Serial.print("newVel, ");
		} else {
			curVel = targetVel;
		}
		
		// Limit Velocity
		if (curVel > maxVel) curVel = maxVel;
		if (curVel < -maxVel) curVel = -maxVel;

		//Serial.print(curPos); Serial.print("pos, ");
		// Update current position
		curPos += curVel * dT / 1000.0;

		//Serial.print(curPos); Serial.print("newPos, ");
		//Serial.print(curVel); Serial.print("newVel, ");
		//Serial.print(acc); Serial.print("acc, ");
		//Serial.print(dec); Serial.print("dec, ");
	}

	if (isnan(curPos)) {
		Serial.println("ERROR: invalid position number...");
		while(1) {
			analogWrite(9, 0);
			analogWrite(10, 0);
		}
	}

	return curPos * scale;
}

// Overloaded "update" function, were the time change has not been specified
float Dynamics::updateVal() {
	// Calculate Time Change
	unsigned long newTime = millis();
	double dT = double(newTime - oldTime);
	oldTime = newTime;
	return updateVal(dT);
}



/*
 * \Func 	void Dynamics::reset()
 * \Desc 	Reset all the runtime variables
 */
void Dynamics::reset() { 
	curPos = 0;
	curVel = 0;
	targetPos = 0;
	targetVel = 0;
	noTasks = true;
	oldTime = millis();
}


/*
 * \Func 	void Dynamics::resetCnts()
 * \Desc 	Reset the current position
 */
void Dynamics::resetCnts() {
	curPos = 0;
}


/*
 * \Func 	bool Dynamics::ready()
 * \Desc 	Check if system has completed its target move
 */
bool Dynamics::ready() { 
	if (noTasks) return true;
	else return false;
}
