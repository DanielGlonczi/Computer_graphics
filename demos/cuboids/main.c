#include <stdio.h>
#include "cuboid.h"

int main() {
    Cuboid box;

    set_size(&box, 3.0, 4.0, 3.0);
    
    printf("Cuboid dimensions: %.2f x %.2f x %.2f\n", box.length, box.width, box.height);
    printf("Volume: %.2f\n", calc_volume(&box));
    printf("Surface area: %.2f\n", calc_surface(&box));

    if(has_square_face(&box)) {
        printf("This cuboid has at least one square face\n");
    } else {
        printf("No square faces\n");
    }

    return 0;
}
