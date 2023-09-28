#ifndef _0_DSA_H_
#define _0_DSA_H_

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <sstream>
#include <functional>

using namespace std;

template <class T>
class List {
public:
	virtual int         getSize() = 0;
	virtual bool        isEmpty() = 0;
	virtual List<T>*    clone() = 0;
	virtual void        clear() = 0;
	
	virtual void        insert(const T & val, int idx) = 0;
	virtual void        remove(int idx) = 0;
	virtual int         findIdx(const T & val) = 0;
	virtual T*          find(const T & val) = 0;
	virtual T&          operator[](int idx) = 0; // precondition: idx must be valid
	// virtual void traverse(void (*op)(T&)) = 0; // need a call back function
	// con tro ham ???
	virtual void        traverse(std::function<void (T&)>) = 0; // need #include <fucntional>

	virtual void        concat(List<T> *) = 0;
	virtual List<T>*    split(int idx) = 0;
	virtual void        reverse() = 0;
};

// Array implementation of list
#define ARRLIST_BLOCK_SIZE 32
template <class T>
class ArrList : virtual public List<T> {
protected:
    int nE, cap; // number elements, capacity
    T *pD; // point data
public:
    ArrList() : nE(0), cap(0), pD(nullptr) {}
    ArrList(const ArrList<T> &aL) : nE(aL.nE), cap(aL.cap), pD(new T[aL.cap]) {
        for (T *pS = aL.pD, *pSE = aL.pD + nE, *p = pD; pS != pSE; ) *p++ = *pS++;
    }
    ArrList(ArrList<T> && aL) : nE(aL.nE), cap(aL.cap), pD(aL.pD) {
        aL.pD = nullptr;
        aL.cap = aL.nE = 0;
    }
    ~ArrList() { clear(); }

	int         getSize() { return nE; }
	bool        isEmpty() { return !nE; }
	void        clear() { delete[] pD; nE = cap = 0; }
	void        resize(int N) {
        int newCap = (N + ARRLIST_BLOCK_SIZE - 1) / ARRLIST_BLOCK_SIZE * ARRLIST_BLOCK_SIZE;
        T *pND = new T[newCap];
        if (nE > newCap) nE = newCap;
        for (T *pS = pD, *p = pND, *pSE = pD + nE; pS != pSE; ) *p++ = std::move(*pS++);
        delete[] pD;
        pD = pND;
        cap = newCap;
    }
    List<T>*    clone() { return nullptr; }
	
	void        insert(const T & val, int idx) {
        if (idx < 0) idx = 0;
        else if (idx > nE) idx = nE;
        if (nE == cap) resize(nE + 1);
        for (T *pS = pD + nE, *pIdx = pD + idx; pS != pIdx; pS--) *pS = std::move(*(pS - 1));
        *(pD + idx) = val;
        nE++;
    }
    void        insert(T && val, int idx) {
        if (idx < 0) idx = 0;
        else if (idx > nE) idx = nE;
        if (nE == cap) resize(nE + 1);
        for (T *pS = pD + nE, *pIdx = pD + idx; pS != pIdx; pS--) *pS = std::move(*(pS - 1));
        *(pD + idx) = std::move(val);
        nE++;
    }
	void        remove(int idx) {
        if (!nE) return;
        for (T *p = pD + idx, *pE = pD + nE - 1; p  != pE; p++) *p = std::move(*(p+1));
        nE--;
    }
	int         findIdx(const T & val) {
        for (T *p = pD, *pE = pE + nE - 1; p != pE; p++) if (*p == val) return (p - pD);
        return -1;
    }
	T*          find(const T & val) {
        for (T *p = pD, *pE = pE + nE - 1; p != pE; p++) if (*p == val) return p;
        return nullptr;
    }
	T&          operator[](int idx) { return pD[idx]; }
	void        traverse(std::function<void (T&)> op) {
        for (T *p = pD, *pE = pD + nE; p != pE; p++) op(*p);
    }

	void        concat(List<T> *pL) {
        resize(nE + pL->getSize());
        T *p = pD + nE;
        pL->traverse([&p](T &val){ *p++ = val; });
        nE += pL->getSize();
    }
	List<T>*    split(int idx) { 
        if (idx < 0 || idx >= nE) return nullptr;
        ArrList<T> *pNL = new ArrList<T>;
        pNL->resize(nE - idx);
        for (T *pS = pD + idx, *pDes = pNL->pD, *pSE = pD + nE; pS != pSE;) *pDes++ = std::move(*pS++); //pSE : point source end
        pNL->nE = nE - idx;
        nE = idx;
        return pNL;  
     }
	void        reverse() {
        for (T *pL = pD, *pR = pD + nE - 1; pL < pR; pL++, pR--) {
            T temp = std::move(*pL);
            *pL = std::move(*pR);
            *pR = std::move(temp);
        }
    }
};

