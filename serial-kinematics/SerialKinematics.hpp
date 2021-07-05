/* * * * * * * * * * * * * * * * * * * * * * *
 * Forward Kinematics for Serial Manipulators
 *
 * @file      SerialKinematics.hpp
 * @brief     Calculate end effector position from joints angles
 * @author    Simon Bluett
 * @website   https://wired.chillibasket.com/
 *
 * @copyright Copyright (C) 2021 - MIT License
 * @date      5th July 2021
 * @version   0.1
 *
 * @note      This class is still a work in progress!
 * @note      Requires the following library to be installed:
 *            "BasicLinearAlgebra" by Tom Stewart
 *            <https://github.com/tomstewart89/BasicLinearAlgebra>
 * * * * * * * * * * * * * * * * * * * * * * */

#ifndef SERIAL_KINEMATICS_HPP
#define SERIAL_KINEMATICS_HPP

// Matrix Library
#include <BasicLinearAlgebra.h>

//using namespace BLA;

struct DhRow {
	float a;
	float alpha;
	float d;
	float omega;
	//bool  distal;
};


/**
 * @class SerialKinematics
 */
class SerialKinematics {
public:
	SerialKinematics(struct DhRow *dhParams, uint8_t dhLength);
	~SerialKinematics();

	void proximalDH(BLA::Matrix<4,4> &tMatrix, struct DhRow params);
	void distalDH(BLA::Matrix<4,4> &tMatrix, struct DhRow params);

	void forwardKinematics(BLA::Matrix<4,4> &tMatrix, uint8_t joints);
	void forwardKinematics(BLA::Matrix<4,4> &tMatrix);
	bool setParameter(float value, uint8_t dhRow, uint8_t dhCol);
	float getParameter(uint8_t dhRow, uint8_t dhCol);

	void tmatrix2coordinates(float (&coordinates)[3], const BLA::Matrix<4,4> &tMatrix);
	void tmatrix2euler(float (&eulerAngles)[3], const BLA::Matrix<4,4> &tMatrix);

private:
	struct DhRow *dhParamList;
	const uint8_t dhListLength;
};


/**
 * Default constructor
 *
 * @param[in] *dhParams Pointer to list containing the Denavit Hartenberg parameters
 * @param[in] dhLength  The number of items (rows) in the DH parameter list
 */
SerialKinematics::SerialKinematics(struct DhRow *dhParams, uint8_t dhLength)
	: dhParamList(dhParams)
	, dhListLength(dhLength)
{
	
}


/**
 * Default Destructor
 */
SerialKinematics::~SerialKinematics() {
	// Empty
}


/**
 * Denavit Hartenberg transformation matrix using Proximal convention
 *
 * @param[out] tMatrix The matrix into which the result should be saved
 * @param[in]  params  DH parameters for current joint (a, alpha, d, omega)
 */
void SerialKinematics::proximalDH(BLA::Matrix<4,4> &tMatrix, struct DhRow params) {
	float cosOmega = cos(params.omega);
	float cosAlpha = cos(params.alpha);
	float sinOmega = sin(params.omega);
	float sinAlpha = sin(params.alpha);

	// Calculate the proximal transformation matrix
	tMatrix(0,0) = cosOmega;
	tMatrix(0,1) = -sinOmega;
	tMatrix(0,2) = 0;
	tMatrix(0,3) = params.a;
	tMatrix(1,0) = sinOmega * cosAlpha;
	tMatrix(1,1) = cosOmega * cosAlpha;
	tMatrix(1,2) = -sinAlpha;
	tMatrix(1,3) = -sinAlpha * params.d;
	tMatrix(2,0) = sinOmega * sinAlpha;
	tMatrix(2,1) = cosOmega * sinAlpha;
	tMatrix(2,2) = cosAlpha;
	tMatrix(2,3) = cosAlpha * params.d;
	tMatrix(3,0) = 0;
	tMatrix(3,1) = 0;
	tMatrix(3,2) = 0;
	tMatrix(3,3) = 1;
}


/**
 * Denavit Hartenberg transformation matrix using Distal convention
 *
 * @param[out] tMatrix The matrix into which the result should be saved
 * @param[in]  params  DH parameters for current joint (a, alpha, d, omega)
 */
