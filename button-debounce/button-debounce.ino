/* * * * * * * * * * * * * * * * * * * * * * *
 * Button Debounce Classes Example Sketch
 *
 * @file     button-debounce.ino
 * @brief    Demonstrate how the button debounce classes can be used
 * @author   Simon Bluett
 * @website  https://wired.chillibasket.com/
 *
 * @license  Copyright (C) 2020 - MIT License
 * @date     16th September 2020
 * @version  1.0
 *
 * @see      <BitDebounce.hpp>
 * @see      <TimeDebounce.hpp>
 *
 * When using buttons, the signal being read at the digital
 * input pin of the Arduino needs to be "debounced" to remove
 * spurious signals during the transition which could be 
 * misinterpreted as multiple button clicks.
 *
 * This sketch demonstrates the use of two different button
 * debounce classes which I wrote to solve this problem:
 * <TimeDebounce.hpp> Uses a timer=based method
 * <BitDebounce.hpp>  Uses a more efficient bit-history method
 * * * * * * * * * * * * * * * * * * * * * * */

#include "BitDebounce.hpp"
#include "TimeDebounce.hpp"

/* Create button objects */
// (Pin, Use Internal Pullup)
BitDebounce  button2( 2, true );
// (Pin, Timer length (ms), Use Internal Pullup)
TimeDebounce button3( 3, 50, true );

unsigned long loopTimer = 0;


/**
 * Setup Code
 */
void setup() {
	// The pinMode() of the buttons is already set in the constructor 
	// of the debounce classes, so no need to do it again here...
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(9600);
}


/**
 * Main Program Loop
 */
void loop() {

	// Run this code every 10ms (100Hz)
	if (millis() - loopTimer >= 10) {
		loopTimer = millis();

		/* * * * * * * * * * * * * * * * * * * * * * * * *
		 * Bitstream-based debounce class <BitDebounce.hpp>
		 * * * * * * * * * * * * * * * * * * * * * * * * */
		button2.update();

		// Note: Using the onChange() function will reset the edge 
		// detection flags, so it cannot be used at the same time 
		// as the onRisingEdge() or onFallingEdge() functions. This 
		// is to prevent multiple detections of the same edge.

		// Check if a change (rising or falling edge) was detected
		if (button2.onChange()) {
			// Read the current button state
			Serial.print("BitDebounce - Button 2 - Changed: ");
			if (button2.read()) Serial.print("High - Count: ");

			// Read the current button count
			else Serial.print("Low - Count: ");
			Serial.println(button2.count());

			// Show HIGH/LOW state using built-in LED
			digitalWrite(LED_BUILTIN, button2.read());
		}


		/* * * * * * * * * * * * * * * * * * * * * * * * *
		 * Timer-based debounce class <TimeDebounce.hpp>
		 * * * * * * * * * * * * * * * * * * * * * * * * */
		button3.update();

		// Note: onRisingEdge() and onFallingEdge() will never both 
		// be true at the same time, so we can use both functions in
		// the same code.
 
		// Check if a rising edge was detected
		if (button3.onRisingEdge()) {
			Serial.print("TimeDebounce - Button 3 - Rising Edge Detected - Count: ");
			Serial.println(button3.count());
		}
		// Check if a falling edge was detected
		else if (button3.onFallingEdge()) {
			Serial.print("TimeDebounce - Button 3 - Falling Edge Detected - Count: ");
			Serial.println(button3.count());
		}
	}
}