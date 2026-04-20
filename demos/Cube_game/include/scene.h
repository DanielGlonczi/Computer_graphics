#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"

#include <obj/model.h>

#define MAX_SCENE_MODELS 8

typedef struct ModelInstance
{
    vec3 position;
    vec3 rotation;
    vec3 scale;
    bool can_move;
    bool rotates;
    bool is_rolling;
    vec3 target_position;
    vec3 move_direction;
    vec3 roll_axis;
    vec3 pivot_center;
    bool will_fall;
    bool is_falling;
    float fall_velocity;
    float remaining_angle;
    float animation_angle;
} ModelInstance;

typedef struct Scene
{
    Model models[MAX_SCENE_MODELS];
    ModelInstance instances[MAX_SCENE_MODELS];
    int model_count;
    int active_model;
    Material material;
    GLuint texture_id;
} Scene;

/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene* scene);

/**
 * Move the currently active model in the scene.
 */
void move_active_model(Scene* scene, int dx, int dy);

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
void update_scene(Scene* scene, double delta_time);

/**
 * Render the scene objects.
 */
void render_scene(const Scene* scene);

/**
 * Draw the origin of the world coordinate system.
 */
void draw_origin();

#endif /* SCENE_H */
