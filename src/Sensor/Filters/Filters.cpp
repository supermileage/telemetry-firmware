#include "Filters.h" 

// Raw filter (basically no filter)
Vec3 RawFilter::filter(const Vec3 &input) { return input; }

// Low-pass filter
LowPassFilter::LowPassFilter(float alpha) : _alpha(alpha), _init(false) {}

// Savitsky-Golay filter
Vec3 LowPassFilter::filter(const Vec3 &input) {
    // Initialize and return first input unfiltered
    if (!_init) {
        _prev = input;
        _init = true;
        return _prev;
    }
    _prev.x = _alpha * input.x + (1.0f - _alpha) * _prev.x;
    _prev.y = _alpha * input.y + (1.0f - _alpha) * _prev.y;
    _prev.z = _alpha * input.z + (1.0f - _alpha) * _prev.z;
    return _prev;
}

SavGolFilter::SavGolFilter(const std::vector<int32_t> &coeff, int32_t norm,
                           size_t windowSize)
    : _coeff(coeff), _norm(norm), _windowSize(windowSize), _buffer(windowSize) {
}

Vec3 SavGolFilter::filter(const Vec3 &input) {
    _buffer.add(input);
    // If buffer isn't filled, return the average of what's available for now
    if (_buffer.size() < _windowSize) {
        Vec3 sum = {0, 0, 0};
        for (size_t i = 0; i < _buffer.size(); i++) {
            sum.x += _buffer.get(i).x;
            sum.y += _buffer.get(i).y;
            sum.z += _buffer.get(i).z;
        }
        sum.x /= _buffer.size();
        sum.y /= _buffer.size();
        sum.z /= _buffer.size();
        return sum;
    }
    Vec3 result = {0, 0, 0};
    for (size_t i = 0; i < _windowSize; i++) {
        result.x += _coeff[i] * _buffer.get(i).x;
        result.y += _coeff[i] * _buffer.get(i).y;
        result.z += _coeff[i] * _buffer.get(i).z;
    }
    result.x /= _norm;
    result.y /= _norm;
    result.z /= _norm;
    return result;
}
