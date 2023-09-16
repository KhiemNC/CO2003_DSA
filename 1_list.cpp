/*
Implement array and single linked list
*/
#include "0_DSA.h"

int main() {
    Stack<int> *pS = new ArrStack<int>;
    for (int i =- 0; i < 8; i++) pS->push(rand() % 100);
    while (!pS->isEmpty()) {
        cout << setw(5) << pS->top();
        pS->pop();
    }
    
    cout << endl << "Ok!";
    return 0;
}
// g++ -g -o 1_list 1_list.cpp -I . -std=c++11 -DDEBUG