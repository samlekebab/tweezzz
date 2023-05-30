Tas::Tas(int* tick) {
	this->tick = tick;
}
bool Tas::comparer(Node* a, Node* b) {
	int w1 = transpose(a->weight, *tick);
	int w2 = transpose(b->weight, *tick);
	if (b->weight == a->weight)
		return a->priority > b->priority;
	return w2 < w1;
}
void Tas::echange(word i, word j) {
	Node* tmp = tas[i];
	tas[i] = tas[j];
	tas[j] = tmp;
}
void Tas::push(Node* node) {
	tas[n] = node;
	n += 1;
	word i = n - 1;
	while (i > 0 && comparer(tas[(i - 1) / 2], tas[i])) {
		word tmp = (i - 1) / 2;
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
	word i = 0;
	word tmp = 2 * i + 1;
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
#ifndef ARDUINO
	cout << "tas de taille " << n << endl;
	for (word i = 0; i < n; i++) {
		cout << "element " << i << " : " << (*tas[i]).weight << ", " << (int)(*tas[i]).priority << endl;
	}
#else
	//TODO serial	
	Serial.println( "tas de taille " + String( n ));
	for (word i = 0; i < n; i++) {
		Serial.println( "element " + String(i) + " : " + String( (*tas[i]).weight ) + ", " + String ( (int)(*tas[i]).priority ));
	}
#endif
}

Task::Task() {}
void Task::execute() {
	//nothing by default
}

