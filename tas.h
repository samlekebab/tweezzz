#ifndef TAS
#define TAS
#define TAS_SIZE 10'000

struct Bounds {
	long start;
	long end;
};

class Node {
public:
	Bounds bounds{.start=0,.end=0};
};

class Tas {
public:
	void push(Node* node);
	Node *pop();
	Node *view();
	bool isEmpty();
	void print();
	int getN(){return n;};
private:
	long* tick;
	void echange(int i, int j);
	Node* tas[TAS_SIZE];
	int n=0;
	bool comparer(Node* a, Node* b);
}; 
#endif