void SerialKinematics::distalDH(BLA::Matrix<4,4> &tMatrix, struct DhRow params) {
	float cosOmega = cos(params.omega);
	float cosAlpha = cos(params.alpha);
	float sinOmega = sin(params.omega);
	float sinAlpha = sin(params.alpha);

	// Calculate the distal transformation matrix
	tMatrix(0,0) = cosOmega;
	tMatrix(0,1) = -cosAlpha * sinOmega;
	tMatrix(0,2) = sinAlpha * sinOmega;
	tMatrix(0,3) = params.a * cosOmega;
	tMatrix(1,0) = sinOmega;
	tMatrix(1,1) = cosAlpha * cosOmega;
	tMatrix(1,2) = -sinAlpha * cosOmega;
	tMatrix(1,3) = params.a * sinOmega;
	tMatrix(2,0) = 0;
	tMatrix(2,1) = sinAlpha;
	tMatrix(2,2) = cosAlpha;
	tMatrix(2,3) = params.d;
	tMatrix(3,0) = 0;
	tMatrix(3,1) = 0;
	tMatrix(3,2) = 0;
	tMatrix(3,3) = 1;
}


/**
 * Calculate forward kinematrics
 *
 * This functions calculates the Denavit Hartenberg transformation
 * matrix for each joint of the manipulator and multiplies them
 * together to get the resultant end-effector angle.
 *
 * @param[out] tMatrix The matrix into which the result should be saved
 * @param[in]  joint   The joint for which to calculate the transformation matrix
 *                     If left blank it is assumed to be the end effector
 * 
 * @note  This currently only uses the distal convention
 *        You can switch to proximal by replacing "distalDH" with "proximalDH"
 */
void SerialKinematics::forwardKinematics(BLA::Matrix<4,4> &tMatrix, uint8_t joints) {

	// Calculate transformation matrix for the first joint
	distalDH(tMatrix, dhParamList[0]);

	// Calculate and multiply transformations of all subsequent joints
	for (int i = 1; i < joints; i++) {
		BLA::Matrix<4,4> tMatrixTemp;
		distalDH(tMatrixTemp, dhParamList[i]);
		tMatrix *= tMatrixTemp;
	}
}

void SerialKinematics::forwardKinematics(BLA::Matrix<4,4> &tMatrix) {
	forwardKinematics(tMatrix, dhListLength);
}


/**
 * Get XYZ coordinates from a transformation matrix
 *
 * @param[out] coordinates The X, Y and Z coordinates
 * @param[in]  tMatrix     The 4x4 transformation matrix
 */
void SerialKinematics::tmatrix2coordinates(float (&coordinates)[3], const BLA::Matrix<4,4> &tMatrix) {

    coordinates[0] = tMatrix(0,3); // X
    coordinates[1] = tMatrix(1,3); // Y
    coordinates[2] = tMatrix(2,3); // Z
}


/**
 * Get euler angles from a transformation matrix
 *
 * @param(out) eulerAngles The euler angles
 * @param(in)  tMatrix     The 4x4 transformation matrix
 * 
 * @note  This function still needs to be properly tested
 */
