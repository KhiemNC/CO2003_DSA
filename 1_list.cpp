/*
Implement array and single linked list
*/
#include "0_DSA.h"

// int main() {
//     Stack<int> *pS = new L1Stack<int>, *pSC = nullptr;
//     for (int i = 0; i < 8; i++) pS->push(rand() % 100);
//     pSC = (Stack<int>*)(void*)pS->clone();
//     while (!pS->isEmpty()) {
//         cout << setw(5) << pS->top();
//         pS->pop();
//     }

//     cout << endl;
//     while (!pSC->isEmpty()) {
//         cout << setw(5) << pSC->top();
//         pSC->pop();
//     }

//     cout << endl << "Ok!";
//     return 0;
// }

// array queue
int main() {
    Queue<int> *pS = new ArrQueue<int>, *pSC = nullptr;
    for (int i = 0; i < 8; i++) pS->enqueue(rand() % 100);
    cout << "     Call---\n";
    pSC = (Queue<int>*)(void*)pS->clone();
    while (!pS->isEmpty()) {
        cout << setw(5) << pS->first();
        pS->dequeue();
    }

    cout << endl;
    while (!pSC->isEmpty()) {
        cout << setw(5) << pSC->first();
        pSC->dequeue();
    }

    cout << endl << "Ok!";
    return 0;
}

// link queue
// int main() {
//     Queue<int> *pS = new L1Queue<int>, *pSC = nullptr;
//     for (int i =- 0; i < 8; i++) pS->enqueue(rand() % 100);
//     pSC = (Queue<int>*)(void*)pS->clone();
//     while (!pS->isEmpty()) {
//         cout << setw(5) << pS->first();
//         pS->dequeue();
//     }

//     cout << endl;
//     while (!pSC->isEmpty()) {
//         cout << setw(5) << pSC->first();
//         pSC->dequeue();
//     }

//     cout << endl << "Ok!";
//     return 0;
// }

// g++ -g -o 1_list 1_list.cpp -I . -std=c++11 -DDEBUG