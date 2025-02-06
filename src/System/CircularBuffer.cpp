#include "CircularBuffer.h"

// Constructor for CircularBuffer of generic type
template <typename T>
CircularBuffer<T>::CircularBuffer(uint32_t size) 
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

// Get an item from the buffer at a specific index
// Note: Buffer cannot be empty and index cannot be out of range
template <typename T>
T CircularBuffer<T>::get(int32_t index) const {
    return buffer[(tail + index) % maxSize];
}

// Get the size of the buffer
template <typename T>
uint32_t CircularBuffer<T>::size() const {
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
