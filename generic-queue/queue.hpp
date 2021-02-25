/* * * * * * * * * * * * * * * * * * * * * * *
 * GENERIC QUEUE CLASS
 * @brief     Ring Buffer, for use as FIFO or LIFO queue
 *
 * @file      Queue.hpp
 * @author    Simon Bluett
 * @website   https://wired.chillibasket.com/
 *
 * @version   1.2
 * @date      7th August 2020
 * @copyright Copyright (C) 2020, MIT License
 * * * * * * * * * * * * * * * * * * * * * * */

#ifndef GENERIC_QUEUE_HPP
#define GENERIC_QUEUE_HPP


/**
 * Uncomment next line to allow size of queue to be dynamically increased
 */
//#define DYNAMIC_QUEUE_RESIZING


/**
 * Queue Class Definition
 */
template<class T>
class Queue {

public:
	/**
	 * Constructor
	 *
	 * @param  max     The maximum queue size
	 * @param  buffer  Pointer to the queue buffer (optional)
	 */
	Queue(int max = 50, T *buffer = NULL) {
		qFront = 0;
		qBack = 0;
		qSize = 0;
		maxSize = max;

		// Allocate data for the queue
		if (buffer == NULL) qData = new T[max];
		else qData = buffer;

		// If allocation has failed, set a warning flag
		if (qData == nullptr) warning = true;
		else warning = false;
	};


	/**
	 * Default Destructor
	 * Delete the queue buffer
	 */
	~Queue() {
		delete[] qData;
	};

	// Manipulation Functions
	void push(const T &item);
	T pop();
	T pop_back();
	T front();
	T peek();
	T back();
	T get(int itemIndex);

	// Management Functions
	bool empty();
	inline int size();
	void clear();
	void remove(int itemIndex);

	/**
	 * Return error if dynamic buffer allocation failed
	 */
	bool errors() { return warning; };

private:
	unsigned int qFront, qBack, qSize, maxSize;
	bool warning;
	T *qData;
};



/**
 * Get the size of the queue
 *
 * @return Current size of the queue
 */
template<class T> inline int Queue<T>::size() {
	return qSize;
}


/**
 * Add an item onto the back of the queue
 *
 * @param  Contents of the new item, passed by reference
 */
template<class T> void Queue<T>::push(const T &item) {
	// Check if there is space
	if(qSize < maxSize) {
		// Add item to the queue
		qData[qBack++] = item;
		qSize++;

		// Ensure circular buffer wraps around
		if (qBack >= maxSize) qBack -= maxSize;
	}

#ifdef DYNAMIC_QUEUE_RESIZING
	// If queue can dynamically allocate more memory, increase size of queue
	else {
		// Create a queue which is twice the size
		T *newQueue = new T[maxSize*2];
		qSize = 0;

		// Fill the new queue with the data from the old one
		do {
			newQueue[qSize++] = qData[qFront++];
			if (qFront >= maxSize) qFront -= maxSize;
		} while (qFront != qBack);

		// Update integer variables
		qFront = 0;
		qBack = qSize;
		maxSize = maxSize*2;

		// Delete the old queue and transfer the new one into its place
		delete[] qData;
		qData = newQueue;

		// Add the new item
		push(item);
	}
#endif /* DYNAMIC_QUEUE_RESIZING */
}


/**
 * Pop the oldest item off the from of the queue
 *
 * @return The item at the front of the queue
 */
template<class T> T Queue<T>::pop() {
	// If queue is empty, return an empty item
	if(qSize <= 0) return T();
	
	// Otherwise, retreive item from front of queue
	else {
		T result = qData[qFront];
		qFront++;
		qSize--;

		// Ensure circular buffer wraps around
		if (qFront >= maxSize) qFront -= maxSize;
		return result;
	} 
}


/**
 * Pop the newest item off the back of the queue
 *
 * @return The item at the back of the queue
 */
template<class T> T Queue<T>::pop_back() {
	// If queue is empty, return an empty item
	if(qSize <= 0) return T();
	
	// Otherwise, retreive item from back of queue
	else {
		int item = qBack - 1;
		if (item < 0) item += maxSize;

		T result = qData[item];
		qBack--;
		qSize--;

		// Ensure circular buffer wraps around
		if (qBack < 0) qBack += maxSize;
		return result;
	} 
}


/**
 * Read the item at the front, but leave it in place
 *
 * @return The item at the front of the queue
 */
template<class T> T Queue<T>::front() {
	if(qSize <= 0) return T();
	else return qData[qFront];
}

// "peek" is the same as "front"
template<class T> T Queue<T>::peek() {
	return front();
}


/**
 * Read the item at the back, but leave it in place
 *
 * @return The item at the back of the queue
 */
template<class T> T Queue<T>::back() {
	if (qSize <= 0) return T();
	else {
		int item = qBack - 1;
		if (item < 0) item += maxSize;
		return qData[item];
	}
}


/**
 * Get the value of an item at a specific index in the queue
 *
 * @param  itemIndex The index number of the item of interest
 */
template<class T> T Queue<T>::get(int itemIndex) {
	
	// Check if index is valid
	if (itemIndex > 0 && itemIndex < qSize) {
		return qData[(qFront + itemIndex) % maxSize];
	}
	return T();
}


/**
 * Remove an item from the queue
 *
 * @param  itemIndex The index number of the item to be removed
 */
template<class T> void Queue<T>::remove(int itemIndex) {
	
	// Check if index is valid
	if (itemIndex > 0 && itemIndex < qSize) {

		// Move all subsequent items down by one space
		for (int i = itemIndex; i < qSize - 1; i++) {
			qData[(qFront + i) % maxSize] = qData[(qFront + i + 1) % maxSize];
		}

		// Remove the last item from the queue
		pop_back();
	}
}


/**
 * Check if the queue is empty
 * 
 * @return True if empty, false if there are still items
 */
template<class T> bool Queue<T>::empty() {
	if (qSize <= 0) return true;
	return false;
}


/**
 * Clear all items from the queue
 */
template<class T> void Queue<T>::clear() {
	qFront = qBack;
	qSize = 0;
}

#endif /* GENERIC_QUEUE_HPP */