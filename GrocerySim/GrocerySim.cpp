// GrocerySim.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <vector>
#include <string>
#include <ctime>

int IntervalAndProcessTime(bool);

static int nextCustomerNumber;

class Customer
{
private:
	static const int size = 14;
	const std::string Groceries[size] = { "Beans", "Ice Cream", "Ramen", "Rice", "Brown Rice", "Soup", "Crackers", "Cookies", "Chips", "Milk", "Cereal", "Salami", "Apples", "Grapes" };// groceries to be chosen from

public:
	std::vector<std::string> groceries;
	int timeElapsed;
	int arrivalTime;
	int finishTime;
	int customerNumber;

	Customer(int time = 0, int num = 1) {
		timeElapsed = 0;
		finishTime = 0;
		arrivalTime = time;
		customerNumber = num;
		int numItems = std::rand() % 8 + 1;
		for (int i = 0; i < numItems; i++) {
			groceries.push_back(Groceries[std::rand() % size]);
		}
	}
};

class QueueNode
{
public:
	Customer* data;
	QueueNode* front;
	QueueNode* behind;
	QueueNode(Customer* customer, QueueNode* forward = NULL) {
		data = customer; // assigns QueueNode customer pointer to Customer Pointer passed in
		front = forward;// assigns inFront pointer to passed object pointer
		behind = NULL; // when enqueing a new node there is never a node behind
	}
};

class Queue
{
private:
	QueueNode* Tail;
	QueueNode* Head;

public:
	int QueueSize = 0;

	Queue() {
		Tail = NULL;
		Head = NULL;
		QueueSize = 0;
	}

	void Enqueue(Customer* customer){
		if (Head == NULL) {
			QueueNode* Node = new QueueNode(customer);
			Head = Node;
			Tail = Node;
		}
		else {
			QueueNode* Node = new QueueNode(customer, Tail);
			Tail->behind = Node;
		}
		QueueSize++;
	}
	Customer* Dequeue() {
		QueueSize--;
		if (Head == NULL)
			return NULL;
		else {
			QueueNode* Node;// node being dequeued
			Node = Head; // sets it to head of line
			Head = Node->behind; // sets new head to node behind
			if(Head!=NULL)
				Head->front = NULL; // sets new head front node to null
			return Node->data; // returns customer of dequeued head
		}
	}
};

class GroceryLine : public Queue {
public:
	int nextCustomerInterval;
	int customerProcessTime;
	std::string LineName;

	GroceryLine(std::string name) {
		LineName = name;
		customerProcessTime = 0;
		nextCustomerInterval = 0;
	}

	Customer* MinutePasses(int minute, bool express){ // this is called on each Queue for each minute that passes
		
		if (minute == 0) {
			customerProcessTime = IntervalAndProcessTime(express);
		}
		if (nextCustomerInterval == 0) {
			this->Enqueue(new Customer(minute, nextCustomerNumber));
			nextCustomerNumber++;
			nextCustomerInterval = IntervalAndProcessTime(express);
		}
		else
			nextCustomerInterval--;

		if (minute == 0) {
			return NULL;
		}
		if (customerProcessTime == 0) { // if the current customer is done being checked-out
			Customer* cust = Dequeue();
			this->customerProcessTime = IntervalAndProcessTime(express);
			cust->finishTime = minute;
			cust->timeElapsed = cust->finishTime - cust->arrivalTime;
			return cust;
		}
		else
			customerProcessTime--;
		return NULL;
	}

	std::string serialize(int minute) {
		std::string serialize = this->LineName + "\n";
		if (this->QueueSize == 0) {
			serialize += "No customers in this line.\n";
		}
		else {
			// to-do impliment recursive queue serialization
			return "will impliment later serialized at " + std::to_string(minute);
		}
	}
};

std::string printCustomerInfo(Customer*);
std::string groceryList(Customer*);

int main(int argc, char* argv[])
{
	static int minutes = 360;// temp untill I get user input setup
	GroceryLine* ExpressLane = new GroceryLine("Express Line");
	GroceryLine* NormalLane = new GroceryLine("Normal line");

	nextCustomerNumber = 1;
	std::string print;
	for (int i = 0; i < minutes; i++) {
		if (i % (24 * 60 * 60) == 0 && i > 0) { // checks if 24 hours have passed
			nextCustomerNumber = 1; // resets customer number to 1

		}
		Customer* customer = NULL;
		customer = NormalLane->MinutePasses(i, false);
		if (customer != NULL) { // if a customer left got checked out
			print += printCustomerInfo(customer);
		}
		customer = NULL;
		customer = ExpressLane->MinutePasses(i, true);
		if (customer != NULL) {// if a customer got checked out form the express line
			print += printCustomerInfo(customer);
		}
		if (minutes % 600 == 0) {
			print += ExpressLane->serialize(i);
			print += "\n";
			print += NormalLane->serialize(i);
			print += "\n";
		}
		std::cout << print;

	}
}

std::string printCustomerInfo(Customer* customer) {
	std::string print;
	print += "Customer " + std::to_string(customer->customerNumber);
	print += " Arrived at Minute " + std::to_string(customer->arrivalTime);
	print += " and Left at " + std::to_string(customer->finishTime);
	print += " and Spent " + std::to_string(customer->timeElapsed);
	print += " Minutes in line";
	print += " and Checked Out With, ";
	print += groceryList(customer) + " " + '\n';
	return print;
}

std::string groceryList(Customer* cur) {
	std::string list = "";
	for (int i = 0; i < cur->groceries.size(); i++) {
		list += cur->groceries[i];
		list += " ";
	}
	return list;
}

int IntervalAndProcessTime(bool express) {
	if (express) {
		return std::rand() % 5 + 1;
	}
	else {
		return std::rand() % 6 + 3;
 	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file