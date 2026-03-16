#ifndef SCENE_H
#define SCENE_H

#include "camera.h"

typedef struct Scene
{
    double time_elapsed;
} Scene;

/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene* scene);

/**
 * Update the scene.
 */
void update_scene(Scene* scene);

/**
 * Render the scene objects.
 */
void render_scene(const Scene* scene);

/**
 * Draw the origin of the world coordinate system.
 */
void draw_origin();

/**
 * Draw a colored triangle with yellow, cyan, and magenta vertices.
 */
void draw_colored_triangle();

/**
 * Draw a tessellated sphere.
 */
void draw_sphere(float radius, int slices, int stacks);

/**
 * Draw a checkerboard pattern on the x-y plane at z=0.
 */
void draw_checkerboard(float size, int divisions);

/**
 * Draw a cylinder approximation using triangle strips.
 */
void draw_cylinder(float radius, float height, int segments);

/**
 * Draw a cone approximation using triangle fan.
 */
void draw_cone(float radius, float height, int segments);

#endif /* SCENE_H */
