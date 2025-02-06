#ifndef _FILTERS_H_
#define _FILTERS_H_

#include <vector>
#include "Math3d.h"
#include "CircularBuffer.h"
//#include "../../lib/eigen-3.4.0/Eigen/Dense"

class SgFilter {
    public:
        SgFilter(const std::vector<int32_t>& coefficients, int32_t normalization, CircularBuffer<Vec3>& buffer);
        Vec3 filter() const;

    private:
        std::vector<int32_t> _coefficients;
        int32_t _normalization;
        CircularBuffer<Vec3>& _buffer;
};

// template<int32_t DIM_STATE_VEC, int32_t DIM_MEAS_VEC>
// class KalmanFilter {
//     public:
//         KalmanFilter() = default;
//         KalmanFilter(const Eigen::Matrix<float, DIM_STATE_VEC, 1> &initialStateVector, const Eigen::Matrix<float, DIM_STATE_VEC, DIM_STATE_VEC> &initialCovarianceMatrix);

//         void prediciton(const Eigen::Matrix<float, DIM_STATE_VEC, DIM_STATE_VEC> &stateTransitionMatrix, const Eigen::Matrix<float, DIM_STATE_VEC, DIM_STATE_VEC> &processNoiseCovariance);
//         void correction(const Eigen::Vector<float, DIM_MEAS_VEC> &measurement, const Eigen::Matrix<float, DIM_MEAS_VEC, DIM_STATE_VEC> &measurementMatrix, const Eigen::Matrix<float, DIM_MEAS_VEC, DIM_MEAS_VEC> &measurementNoiseCovariance);

//     private:
//         // State vector
//         Eigen::Vector<float, DIM_STATE_VEC> _stateVector;

//         // State covariance matrix
//         Eigen::Matrix<float, DIM_STATE_VEC, DIM_STATE_VEC> _covarianceMatrix;
// };

#endif
