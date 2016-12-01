#include <iostream>
using namespace std;
int main(){
	double f,h;
	while(!(cin >> f >> h)){
		cout << "error"  << endl;
		cin.clear();
		while(cin.get() != '\n')
			continue;
	}

	cout << f <<" " <<  h <<endl;
	return 0;
}