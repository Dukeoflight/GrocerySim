// GrocerySim.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char* argv[])
{
	static int minutes = 360;// temp untill I get 
	Queue* expressLane = new Queue("Express");
	Queue* normalLane = new Queue("Normal");
	std::cout << "Hello World!\n" << argc;
    if (argc == 0)
    {
        std::cout << "This program simulats a grocery store line and takes argument for the number of minutes being simulated";
    }

	//
	for (int i = 0; i < minutes; i++) {
		std::cout << SimulateMinute(i, expressLane, normalLane);
	}
}

std::string SimulateMinute(int minute, Queue* express, Queue* normal) {
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
}
void newCustomerExpress(int minute, Queue* line){
	Customer* c = new Customer(minute);
	c->finishTime = std::rand() % 5 + 1 + c->arrivalTime; // sets finish time to random value 1-5 plus when they arrived
	line->Enqueue(c);
}
void newCustomerNormal(int minute, Queue* line) {
	Customer* c = new Customer(minute);
	c->finishTime = std::rand() % 6 + 3 + c->arrivalTime; // sets finish time to random value 3-8 plus when they arrived
	line->Enqueue(c);
}

class GroceryLine : Queue {

public:
	int customerProcessTime;
	int nextCustomerInterval;
	std::string LineName;
	bool empty;
	void MinutePasses() {
		this->customerProcessTime--;
		this->nextCustomerInterval--;
	}
};

class ExpressLine : GroceryLine {

	ExpressLine() {
		this->nextCustomerInterval = std::rand() % 5 + 1;
	}
};

class NormalLine : GroceryLine {
	NormalLine() {
		this->nextCustomerInterval = std::rand() % 6 + 3;
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
			QueueNode* Node;
			Node = Head;
			Head = Node->behind;
			Head->front = NULL;
			return Node->data;
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

	Customer(int time = 0) {
		arrivalTime = time;
		timeElapsed = 0;
		int numItems = std::rand() % 8;
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
