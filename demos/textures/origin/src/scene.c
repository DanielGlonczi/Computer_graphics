#include "scene.h"

#include <GL/gl.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

void init_scene(Scene* scene)
{
    scene->time_elapsed = 0.0;

    generate_checker_texture(&(scene->checker_texture));
    generate_circle_texture(&(scene->circle_texture));
    generate_gradient_texture(&(scene->gradient_texture));
    generate_image_texture(&(scene->image_texture));
}

void update_scene(Scene* scene)
{
    (void)scene;  /* Unused parameter */
}

void render_scene(const Scene* scene)
{
    // Draw textured checkerboard
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, scene->checker_texture);
    glColor3f(1.0f, 1.0f, 1.0f); // Ensure white color for texture
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
    glTexCoord2f(8, 0); glVertex3f(1, 0, 0);
    glTexCoord2f(8, 8); glVertex3f(1, 1, 0);
    glTexCoord2f(0, 8); glVertex3f(0, 1, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    
    glDisable(GL_LIGHTING);
    
    draw_origin();

    glPushMatrix();
    glTranslatef(0.1f, 0.1f, 0.15f);
    draw_colored_triangle();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.5f, 0.2f, 0.15f);
    glRotatef((float)(scene->time_elapsed * 50), 1.0f, 1.0f, 1.0f);
    // Time-based texture switching
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    if ((int)scene->time_elapsed % 4 < 2) {
        glBindTexture(GL_TEXTURE_2D, scene->checker_texture);
    } else {
        glBindTexture(GL_TEXTURE_2D, scene->gradient_texture);
    }
    draw_sphere(0.15f, 12, 12);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.2f, 0.2f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    // Moving (u,v) coordinates
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, scene->gradient_texture);
    draw_cylinder_with_moving_tex(0.1f, 0.3f, 16, scene->time_elapsed);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.8f, 0.8f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, scene->circle_texture);
    draw_cone(0.1f, 0.3f, 16);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    glEnable(GL_LIGHTING);
}

void draw_origin()
{
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
}

void draw_colored_triangle()
{
    glBegin(GL_TRIANGLES);
    
    glColor3f(1.0f, 1.0f, 0.0f);  /* Yellow */
    glVertex3f(0.2f, 0.2f, 0.3f);
    
    glColor3f(0.0f, 1.0f, 1.0f);  /* Cyan */
    glVertex3f(0.8f, 0.2f, 0.3f);
    
    glColor3f(1.0f, 0.0f, 1.0f);  /* Magenta */
    glVertex3f(0.5f, 0.8f, 0.3f);
    
    glEnd();
}

void draw_sphere(float radius, int slices, int stacks)
{
    int i, j;
    float lat0, lat1;
    float lng0, lng1;
    float x0, y0, z0, x1, y1, z1;
    
    for (i = 0; i < stacks; ++i) {
        lat0 = M_PI * (-0.5f + (float)i / stacks);
        lat1 = M_PI * (-0.5f + (float)(i + 1) / stacks);
        
        glBegin(GL_QUAD_STRIP);
        for (j = 0; j <= slices; ++j) {
            lng0 = 2.0f * M_PI * (float)j / slices;
            lng1 = 2.0f * M_PI * (float)j / slices;
            
            x0 = cosf(lat0) * cosf(lng0);
            y0 = sinf(lat0);
            z0 = cosf(lat0) * sinf(lng0);
            
            glTexCoord2f((float)j / slices, (float)i / stacks);
            glVertex3f(x0 * radius, y0 * radius, z0 * radius);
            
            x1 = cosf(lat1) * cosf(lng1);
            y1 = sinf(lat1);
            z1 = cosf(lat1) * sinf(lng1);
            glTexCoord2f((float)j / slices, (float)(i + 1) / stacks);
            glVertex3f(x1 * radius, y1 * radius, z1 * radius);
        }
        glEnd();
    }
}

void draw_checkerboard(float size, int divisions)
{
    int i, j;
    float cell_size = size / divisions;
    
    glDisable(GL_LIGHTING);
    
    for (i = 0; i < divisions; ++i) {
        for (j = 0; j < divisions; ++j) {
            if ((i + j) % 2 == 0) {
                glColor3f(0.8f, 0.8f, 0.8f);  /* Light gray */
            } else {
                glColor3f(0.2f, 0.2f, 0.2f);  /* Dark gray */
            }
            
            glBegin(GL_QUADS);
            glVertex3f(i * cell_size, j * cell_size, 0.0f);
            glVertex3f((i + 1) * cell_size, j * cell_size, 0.0f);
            glVertex3f((i + 1) * cell_size, (j + 1) * cell_size, 0.0f);
            glVertex3f(i * cell_size, (j + 1) * cell_size, 0.0f);
            glEnd();
        }
    }
    
    glEnable(GL_LIGHTING);
}

void draw_cylinder(float radius, float height, int segments)
{
    int i;
    float angle, x, z;
    
    glColor3f(0.8f, 0.4f, 0.2f);  /* Orange */
    
    /* Side surface using triangle strips */
    glBegin(GL_TRIANGLE_STRIP);
    for (i = 0; i <= segments; ++i) {
        angle = 2.0f * M_PI * i / segments;
        x = cosf(angle) * radius;
        z = sinf(angle) * radius;
        
        glVertex3f(x, 0, z);
        glVertex3f(x, height, z);
    }
    glEnd();
    
    /* Bottom cap using triangle fan */
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, 0);
    for (i = 0; i <= segments; ++i) {
        angle = 2.0f * M_PI * i / segments;
        x = cosf(angle) * radius;
        z = sinf(angle) * radius;
        glVertex3f(x, 0, z);
    }
    glEnd();
    
    /* Top cap using triangle fan */
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, height, 0);
    for (i = segments; i >= 0; --i) {
        angle = 2.0f * M_PI * i / segments;
        x = cosf(angle) * radius;
        z = sinf(angle) * radius;
        glVertex3f(x, height, z);
    }
    glEnd();
}

