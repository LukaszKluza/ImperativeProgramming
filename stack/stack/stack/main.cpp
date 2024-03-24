#include <iostream>
#include "stack.hpp"

void test1() {
    stack s;
    MY_STUDENT* a = new MY_STUDENT("balicki", 2003, CS);
    MY_STUDENT* b = new MY_STUDENT("kluza", 2000, BIOTECH);
    MY_STUDENT* c = new MY_STUDENT("ola", 2000, BIOTECH);
    MY_STUDENT* d = new MY_STUDENT("kasia", 2003, CS);
    MY_STUDENT* e = new MY_STUDENT("kasia", 2001, AR);
    void* data = a;
    s.add(data);
    data = b;
    s.add(data);
    data = c;
    s.add(data);
    s.show_first();
}

int main(){
    //test2();
    
    bool exit = true;
	stack s;
	while (exit) {
		menu(exit,s);
	}
}

