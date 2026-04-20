#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"
#include "utils.h"

#include <GL/gl.h>

typedef struct Scene
{
    Material material;
    int animation_mode;
    double total_time;
    double ball_ideal_y;
    double ball_ideal_v;
    double ball_nonideal_y;
    double ball_nonideal_v;
    double ball_radius;
    double ball_floor;
    double ball_ideal_restitution;
    double ball_nonideal_restitution;
} Scene;

/**
 * Initialize the scene.
 */
void init_scene(Scene* scene);

/**
 * Set the lighting of the scene.
 */
void set_lighting();

/**
 * Set the current material.
 */
void set_material(const Material* material);

/**
 * Update the scene.
 */
void update_scene(Scene* scene, double elapsed_time);

/**
 * Render the scene objects.
 */
void render_scene(const Scene* scene);

/**
 * Set the current animation mode.
 */
void set_scene_animation_mode(Scene* scene, int animation_mode);

/**
 * Draw the origin of the world coordinate system.
 */
void draw_origin();

#endif /* SCENE_H */
