#ifndef Q1_BUFFER_H
#define Q1_BUFFER_H

#include <uC++.h>
#include <assert.h>
#include <list>

using namespace std;

#ifdef BUSY
template<typename T> class BoundedBuffer {
  private:
    const unsigned int size;
    list<T> buffer;
    uOwnerLock lock;
    uCondLock fullCond;
    uCondLock emptyCond;

    bool isFull();
    bool isEmpty();
  public:
    BoundedBuffer( const unsigned int size = 10 );
    void insert( T elem );
    T remove();
};

template<typename T>
BoundedBuffer<T>::BoundedBuffer( const unsigned int size) : size(size) {}

template<typename T>
bool BoundedBuffer<T>::isFull() {
    return buffer.size() == size;
}

template<typename T>
bool BoundedBuffer<T>::isEmpty() {
    return buffer.size() == 0;
}

template<typename T>
void BoundedBuffer<T>::insert(T elem) {
    lock.acquire();
    while (isFull()) {
        fullCond.wait(lock);
    }

    assert(!isFull());
    buffer.push_back(elem);
    emptyCond.signal();
    lock.release();
}

template<typename T>
T BoundedBuffer<T>::remove() {
    lock.acquire();
    while (isEmpty()) {
        emptyCond.wait(lock);
    }

    assert(!isEmpty());
    T temp = buffer.front();
    buffer.pop_front();
    fullCond.signal();
    lock.release();
    return temp;
}

#endif // BUSY

#ifdef NOBUSY
template<typename T> class BoundedBuffer {
  private:
    const unsigned int size;
    list<T> buffer;
    uOwnerLock lock;
    uCondLock fullCond;
    uCondLock emptyCond;
    uCondLock bargingCond;
    bool signalFlag;

    bool isFull();
    bool isEmpty();
  public:
    BoundedBuffer( const unsigned int size = 10 );
    void insert( T elem );
    T remove();
};

template<typename T>
BoundedBuffer<T>::BoundedBuffer( const unsigned int size)
    : size(size), signalFlag(false) {
}

template<typename T>
bool BoundedBuffer<T>::isFull() {
    return buffer.size() == size;
}

template<typename T>
bool BoundedBuffer<T>::isEmpty() {
    return buffer.size() == 0;
}

template<typename T>
void BoundedBuffer<T>::insert(T elem) {
    lock.acquire();

    static int signalEmpty = 0;

    // check if we are barging
    if (signalFlag) {
        cout << "  > barging into insert : " << buffer.size() << " [" << lock.owner() << "] " << endl;
        bargingCond.wait(lock);
    }

    if (isFull()) {
        if (!bargingCond.empty()) {
            cout << " - signal barging (insert wait) : " << buffer.size() << " [" << lock.owner() << "] " << endl;
            bargingCond.signal();
        }
        cout << " > sleep on full : " << buffer.size() << " [" << lock.owner() << "] " << endl;
        fullCond.wait(lock);
        cout << " < wake from full : " << buffer.size() << " [" << lock.owner() << "] " << endl;
    }

    assert(!isFull());
    buffer.push_back(elem);
    cout << "insert : " << buffer.size() << " [" << lock.owner() << "] " << endl;

    signalFlag = !emptyCond.empty();
    if (!emptyCond.empty()) {
        assert(!isEmpty());
        cout << " - signal not empty : " << buffer.size() << " [" << lock.owner() << "] " << endl;
        signalFlag = true;
        signalEmpty++;
        cout << "   - signal empty : " << signalEmpty << " times." << endl;
        emptyCond.signal();
    } else if (!bargingCond.empty()) {
        cout << " - signal barging (insert) : " << buffer.size() << " [" << lock.owner() << "] " << endl;
        bargingCond.signal();
    }

    lock.release();
}

template<typename T>
T BoundedBuffer<T>::remove() {
    lock.acquire();

    static int waitEmpty = 0;

    if (signalFlag) {
        cout << "  > barging into remove : " << buffer.size() << " [" << lock.owner() << "] " << endl;
        bargingCond.wait(lock);
    }

    if (isEmpty()) {
        if (!bargingCond.empty()) {
            cout << " - signal barging (remove wait) : " << buffer.size() << " [" << lock.owner() << "] " << endl;
            bargingCond.signal();
        }
        cout << " > sleep on empty : " << buffer.size() << " [" << lock.owner() << "] " << endl;
        waitEmpty++;
        cout << "   - wait empty : " << waitEmpty << " times." << endl;
        emptyCond.wait(lock);
        cout << " < wake from empty : " << buffer.size() << " [" << lock.owner() << "] " << endl;
    }
    cout << "remove : " << buffer.size() << " [" << lock.owner() << "] " << endl;

    assert(!isEmpty());
    T temp = buffer.front();
    buffer.pop_front();

    signalFlag = !fullCond.empty();
    if (!fullCond.empty()) {
        cout << " - signal not full : " << buffer.size() << " [" << lock.owner() << "] " << endl;
        fullCond.signal();
    } else if (!bargingCond.empty()) {
        cout << " - signal barging (remove) : " << buffer.size() << " [" << lock.owner() << "] " << endl;
        bargingCond.signal();
    } else {
        cout << "nothing on the queue : " << fullCond.empty() << " " << bargingCond.empty() << " " << signalFlag << endl;
    }

    lock.release();
    return temp;
}
#endif


#endif // Q1_BUFFER_H
