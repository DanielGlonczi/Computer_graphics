#include "camera.h"

#include <GL/gl.h>

#include <math.h>

void init_camera(Camera *camera)
{
    camera->position.x = 0.0;
    camera->position.y = 0.0;
    camera->position.z = 1.0;
    camera->rotation.x = 0.0;
    camera->rotation.y = 0.0;
    camera->rotation.z = 0.0;
    camera->speed.x = 0.0;
    camera->speed.y = 0.0;
    camera->speed.z = 0.0;
    camera->target_position = camera->position;
}

void update_camera(Camera *camera, double time)
{
    // Smooth follow the target
    float smooth_factor = 0.05f;
    camera->position.x += (camera->target_position.x - camera->position.x) * smooth_factor;
    camera->position.y += (camera->target_position.y - camera->position.y) * smooth_factor;
    camera->position.z += (camera->target_position.z - camera->position.z) * smooth_factor;

    double angle;
    double side_angle;

    angle = degree_to_radian(camera->rotation.z);
    side_angle = degree_to_radian(camera->rotation.z + 90.0);

    camera->position.x += cos(angle) * camera->speed.y * time;
    camera->position.y += sin(angle) * camera->speed.y * time;
    camera->position.x += cos(side_angle) * camera->speed.x * time;
    camera->position.y += sin(side_angle) * camera->speed.x * time;
}

void set_view(const Camera *camera)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(-(camera->rotation.x + 90), 1.0, 0, 0);
    glRotatef(-(camera->rotation.z - 90), 0, 0, 1.0);
    glTranslatef(-camera->position.x, -camera->position.y, -camera->position.z);
}

void rotate_camera(Camera *camera, double horizontal, double vertical)
{
    camera->rotation.z += horizontal;
    camera->rotation.x += vertical;

    if (camera->rotation.z < 0)
    {
        camera->rotation.z += 360.0;
    }

    if (camera->rotation.z > 360.0)
    {
        camera->rotation.z -= 360.0;
    }

    if (camera->rotation.x < 0)
    {
        camera->rotation.x += 360.0;
    }

    if (camera->rotation.x > 360.0)
    {
        camera->rotation.x -= 360.0;
    }
}

void set_camera_speed(Camera *camera, double speed)
{
    camera->speed.y = speed;
}

void set_camera_side_speed(Camera *camera, double speed)
{
    camera->speed.x = speed;
}


