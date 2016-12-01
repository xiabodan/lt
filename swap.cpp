#include <iostream>

void swap (int& a ,int& b){
	int c = a;
	a = b;
	b = c;
}

int main(){
	
	int a = 10;
	int b = 20;
	swap(a,b);
	std::cout << a << " " << b <<std::endl;
	return 0;
}