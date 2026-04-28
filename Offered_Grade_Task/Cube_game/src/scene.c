#include "scene.h"

#include <obj/load.h>
#include <obj/draw.h>
#include <stdio.h>

static const int LEVEL_WIDTH = 16;
static const int LEVEL_HEIGHT = 16;
static const int LEVEL_START_X = 1;
static const int LEVEL_START_Y = 1;
static int level[16][16] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

static bool is_valid_tile(int x, int y)
{
    return x >= 0 && x < LEVEL_WIDTH && y >= 0 && y < LEVEL_HEIGHT && (level[y][x] == 1 || level[y][x] == 3 || level[y][x] == 4);
}

static void draw_tile(int x, int y, int type)
{
    const float tile_z = -2.5f;
    const float half = 0.5f;
    const float height = 1.0f;

    Material mat;
    if (type == 1)
    {
        mat = (Material){.ambient = {0.10f, 0.30f, 0.45f}, .diffuse = {0.15f, 0.50f, 0.75f}, .specular = {0.10f, 0.10f, 0.10f}, .shininess = 16.0f};
    }
    else if (type == 3)
    {
        mat = (Material){.ambient = {0.0f, 0.5f, 0.0f}, .diffuse = {0.0f, 1.0f, 0.0f}, .specular = {0.0f, 0.0f, 0.0f}, .shininess = 16.0f}; // Green for finish
    }
    else if (type == 4)
    {
        mat = (Material){.ambient = {0.5f, 0.5f, 0.0f}, .diffuse = {1.0f, 1.0f, 0.0f}, .specular = {0.0f, 0.0f, 0.0f}, .shininess = 16.0f}; // Yellow for collectible
    }
    else if (type == 5)
    {
        mat = (Material){.ambient = {0.5f, 0.0f, 0.0f}, .diffuse = {1.0f, 0.0f, 0.0f}, .specular = {0.0f, 0.0f, 0.0f}, .shininess = 16.0f}; // Red for danger
    }
    else
    {
        mat = (Material){.ambient = {0.10f, 0.30f, 0.45f}, .diffuse = {0.15f, 0.50f, 0.75f}, .specular = {0.10f, 0.10f, 0.10f}, .shininess = 16.0f};
    }
    set_material(&mat);

    glBegin(GL_QUADS);
    // Top
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f((float)x - half, (float)y - half, tile_z);
    glVertex3f((float)x + half, (float)y - half, tile_z);
    glVertex3f((float)x + half, (float)y + half, tile_z);
    glVertex3f((float)x - half, (float)y + half, tile_z);
    // Bottom
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f((float)x - half, (float)y - half, tile_z - height);
    glVertex3f((float)x + half, (float)y - half, tile_z - height);
    glVertex3f((float)x + half, (float)y + half, tile_z - height);
    glVertex3f((float)x - half, (float)y + half, tile_z - height);
    // Front
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f((float)x - half, (float)y + half, tile_z);
    glVertex3f((float)x + half, (float)y + half, tile_z);
    glVertex3f((float)x + half, (float)y + half, tile_z - height);
    glVertex3f((float)x - half, (float)y + half, tile_z - height);
    // Back
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f((float)x - half, (float)y - half, tile_z);
    glVertex3f((float)x + half, (float)y - half, tile_z);
    glVertex3f((float)x + half, (float)y - half, tile_z - height);
    glVertex3f((float)x - half, (float)y - half, tile_z - height);
    // Left
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f((float)x - half, (float)y - half, tile_z);
    glVertex3f((float)x - half, (float)y + half, tile_z);
    glVertex3f((float)x - half, (float)y + half, tile_z - height);
    glVertex3f((float)x - half, (float)y - half, tile_z - height);
    // Right
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f((float)x + half, (float)y - half, tile_z);
    glVertex3f((float)x + half, (float)y + half, tile_z);
    glVertex3f((float)x + half, (float)y + half, tile_z - height);
    glVertex3f((float)x + half, (float)y - half, tile_z - height);
    glEnd();
}

