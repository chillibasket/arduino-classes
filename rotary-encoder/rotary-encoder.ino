/* * * * * * * * * * * * * * * * * * * * * * *
 * Arduino Rotary Encoder Example Sketch
 *
 * @file     rotary-encoder.ino
 * @brief    Demonstrate the use of the RotaryEncoder class
 * @author   Simon Bluett
 * @website  https://wired.chillibasket.com/
 *
 * @license  Copyright (C) 2020 - MIT License
 * @date     13th December 2020
 * @version  0.1
 *
 * This sketch is still a work in progress!
 * * * * * * * * * * * * * * * * * * * * * * */

#include "RotaryEncoder.hpp"

RotaryEncoder myEncoder(2, 3, false);
int16_t lastCount = 0;


void setup() {
	Serial.begin(115200);
	while(!Serial);

	myEncoder.begin(true, []{myEncoder.interruptSignal();});

	Serial.println("--- Rotary Encoder Test Code ---");
}



void loop() {
	if (lastCount != myEncoder.getCount()) {
		lastCount = myEncoder.getCount();
		//Serial.println(lastCount);
	}
}