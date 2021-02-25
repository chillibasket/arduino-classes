/* * * * * * * * * * * * * * * * * * * * * * *
 * Example Code for Queue Class
 *
 * Code by:  Simon Bluett
 * Email:    hello@chillibasket.com
 * Version:  1.1
 * Date:     20th June 2020
 * Copyright (C) 2020, MIT License
 * * * * * * * * * * * * * * * * * * * * * * */

// Include the queue class
#include "Queue.hpp"


// An example data structure
struct Receipt {
	int transactionID;
	int numberOfItems;
	unsigned long time;
	float finalPrice;
};


/**
 * Instantiate a queue for this data structure,
 * with a capacity of 30 items
 */
Queue <Receipt> queue(30);


/**
 * If this code is being used on a microcontroller, we can
 * define a global buffer to avoid dynamic memory allocation
 */
//Receipt buffer[30];
//Queue <Receipt> queue(30, buffer);


/**
 * Setup function
 */
void setup() {
	// Start serial communication
	Serial.begin(9600);
	while(!Serial);
	Serial.println("");

	// Check if the queue class was not able to allocate sufficient
	// space in memory, then stop the sketch early
	if (queue.errors()) {
		Serial.println("Error: queue was unable to allocate space in memeory");
		while(1);
	}

	// Otherwise, let's start the test
	Serial.println("--- Starting Test ---");
	addToQueue();
}


/**
 * Add new items to the queue
 */
void addToQueue() {

	// Try adding 40 items onto the queue
	for (int i = 0; i < 40; i++) {
		// Create a new item
		Receipt newTransaction;
		newTransaction.transactionID = i;
		newTransaction.numberOfItems = i*2;
		newTransaction.time = i*3;
		newTransaction.finalPrice = i*4;

		// Push the item onto the queue
		queue.push(newTransaction);
	}

	// Check the size of the queue
	Serial.print("Queue Size: ");
	Serial.println(queue.size());

	// See what the ID of the item at the front of the queue is
	Serial.print("Item ID at Front: ");
	Serial.println(queue.front().transactionID);

	// And at the back
	Serial.print("Item ID at Back: ");
	Serial.println(queue.back().transactionID);

	// See item at index 5
	Serial.print("Item at index 5: ");
	Serial.println(queue.get(5).transactionID);

	// Remove item from index 5
	queue.remove(5);

	// Check the new size of the queue
	Serial.print("Queue Size: ");
	Serial.println(queue.size());

	// See the new item at index 5
	Serial.print("New item at index 5: ");
	Serial.println(queue.get(5).transactionID);
}


/**
 * Main program loop
 */
void loop() {

	// Keep poping items off the front queue until it is empty
	if (!queue.empty()) {
		
		Serial.print("Queue Size: "); Serial.print(queue.size());
		
		// To use as a First-in-first-out (FIFO) queue
		Receipt nextItem = queue.pop();

		// To use as a Last-in-first-out (LIFO) queue
		//Receipt nextItem = queue.pop_back();

		Serial.print(", Pop Item: ");
		Serial.print(nextItem.transactionID); Serial.print(", ");
		Serial.print(nextItem.numberOfItems); Serial.print(", ");
		Serial.print(nextItem.time); Serial.print(", ");
		Serial.println(nextItem.finalPrice);
	}
}
