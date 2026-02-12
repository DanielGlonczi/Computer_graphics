#include <stdio.h>
#include "cuboid.h"

void set_size(Cuboid *c, double length, double width, double height) {
    if(length <= 0 || width <= 0 || height <= 0) {
        printf("Error: All dimensions must be positive!\n");
        return;
    }
    c->length = length;
    c->width = width;
    c->height = height;
}

double calc_volume(const Cuboid *c) {
    return c->length * c->width * c->height;
}

double calc_surface(const Cuboid *c) {
    return 2 * (c->length * c->width + c->length * c->height + c->width * c->height);
}

int has_square_face(const Cuboid *c) {
    return (c->length == c->width) || (c->length == c->height) || (c->width == c->height);
}
