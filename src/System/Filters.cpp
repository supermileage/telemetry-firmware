#include <vector>
#include "Filters.h"

// Note, windowSize must be an odd number
SgFilter::SgFilter(const std::vector<int32_t>& coefficients, int32_t normalization, CircularBuffer<Vec3>& buffer)
    : _coefficients(coefficients), _normalization(normalization), _buffer(buffer) {}

Vec3 SgFilter::filter() const {
    Vec3 result {0.0, 0.0, 0.0};
    for (size_t i = 0; i < _coefficients.size(); ++i) {
        result += _buffer.get(i) * _coefficients[i];
    }
    return result / _normalization;
}
