#ifndef _FILTERS_H_
#define _FILTERS_H_

#include <vector>
#include "Math3d.h"
#include "CircularBuffer.h"

class SgFilter {
    public:
        SgFilter(const std::vector<int32_t>& coefficients, int32_t normalization, CircularBuffer<Vec3>& buffer);
        Vec3 filter() const;

    private:
        std::vector<int32_t> _coefficients;
        int32_t _normalization;
        CircularBuffer<Vec3>& _buffer;
};

#endif
