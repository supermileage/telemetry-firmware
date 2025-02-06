#ifndef _CIRCULARBUFFER_H_
#define _CIRCULARBUFFER_H_

#include <cstdint>
#include <vector>
#include "Math3d.h"

template <typename T>
class CircularBuffer {
    public:
        CircularBuffer(uint32_t size);
        void add(T item);
        T get(int32_t index) const;
        uint32_t size() const;
        bool isFull() const;
        bool isEmpty() const;

    private:
        std::vector<T> buffer;
        int32_t head;
        int32_t tail;
        uint32_t maxSize;
        bool full;
};

#endif
