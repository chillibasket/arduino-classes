/* * * * * * * * * * * * * * * * * * * * * * *
 * Rotary Encoder Input Class Header File
 *
 * @file      RotaryEncoder.hpp
 * @brief     Keep a count of rotary encoder steps
 * @author    Simon Bluett
 * @website   https://wired.chillibasket.com/
 *
 * @copyright Copyright (C) 2020 - MIT License
 * @date      16th January 2021
 * @version   0.2
 *
 * This class is still a work in progress!
 * * * * * * * * * * * * * * * * * * * * * * */

#ifndef ROTARY_ENCODER_HPP
#define ROTARY_ENCODER_HPP

#include <Arduino.h>

/**
 * Define the valid interrupt trigger types
 */
enum trigger_t {
	sig_a_rising = 0,
	sig_a_change = 1,
	sig_ab_change = 2
};


/**
 * @class RotaryEncoder
 */
class RotaryEncoder {
public:
	// Constructor and destructor
	RotaryEncoder(uint8_t pinA, uint8_t pinB, bool pullup = false, enum trigger_t interruptType = sig_ab_change, uint8_t scaling = 3);
	~RotaryEncoder();

	// Update button reading
	int16_t update();
	void begin(void (interruptFunction)(void));

	// Click counter functions
	void reset();
	int16_t getCount();
	int16_t getCountDebug();
	int16_t getCountAndReset();
	uint16_t isrCounter;

	void interruptSignal();

private:

	uint8_t scaleFactor;
	volatile int16_t outputCounter;
	volatile int16_t stepCounter;

	volatile bool lastStateA;
	volatile bool lastStateB;

	const uint8_t signalPinA;
	const uint8_t signalPinB;
	const bool enablePullups;
	const enum trigger_t triggerType;
};


/**
 * Constructor
 *
 * @param  pinA          The I/O pin used for encoder count A
 * @param  pinB          The I/O pin used for encoder count B
 * @param  pullup        Whether to enable the internal pullups [default=false]
 * @param  interruptType Specify on which edges to trigger an interrupt:
 *                       [sig_a_rising] [sig_a_change] [default=sig_ab_change]
 * @param  scaling       Scale output depending on # of pulses per step [default=3]
 *
 * @note Using interrupts on both pins provides the highest resolution
 *       angle measurement, however uses up two interrupt pins. For 
 *       precise motor position control using encoders, both interrupts
 *       should be used. If micro-steps are not required (eg. a rotary 
 *       encorder switch), only interruptA needs to be used. 
 */
RotaryEncoder::RotaryEncoder(uint8_t pinA, uint8_t pinB, bool pullup, enum trigger_t interruptType, uint8_t scaling)
	: signalPinA(pinA)
	, signalPinB(pinB)
	, enablePullups(pullup)
	, triggerType(interruptType)
{
	// Validate the scale factor
	if (triggerType < scaling) scaleFactor = triggerType;
	else scaleFactor = scaling;

	stepCounter = 0;
	outputCounter = 0;
	isrCounter = 0;
}


/**
 * Default Destructor
 */
RotaryEncoder::~RotaryEncoder() {
	// Empty
}


/**
 * Initialise the pins and interrupts
 *
 * @param interruptFunction  The function to be called when an interrupt is triggered
 *                           This should always be: []{objectName.interruptSignal();}
 *                           If left blank, no interrupts will be used.
 */
void RotaryEncoder::begin(void (interruptFunction)(void)) {

	pinMode(signalPinA, (enablePullups)? INPUT_PULLUP : INPUT);
	pinMode(signalPinB, (enablePullups)? INPUT_PULLUP : INPUT);

	lastStateA = digitalRead(signalPinA);
	lastStateB = digitalRead(signalPinB);

	// Default case = no interrupts
	switch (triggerType) {
		case sig_a_rising:
			attachInterrupt(digitalPinToInterrupt(signalPinA), interruptFunction, RISING);
			break;
		case sig_ab_change:
			attachInterrupt(digitalPinToInterrupt(signalPinB), interruptFunction, CHANGE);
		case sig_a_change:
			attachInterrupt(digitalPinToInterrupt(signalPinA), interruptFunction, CHANGE);
			break;
	}
}


/**
 * Manually update the encoder state (polling) 
 *
 * @note   This should only be used if no interrupts are enabled
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
 * Count how many button steps have occurred
 *
 * @return The number of steps
 */
int16_t RotaryEncoder::getCountDebug() {
	cli();
	uint16_t currentCount = outputCounter;	
	Serial.print(lastStateA); Serial.print(",");
	Serial.print(lastStateB); Serial.print(" - int:");
	Serial.print(stepCounter); Serial.print(" - out:");
	Serial.print(outputCounter); Serial.print(" - isr:");
	Serial.print(isrCounter); Serial.print(" - ");
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

	// Truth Table (CHANGE A and B detection code)
	// Last A |  New B |  New A | Last B | Step Value
	// 0    0 |      0 |      0 |      0 |   0 (no change)
	// 1    0 |      1 |      0 |      0 |  -1
	// 2    0 |      0 |      0 |      1 |  +1
	// 3    0 |      1 |      0 |      1 |   0 (no change)
	// 4    0 |      0 |      1 |      0 |  +1
	// 5    0 |      1 |      1 |      0 |   0 (invalid)
	// 6    0 |      0 |      1 |      1 |   0 (invalid)
	// 7    0 |      1 |      1 |      1 |  -1
	// 8    1 |      0 |      0 |      0 |  -1
	// 9    1 |      1 |      0 |      0 |   0 (invalid)
	// 10   1 |      0 |      0 |      1 |   0 (invalid)
	// 11   1 |      1 |      0 |      1 |  +1
	// 12   1 |      0 |      1 |      0 |   0 (no change)
	// 13   1 |      1 |      1 |      0 |  +1
	// 14   1 |      0 |      1 |      1 |  -1
	// 15   1 |      1 |      1 |      1 |   0 (no change)

	bool signalA = digitalRead(signalPinA);
	bool signalB = digitalRead(signalPinB);

	switch (triggerType) {
		// Signal A RISING detection mode
		case sig_a_rising:
			stepCounter += (lastStateA & signalA & !signalB) - (lastStateA & signalA & signalB);
			break;

		// Signal A CHANGE detection mode
		case sig_a_change:
			stepCounter += (signalA ^ signalB) - (lastStateA ^ signalB);
			break;

		// Signal A and Signal B CHANGE detection mode
		default:
		case sig_ab_change:
			stepCounter += (lastStateB ^ signalA) - (lastStateA ^ signalB);
			break;
	}

	// Scale the output with hysteresis
	if ((stepCounter & ((1 << scaleFactor) - 1)) == 0) {
		outputCounter = stepCounter >> scaleFactor;
	}

	isrCounter++;
	lastStateA = signalA;
	lastStateB = signalB;
}


#endif /* ROTARY_ENCODER_HPP */