/* * * * * * * * * * * * * * * * * * * * * * *
 * Bitstream-based Debounce Class Header File
 *
 * @file      BitDebounce.hpp
 * @brief     Uses 8-bit history to debounce the button state
 * @author    Simon Bluett
 * @website   https://wired.chillibasket.com/
 *
 * @copyright Copyright (C) 2021 - MIT License
 * @date      6th February 2021
 * @version   1.1
 *
 * Based on the Ultimate Debouncer described by Elliot Williams:
 * https://hackaday.com/2015/12/10/embed-with-elliot-debounce-your-noisy-buttons-part-ii/
 * * * * * * * * * * * * * * * * * * * * * * */

#ifndef BIT_DEBOUNCE_HPP
#define BIT_DEBOUNCE_HPP

/* To turn off the click counter, comment out the next line */
#define BIT_DEBOUNCE_COUNTER


// Struct used to pack boolean bits
struct BitDebounceFlags {
	unsigned currentState   :  1;
	unsigned changeDetected :  1;
	#ifdef BIT_DEBOUNCE_COUNTER
		unsigned changeCounter  : 14;
	#endif /* BIT_DEBOUNCE_COUNTER */
};


/**
 * @class BitDebounce
 */
class BitDebounce {
public:
	// Constructor and destructor
	BitDebounce(uint8_t pin);
	~BitDebounce();

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
	#ifdef BIT_DEBOUNCE_COUNTER
		void reset();
		uint16_t count();
	#endif /* BIT_DEBOUNCE_COUNTER */

private:
	struct BitDebounceFlags flags;
	uint8_t buttonHistory;
	const uint8_t buttonPin;
};


/**
 * Constructor
 *
 * @param  pin The I/O pin used for the button
 */
BitDebounce::BitDebounce(uint8_t pin)
	: buttonPin(pin)
{
	// Empty, as all initialisation is done in the begin() function
}


/**
 * Default Destructor
 */
BitDebounce::~BitDebounce() {
	// Empty
}


/**
 * Initialise the button pins
 *
 * @param enablePullup  Whether to enable the internal pullup (default = true)
 */
void BitDebounce::begin(bool enablePullup) {
	
	// Setup the pin
	pinMode(buttonPin, (enablePullup)? INPUT_PULLUP : INPUT);

	// Initialise other variables
	const bool readState = digitalRead(buttonPin);
	flags.currentState = readState;
	flags.changeDetected = false;
	#ifdef BIT_DEBOUNCE_COUNTER
		flags.changeCounter = 0;
	#endif /* BIT_DEBOUNCE_COUNTER */
	if (readState) buttonHistory = 0b11111111;
	else buttonHistory = 0;
}


/**
 * Update the button state
 *
 * @return Debounced button state
 */
bool BitDebounce::update() {

	const bool readState = digitalRead(buttonPin);

	buttonHistory = (buttonHistory << 1);
	buttonHistory |= readState;

	if (flags.currentState != readState) {
		if (flags.currentState) {
			if ((buttonHistory & 0b11000111) == 0b11000000 || buttonHistory == 0b00000000) {
				flags.currentState = false;
				flags.changeDetected = true;
				#ifdef BIT_DEBOUNCE_COUNTER
					flags.changeCounter++;
				#endif /* BIT_DEBOUNCE_COUNTER */
			}
		} else {
			if ((buttonHistory & 0b11000111) == 0b00000111 || buttonHistory == 0b11111111) {
				flags.currentState = true;
				flags.changeDetected = true;
				#ifdef BIT_DEBOUNCE_COUNTER
					flags.changeCounter++;
				#endif /* BIT_DEBOUNCE_COUNTER */
			}
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


#ifdef BIT_DEBOUNCE_COUNTER
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
	flags.changeCounter = !flags.currentState;
}
#endif /* BIT_DEBOUNCE_COUNTER */

#endif /* BIT_DEBOUNCE_HPP */