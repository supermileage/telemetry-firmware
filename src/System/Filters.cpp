#include <vector>
#include "../../lib/eigen-3.4.0/Eigen/Dense"
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

// template<int32_t DIM_STATE_VEC, int32_t DIM_MEAS_VEC>
// KalmanFilter<DIM_STATE_VEC, DIM_MEAS_VEC>::KalmanFilter() {
//     _stateVector.setZero();
//     _covarianceMatrix.setIdentity();
// }

// template<int32_t DIM_STATE_VEC, int32_t DIM_MEAS_VEC>
// KalmanFilter<DIM_STATE_VEC, DIM_MEAS_VEC>::KalmanFilter(const Eigen::Matrix<float, DIM_STATE_VEC, 1> &initialStateVector, const Eigen::Matrix<float, DIM_STATE_VEC, DIM_STATE_VEC> &initialCovarianceMatrix) {
//     _stateVector = initialStateVector;
//     _covarianceMatrix = initialCovarianceMatrix;
// }

// template<int32_t DIM_STATE_VEC, int32_t DIM_MEAS_VEC>
// void KalmanFilter<DIM_STATE_VEC, DIM_MEAS_VEC>::prediciton(const Eigen::Matrix<float, DIM_STATE_VEC, DIM_STATE_VEC> &stateTransitionMatrix, const Eigen::Matrix<float, DIM_STATE_VEC, DIM_STATE_VEC> &processNoiseCovariance) {
//     _stateVector = stateTransitionMatrix * _stateVector;
//     _covarianceMatrix = stateTransitionMatrix * _covarianceMatrix * stateTransitionMatrix.transpose() + processNoiseCovariance;
// }
// template<int32_t DIM_STATE_VEC, int32_t DIM_MEAS_VEC>
// void KalmanFilter<DIM_STATE_VEC, DIM_MEAS_VEC>::correction(const Eigen::Vector<float, DIM_MEAS_VEC> &measurement, const Eigen::Matrix<float, DIM_MEAS_VEC, DIM_STATE_VEC> &measurementMatrix, const Eigen::Matrix<float, DIM_MEAS_VEC, DIM_MEAS_VEC> &measurementNoiseCovariance) {
//     Eigen::Matrix<float, DIM_MEAS_VEC, DIM_MEAS_VEC> innovationCovariance = measurementMatrix * _covarianceMatrix * measurementMatrix.transpose() + measurementNoiseCovariance;
//     Eigen::Matrix<float, DIM_STATE_VEC, DIM_MEAS_VEC> kalmanGain = _covarianceMatrix * measurementMatrix.transpose() * innovationCovariance.inverse();
//     _stateVector = _stateVector + kalmanGain * (measurement - measurementMatrix * _stateVector);
//     _covarianceMatrix = (Eigen::Matrix<float, DIM_STATE_VEC, DIM_STATE_VEC>::Identity() - kalmanGain * measurementMatrix) * _covarianceMatrix;
// }

