#include "scene.h"

#include <obj/load.h>
#include <obj/draw.h>
#include <stdio.h>

static void draw_instance(const Model* model, const ModelInstance* inst)
{
    glPushMatrix();
    glTranslatef(inst->position.x, inst->position.y, inst->position.z);
    glRotatef(inst->rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(inst->rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(inst->rotation.z, 0.0f, 0.0f, 1.0f);
    glScalef(inst->scale.x, inst->scale.y, inst->scale.z);
    draw_model(model);
    glPopMatrix();
}

void init_scene(Scene* scene)
{
    scene->model_count = 0;
    scene->active_model = 0;

    if (load_model(&(scene->models[scene->model_count]), "assets/models/cube.obj") == TRUE) {
        scene->instances[scene->model_count].position = (vec3){0.0f, 0.0f, -2.0f};
        scene->instances[scene->model_count].rotation = (vec3){0.0f, 0.0f, 0.0f};
        scene->instances[scene->model_count].scale = (vec3){1.0f, 1.0f, 1.0f};
        scene->instances[scene->model_count].can_move = true;
        scene->instances[scene->model_count].rotates = false;
        scene->model_count++;
    }

    if (load_model(&(scene->models[scene->model_count]), "assets/models/house.obj") == TRUE) {
        scene->instances[scene->model_count].position = (vec3){3.0f, 0.0f, -2.5f};
        scene->instances[scene->model_count].rotation = (vec3){0.0f, 0.0f, 0.0f};
        scene->instances[scene->model_count].scale = (vec3){0.03f, 0.03f, 0.03f};
        scene->instances[scene->model_count].can_move = false;
        scene->instances[scene->model_count].rotates = true;
        scene->model_count++;
    }

    scene->texture_id = load_texture("assets/textures/cube.png");
    glBindTexture(GL_TEXTURE_2D, scene->texture_id);

    scene->material.ambient = (Color){0.2f, 0.2f, 0.2f};
    scene->material.diffuse = (Color){0.8f, 0.4f, 0.2f};
    scene->material.specular = (Color){0.5f, 0.5f, 0.5f};
    scene->material.shininess = 32.0f;
}

void set_lighting()
{
    float ambient_light[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float diffuse_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float specular_light[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    float position[] = { 0.0f, 4.0f, 4.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void set_material(const Material* material)
{
    float ambient_material_color[] = { material->ambient.red, material->ambient.green, material->ambient.blue };
    float diffuse_material_color[] = { material->diffuse.red, material->diffuse.green, material->diffuse.blue };
    float specular_material_color[] = { material->specular.red, material->specular.green, material->specular.blue };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void update_scene(Scene* scene)
{
    for (int i = 0; i < scene->model_count; ++i) {
        if (scene->instances[i].rotates) {
            scene->instances[i].rotation.y += 3.0f * 0.016f;
            if (scene->instances[i].rotation.y > 360.0f) {
                scene->instances[i].rotation.y -= 360.0f;
            }
        }
    }
}

void render_scene(const Scene* scene)
{
    set_material(&(scene->material));
    set_lighting();
    draw_origin();

    if (scene->model_count > 0) {
        const Model* grid_model = &(scene->models[0]);
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                glPushMatrix();
                glTranslatef((c - 1.5f) * 2.0f, (r - 1.5f) * 2.0f, -1.0f);
                glScalef(0.3f, 0.3f, 0.3f);
                draw_model(grid_model);
                glPopMatrix();
            }
        }
    }

    for (int i = 0; i < scene->model_count; ++i) {
        draw_instance(&(scene->models[i]), &(scene->instances[i]));
    }
}

void draw_origin()
{
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(1, 0, 0);
    glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 1, 0);
    glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 1);
    glEnd();
    glEnable(GL_LIGHTING);
}

