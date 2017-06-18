#include <stdlib.h>

extern "C" {
int fun4();
}

int main() {
    fun4();

    char* a = new char;
    while(1){}
}

