#include "scene.h"

#include <GL/gl.h>
#include <math.h>

void init_scene(Scene* scene)
{
    scene->time_elapsed = 0.0;
}

void update_scene(Scene* scene)
{
    (void)scene;  /* Unused parameter */
}

void render_scene(const Scene* scene)
{
    draw_checkerboard(1.0f, 8);
    
    glDisable(GL_LIGHTING);
    
    draw_origin();

    glPushMatrix();
    glTranslatef(0.1f, 0.1f, 0.15f);
    draw_colored_triangle();
    
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.5f, 0.2f, 0.15f);
    glRotatef((float)(scene->time_elapsed * 50), 1.0f, 1.0f, 1.0f);
    draw_sphere(0.15f, 12, 12);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.2f, 0.2f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    draw_cylinder(0.1f, 0.3f, 16);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.8f, 0.8f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    draw_cone(0.1f, 0.3f, 16);
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
    
    glColor3f(0.5f, 0.5f, 0.5f);  /* Gray */
    
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
            
            if (j%2==0)
            {
                glColor3f(0.9f, 0.9f, 0.9f); 
            }
            else
            {
                glColor3f(0.5f, 0.5f, 0.5f); 
            }
            
            glVertex3f(x0 * radius, y0 * radius, z0 * radius);
            
            x1 = cosf(lat1) * cosf(lng1);
            y1 = sinf(lat1);
            z1 = cosf(lat1) * sinf(lng1);
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

void draw_cone(float radius, float height, int segments)
{
    int i;
    float angle, x, z;
    
    glColor3f(0.2f, 0.8f, 0.4f);  /* Green */
    
    /* Side surface using triangle fan */
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, height, 0);  /* Apex */
    for (i = 0; i <= segments; ++i) {
        angle = 2.0f * M_PI * i / segments;
        x = cosf(angle) * radius;
        z = sinf(angle) * radius;
        glVertex3f(x, 0, z);
    }
    glEnd();
    
    /* Base using triangle fan */
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, 0);
    for (i = segments; i >= 0; --i) {
        angle = 2.0f * M_PI * i / segments;
        x = cosf(angle) * radius;
        z = sinf(angle) * radius;
        glVertex3f(x, 0, z);
    }
    glEnd();
}


