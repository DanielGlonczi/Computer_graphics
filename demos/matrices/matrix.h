#ifndef MATRIX_H
#define MATRIX_H

/**
 * Initializes all elements of the matrix to zero.
 */
void init_zero_matrix(float matrix[3][3]);

/**
 * Print the elements of the matrix.
 */
void print_matrix(const float matrix[3][3]);

/**
 * Add matrices.
 */
void add_matrices(const float a[3][3], const float b[3][3], float c[3][3]);

/**
 * Initialize matrix as identity (1 on diagonal, 0 elsewhere).
 */
void init_identity_matrix(float matrix[3][3]);

/**
 * Multiply matrix by a scalar, storing result in dest (may be same as source).
 */
void scale_matrix(const float src[3][3], float scalar, float dest[3][3]);

/**
 * Multiply two matrices (3x3) a and b, result in dest.
 */
void multiply_matrices(const float a[3][3], const float b[3][3], float dest[3][3]);

/**
 * Transform a homogeneous 2D point by a 3x3 matrix.
 * point and result are arrays of length 3.
 */
void transform_point(const float matrix[3][3], const float point[3], float result[3]);

/**
 * Modify the first parameter matrix to add a scaling (about origin).
 */
void scale(float matrix[3][3], float sx, float sy);

/**
 * Modify the first parameter matrix to add a translation.
 */
void shift(float matrix[3][3], float dx, float dy);

/**
 * Modify the first parameter matrix to add a rotation (about origin).
 */
void rotate(float matrix[3][3], float angle_radians);

#endif // MATRIX_H