// Singly Linked List implementation
template <class T>
class L1List : virtual public List<T> {
protected:
    struct Node {
        T data;
        Node *pNext;
        Node() : pNext(nullptr) {}
        Node(const T & val, Node *pN = nullptr) : data(val), pNext(pN) {}
        Node(T && val, Node *pN = nullptr) : data(std::move(val)), pNext(pN) {}
    };
    Node *pHead;
    int nE;
public:
    L1List() : nE(0), pHead(nullptr) {}
    L1List(const L1List<T> &aL) : nE(aL.nE), pHead(nullptr) {
        /* Chan phuong
        Node *p = nullptr;
        for (Node *pS = aL.pHead; pS; pS = pS->pNext) {
            if (p) { p->pNext = new Node(pS->data); p = p->pNext; }
            else { p = pHead = new Node(pS->data); }
        }
        */
       // Advanced
       Node **p = &pHead; // p point to pHead (null or not)
       ((L1List<T>*)&aL)->traverse([&p](T &val){ *p = new Node(val); p = &((*p)->pNext); });
    }
    L1List(L1List<T> && aL) : nE(aL.nE), pHead(aL.pHead) {
        aL.pHead = nullptr;
        aL.nE = 0;
    }
    ~L1List() { clear(); }

	int         getSize() { return nE; }
	bool        isEmpty() { return !nE; }
	void        clear() { 
        while (pHead) {
            Node *p = pHead;
            pHead = p->pNext;
            delete p;
        }
        nE = 0;
    }
    List<T>*    clone() { return new L1List<T>(*this); }
	
	void        insert(const T & val, int idx) {
        if (idx < 0) idx = 0;
        else if (idx > nE) idx = nE;
        /* Chan phuong
        if (idx) {
            Node *pPre = pHead;
            while (idx > 1 && pPre->pNext) {
                pPre = pPre->pNext; idx--;
            }
            pPre->pNext = new Node(val, pPre->pNext);
        } else pHead = new Node(val, pHead);
        */
        // Advanced
        Node **p = &pHead;
        while (idx && *p) {// idx != 0 va *p != null
            idx--; p = &((*p)->pNext);
        }
        *p = new Node(val, *p);
        nE++;
    }
    void        insert(T && val, int idx) {
        if (idx < 0) idx = 0;
        else if (idx > nE) idx = nE;
        Node **p = &pHead;
        while (idx && *p) {// idx != 0 va *p != null
            idx--; p = &((*p)->pNext);
        }
        *p = new Node(std::move(val), *p);
        nE++;
    }
	void        remove(int idx) {
        if (!nE) return;
        if (idx < 0 || idx >= nE) return;
        /* Chan phuong
        if (idx) {
            Node *pPre = pHead;
            while (idx > 1 && pPre->pNext) { idx--; pPre = pPre->pNext; }
            Node *p = pPre->pNext;
            pPre->Next = p->Next;
            delete p;
        }
        else {
            Node *p = pHead;
            pHead = p->Next;
            delete p;
        }
        */
       // Advanced
        Node **p = &pHead;
        while (*p && idx) {
            idx--; p = &((*p)->pNext);
        }
        Node *pTD = *p; // p to del
        *p = pTD->pNext;
        delete pTD;

        nE--;
    }
	int         findIdx(const T & val) {
        int idx = 0;
        for (Node *p = pHead; p; idx++, p = p->pNext) if (p->data == val) return idx;
        return -1; 
    }
	T*          find(const T & val) {
        for (Node *p = pHead; p; p = p->pNext) if (p->data == val) return &(p->data);
        return nullptr;
    }
	T&          operator[](int idx) {
        Node *p;
        for (p = pHead; p && idx; idx--, p = p->pNext); // if (p->data == val);
        return p->data;
    }
	void        traverse(std::function<void (T&)> op) {
        for (Node *p = pHead; p; p = p->pNext) op(p->data);
    }

	void        concat(List<T> *pL) { // concat and no delete
        Node **p = &pHead;
        while (*p) p = &((*p)->pNext);
        pL->traverse([&p](T &val){ *p = new Node(val); p = &((*p)->pNext); });
        nE += pL->getSize();
    }
	List<T>*    split(int idx) { 
        if (idx < 0 || idx >= nE) return nullptr;
        L1List<T> *pNL = new L1List<T>;
        Node **p = &pHead;
        pNL->nE = nE - idx;
        nE = idx;
        while (*p && idx) idx--, p = &((*p)->pNext);
        pNL->pHead = *p;
        *p = nullptr;
        return pNL;
     }
	void        reverse() {
        Node *pNL = nullptr, *p;
        while (p == pHead) {
            pHead = pHead->pNext;
            p->pNext = pNL;
            pNL = p;
        }
        pHead = pNL;
    }
};

/*
list -----------v-------------v------------X-----v
                array         linked list       stack-----------v-----------------v
                  |               |                           ArrStack            LLStack
                  |               |-----------------------------------------------^
                  ------------------------------------------------^
Da ke thua
*/
// stack interface
template <class T>
class Stack : virtual protected List<T> {
public:
	virtual int         getSize() = 0;
	virtual bool        isEmpty() = 0;
	virtual void        clear() = 0;
	virtual List<T>*    clone() = 0; 
	
