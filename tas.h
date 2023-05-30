#ifndef TAS
#define TAS
#define TAS_SIZE 100

class Node {
public:
	unsigned int weight = 0;
	byte priority = LOW_PRIORITY;
};

class Tas {
public:
    void push(Node* node);
    Node *pop();
	Node *view();
	bool isEmpty();
	Tas(int* tick);
	void print();
	int getN(){return n;};
private:
	int* tick;
	void echange(word i, word j);
	Node* tas[TAS_SIZE];
	word n=0;
	bool comparer(Node* a, Node* b);
}; 
#endif
