#ifndef FILTERS_H
#define FILTERS_H

#include "CircularBuffer.h"
#include "Math3d.h"

// Abstract base class for filters
class Filters {
    public:
        virtual ~Filters() = default;
        virtual Vec3 filter(const Vec3 &input) = 0;
};

class RawFilter : public Filters {
    public:
        Vec3 filter(const Vec3 &input) override;
};

class LowPassFilter : public Filters {
    public:
        LowPassFilter(float alpha);
        Vec3 filter(const Vec3 &input) override;

    private:
        float _alpha;
        Vec3 _prev;
        bool _init;
};

class SavGolFilter : public Filters {
    public:
        SavGolFilter(const std::vector<int32_t> &coeff, int32_t norm,
                     size_t windowSize);
        Vec3 filter(const Vec3 &input) override;

    private:
        std::vector<int32_t> _coeff;
        int32_t _norm;
        size_t _windowSize;
        CircularBuffer<Vec3> _buffer;
};

#endif
