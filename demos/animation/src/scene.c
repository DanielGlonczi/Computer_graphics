#include "scene.h"

#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>

static void set_object_color(float red, float green, float blue)
{
    GLfloat ambient_material_color[] = { red * 0.16f, green * 0.16f, blue * 0.16f, 1.0f };
    GLfloat diffuse_material_color[] = { red, green, blue, 1.0f };
    GLfloat specular_material_color[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat shininess = 64.0f;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);
}

static void draw_box(float width, float height, float depth)
{
    const float w = width * 0.5f;
    const float h = height * 0.5f;
    const float d = depth * 0.5f;

    glBegin(GL_QUADS);
    // front
    glNormal3f(0, 0, 1);
    glVertex3f(-w, -h, d);
    glVertex3f(w, -h, d);
    glVertex3f(w, h, d);
    glVertex3f(-w, h, d);
    // back
    glNormal3f(0, 0, -1);
    glVertex3f(w, -h, -d);
    glVertex3f(-w, -h, -d);
    glVertex3f(-w, h, -d);
    glVertex3f(w, h, -d);
    // left
    glNormal3f(-1, 0, 0);
    glVertex3f(-w, -h, -d);
    glVertex3f(-w, -h, d);
    glVertex3f(-w, h, d);
    glVertex3f(-w, h, -d);
    // right
    glNormal3f(1, 0, 0);
    glVertex3f(w, -h, d);
    glVertex3f(w, -h, -d);
    glVertex3f(w, h, -d);
    glVertex3f(w, h, d);
    // top
    glNormal3f(0, 1, 0);
    glVertex3f(-w, h, d);
    glVertex3f(w, h, d);
    glVertex3f(w, h, -d);
    glVertex3f(-w, h, -d);
    // bottom
    glNormal3f(0, -1, 0);
    glVertex3f(-w, -h, -d);
    glVertex3f(w, -h, -d);
    glVertex3f(w, -h, d);
    glVertex3f(-w, -h, d);
    glEnd();
}

static void draw_sphere(float radius, int slices, int stacks)
{
    for (int i = 0; i < stacks; ++i) {
        const float phi1 = (float)M_PI * ((float)i / stacks - 0.5f);
        const float phi2 = (float)M_PI * ((float)(i + 1) / stacks - 0.5f);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; ++j) {
            const float theta = 2.0f * (float)M_PI * ((float)j / slices);
            const float x1 = cosf(phi1) * cosf(theta);
            const float y1 = sinf(phi1);
            const float z1 = cosf(phi1) * sinf(theta);
            const float x2 = cosf(phi2) * cosf(theta);
            const float y2 = sinf(phi2);
            const float z2 = cosf(phi2) * sinf(theta);

            glNormal3f(x1, y1, z1);
            glVertex3f(radius * x1, radius * y1, radius * z1);
            glNormal3f(x2, y2, z2);
            glVertex3f(radius * x2, radius * y2, radius * z2);
        }
        glEnd();
    }
}

