#include "scene.h"
#include "texture.h"

#include <GL/gl.h>

static void draw_textured_cube(const Scene* scene);

void init_scene(Scene* scene)
{
    scene->textures[0] = create_color_texture(240,  80,  80);  // front
    scene->textures[1] = create_color_texture( 80, 240,  80);  // back
    scene->textures[2] = create_color_texture( 80,  80, 240);  // left
    scene->textures[3] = create_color_texture(240, 240,  80);  // right
    scene->textures[4] = create_color_texture(240,  80, 240);  // top
    scene->textures[5] = create_color_texture( 80, 240, 240);  // bottom

    scene->material.ambient.red = 0.2;
    scene->material.ambient.green = 0.2;
    scene->material.ambient.blue = 0.2;

    scene->material.diffuse.red = 0.9;
    scene->material.diffuse.green = 0.9;
    scene->material.diffuse.blue = 0.9;

    scene->material.specular.red = 0.8;
    scene->material.specular.green = 0.8;
    scene->material.specular.blue = 0.8;

    scene->material.shininess = 50.0;
}

void set_lighting()
{
    float ambient_light[] = { 0.15f, 0.15f, 0.15f, 1.0f };
    float diffuse_light[] = { 0.85f, 0.85f, 0.85f, 1.0f };
    float specular_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float position[] = { 1.5f, 1.0f, 2.0f, 0.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void set_material(const Material* material)
{
    float ambient_material_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue
    };

    float diffuse_material_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue
    };

    float specular_material_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void update_scene(Scene* scene)
{
    (void)scene;
}

void render_scene(const Scene* scene)
{
    set_material(&(scene->material));
    set_lighting();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    draw_origin();
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    draw_textured_cube(scene);
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

static void draw_textured_cube(const Scene* scene)
{
    const GLfloat half = 0.5f;

    glColor3f(1.0f, 1.0f, 1.0f);

    // front face
    glBindTexture(GL_TEXTURE_2D, scene->textures[0]);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glTexCoord2f(0, 0); glVertex3f(-half, -half, half);
    glTexCoord2f(1, 0); glVertex3f(half, -half, half);
    glTexCoord2f(1, 1); glVertex3f(half, half, half);
    glTexCoord2f(0, 1); glVertex3f(-half, half, half);
    glEnd();

    // back face
    glBindTexture(GL_TEXTURE_2D, scene->textures[1]);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, -1);
    glTexCoord2f(0, 0); glVertex3f(half, -half, -half);
    glTexCoord2f(1, 0); glVertex3f(-half, -half, -half);
    glTexCoord2f(1, 1); glVertex3f(-half, half, -half);
    glTexCoord2f(0, 1); glVertex3f(half, half, -half);
    glEnd();

    // left face
    glBindTexture(GL_TEXTURE_2D, scene->textures[2]);
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(-half, -half, -half);
    glTexCoord2f(1, 0); glVertex3f(-half, -half, half);
    glTexCoord2f(1, 1); glVertex3f(-half, half, half);
    glTexCoord2f(0, 1); glVertex3f(-half, half, -half);
    glEnd();

    // right face
    glBindTexture(GL_TEXTURE_2D, scene->textures[3]);
    glBegin(GL_QUADS);
    glNormal3f(1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(half, -half, half);
    glTexCoord2f(1, 0); glVertex3f(half, -half, -half);
    glTexCoord2f(1, 1); glVertex3f(half, half, -half);
    glTexCoord2f(0, 1); glVertex3f(half, half, half);
    glEnd();

    // top face
    glBindTexture(GL_TEXTURE_2D, scene->textures[4]);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0); glVertex3f(-half, half, half);
    glTexCoord2f(1, 0); glVertex3f(half, half, half);
    glTexCoord2f(1, 1); glVertex3f(half, half, -half);
    glTexCoord2f(0, 1); glVertex3f(-half, half, -half);
    glEnd();

    // bottom face
    glBindTexture(GL_TEXTURE_2D, scene->textures[5]);
    glBegin(GL_QUADS);
    glNormal3f(0, -1, 0);
    glTexCoord2f(0, 0); glVertex3f(-half, -half, -half);
    glTexCoord2f(1, 0); glVertex3f(half, -half, -half);
    glTexCoord2f(1, 1); glVertex3f(half, -half, half);
    glTexCoord2f(0, 1); glVertex3f(-half, -half, half);
    glEnd();
}
