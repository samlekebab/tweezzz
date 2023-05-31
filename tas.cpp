#include "tas.h"
#include <iostream>
using namespace std;
bool Tas::comparer(Node* a, Node* b) {
	int w1 = a->bounds.start;
	int w2 = b->bounds.start;
	return w2 < w1;
}
void Tas::echange(int i, int j) {
	Node* tmp = tas[i];
	tas[i] = tas[j];
	tas[j] = tmp;
}
void Tas::push(Node* node) {
	tas[n] = node;
	n += 1;
	int i = n - 1;
	while (i > 0 && comparer(tas[(i - 1) / 2], tas[i])) {
		int tmp = (i - 1) / 2;
		echange(i, tmp);
		i = tmp;
	}
}
bool Tas::isEmpty() {
	return n <= 0;
}
Node* Tas::pop() {
	Node* res = tas[0];
	n -= 1;
	tas[0] = tas[n];
	bool end = false;
	int i = 0;
	int tmp = 2 * i + 1;
	while (!end && tmp < n) {
		end = true;
		if (comparer(tas[i], tas[tmp])) {
			echange(i, tmp);
			end = false;
			i = tmp;
		}
		else if (tmp + 1 < n && comparer(tas[i], tas[tmp + 1])) {
			tmp++;
			echange(i, tmp);
			end = false;
			i = tmp;
		}
	}
	return res;
}
Node* Tas::view() {
	return tas[0];
}
void Tas::print() {
	cout << "tas de taille " << n << endl;
	for (int i = 0; i < n; i++) {
		cout << "element " << i << " : " << (*tas[i]).bounds.start<<endl;
	}
}

