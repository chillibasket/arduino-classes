/* * * * * * * * * * * * * * * * * * * * * * *
 * Arduino Rotary Encoder Example Sketch
 *
 * @file     rotary-encoder.ino
 * @brief    Demonstrate the use of the RotaryEncoder class
 * @author   Simon Bluett
 * @website  https://wired.chillibasket.com/
 *
 * @copyright Copyright (C) 2020 - MIT License
 * @date      16th January 2021
 * @version   0.2
 *
 * This sketch is still a work in progress!
 * * * * * * * * * * * * * * * * * * * * * * */

// Include the class library
#include "RotaryEncoder.hpp"

/**
 * Define a new encoder object
 * @param SignalPinA = 2
 * @param SignalPinB = 3
 * @param EnablePullups = false
 * @param InterruptType = sig_ab_change
 * @param ScalingFactor = 2
 */
RotaryEncoder myEncoder(2, 3, false, sig_ab_change, 2);


int16_t lastCount = 0;

/**
 * Setup
 */
void setup() {
	Serial.begin(115200);
	while(!Serial);

	// Begin the econder, and specify the function which should run 
	// each time the interrupt is triggered
	myEncoder.begin([]{myEncoder.interruptSignal();});

	Serial.println("--- Rotary Encoder Test Code ---");
}


/**
 * Main Program Loop
 */
void loop() {
	if (lastCount != myEncoder.getCount()) {
		lastCount = myEncoder.getCountDebug();
		Serial.println(lastCount);
	}
}