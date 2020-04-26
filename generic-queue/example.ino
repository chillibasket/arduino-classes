/* * * * * * * * * * * * * * * * * * * * * * *
 * Example Code for Queue Class
 *
 * Code by:  Simon Bluett
 * Email:    hello@chillibasket.com
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

// Instantiate a queue for this data structure,
// with a capacity of 30 items
Queue <Receipt> queue(30);


/* SETUP */
void setup() {
	Serial.begin(9600);
	delay(5000);
	Serial.println("Starting Test");

	addToQueue();
}


/* ADD ITEMS TO QUEUE */
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

	// See what the ID of the item at the front of the queue is
	Serial.print("Item ID at Front: ");
	Serial.println(queue.front().transactionID);

	// And at the back
	Serial.print("Item ID at Back: ");
	Serial.println(queue.back().transactionID);
}


/* MAIN LOOP */
void loop() {

	// Empty the queue
	if (!queue.empty()) {
		
		Serial.print("Queue Size: "); Serial.print(queue.size());
		
		// To use as a FIFO queue
		Receipt nextItem = queue.pop();

		// To use as a LIFO queue
		//Receipt nextItem = queue.pop_back();

		Serial.print(", Pop Item: ");
		Serial.print(nextItem.transactionID); Serial.print(", ");
		Serial.print(nextItem.numberOfItems); Serial.print(", ");
		Serial.print(nextItem.time); Serial.print(", ");
		Serial.println(nextItem.finalPrice);
	}
}