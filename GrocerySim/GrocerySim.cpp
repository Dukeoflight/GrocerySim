// GrocerySim.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <array>

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
	std::string line;

	Customer(int time = 0, int num = 0) {
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
	QueueNode(Customer* customer) {
		data = customer; // assigns QueueNode customer pointer to Customer Pointer passed in
		front = NULL;// assigns inFront pointer to passed object pointer
		behind = NULL; // when enqueing a new node there is never a node behind
	}
};

class Queue
{
private:
	

public:
	QueueNode* Tail;
	QueueNode* Head;
	int QueueSize = 0;

	Queue() {
		Tail = NULL;
		Head = NULL;
		QueueSize = 0;
	}

	void Enqueue(Customer* customer){
		QueueNode* Node = new QueueNode(customer);
		if (Head == NULL) { // if no items are in queue
			Head = Node;
			Tail = Node;
		}
		else {
			Node->front = Tail;
			Tail->behind = Node;
			Tail = Node;
		}
		QueueSize++;
	}
	Customer* Dequeue() {
		
		if (empty())
			return NULL;
		else {
			QueueNode* Node;// node being dequeued
			Node = Head; // sets it to head of line
			Customer* data = Node->data; // gets data
			if (Head == Tail) { // If there is one item in queue both head and tail are null
				Head = NULL;
				Tail = NULL;
			}
			else {
				Head = Node->behind; // if there are more items in queue new head is just node behind head 
			}
			if (QueueSize > 1){
				Head->front = NULL; // sets new head front node to null
			}
			QueueSize--;
			delete Node;
			return data; // returns customer of dequeued head
		}
	}
	bool empty() {
		if (Head == NULL || Tail == NULL) {
			return true;
		}
		else {
			return false;
		}
	}
};

class GroceryLine : public Queue {
public:
	static int nextCustomerNumber;
	int nextCustomerInterval;
	int customerProcessTime;
	std::string LineName;

	GroceryLine(std::string name = "Normal Line", bool express = true) {
		LineName = name;
		customerProcessTime = 0;
		nextCustomerInterval = 0;
	}

	Customer* MinutePasses(int minute, bool express){ // this is called on each Queue for each minute that passes
		nextCustomerInterval--;
		customerProcessTime--;

		if (nextCustomerInterval <= 0) {// time to enqueue
			nextCustomerInterval = IntervalAndProcessTime(express);
			Customer* cust = new Customer(minute, nextCustomerNumber);
			cust->line = LineName;
			Enqueue(cust);
			nextCustomerNumber++;
		}

		if (customerProcessTime <= 0) { // if the current customer is done being checked-out
			customerProcessTime = IntervalAndProcessTime(express)/*-1*/; // time spent in line is slightly less than arrival time
			Customer* cust = Dequeue();
			if (cust != NULL) {
				cust->finishTime = minute;
				cust->timeElapsed = cust->finishTime - cust->arrivalTime;
				return cust;
			}
		}
		return NULL;
	}

	std::string serialize(int minute) {
		std::string serialize = this->LineName + " serialized at " + std::to_string(minute) + " " + "With " + std::to_string(QueueSize) + " in line.";
		return serialize;
	}
	static int IntervalAndProcessTime(bool express) {
		if (express) {
			return std::rand() % 5 + 1;
		}
		else {
			return std::rand() % 6 + 3;
		}
	}
	int newIntervalAndProcessTime() {
		return std::rand() % 6 + 3;
	}
};

class ExpressLine : public GroceryLine {
public:
	ExpressLine(std::string name, bool express) {
		LineName = name;
	}
	int newIntervalAndProcessTime() {
		return std::rand() % 5 + 1;
	}
};

//void testCustomerConstruction();
std::string getCustInfo(Customer*);
void printCustInfo(Customer*);
std::string groceryList(Customer*);
//void testQueue();
std::string doLane(int, bool, GroceryLine*);
int GroceryLine::nextCustomerNumber = 1;
std::string minForm(int);

int main(int argc, char* argv[])
{
	ExpressLine line("Express Line", true);
	GroceryLine ExpressLane = line;

	static int minutes = 1000;// temp untill I get user input setup
	GroceryLine* ExpressLane = new GroceryLine("Express Line", true);
	GroceryLine* NormalLane = new GroceryLine("Normal line", false);

	srand(time(NULL));
	//testCustomerConstruction();
	do {
		std::cin.clear();
		std::cout << "Please input simulation minutes: ";
		std::cin >> minutes;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	} while(std::cin.fail());

	std::string print = "";
	for (int i = 0; i < minutes; i++) {
		if (i % (24 * 60 * 60) == 0) { // checks if 24 hours have passed
			GroceryLine::nextCustomerNumber = 1; // resets customer number to 1
		}
		doLane(i, true, ExpressLane); // process express lane
		doLane(i, false, NormalLane); // process normal lane
		if (i % 60 == 0) { // Every 60 minutes serialize queues
			print += ExpressLane->serialize(i);
			print += "\n";
			print += NormalLane->serialize(i);
			print += "\n";
			std::cout << print;
			print = "";
		}
	}
}

std::string doLane(int min, bool express, GroceryLine* line) {
	Customer* cust;
	std::string output = "";
	cust = line->MinutePasses(min, express);
	if (cust != NULL) {
		printCustInfo(cust);
		delete cust;
		cust = NULL;
	}
	return output;
}

//void testQueue() {
//	
//}

std::string getCustInfo(Customer* customer) {
	std::string print;
	print += "Customer: " + std::to_string(customer->customerNumber);
	print += " Line: " + customer->line;
	print += " Arrived At: " + std::to_string(customer->arrivalTime);
	print += " Left At: " + std::to_string(customer->finishTime);
	print += " Time In Line: " + std::to_string(customer->timeElapsed);
	print += " Items: ";
	print += groceryList(customer) + " " + '\n';
	return print;
}

void printCustInfo(Customer* cust) {
	using std::cout;
	using std::to_string;
	using std::setw;
	cout << "Customer: " << setw(3) << to_string(cust->customerNumber) << " |"
		<< " Line: " << setw(12) << cust->line << " |"
		<< " Arrived At: " << setw(3) << to_string(cust->arrivalTime) << " |"
		<< " Left At: " << setw(3) << to_string(cust->finishTime) << " |"
		<< " Time In Line: " << setw(3) << to_string(cust->timeElapsed) << " |"
		<< " Items: " << groceryList(cust)
		<< '\n';
}

std::string minForm(int minute) {
	int days;
	int hours;
	int minutes;

	return NULL;
}

std::string groceryList(Customer* cur) {
	std::string list = "";
	for (int i = 0; i < cur->groceries.size(); i++) {
		list += cur->groceries[i];
		if (i != cur->groceries.size() - 1)
			list += ", ";
	}
	return list;
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