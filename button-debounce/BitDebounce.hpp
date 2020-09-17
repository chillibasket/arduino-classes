/* * * * * * * * * * * * * * * * * * * * * * *
 * Bitstream-based Debounce Class Header File
 *
 * @file     BitDebounce.hpp
 * @brief    Uses 8-bit history to debounce the button state
 * @author   Simon Bluett
 * @website  https://wired.chillibasket.com/
 *
 * @license  Copyright (C) 2020 - MIT License
 * @date     16th September 2020
 * @version  1.0
 *
 * Based on the Ultimate Debouncer described by Elliot Williams:
 * https://hackaday.com/2015/12/10/embed-with-elliot-debounce-your-noisy-buttons-part-ii/
 * * * * * * * * * * * * * * * * * * * * * * */

#ifndef BIT_DEBOUNCE_HPP
#define BIT_DEBOUNCE_HPP


// Struct used to pack boolean bits
struct BitDebounceFlags {
	unsigned currentState   :  1;
	unsigned changeDetected :  1;
	unsigned changeCounter  : 14;
};


/**
 * @class BitDebounce
 */
class BitDebounce {
public:
	// Constructor and destructor
	BitDebounce(uint8_t pin, bool pullup);
	~BitDebounce();

	// Update button reading
	bool update();

	// Functions to check for current state
	bool read();
	bool onChange();
	bool onRisingEdge();
	bool onFallingEdge();

	// Button press counter functions
	void reset();
	uint16_t count();

private:
	struct BitDebounceFlags flags;
	uint8_t buttonHistory;
	const uint8_t buttonPin;
};


/**
 * Default Constructor
 *
 * @param  pin The I/O pin used for the button
 */
BitDebounce::BitDebounce(uint8_t pin, bool pullup = true)
	: buttonPin(pin)
{
	pinMode(buttonPin, (pullup)? INPUT_PULLUP : INPUT);
	const bool readState = digitalRead(buttonPin);
	flags.currentState = readState;
	flags.changeDetected = false;
	flags.changeCounter = 0;
	if (readState) buttonHistory = 0b11111111;
	else buttonHistory = 0;
}


/**
 * Default Destructor
 */
BitDebounce::~BitDebounce() {
	// Empty
}


/**
 * Update the button state
 *
 * @return Debounced button state
 */
bool BitDebounce::update() {
	buttonHistory = (buttonHistory << 1);
	buttonHistory |= digitalRead(buttonPin);

	if (flags.currentState) {
		if ((buttonHistory & 0b11000111) == 0b11000000) {
			flags.currentState = false;
			flags.changeDetected = true;
			flags.changeCounter++;
		}
	} else {
		if ((buttonHistory & 0b11000111) == 0b00000111) {
			flags.currentState = true;
			flags.changeDetected = true;
			flags.changeCounter++;
		}
	}

	return flags.currentState;
}


/**
 * Read the current button state
 *
 * @return Debounced button State
 */
bool BitDebounce::read() {
	return flags.currentState;
}


/**
 * Check if the button state has changed
 *
 * @return True on rising/falling edge, false otherwise
 */
bool BitDebounce::onChange() {
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
bool BitDebounce::onRisingEdge() {
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
bool BitDebounce::onFallingEdge() {
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
uint16_t BitDebounce::count() {
	// Counter records presses and releases, so divide by 2
	return flags.changeCounter / 2;
}


/**
 * Reset the button click counter
 */
void BitDebounce::reset() {
	// Set counter to 1 if button is currently pressed, 0 otherwise
	flags.changeCounter = flags.currentState;
}

#endif /* BIT_DEBOUNCE_HPP */