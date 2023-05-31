#include <iostream>
#include <list>
using namespace std;
class A{
	public:
	int b{0};
	A(int b):b(b){};
	list<int> test;
};
int main(){
	list<A*> l;
	l.insert(l.begin(),new A(1));
	l.insert(l.begin(),new A(2));
	auto it = l.begin();
	for(;it!=l.end();it++){
		cout<<(*it)->b<<endl;

	}
	return 0;
}
