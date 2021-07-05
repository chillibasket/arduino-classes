/* * * * * * * * * * * * * * * * * * * * * * *
 * Timer-based Debounce Class Header File
 *
 * @file      TimeDebounce.hpp
 * @brief     Uses a timer to debounce the button state
 * @author    Simon Bluett
 * @website   https://wired.chillibasket.com/
 *
 * @copyright Copyright (C) 2021 - MIT License
 * @date      6th February 2021
 * @version   1.1
 *
 * Based on the Debounce Class by wkoch:
 * https://github.com/wkoch/Debounce
 * * * * * * * * * * * * * * * * * * * * * * */

#ifndef TIME_DEBOUNCE_HPP
#define TIME_DEBOUNCE_HPP

// Required for millis()
#include <Arduino.h>


// Struct used to pack boolean bits
struct TimeDebounceFlags {
	unsigned currentState   :  1;
	unsigned changeDetected :  1;
	unsigned waiting        :  1;
	unsigned changeCounter  : 13;
};


/**
 * @class TimeDebounce
 */
class TimeDebounce {
public:
	// Constructor and destructor
	TimeDebounce(uint8_t pin, uint8_t delay = 50);
	~TimeDebounce();

	// Initialise the button
	void begin(bool enablePullup = true);

	// Update button reading
	bool update();

	// Functions to check for current state
	bool read();
	bool onChange();
	bool onRisingEdge();
	bool onFallingEdge();

	// Button press counter functions
	uint16_t count();
	void reset();

private:
	unsigned long debounceTimer;
	const uint8_t buttonPin;
	const uint16_t debounceDelay;
	struct TimeDebounceFlags flags;
};


/**
 * Constructor
 *
 * @param  pin    The I/O pin used for the button
 * @param  delay  Time in milliseconds for the debounce (defaut = 50)
 */
TimeDebounce::TimeDebounce(uint8_t pin, uint8_t delay)
	: buttonPin(pin)
	, debounceDelay(delay)
{
	// Empty, as all initialisation is done in the begin() function
}


/**
 * Default Destructor
 */
TimeDebounce::~TimeDebounce() {
	// Empty - no dynamic memory used
}


/**
 * Initialise the button pins
 *
 * @param enablePullup  Whether to enable the internal pullup (default = true)
 */
void TimeDebounce::begin(bool enablePullup) {
	
	// Setup the pin
	pinMode(buttonPin, (enablePullup)? INPUT_PULLUP : INPUT);

	// Initialise other variables
	const bool readState = digitalRead(buttonPin);
	debounceTimer = 0;
	flags.currentState = readState;
	flags.waiting = false;
	flags.changeCounter = 0;
	flags.changeDetected = false;
}


/**
 * Update Button State
 *
 * @return Current state after applying debounce
 */
bool TimeDebounce::update() {

	const bool readState = digitalRead(buttonPin);

	// For each change of state
	if (readState != flags.currentState) {

		// Start the debounce timer
		if (!flags.waiting) {
			debounceTimer = millis();
			flags.waiting = true;

		// If reading stayed the same during debounce, update the state
		} else if (millis() - debounceTimer > debounceDelay) {
			flags.currentState = readState;
			flags.changeDetected = true;
			flags.changeCounter++;
			flags.waiting = false;
		}

	// If debounce was active but state reverted, reset the timer
	} else {
		flags.waiting = false;
	}

	return flags.currentState;
}


/**
 * Read the current button state
 *
 * @return Debounced button State
 */
bool TimeDebounce::read() {
	return flags.currentState;
}


/**
 * Check if the button state has changed
 *
 * @return True on rising/falling edge, false otherwise
 */
bool TimeDebounce::onChange() {
	if (flags.changeDetected) {
		flags.changeDetected = false;
		return true;
	}
	return false;
}


/**
 * Check for a rising edge
 *
 * @return True if rising edge detected
 */
bool TimeDebounce::onRisingEdge() {
	if (flags.currentState && flags.changeDetected) {
		flags.changeDetected = false;
		return true;
	}
	return false;
}


/**
 * Check for a falling edge
 *
 * @return True if falling edge detected
 */
bool TimeDebounce::onFallingEdge() {
	if (!flags.currentState && flags.changeDetected) {
		flags.changeDetected = false;
		return true;
	}
	return false;
}


/**
 * Count how many button clicks have occurred
 *
 * @return The number of full button press & releases
 */
uint16_t TimeDebounce::count() {
	// Counter records presses and releases, so divide by 2
	return flags.changeCounter / 2;
}


/**
 * Reset the button click counter
 */
void TimeDebounce::reset() {
	// Set counter to 1 if button is currently pressed, 0 otherwise
	flags.changeCounter = !flags.currentState;
}

#endif /* TIME_DEBOUNCE_HPP */