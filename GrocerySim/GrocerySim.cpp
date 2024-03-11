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
		SimulateMinute();
	}
}

void SimulateMinute() {

}

class Queue
{
private:
	QueueNode* Tail;
	QueueNode* Head;

public:
	std::string QueueName;
	int QueueSize = 0;

	Queue(std::string QueueName){
		this->QueueName = QueueName;
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
