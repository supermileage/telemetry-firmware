#ifndef KALMAN_FILTER_H
#define KALMAN_FILTER_H

#include "Filters.h"

class KalmanFilter : public Filters {
    public:
        KalmanFilter();
        Vec3 filter(const Vec3 &input) override;
    private:
        
};

#endif

/* Notes: https://www.st.com/resource/en/datasheet/lsm6dso.pdf

Gyroscope RMS noise in normal/low-power mode: 75 mdps

Acceleration RMS noise in normal/low-power mode: 1.8 mg(RMS)
    at FS = ±2 g (perfectly reasonable)
*/