static void draw_floor_grid(void)
{
    glDisable(GL_LIGHTING);
    glColor3f(0.45f, 0.45f, 0.45f);
    glBegin(GL_LINES);
    for (int i = -5; i <= 5; ++i)
    {
        glVertex3f((float)i, -5.0f, -2.5f);
        glVertex3f((float)i, 5.0f, -2.5f);
        glVertex3f(-5.0f, (float)i, -2.5f);
        glVertex3f(5.0f, (float)i, -2.5f);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

static void draw_floor_tiles(bool finished)
{
    for (int y = 0; y < LEVEL_HEIGHT; ++y)
    {
        for (int x = 0; x < LEVEL_WIDTH; ++x)
        {
            int type = level[y][x];
            if (type == 1 || type == 3 || type == 4 || type == 5)
            {
                draw_tile(x, y, finished ? 3 : type);
            }
        }
    }
}

static void draw_instance(const Model *model, const ModelInstance *inst)
{
    glPushMatrix();

    if (inst->is_rolling)
    {
        glTranslatef(inst->pivot_center.x, inst->pivot_center.y, inst->pivot_center.z);
        glRotatef(inst->animation_angle, inst->roll_axis.x, inst->roll_axis.y, inst->roll_axis.z);
        glTranslatef(-inst->pivot_center.x, -inst->pivot_center.y, -inst->pivot_center.z);
    }

    glTranslatef(inst->position.x, inst->position.y, inst->position.z);
    glRotatef(inst->rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(inst->rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(inst->rotation.z, 0.0f, 0.0f, 1.0f);
    glScalef(inst->scale.x, inst->scale.y, inst->scale.z);
    draw_model(model);
    glPopMatrix();
}

static float minf(float a, float b)
{
    return (a < b) ? a : b;
}

static void start_cube_roll(ModelInstance *instance, int dx, int dy, bool will_fall)
{
    if (instance->is_rolling || instance->is_falling)
    {
        return;
    }

    instance->move_direction = (vec3){(float)dx, (float)dy, 0.0f};
    instance->roll_axis = (vec3){-(float)dy, (float)dx, 0.0f};
    instance->remaining_angle = 90.0f;
    instance->animation_angle = 0.0f;
    instance->target_position = (vec3){
        instance->position.x + (float)dx,
        instance->position.y + (float)dy,
        instance->position.z};
    instance->pivot_center = (vec3){
        instance->position.x + 0.5f * instance->move_direction.x,
        instance->position.y + 0.5f * instance->move_direction.y,
        instance->position.z - 0.5f};
    instance->will_fall = will_fall;
    instance->is_rolling = true;
}

void move_active_model(Scene *scene, int dx, int dy)
{
    if (scene->active_model < 0 || scene->active_model >= scene->model_count)
    {
        return;
    }

    ModelInstance *instance = &scene->instances[scene->active_model];
    if (!instance->can_move || instance->is_rolling || instance->is_falling)
    {
        return;
    }

    int next_x = (int)(instance->position.x + dx);
    int next_y = (int)(instance->position.y + dy);
    bool valid_tile = is_valid_tile(next_x, next_y);

    start_cube_roll(instance, dx, dy, !valid_tile);
}

void init_scene(Scene *scene)
{
    scene->model_count = 0;
    scene->active_model = 0;
    scene->collected_count = 0;
    scene->time_counter = 0.0f;
    scene->finished = false;

    if (load_model(&(scene->models[scene->model_count]), "assets/models/cube.obj") == TRUE)
    {
        scene->instances[scene->model_count].position = (vec3){(float)LEVEL_START_X, (float)LEVEL_START_Y, -2.0f};
        scene->instances[scene->model_count].rotation = (vec3){0.0f, 0.0f, 0.0f};
        scene->instances[scene->model_count].scale = (vec3){1.0f, 1.0f, 1.0f};
        scene->instances[scene->model_count].can_move = true;
        scene->instances[scene->model_count].rotates = false;
        scene->instances[scene->model_count].is_rolling = false;
        scene->instances[scene->model_count].will_fall = false;
        scene->instances[scene->model_count].is_falling = false;
        scene->instances[scene->model_count].fall_velocity = 0.0f;
        scene->instances[scene->model_count].target_position = scene->instances[scene->model_count].position;
        scene->instances[scene->model_count].move_direction = (vec3){0.0f, 0.0f, 0.0f};
        scene->instances[scene->model_count].roll_axis = (vec3){0.0f, 0.0f, 0.0f};
        scene->instances[scene->model_count].pivot_center = (vec3){0.0f, 0.0f, 0.0f};
        scene->instances[scene->model_count].remaining_angle = 0.0f;
        scene->instances[scene->model_count].animation_angle = 0.0f;
        scene->model_count++;
    }

    scene->material.ambient = (Color){0.2f, 0.2f, 0.2f};
    scene->material.diffuse = (Color){0.8f, 0.4f, 0.2f};
    scene->material.specular = (Color){0.5f, 0.5f, 0.5f};
    scene->material.shininess = 32.0f;
}

void set_lighting()
{
    float ambient_light[] = {0.2f, 0.2f, 0.2f, 1.0f};
    float diffuse_light[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float specular_light[] = {0.7f, 0.7f, 0.7f, 1.0f};
    float position[] = {0.0f, 4.0f, 4.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void set_material(const Material *material)
{
    float ambient_material_color[] = {material->ambient.red, material->ambient.green, material->ambient.blue};
    float diffuse_material_color[] = {material->diffuse.red, material->diffuse.green, material->diffuse.blue};
    float specular_material_color[] = {material->specular.red, material->specular.green, material->specular.blue};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void update_scene(Scene *scene, double delta_time)
{
    for (int i = 0; i < scene->model_count; ++i)
    {
        ModelInstance *instance = &scene->instances[i];

        if (instance->is_rolling)
        {
            float step_angle = minf(180.0f * (float)delta_time, instance->remaining_angle);
            instance->animation_angle += step_angle;
            instance->remaining_angle -= step_angle;

            if (instance->remaining_angle <= 0.0f)
            {
                instance->is_rolling = false;
                instance->animation_angle = 0.0f;
                instance->position = instance->target_position;
                instance->rotation.x += instance->roll_axis.x * 90.0f;
                instance->rotation.y += instance->roll_axis.y * 90.0f;
                instance->rotation.z += instance->roll_axis.z * 90.0f;

                if (instance->rotation.x < 0.0f)
                    instance->rotation.x += 360.0f;
                if (instance->rotation.x >= 360.0f)
                    instance->rotation.x -= 360.0f;
                if (instance->rotation.y < 0.0f)
                    instance->rotation.y += 360.0f;
                if (instance->rotation.y >= 360.0f)
                    instance->rotation.y -= 360.0f;
                if (instance->rotation.z < 0.0f)
                    instance->rotation.z += 360.0f;
                if (instance->rotation.z >= 360.0f)
                    instance->rotation.z -= 360.0f;

                if (i == scene->active_model && !scene->finished && (int)instance->position.x == 14 && (int)instance->position.y == 14)
                {
                    scene->finished = true;
                }

                if (instance->will_fall)
                {
                    instance->is_falling = true;
                    instance->fall_velocity = 0.0f;
                }
            }
        }

        if (instance->is_falling)
        {
            instance->fall_velocity += 9.8f * (float)delta_time;
            instance->position.z -= instance->fall_velocity * (float)delta_time;
            if (instance->position.z < -7.0f)
            {
                instance->position.x = (float)LEVEL_START_X;
                instance->position.y = (float)LEVEL_START_Y;
                instance->position.z = -2.0f;
                instance->rotation = (vec3){0.0f, 0.0f, 0.0f};
                instance->is_falling = false;
                instance->will_fall = false;
                instance->is_rolling = false;
                instance->fall_velocity = 0.0f;
            }
        }
    }
}

void render_scene(const Scene *scene)
{
    set_material(&(scene->material));
    set_lighting();
    draw_origin();
    draw_floor_tiles(scene->finished);
    draw_floor_grid();

    for (int i = 0; i < scene->model_count; ++i)
    {
        draw_instance(&(scene->models[i]), &(scene->instances[i]));
    }

    if (scene->finished)
    {
        draw_green_dots();
    }
}

void draw_origin()
{
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);
    glEnd();
    glEnable(GL_LIGHTING);
}

void draw_green_dots()
{
    glDisable(GL_LIGHTING);
    glColor3f(0.0f, 1.0f, 0.0f);
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    // Add some green dots at various positions
    glVertex3f(-5.0f, -5.0f, -1.0f);
    glVertex3f(5.0f, -5.0f, -1.0f);
    glVertex3f(-5.0f, 5.0f, -1.0f);
    glVertex3f(5.0f, 5.0f, -1.0f);
    glVertex3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-3.0f, 2.0f, -1.0f);
    glVertex3f(3.0f, -2.0f, -1.0f);
    glVertex3f(-2.0f, -3.0f, -1.0f);
    glVertex3f(2.0f, 3.0f, -1.0f);
    glVertex3f(1.0f, -4.0f, -1.0f);
    glVertex3f(-1.0f, 4.0f, -1.0f);
    glVertex3f(4.0f, 1.0f, -1.0f);
    glVertex3f(-4.0f, -1.0f, -1.0f);
    glVertex3f(0.5f, 2.5f, -1.0f);
    glVertex3f(-0.5f, -2.5f, -1.0f);
    glEnd();
    glEnable(GL_LIGHTING);
}
