#ifndef SCENE_H
#define SCENE_H

#include "camera.h"

#include <GL/gl.h>

typedef struct Scene
{
    double time_elapsed;
    GLuint checker_texture;
    GLuint circle_texture;
    GLuint gradient_texture;
    GLuint image_texture;
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
 * Draw a cylinder approximation with moving texture coordinates.
 */
void draw_cylinder_with_moving_tex(float radius, float height, int segments, double time);

/**
 * Draw a cone approximation using triangle fan.
 */
void draw_cone(float radius, float height, int segments);

/**
 * Generate checkerboard texture.
 */
void generate_checker_texture(GLuint* texture);

/**
 * Generate circle texture.
 */
void generate_circle_texture(GLuint* texture);

/**
 * Generate gradient texture.
 */
void generate_gradient_texture(GLuint* texture);

/**
 * Load image texture (for demonstration, we'll generate a simple one).
 */
void generate_image_texture(GLuint* texture);

/**
 * Render a textured quad with orthogonal projection.
 */
void render_image_overlay(const Scene* scene);

#endif /* SCENE_H */
