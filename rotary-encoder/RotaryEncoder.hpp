/* * * * * * * * * * * * * * * * * * * * * * *
 * Rotary Encoder Input Class Header File
 *
 * @file     RotaryEncoder.hpp
 * @brief    Keep a count of rotary encoder steps
 * @author   Simon Bluett
 * @website  https://wired.chillibasket.com/
 *
 * @license  Copyright (C) 2020 - MIT License
 * @date     13th December 2020
 * @version  0.1
 *
 * This class is still a work in progress!
 * * * * * * * * * * * * * * * * * * * * * * */

#ifndef ROTARY_ENCODER_HPP
#define ROTARY_ENCODER_HPP


/**
 * @class RotaryEncoder
 */
class RotaryEncoder {
public:
	// Constructor and destructor
	RotaryEncoder(uint8_t pinA, uint8_t pinB, bool pullup);
	~RotaryEncoder();

	// Update button reading
	int16_t update();
	void begin(boolean bothEdges, void (interruptFunction)(void));

	// Click counter functions
	void reset();
	int16_t getCount();
	int16_t getCountAndReset();

	void interruptSignal();

private:

	const uint8_t signalPinA;
	const uint8_t signalPinB;

	uint8_t scaleFactor;
	volatile int16_t outputCounter;
	volatile int16_t stepCounter;

	volatile bool lastStateA;
	volatile bool lastStateB;
	const bool enablePullups;
};


/**
 * Default Constructor
 *
 * @param  pinA          The I/O pin used for encoder count A
 * @param  pinB          The I/O pin used for encoder count B
 * @param  pullup        Whether to enable the internal pullups
 *
 * @note Using interrupts on both pins provides the highest resolution
 *       angle measurement, however uses up two interrupt pins. If 
 *       micro-steps are not required (eg. a rotary encorder switch), 
 *       only interruptA should be used. For precise motor position
 *       control using encoders, both interrupts should be used.
 */
RotaryEncoder::RotaryEncoder(uint8_t pinA, uint8_t pinB, bool pullup = false)
	: signalPinA(pinA)
	, signalPinB(pinB)
	, enablePullups(pullup)
{
	scaleFactor = 0;
	stepCounter = 0;
	outputCounter = 0;
}


/**
 * Default Destructor
 */
RotaryEncoder::~RotaryEncoder() {
	// Empty
}


void RotaryEncoder::begin(boolean bothEdges, void (interruptFunction)(void) = NULL) {
	pinMode(signalPinA, (enablePullups)? INPUT_PULLUP : INPUT);
	pinMode(signalPinB, (enablePullups)? INPUT_PULLUP : INPUT);

	lastStateA = digitalRead(signalPinA);
	lastStateB = digitalRead(signalPinB);

	stepCounter = 0;
	outputCounter = 0;

	if (interruptFunction != NULL) {
		scaleFactor = 2;
		attachInterrupt(digitalPinToInterrupt(signalPinA), interruptFunction, CHANGE);
		attachInterrupt(digitalPinToInterrupt(signalPinB), interruptFunction, CHANGE);
	}
}


/**
 * Update the encoder state 
 *
 * @return The current encoder step count
 */
int16_t RotaryEncoder::update() {
	interruptSignal();
	return outputCounter;
}


/**
 * Count how many button steps have occurred
 *
 * @return The number of steps
 */
int16_t RotaryEncoder::getCount() {
	cli();
	uint16_t currentCount = outputCounter;	
	sei();
	return currentCount;
}


/**
 * Count how many button steps have occurred and reset the counter
 *
 * @return The number of steps
 */
int16_t RotaryEncoder::getCountAndReset() {
	cli();
	uint16_t currentCount = outputCounter;
	outputCounter = 0;	
	stepCounter = 0;
	sei();
	return currentCount >> scaleFactor;
}


/**
 * Reset the rotary step counter
 */
void RotaryEncoder::reset() {
	cli();
	stepCounter = 0;
	outputCounter = 0;
	sei();
}


/**
 * Encoder interrupt service routine (ISR)
 */
void RotaryEncoder::interruptSignal() {

	//               ____      ____
	//  Signal A  __|    |____|    |____
	//                 ____      ____
	//  Signal B  ____|    |____|    |__

	// Truth Table (All edges detection code)
	// Last A |  New B |  New A | Last B | Step Value
	// 0    0 |      0 |      0 |      0 |   0 (invalid)
	// 1    0 |      1 |      0 |      0 |  -1
	// 2    0 |      0 |      0 |      1 |  +1
	// 3    0 |      1 |      0 |      1 |   0 (invalid)
	// 4    0 |      0 |      1 |      0 |  +1
	// 5    0 |      1 |      1 |      0 | invalid
	// 6    0 |      0 |      1 |      1 | invalid
	// 7    0 |      1 |      1 |      1 |  -1
	// 8    1 |      0 |      0 |      0 |  -1
	// 9    1 |      1 |      0 |      0 | invalid
	// 10   1 |      0 |      0 |      1 | invalid
	// 11   1 |      1 |      0 |      1 |  +1
	// 12   1 |      0 |      1 |      0 |   0 (invalid)
	// 13   1 |      1 |      1 |      0 |  +1
	// 14   1 |      0 |      1 |      1 |  -1
	// 15   1 |      1 |      1 |      1 |   0 (invalid)
	//
	// Truth Table (Rising edge only detection mode)
	// Last A |  New B |  New A | Last B | Step Value
	// 0    0 |      0 |      0 |      0 |   0 (invalid)
	// 1    0 |      1 |      0 |      0 |  -1 (invalid)
	// 2    0 |      0 |      0 |      1 |  +1 (invalid)
	// 3    0 |      1 |      0 |      1 |   0 (invalid)
	// 4    0 |      0 |      1 |      0 |  +1 (invalid)
	// 5    0 |      1 |      1 |      0 |  -1
	// 6    0 |      0 |      1 |      1 |  +1
	// 7    0 |      1 |      1 |      1 |  -1 (invalid)
	// 8    1 |      0 |      0 |      0 |  -1 (invalid)
	// 9    1 |      1 |      0 |      0 |  +1
	// 10   1 |      0 |      0 |      1 |  -1
	// 11   1 |      1 |      0 |      1 |  +1 (invalid)
	// 12   1 |      0 |      1 |      0 |   0 (invalid)
	// 13   1 |      1 |      1 |      0 |  +1 (invalid)
	// 14   1 |      0 |      1 |      1 |  -1 (invalid)
	// 15   1 |      1 |      1 |      1 |   0 (invalid)	

	bool signalA = digitalRead(signalPinA);
	bool signalB = digitalRead(signalPinB);

	// All edges detection mode
	stepCounter += (lastStateB ^ signalA) - (lastStateA ^ signalB);

	// Only signal A edges detection mode
	
	//if ((lastStateA ^ signalA) && (lastStateB ^ signalB)) {
	//	if (lastStateB ^ lastStateA) stepCounter++;
	//	else stepCounter--;
	//} else {
	//	Serial.println("Disregard");
	//}*/

	if ((stepCounter & ((1 << scaleFactor) - 1)) == 0) {
		outputCounter = stepCounter >> scaleFactor;
	}

	//Serial.print(lastStateA); Serial.print(",");
	//Serial.print(signalB); Serial.print(",");
	//Serial.print(lastStateB); Serial.print(",");
	//Serial.print(signalA); Serial.print(" - ");
	//Serial.print(stepCounter); Serial.print(" - ");
	//Serial.println(outputCounter);

	lastStateA = signalA;
	lastStateB = signalB;
}


#endif /* ROTARY_ENCODER_HPP */