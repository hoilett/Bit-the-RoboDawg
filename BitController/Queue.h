/*
* FILENAME:	Queue.h
* AUTHOR:	Orlando S. Hoilett
* EMAIL:	orlandohoilett@gmail.com
* VERSION:	0.0


* AFFILIATIONS
* Calvary Engineering Family Group, USA
*	A group of electronics and science enthusiasts who enjoy builing things


* DESCRIPTION
* This templatizes library provides use of a queue data structure with the
* Arduino environment since namespace std is not natively supported in Arduino.


* UPDATES
* Version 0.0
* 2014/05/02:1357>
*			Submitted code for a CS 201 Introduction to Data Structures class
*			assignment.
* 2014/12/14:0000>
*			Made a few modifications to the code to allow compatability with the
*			Arduino microcontroller	environment and IDE. These modifications
*			include changing "size_t" to "int" and removing the assignment opeartor.
* 2015/03/31:2039>
*			Added message to indicate to the user when he/she tries to peek when the
*			queue is empty.


* DISCLAIMER
* This code is in the public domain. Please feel free to use, modify, distribute,
* etc. as needed, but please give reference to original author as a courtesy to
* open source developing/-ers.
*
* If you find any bugs in the code, or have any questions, please feel free to
* contact me at orlandohoilett@gmail.com

*/


#ifndef QUEUE_H
#define QUEUE_H
#include <Arduino.h>
template<typename ItemType>

class Queue
{
private:

	struct Node
	{
		ItemType data;
		Node *next;
	};
	typedef Node* NodePtr;

	NodePtr myFront;
	NodePtr myBack;

	int mySize; //number of items on stack
	//struct Node
	//size_t mySize;
	// your job to define


public:
	// Class Constructor
	Queue();

	// Copy Constructor
	// pre:  Class object, aQueue, exists
	// post: Object is initialized to be a copy of the parameter
	Queue(const Queue<ItemType>& aQueue);

	// Class Destructor
	// Destroys a queue
	// pre:  Class object exists
	// post: Class object does not exist
	~Queue();

	// Assignment operator
	// Assigns a queue to another
	// pre: both class objects exist
	// post: this class object gets assigned a copy of the parameter class object
	const Queue<ItemType>& operator= (const Queue<ItemType>& rhs);

	// isEmpty
	// Checks if the queue is empty
	// pre:  A queue exists.
	// post: Returns true if it IS empty, false if NOT empty.
	bool isEmpty() const;

	// enqueue
	// enqueues an item to back of the queue.
	// pre:  Queue exists and item is passed.
	// post: adds the given item to the end of the queue.
	void enqueue(const ItemType& item);

	// dequeue
	// dequeues the front item off the queue 
	// pre:  Queue exists.
	// post: Removes item on front of the queue. If the queue
	//       was already empty, throws an std::underflow_error exception.
	void dequeue();

	// front
	// Returns the front item of the queue without dequeueing it.
	// pre:  Queue exists.
	// post: Returns item at front of queue.  If the queue is empty,
	//       throws an std::underflow_error exception.
	ItemType front() const;

	// size
	// Returns the number of items on the queue.
	int size() const;

};

// Class Constructor
// post: queue is created & initialized to be empty
template<typename ItemType>
Queue<ItemType>::Queue() :myFront(0), myBack(0), mySize(0)
{
}


// Copy Constructor
// pre: parameter object, rhs, exists
// post: queue is created to be a copy of the parameter queue
template<typename ItemType>
Queue<ItemType>::Queue(const Queue<ItemType>& aQueue) :
myFront(0), myBack(0), mySize(aQueue.mySize)
{
	//if (aQueue.mySize == 0) {
	//	myFront = new Node;
	//	myFront->data = aQueue.myFront->data;
	//}
	if (aQueue.myFront != 0) {
		//copy first node
		myFront = new Node;
		myFront->data = aQueue.myFront->data;

		//copy rest of list
		NodePtr newPtr = myFront;
		for (NodePtr origPtr = aQueue.myFront->next; origPtr != 0;
			origPtr = origPtr->next) {
			newPtr->next = new Node;
			newPtr = newPtr->next;
			newPtr->data = origPtr->data;
		}
		newPtr->next = 0; //end of list
		myBack = newPtr;
		myBack->next = newPtr->next;
	} //end of statement
} //end of copy constructor



// Class Deconstructor
// pre: the queue exists
// post: the queue is destroyed and any dynamic memory is returned to the system
template<typename ItemType>
Queue<ItemType>::~Queue()
{
	while (!isEmpty()) {
		dequeue();
	}
}



// Assignment operator
// Assigns a queue to another
// pre: both class objects exist
// post: this class object gets assigned a copy of the parameter class object
//template <typename ItemType>
//const Queue<ItemType>& Queue<ItemType>::operator= (const Queue<ItemType>& rhs)
//{
//	if (this != &rhs) {
//		//Queue<ItemType> tmp(rhs);
//		//std::swap(mySize, tmp.mySize);
//		//std::swap(myFront, tmp.myFront);
//		//std::swap(myBack, tmp.myBack);
//		Queue<ItemType> tmp(rhs);
//		swap(mySize, tmp.mySize);
//		swap(myFront, tmp.myFront);
//		swap(myBack, tmp.myBack);
//	}
//	return *this;
//}


// isEmpty
// Checks if the queue is empty
// pre:  A queue exists.
// post: Returns true if it IS empty, false if NOT empty.
template<typename ItemType>
bool Queue<ItemType>::isEmpty() const
{
	return mySize == 0;
}


// enqueue
// enqueues an item to back of the queue.
// pre:  Queue<ItemType> exists and item is passed.
// post: adds the given item to the end of the queue.
template<typename ItemType>
void Queue<ItemType>::enqueue(const ItemType& item)
{
	//create new node
	NodePtr p;
	p = new Node;
	p->data = item;
	p->next = 0;

	//if the list is empty, add new node
	if (myFront == 0) {
		myFront = p;
		myBack = p;
	}

	//if not add to the back of queue
	else {
		myBack->next = p;
		myBack = p;
	}

	mySize++;
}


// dequeue
// dequeues the front item off the queue 
// pre:  Queue<ItemType> exists.
// post: Removes item on front of the queue. If the queue
//       was already empty, throws an std::underflow_error exception.
template<typename ItemType>
void Queue<ItemType>::dequeue()
{
	//TODO fix this so that it only checks if queue is empty once and not on every call
	//if (mySize == 0) {
	//	throw std::underflow_error("queue is already empty");
	//}
	//else {
		NodePtr tmp = myFront;
		myFront = myFront->next;
		delete tmp;
		mySize--;
		if (myFront == 0) {
		//	NodePtr tmp2 = myBack;
			myBack = 0;
		//	delete tmp2;
		}
	//}
}


// front
// Returns the front item of the queue without dequeueing it.
// pre:  Queue<ItemType> exists.
// post: Returns item at front of queue.  If the queue is empty,
//       throws an std::underflow_error exception.
template<typename ItemType>
ItemType Queue<ItemType>::front() const
{
	//if (mySize == 0) {
	//	throw std::underflow_error("queue is empty");
	//}
	//else {
	if (mySize == 0)
	{
		Serial.println("Queue is empty");
		return NULL;
	}
	else
	{
		return myFront->data;
	}
}


// size
// Returns the number of items on the queue.
// post: Returns size from the private section of class.
template<typename ItemType>
int Queue<ItemType>::size() const
{
	return mySize;
}


//#include "Queue.cpp"

#endif