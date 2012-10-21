#ifndef Q1_BUFFER_H
#define Q1_BUFFER_H

#include <uC++.h>
#include <assert.h>
#include <list>

using namespace std;

template<typename T> class BoundedBuffer {
  private:
    const unsigned int maxSize;
    T *buffer;
    unsigned int head;
    unsigned int size;
    uOwnerLock lock;
    uCondLock fullCond;
    uCondLock emptyCond;

#ifdef NOBUSY
    uCondLock bargingCond;
    bool stopBarging;
#endif

    bool isFull();
    bool isEmpty();
    void push(T elem);
    T pop();
  public:
    BoundedBuffer( const unsigned int maxSize = 10 );
    ~BoundedBuffer();
    void insert( T elem );
    T remove();
};

template<typename T>
BoundedBuffer<T>::~BoundedBuffer() {
    delete [] buffer;
}

template<typename T>
bool BoundedBuffer<T>::isFull() {
    return size == maxSize;
}

template<typename T>
bool BoundedBuffer<T>::isEmpty() {
    return size == 0;
}

template<typename T>
void BoundedBuffer<T>::push(T elem) {
    assert(!isFull());
    buffer[(head + size) % maxSize] = elem;
    size++;
}

template<typename T>
T BoundedBuffer<T>::pop() {
    assert(!isEmpty());
    T temp = buffer[head];
    head = (head + 1) % maxSize;
    size--;
    return temp;
}

#ifdef BUSY

template<typename T>
BoundedBuffer<T>::BoundedBuffer( const unsigned int maxSize)
    : maxSize(maxSize), buffer(new T[maxSize]), head(0), size(0)  {
}

template<typename T>
void BoundedBuffer<T>::insert(T elem) {
    lock.acquire();
    while (isFull()) {
        fullCond.wait(lock);
    }

    push(elem);

    emptyCond.signal();
    lock.release();
}

template<typename T>
T BoundedBuffer<T>::remove() {
    lock.acquire();
    while (isEmpty()) {
        emptyCond.wait(lock);
    }

    T temp = pop();

    fullCond.signal();
    lock.release();
    return temp;
}

#endif // BUSY

#ifdef NOBUSY

template<typename T>
BoundedBuffer<T>::BoundedBuffer( const unsigned int size)
    : maxSize(size), buffer(new T[maxSize]), head(0), size(0),
      stopBarging(false)  {
}

template<typename T>
void BoundedBuffer<T>::insert(T elem) {
    lock.acquire();

    if (stopBarging) {
        bargingCond.wait(lock);
        if (bargingCond.empty()) {
            stopBarging = false;
        }
    }

    if (isFull()) {
        if (!bargingCond.empty()) {
            bargingCond.signal();
        }
        fullCond.wait(lock);
    }

    push(elem);

    stopBarging = !emptyCond.empty() || !bargingCond.empty();
    if (!emptyCond.empty()) {
        emptyCond.signal();
    } else if (!bargingCond.empty()) {
        bargingCond.signal();
    }

    lock.release();
}

template<typename T>
T BoundedBuffer<T>::remove() {
    lock.acquire();

    if (stopBarging) {
        bargingCond.wait(lock);
        if (bargingCond.empty()) {
            stopBarging = false;
        }
    }

    if (isEmpty()) {
        if (!bargingCond.empty()) {
            bargingCond.signal();
        }
        emptyCond.wait(lock);
    }

    T temp = pop();

    stopBarging = !fullCond.empty() || !bargingCond.empty();
    if (!fullCond.empty()) {
        fullCond.signal();
    } else if (!bargingCond.empty()) {
        bargingCond.signal();
    }

    lock.release();
    return temp;
}
#endif


#endif // Q1_BUFFER_H
