// GrocerySim.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <vector>
#include <string>



int main(int argc, char* argv[])
{
	static int minutes = 360;// temp untill I get
	GroceryLine* ExpressLane = new ExpressLine("Express Line");
	GroceryLine* NormalLane = new NormalLine("Normal Lane");

	std::cout << "Hello World!\n" << argc;
    if (argc == 0)
    {
        std::cout << "This program simulats a grocery store line and takes argument for the number of minutes being simulated";
    }

	ExpressLane->nextCustomerNumber = 1;
	std::string print;
	for (int i = 0; i < minutes; i++) {
		if (i % (24 * 60 * 60) == 0) { // checks if 24 hours have passed
			ExpressLane->nextCustomerNumber = 1; // resets customer number to 1
		}
		Customer* customer = NULL;
		customer = ExpressLane->MinutePasses(i);
		if (customer != NULL) { // if a customer left got checked out
			print += printCustomerInfo(customer);
		}
		customer = NULL;
		customer = ExpressLane->MinutePasses(i);
		if (customer != NULL) {
			print += printCustomerInfo(customer);
		}
		std::cout << print;

	}
}

std::string printCustomerInfo(Customer* customer) {
	std::string print;
	print += "Customer " + customer->customerNumber;
	print += " Arrived at Minute " + customer->arrivalTime;
	print += " and Left at " + customer->finishTime;
	print += " and Spent " + customer->timeElapsed;
	print += " Minutes in line";
	print += " and Checked Out With, ";
	print += groceryList(customer) + " " +'\n';
	return print;
}

/*std::string SimulateMinute(int minute, Queue* express, Queue* normal) {
	if (minute == 0) {// first minute program runs
		express->customerProcessTime = std::rand() % 5 + 1; // set values for how long each customer takes
		normal->customerProcessTime = std::rand() % 6 + 3;
		Customer* cn = new Customer(minute); // create customers for each line
		Customer* ce = new Customer(minute);
	}
	else {
		
	}
	express->customerProcessTime--;
	express->customerProcessTime--;
}*/

std::string groceryList(Customer* cur){
	std::string list = "";
	for (int i = 0; i < cur->groceries.size(); i++){
		list += cur->groceries[i];
		list += " ";
	}
	return list;
}

class GroceryLine : Queue {

public:
	static int nextCustomerNumber;
	int customerProcessTime;
	int nextCustomerInterval;
	std::string LineName;
	bool empty;

	Customer* MinutePasses(int minute) { // this is called on each Queue for each minute that passes
		this->customerProcessTime--;
		this->nextCustomerInterval--;
		if (nextCustomerInterval == 0) {
			if (QueueSize == 0) {// if there was no one in line process time needs to be set
				customerProcessTime = IntervalAndProcessTime();
			}
			this->Enqueue(new Customer(minute, nextCustomerNumber));
			nextCustomerNumber++;
			nextCustomerInterval = IntervalAndProcessTime();
		}
		if (customerProcessTime == 0) { // if the current customer is done being checked-out
			Customer* cust = Dequeue();
			if (this->QueueSize != 0) {// if there is someone else in line reset process time
				this->customerProcessTime = this->IntervalAndProcessTime();
			}
			cust->finishTime = minute;
			cust->timeElapsed = cust->finishTime - cust->arrivalTime;
			return cust;
		}
	}

	int IntervalAndProcessTime() { 
		return 0; 
	};

	std::string serialize(int minute) {
		std::string serialize = this->LineName + "\n";
		if (this->QueueSize == 0) {
			serialize += "No customers in this line.\n";
		}
		else {
			// to-do impliment recursive queue serialization
		}
	}
};

class ExpressLine : public GroceryLine {
public:
	ExpressLine(std::string name) {
		this->LineName = name;
		this->nextCustomerInterval = this->IntervalAndProcessTime();
	}
	int IntervalAndProcessTime() {
		return std::rand() % 5 + 1;
	}
};

class NormalLine : public GroceryLine {
public:
	NormalLine(std::string name) {
		this->LineName = name;
		this->nextCustomerInterval = this->IntervalAndProcessTime();
	}
	int IntervalAndProcessTime() {
		return std::rand() % 6 + 3;
	}
};

class Queue
{
private:
	QueueNode* Tail;
	QueueNode* Head;

public:
	int QueueSize = 0;

	Queue(){
		Tail = NULL;
		Head = NULL;
		QueueSize = 0;
	}

	void Enqueue(Customer* customer)
	{
		
		if (Head == NULL){
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
	Customer* Dequeue(){
		QueueSize--;
		if (Head == NULL)
			return NULL;
		else {
			QueueNode* Node;// node being dequeued
			Node = Head; // sets it to head of line
			Head = Node->behind; // sets new head to node behind
			Head->front = NULL; // sets new head front node to null
			return Node->data; // returns customer of dequeued head
		}
	}
};
class QueueNode
{
public:
	Customer* data;
	QueueNode* front;
	QueueNode* behind;
	QueueNode(Customer* customer, QueueNode* forward = NULL){
		data = customer; // assigns QueueNode customer pointer to Customer Pointer passed in
		front = forward;// assigns inFront pointer to passed object pointer
		behind = NULL; // when enqueing a new node there is never a node behind
	}
};

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

	Customer(int time = 0, int num) {
		arrivalTime = time;
		customerNumber = num;
		int numItems = std::rand() % 8 + 1;
		for (int i = 0; i < numItems; i++) {
			groceries.push_back(Groceries[std::rand() % size]);
		}
	}
};
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