void draw_cylinder_with_moving_tex(float radius, float height, int segments, double time)
{
    int i;
    float angle, x, z;
    float u_offset = (float)time * 0.1f; // moving u
    float v_offset = (float)time * 0.05f; // moving v
    
    /* Side surface using triangle strips */
    glBegin(GL_TRIANGLE_STRIP);
    for (i = 0; i <= segments; ++i) {
        angle = 2.0f * M_PI * i / segments;
        x = cosf(angle) * radius;
        z = sinf(angle) * radius;
        
        glTexCoord2f((float)i / segments + u_offset, 0 + v_offset);
        glVertex3f(x, 0, z);
        glTexCoord2f((float)i / segments + u_offset, 1 + v_offset);
        glVertex3f(x, height, z);
    }
    glEnd();
    
    /* Bottom cap using triangle fan */
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.5f + u_offset, 0.5f + v_offset);
    glVertex3f(0, 0, 0);
    for (i = 0; i <= segments; ++i) {
        angle = 2.0f * M_PI * i / segments;
        x = cosf(angle) * radius;
        z = sinf(angle) * radius;
        glTexCoord2f(0.5f + cosf(angle) * 0.5f + u_offset, 0.5f + sinf(angle) * 0.5f + v_offset);
        glVertex3f(x, 0, z);
    }
    glEnd();
    
    /* Top cap using triangle fan */
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.5f + u_offset, 0.5f + v_offset);
    glVertex3f(0, height, 0);
    for (i = segments; i >= 0; --i) {
        angle = 2.0f * M_PI * i / segments;
        x = cosf(angle) * radius;
        z = sinf(angle) * radius;
        glTexCoord2f(0.5f + cosf(angle) * 0.5f + u_offset, 0.5f + sinf(angle) * 0.5f + v_offset);
        glVertex3f(x, height, z);
    }
    glEnd();
}

void draw_cone(float radius, float height, int segments)
{
    int i;
    float angle, x, z;
    
    /* Side surface using triangle fan */
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.5f, 1.0f);
    glVertex3f(0, height, 0);  /* Apex */
    for (i = 0; i <= segments; ++i) {
        angle = 2.0f * M_PI * i / segments;
        x = cosf(angle) * radius;
        z = sinf(angle) * radius;
        glTexCoord2f((float)i / segments, 0.0f);
        glVertex3f(x, 0, z);
    }
    glEnd();
    
    /* Base using triangle fan */
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0, 0, 0);
    for (i = segments; i >= 0; --i) {
        angle = 2.0f * M_PI * i / segments;
        x = cosf(angle) * radius;
        z = sinf(angle) * radius;
        glTexCoord2f(0.5f + cosf(angle) * 0.5f, 0.5f + sinf(angle) * 0.5f);
        glVertex3f(x, 0, z);
    }
    glEnd();
}

void generate_checker_texture(GLuint* texture)
{
    const int width = 256;
    const int height = 256;
    unsigned char* data = (unsigned char*)malloc(width * height * 3 * sizeof(unsigned char));

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int idx = (i * width + j) * 3;
            if (((i / 32) + (j / 32)) % 2 == 0) {
                data[idx] = 255; data[idx+1] = 255; data[idx+2] = 255; // white
            } else {
                data[idx] = 0; data[idx+1] = 0; data[idx+2] = 0; // black
            }
        }
    }

    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    free(data);
}

void generate_circle_texture(GLuint* texture)
{
    const int width = 256;
    const int height = 256;
    unsigned char* data = (unsigned char*)malloc(width * height * 3 * sizeof(unsigned char));

    int center_x = width / 2;
    int center_y = height / 2;
    int radius = 100;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int idx = (i * width + j) * 3;
            int dx = j - center_x;
            int dy = i - center_y;
            if (dx * dx + dy * dy <= radius * radius) {
                data[idx] = 255; data[idx+1] = 0; data[idx+2] = 0; // red circle
            } else {
                data[idx] = 0; data[idx+1] = 0; data[idx+2] = 0; // black background
            }
        }
    }

    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    free(data);
}

void generate_gradient_texture(GLuint* texture)
{
    const int width = 256;
    const int height = 256;
    unsigned char* data = (unsigned char*)malloc(width * height * 3 * sizeof(unsigned char));

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int idx = (i * width + j) * 3;
            float t = (float)j / width;
            data[idx] = (unsigned char)(255 * (1 - t)); // red to blue
            data[idx+1] = 0;
            data[idx+2] = (unsigned char)(255 * t);
        }
    }

    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    free(data);
}

void generate_image_texture(GLuint* texture)
{
    // For demonstration, generate a simple colored image
    const int width = 256;
    const int height = 256;
    unsigned char* data = (unsigned char*)malloc(width * height * 3 * sizeof(unsigned char));

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int idx = (i * width + j) * 3;
            data[idx] = (unsigned char)(255 * (float)j / width); // gradient
            data[idx+1] = (unsigned char)(255 * (float)i / height);
            data[idx+2] = 128;
        }
    }

    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    free(data);
}

void render_image_overlay(const Scene* scene)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, -1, 1); // assuming window size 800x600

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, scene->image_texture);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(0, 0);
    glTexCoord2f(1, 0); glVertex2f(200, 0);
    glTexCoord2f(1, 1); glVertex2f(200, 200);
    glTexCoord2f(0, 1); glVertex2f(0, 200);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}