	virtual void        push(const T & val) = 0;
	virtual void        pop() = 0;
	virtual T&          top() = 0;
};

template <typename T>
class ArrStack : protected ArrList<T>, public Stack<T> {
public:
    ArrStack() {}
    ArrStack(const ArrStack & s) : ArrList<T>(s) {} // copy instructor

    int         getSize() { return ArrList<T>::getSize(); }
	bool        isEmpty() { return ArrList<T>:: isEmpty(); }
	void        clear() { return ArrList<T>::clear(); }
	List<T>*    clone() { return new ArrStack(*this); }
    // conflict about signature of a function
	// khong the stack<T> *p = new arrstack<T>
    // de giai quyet thi stack ke thua list
    // hai nhanh ke thua bi trung lap, mot nhanh arrlist da hien thuc tat ca phuong thuc cua list, nhanh stack van con virtual do do arrstack van con virtual
    // de giai quyet thi stack va arrlist ke thua virtual list

	void        push(const T & val) { ArrList<T>::insert(val, this->nE); }
	void        pop() { this->remove(this->nE - 1); }
	T&          top() { return (*this)[this->nE - 1]; }
};

template <typename T>
class L1Stack : protected L1List<T>, public Stack<T> {
public:
    L1Stack() {}
    L1Stack(const L1Stack & s) : L1List<T>(s) {} // copy instructor

    int         getSize() { return L1List<T>::getSize(); }
	bool        isEmpty() { return L1List<T>::isEmpty(); }
	void        clear() { L1List<T>::clear(); }
	List<T>*    clone() { return new L1Stack<T>(*this);}
	
	void        push(const T & val) { L1List<T>::insert(val, 0); }
	void        pop() { L1List<T>::remove(0);}
	T&          top() { return this->pHead->data; }
};

// queue interface
template <class T>
class Queue : virtual protected List<T> {
public:
	virtual int         getSize() = 0;
	virtual bool        isEmpty() = 0;
	virtual void        clear() = 0;
	virtual List<T>*    clone() = 0; 
	
	virtual void        enqueue(const T & val) = 0;
	virtual void        dequeue() = 0;
	virtual T&          first() = 0;
};

template <typename T>
class ArrQueue : protected ArrList<T>, public Queue<T> {
protected:
    int firstIdx, lastIdx;
public:
    ArrQueue() : firstIdx(0), lastIdx(0) {}
    ArrQueue(const ArrQueue & s) : firstIdx(0), lastIdx(0) {
        this->nE = s.nE;
        resize(this->nE);
        for (; lastIdx < this->nE; lastIdx++) {
            this->pD[lastIdx] = s.pD[(s.firstIdx + lastIdx) % s.cap];
        }
    } // copy instructor and align data

    void        resize(int N) {
        int newCap = (N + ARRLIST_BLOCK_SIZE - 1) / ARRLIST_BLOCK_SIZE * ARRLIST_BLOCK_SIZE;
        T *pND = new T[newCap];
        if (this->nE > newCap) this->nE = newCap;
        for (int i = 0; i < this->nE; i++) pND[i] = std::move(this->pD[(firstIdx + i) % this->cap]);
        firstIdx = 0;
        lastIdx = this->nE;
        delete[] this->pD;
        this->pD = pND;
        this->cap = newCap;
    }

    int         getSize() { return ArrList<T>::getSize(); }
	bool        isEmpty() { return ArrList<T>:: isEmpty(); }
	void        clear() { return ArrList<T>::clear(); }
	List<T>*    clone() { return new ArrQueue(*this); }

	void        enqueue(const T & val) {
        if (this->nE == this->cap) resize(this->nE + 1);
        this->pD[this->lastIdx++] = val;
        this->lastIdx = this->lastIdx % this->cap;
        this->nE++;
    }
	void        dequeue() {
        if (!this->nE) return;
        this->firstIdx = (this->firstIdx + 1) % this->cap;
        this->nE--;
    }
	T&          first() { return this->pD[this->firstIdx]; }
};

template <typename T>
class L1Queue : protected L1List<T>, public Queue<T> {
protected:
    typedef typename L1List<T>::Node Node;
    Node *pLast; // ~ pTail
public:
    L1Queue() : pLast(nullptr) {}
    L1Queue(const L1Queue & s) : L1List<T>(s) {
        pLast = this->pHead;
        while (pLast->pNext) pLast = pLast->pNext;
    } // copy instructor and align data

    int         getSize() { return L1List<T>::getSize(); }
	bool        isEmpty() { return L1List<T>:: isEmpty(); }
	void        clear() { L1List<T>::clear(); pLast = nullptr; }
	List<T>*    clone() { return new L1Queue(*this); }

	void        enqueue(const T & val) {
        if (pLast) {
            pLast = (pLast->pNext = new Node(val));
        }
        else {
            this->pHead = pLast = new Node(val);
        }
        this->nE++;
    }
	void        dequeue() {
        if (L1List<T>::isEmpty()) return;
        L1List<T>::remove(0);
        if (!this->pHead) pLast = nullptr;
    }
	T&          first() { return this->pHead->data; }
};


#endif //_0_DSA_H_