static void draw_floor(float size)
{
    set_object_color(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(-size, 0, -size);
    glVertex3f(size, 0, -size);
    glVertex3f(size, 0, size);
    glVertex3f(-size, 0, size);
    glEnd();
}

static void render_pendulum(const Scene* scene)
{
    const double period = 3.0;
    const double amplitude = 40.0 * M_PI / 180.0;
    const double theta = amplitude * sin(M_PI * scene->total_time / period - M_PI_2);
    const double length = 1.3;
    const double pivot_y = 1.7;
    const double bob_x = sin(theta) * length;
    const double bob_y = pivot_y - cos(theta) * length;

    set_object_color(0.8f, 0.6f, 0.2f);
    draw_floor(2.2f);

    glLineWidth(4.0f);
    glBegin(GL_LINES);
    glColor3f(0.8f, 0.8f, 0.8f);
    glVertex3f(0.0f, pivot_y, 0.0f);
    glVertex3f((float)bob_x, (float)bob_y, 0.0f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.0f, (float)pivot_y, 0.0f);
    set_object_color(0.2f, 0.6f, 0.8f);
    draw_sphere(0.06f, 12, 12);
    glPopMatrix();

    glPushMatrix();
    glTranslatef((float)bob_x, (float)bob_y, 0.0f);
    set_object_color(0.8f, 0.2f, 0.2f);
    draw_sphere(0.15f, 20, 20);
    glPopMatrix();
}

static void render_bouncing_balls(const Scene* scene)
{
    draw_floor(2.5f);

    glPushMatrix();
    glTranslatef(-0.9f, (float)scene->ball_ideal_y, 0.0f);
    set_object_color(0.2f, 0.8f, 0.3f);
    draw_sphere((float)scene->ball_radius, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.9f, (float)scene->ball_nonideal_y, 0.0f);
    set_object_color(0.8f, 0.3f, 0.2f);
    draw_sphere((float)scene->ball_radius, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.25f, 0.0f);
    set_object_color(0.9f, 0.9f, 0.2f);
    glScalef(1.2f, 0.05f, 1.2f);
    draw_box(1.0f, 1.0f, 1.0f);
    glPopMatrix();
}

static void render_quadruped(const Scene* scene)
{
    const double phase = scene->total_time * 2.5;
    const double leg_swing = 18.0 * M_PI / 180.0;
    const double body_bob = 0.05 * sin(scene->total_time * 3.0);
    const double front_left = sin(phase) * leg_swing;
    const double back_right = sin(phase) * leg_swing;
    const double front_right = sin(phase + M_PI) * leg_swing;
    const double back_left = sin(phase + M_PI) * leg_swing;

    draw_floor(3.0f);

    glPushMatrix();
    glTranslatef(0.0f, 0.8f + (float)body_bob, 0.0f);
    set_object_color(0.4f, 0.3f, 0.2f);
    glScalef(1.2f, 0.4f, 0.6f);
    draw_box(1.0f, 1.0f, 1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.55f, 0.85f + (float)body_bob, 0.3f);
    glRotatef((float)(front_left * 180.0 / M_PI), 1, 0, 0);
    glTranslatef(0.0f, -0.25f, 0.0f);
    set_object_color(0.7f, 0.4f, 0.3f);
    glScalef(0.18f, 0.5f, 0.18f);
    draw_box(1.0f, 1.0f, 1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.55f, 0.85f + (float)body_bob, 0.3f);
    glRotatef((float)(front_right * 180.0 / M_PI), 1, 0, 0);
    glTranslatef(0.0f, -0.25f, 0.0f);
    set_object_color(0.7f, 0.4f, 0.3f);
    glScalef(0.18f, 0.5f, 0.18f);
    draw_box(1.0f, 1.0f, 1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.55f, 0.85f + (float)body_bob, -0.3f);
    glRotatef((float)(back_left * 180.0 / M_PI), 1, 0, 0);
    glTranslatef(0.0f, -0.25f, 0.0f);
    set_object_color(0.7f, 0.4f, 0.3f);
    glScalef(0.18f, 0.5f, 0.18f);
    draw_box(1.0f, 1.0f, 1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.55f, 0.85f + (float)body_bob, -0.3f);
    glRotatef((float)(back_right * 180.0 / M_PI), 1, 0, 0);
    glTranslatef(0.0f, -0.25f, 0.0f);
    set_object_color(0.7f, 0.4f, 0.3f);
    glScalef(0.18f, 0.5f, 0.18f);
    draw_box(1.0f, 1.0f, 1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.9f, 1.0f + (float)body_bob, 0.0f);
    set_object_color(0.4f, 0.3f, 0.2f);
    glScalef(0.3f, 0.25f, 0.25f);
    draw_box(1.0f, 1.0f, 1.0f);
    glPopMatrix();
}

static void reset_ball_states(Scene* scene)
{
    scene->ball_radius = 0.18;
    scene->ball_floor = 0.18;
    scene->ball_ideal_restitution = 1.0;
    scene->ball_nonideal_restitution = 0.75;
    scene->ball_ideal_y = 2.3;
    scene->ball_ideal_v = 0.0;
    scene->ball_nonideal_y = 2.3;
    scene->ball_nonideal_v = 0.0;
}

void set_scene_animation_mode(Scene* scene, int animation_mode)
{
    scene->animation_mode = animation_mode;
    scene->total_time = 0.0;
    reset_ball_states(scene);
    if (animation_mode == 1) {
        printf("Animation mode: pendulum\n");
    } else if (animation_mode == 2) {
        printf("Animation mode: bouncing balls (ideal + non-ideal)\n");
    } else if (animation_mode == 3) {
        printf("Animation mode: quadruped walk\n");
    } else {
        printf("Animation mode: unknown, defaulting to pendulum\n");
    }
}

void init_scene(Scene* scene)
{
    scene->material.ambient.red = 0.2;
    scene->material.ambient.green = 0.2;
    scene->material.ambient.blue = 0.2;

    scene->material.diffuse.red = 0.9;
    scene->material.diffuse.green = 0.9;
    scene->material.diffuse.blue = 0.9;

    scene->material.specular.red = 0.5;
    scene->material.specular.green = 0.5;
    scene->material.specular.blue = 0.5;

    scene->material.shininess = 32.0f;

    set_scene_animation_mode(scene, 1);
}

void set_lighting()
{
    GLfloat ambient_light[] = { 0.15f, 0.15f, 0.15f, 1.0f };
    GLfloat diffuse_light[] = { 0.9f, 0.9f, 0.9f, 1.0f };
    GLfloat specular_light[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat position[] = { 0.0f, 3.0f, 4.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void set_material(const Material* material)
{
    GLfloat ambient_material_color[] = {
        (GLfloat)material->ambient.red,
        (GLfloat)material->ambient.green,
        (GLfloat)material->ambient.blue,
        1.0f
    };

    GLfloat diffuse_material_color[] = {
        (GLfloat)material->diffuse.red,
        (GLfloat)material->diffuse.green,
        (GLfloat)material->diffuse.blue,
        1.0f
    };

    GLfloat specular_material_color[] = {
        (GLfloat)material->specular.red,
        (GLfloat)material->specular.green,
        (GLfloat)material->specular.blue,
        1.0f
    };

    GLfloat shininess = (GLfloat)material->shininess;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);
}

void update_scene(Scene* scene, double elapsed_time)
{
    scene->total_time += elapsed_time;

    const double gravity = -9.81;
    scene->ball_ideal_v += gravity * elapsed_time;
    scene->ball_ideal_y += scene->ball_ideal_v * elapsed_time;
    if (scene->ball_ideal_y < scene->ball_radius) {
        scene->ball_ideal_y = scene->ball_radius;
        scene->ball_ideal_v = -scene->ball_ideal_v * scene->ball_ideal_restitution;
    }

    scene->ball_nonideal_v += gravity * elapsed_time;
    scene->ball_nonideal_y += scene->ball_nonideal_v * elapsed_time;
    if (scene->ball_nonideal_y < scene->ball_radius) {
        scene->ball_nonideal_y = scene->ball_radius;
        scene->ball_nonideal_v = -scene->ball_nonideal_v * scene->ball_nonideal_restitution;
        if (fabs(scene->ball_nonideal_v) < 0.05) {
            scene->ball_nonideal_v = 0.0;
        }
    }
}

void render_scene(const Scene* scene)
{
    set_material(&(scene->material));
    set_lighting();
    draw_origin();

    switch (scene->animation_mode) {
    case 1:
        render_pendulum(scene);
        break;
    case 2:
        render_bouncing_balls(scene);
        break;
    case 3:
        render_quadruped(scene);
        break;
    default:
        render_pendulum(scene);
        break;
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
