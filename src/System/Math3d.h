#ifndef _MATH_3D_H_
#define _MATH_3D_H_

#include <array>

struct Vec3 {
    float x;
    float y;
    float z;
};

typedef std::array<std::array<float, 3>, 3> Matrix3x3;

// defines a simple 3x3 matrix which can be used to multiply a 3d vector
class Matrix3d {
    public:
        Matrix3d() { }

        /**
         * @brief initializes new Matrix3d with matrix
         * @param matrix 3x3 matrix {{a, b, c}} where each of a, b, c is a 3-element column vector {x, y, z}
         */
        Matrix3d(Matrix3x3 matrix) : _matrix(matrix) { }
        
        /**
         * @brief sets this Matrix3d's 3x3 matrix
         * @param matrix 3x3 matrix {{a, b, c}} where each of a, b, c is a 3-element column vector {x, y, z}
         */
        void setMatrix(Matrix3x3 matrix) { _matrix = matrix; }

        /**
         * @brief performs matrix multiplication Ax where A is this matrix and x is vec
         * @param vec 3d vector
        */
        inline Vec3 multiply(Vec3 vec) {
            Vec3 result = Vec3 { 0, 0, 0 };
            float *resultBuf = (float*)&result;
            float *vecBuf = (float*)&vec;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    // for row i, get sum of each column entry * vec.i
                    resultBuf[j] +=  vecBuf[i] * _matrix[i][j];
                }
            }
            return result;
        }

    private:
        Matrix3x3 _matrix; 
};

#endif