void SerialKinematics::tmatrix2euler(float (&eulerAngles)[3], const BLA::Matrix<4,4> &tMatrix) {

	// Calculate Euler angles using ZYX convention
	if (tMatrix(2,0) != 1 && tMatrix(2,0) != -1) {
		eulerAngles[1] = -asin(tMatrix(2,0));
		eulerAngles[2] = atan2(tMatrix(2,1) / cos(eulerAngles[1]), tMatrix(2,2) / cos(eulerAngles[1]));
		eulerAngles[0] = atan2(tMatrix(1,0) / cos(eulerAngles[1]), tMatrix(0,0) / cos(eulerAngles[1]));
	} else {
		eulerAngles[2] = 0;
		if (tMatrix(2,0) == -1) {
			eulerAngles[1] = PI / 2.0;
			eulerAngles[0] = atan2(tMatrix(0,1), tMatrix(0,2));
		} else {
			eulerAngles[1] = -PI / 2.0;
			eulerAngles[0] = atan2(-tMatrix(0,1), -tMatrix(0,2));
		}
	}

	// Calculate Euler angles using ZYX convention
	// If r31 is not 1, then sin(r31) only has one value:
	/*
	if (tMatrix(2,0) < 1) {
		// case 1: if r31 ~= ±1
		if (tMatrix(2,0) > -1) {
			// Solution with positive sign. It limits the range of the values
			// of theta_y to (-pi/2, pi/2):
			eulerAngles[0] = atan2(tMatrix(1,0), tMatrix(0,0));
			eulerAngles[1] = asin(-tMatrix(2,0));
			eulerAngles[2] = atan2(tMatrix(2,1), tMatrix(2,2));
		// case 2: if r31 = -1
		} else {
			// theta_x and theta_z are linked --> Gimbal lock:
			// There are infinity number of solutions for theta_x - theta_z = atan2(-r23, r22).
			// To find a solution, set theta_x = 0 by convention.
			eulerAngles[0] = -atan2(-tMatrix(1,2), tMatrix(1,1));
			eulerAngles[1] = PI / 2.0;
			eulerAngles[2] = 0;
		}
	// case 3: if r31 = 1, then sin(r31) has two possible values
	} else {
		// Gimbal lock: There is not a unique solution for
		// theta_x + theta_z = atan2(-r23, r22), by convention, set theta_x = 0.
		eulerAngles[0] = atan2(-tMatrix(1,2), tMatrix(1,1));
		eulerAngles[1] = PI / 2.0;
		eulerAngles[2] = 0;
	}*/

	/*
    // Calculate Euler angles using ZYZ convention
	if (tMatrix(2,2) < 1) {

		// Solution with positive sign, i.e. theta_y is in the range (0, pi):
		if (tMatrix(2,2) > -1) {
			eulerAngles[0] = atan2(tMatrix(1,2), tMatrix(0,2));
			eulerAngles[1] = acos(tMatrix(2,2));
			eulerAngles[2] = atan2(tMatrix(2,1), -tMatrix(2,0));

		// if r33 = -1:
		} else {
			// Gimbal lock: infinity number of solutions for
			// theta_z2 - theta_z1 = atan2(r21, r22), --> set theta_z2 = 0.
			eulerAngles[0] = -atan2(tMatrix(1,0), tMatrix(1,1));
			eulerAngles[1] = PI;
			eulerAngles[2] = 0;
		}

	// if r33 = 1:
	} else {
		// Gimbal lock: infinity number of solutions for
		// theta_z2 + theta_z1 = atan2(r21, r22), --> set theta_z2 = 0.
		eulerAngles[0] = atan2(tMatrix(1,0), tMatrix(1,1));
		eulerAngles[1] = 0;
		eulerAngles[2] = 0;
	}
	*/
}


/**
 * Change the value of a Denavit Hartenberg parameter
 *
 * For example, this function would be used to change
 * the current value of one of the joints.
 *
 * @param  value The new value of the parameter
 * @param  dhRow The DH row in which the parameter is stored
 * @param  dhCol The DH column/item which should be updated
 * @return True if update was successful, false otherwise
 */
bool SerialKinematics::setParameter(float value, uint8_t dhRow, uint8_t dhCol) {

	// Validate inputs - check parameter exists in the list
	if (dhRow >= dhListLength || dhCol >= 4) {
		return false;
	}

	switch(dhCol) {
		case 0: dhParamList[dhRow].a = value;     break;
		case 1: dhParamList[dhRow].alpha = value; break;
		case 2: dhParamList[dhRow].d = value;     break;
		case 3: dhParamList[dhRow].omega = value; break;
		default: return false;
	}

	return true;
}


/**
 * Get the value of a Denavit Hartenberg parameter
 *
 * @param  dhRow The DH row in which the parameter is stored
 * @param  dhCol The DH column/item which should be retrieved
 * @return The current value of the parameter
 */
float SerialKinematics::getParameter(uint8_t dhRow, uint8_t dhCol) {

	// Validate inputs - check parameter exists in the list
	if (dhRow >= dhListLength || dhCol >= 4) {
		return 0;
	}

	switch(dhCol) {
		case 0: return dhParamList[dhRow].a;
		case 1: return dhParamList[dhRow].alpha;
		case 2: return dhParamList[dhRow].d;
		case 3: return dhParamList[dhRow].omega;
		default: return 0;
	}
}


#endif /* SERIAL_KINEMATICS_HPP */