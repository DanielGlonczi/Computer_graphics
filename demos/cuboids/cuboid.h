#ifndef CUBOID_H
#define CUBOID_H

typedef struct {
    double length;
    double width;
    double height;
} Cuboid;

void set_size(Cuboid *c, double length, double width, double height);

double calc_volume(const Cuboid *c);

double calc_surface(const Cuboid *c);

int has_square_face(const Cuboid *c);

#endif
