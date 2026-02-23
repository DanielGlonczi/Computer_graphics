#include "matrix.h"

#include <stdio.h>
#include <math.h>

void init_zero_matrix(float matrix[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            matrix[i][j] = 0.0;
        }
    }
}

void print_matrix(const float matrix[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            printf("%4.4f ", matrix[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(const float a[3][3], const float b[3][3], float c[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
}

void init_identity_matrix(float matrix[3][3])
{
    int i, j;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            matrix[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

void scale_matrix(const float src[3][3], float scalar, float dest[3][3])
{
    int i, j;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            dest[i][j] = src[i][j] * scalar;
        }
    }
}

void multiply_matrices(const float a[3][3], const float b[3][3], float dest[3][3])
{
    int i, j, k;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            float sum = 0.0f;
            for (k = 0; k < 3; ++k) {
                sum += a[i][k] * b[k][j];
            }
            dest[i][j] = sum;
        }
    }
}

void transform_point(const float matrix[3][3], const float point[3], float result[3])
{
    int i, j;
    for (i = 0; i < 3; ++i) {
        float sum = 0.0f;
        for (j = 0; j < 3; ++j) {
            sum += matrix[i][j] * point[j];
        }
        result[i] = sum;
    }
}

static void premultiply(const float a[3][3], const float b[3][3], float dest[3][3])
{
    multiply_matrices(a, b, dest);
}

void scale(float matrix[3][3], float sx, float sy)
{
    float m[3][3];
    init_identity_matrix(m);
    m[0][0] = sx;
    m[1][1] = sy;
    multiply_matrices(matrix, m, matrix);
}

void shift(float matrix[3][3], float dx, float dy)
{
    float m[3][3];
    init_identity_matrix(m);
    m[0][2] = dx;
    m[1][2] = dy;
    multiply_matrices(matrix, m, matrix);
}

void rotate(float matrix[3][3], float angle_radians)
{
    float m[3][3];
    float c = cosf(angle_radians);
    float s = sinf(angle_radians);
    init_identity_matrix(m);
    m[0][0] = c;
    m[0][1] = -s;
    m[1][0] = s;
    m[1][1] = c;
    multiply_matrices(matrix, m, matrix);
}

