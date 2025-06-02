#ifndef _CIRCULARBUFFER_H_
#define _CIRCULARBUFFER_H_

#include <cstdint>
#include <vector>
#include <cstddef>
#include "Math3d.h"

template <typename T>
class CircularBuffer {
    public:
        CircularBuffer(size_t size);
        void add(T item);
        T get(int32_t index) const;
        size_t size() const;
        bool isFull() const;
        bool isEmpty() const;

    private:
        std::vector<T> buffer;
        int32_t head;
        int32_t tail;
        size_t maxSize;
        bool full;
};

template <typename T>
CircularBuffer<T>::CircularBuffer(size_t size) 
    : buffer(size)
    , head(0)
    , tail(0)
    , maxSize(size)
    , full(false) {
}

// Add an item to the buffer
template <typename T>
void CircularBuffer<T>::add(T item) {
    buffer[head] = item;
    if (full) {
        tail = (tail + 1) % maxSize;
    }
    head = (head + 1) % maxSize;
    full = head == tail;
}

// Get an item from the buffer at a specific index within range
template <typename T>
T CircularBuffer<T>::get(int32_t index) const {
    if (isEmpty()) {
        return T();
    }
    size_t currSize = size();
    if (index < 0 || static_cast<size_t>(index) >= currSize) {
        return T();
    }
    return buffer[(tail + index) % maxSize];
}

// Get the size of the buffer
template <typename T>
size_t CircularBuffer<T>::size() const {
    if (full) {
        return maxSize;
    }
    if (head >= tail) {
        return head - tail;
    }
    return maxSize + head - tail;
}

// Check if the buffer is full
template <typename T>
bool CircularBuffer<T>::isFull() const {
    return full;
}

// Check if the buffer is empty
template <typename T>
bool CircularBuffer<T>::isEmpty() const {
    return (!full && (head == tail));
}

// Explicit template instantiation
template class CircularBuffer<int>;
template class CircularBuffer<float>;
template class CircularBuffer<double>;
template class CircularBuffer<Vec3>;

#endif
