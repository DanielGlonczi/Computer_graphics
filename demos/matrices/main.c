#include "matrix.h"
#include <stdio.h>

void print_point(const char *label, const float p[3])
{
    printf("%s: (%f, %f, %f)\n", label, p[0], p[1], p[2]);
}

int main(int argc, char* argv[])
{
    float a[3][3] = {
        { 1.0f, -2.0f,  3.0f},
        { 5.0f, -3.0f,  0.0f},
        {-2.0f,  1.0f, -4.0f}
    };
    float b[3][3];
    float c[3][3];

    init_zero_matrix(b);
    b[1][1] =  8.0f;
    b[2][0] = -3.0f;
    b[2][2] =  5.0f;

    printf("matrix a:\n"); print_matrix(a);
    printf("matrix b (zero + some entries):\n"); print_matrix(b);

    add_matrices(a, b, c);
    printf("a + b =\n"); print_matrix(c);

    init_identity_matrix(c);
    printf("identity matrix:\n"); print_matrix(c);

    scale_matrix(a, 2.0f, c);
    printf("a * 2:\n"); print_matrix(c);

    multiply_matrices(a, b, c);
    printf("a * b:\n"); print_matrix(c);

    float pt[3] = {1.0f, 2.0f, 1.0f};
    float res[3];
    transform_point(a, pt, res);
    print_point("a * pt", res);

    float t[3][3];
    init_identity_matrix(t);
    scale(t, 2.0f, 2.0f);
    rotate(t, 3.14159265f / 4.0f);
    shift(t, 3.0f, 4.0f);
    printf("composite transform t:\n"); print_matrix(t);

    transform_point(t, pt, res);
    print_point("t * pt", res);

    return 0;
}

