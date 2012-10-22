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

    /**
     * Check if the buffer is full.
     *
     * return true if it is full, false otherwise.
     */
    bool isFull();

    /**
     * Check if the buffer is empty.
     *
     * return true if it is empty, false otherwise.
     */
    bool isEmpty();

    /**
     * Push a new element onto the end of the buffer
     *
     * elem - the new element
     */
    void push(T elem);

    /**
     * Pop a new element from the front of the buffer
     *
     * returns the popped element.
     */
    T pop();
  public:
    /**
     * Construct a new buffer with a size of maxSize
     */
    BoundedBuffer( const unsigned int maxSize = 10 );
    ~BoundedBuffer();

    /**
     * Inserts a new element into the buffer.
     *
     * elem - the element to insert.
     */
    void insert( T elem );

    /**
     * Removes the front element of the buffer an returns it.
     *
     * returns the removed element.
     */
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

        // reset the flag if we have no more barging
        if (bargingCond.empty()) {
            stopBarging = false;
        }
    }

    // If nessesary, wait until the buffer is not full
    if (isFull()) {
        bargingCond.signal();
        fullCond.wait(lock);
    }

    push(elem);

    // set a signal flag to prevent barging as long as we have tasks waiting
    // on the condition variables
    stopBarging = !emptyCond.empty() || !bargingCond.empty();
    if (!emptyCond.empty()) {
        emptyCond.signal();
    } else if (!bargingCond.empty()) {
        // if the empty condition variable is empty then start signalling
        // the barging queue
        bargingCond.signal();
    }

    lock.release();
}

template<typename T>
T BoundedBuffer<T>::remove() {
    lock.acquire();

    if (stopBarging) {
        bargingCond.wait(lock);

        // reset the flag if we have no more barging
        if (bargingCond.empty()) {
            stopBarging = false;
        }
    }

    // If nessesary, wait until the buffer is not empty
    if (isEmpty()) {
        bargingCond.signal();
        emptyCond.wait(lock);
    }

    T temp = pop();

    // set a signal flag to prevent barging as long as we have tasks waiting
    // on the condition variables
    stopBarging = !fullCond.empty() || !bargingCond.empty();
    if (!fullCond.empty()) {
        fullCond.signal();
    } else if (!bargingCond.empty()) {
        // if the full condition variable is empty then start signalling
        // the barging queue
        bargingCond.signal();
    }

    lock.release();
    return temp;
}
#endif


#endif // Q1_BUFFER_H
