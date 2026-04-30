#include "app.h"

#include <SDL2/SDL_image.h>
#include <stdio.h>

void init_app(App *app, int width, int height)
{
    int error_code;
    int inited_loaders;

    app->is_running = false;

    error_code = SDL_Init(SDL_INIT_EVERYTHING);
    if (error_code != 0)
    {
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return;
    }

    app->window = SDL_CreateWindow(
        "Cube!",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL);
    if (app->window == NULL)
    {
        printf("[ERROR] Unable to create the application window!\n");
        return;
    }

    inited_loaders = IMG_Init(IMG_INIT_PNG);
    if (inited_loaders == 0)
    {
        printf("[ERROR] IMG initialization error: %s\n", IMG_GetError());
        return;
    }

    app->gl_context = SDL_GL_CreateContext(app->window);
    if (app->gl_context == NULL)
    {
        printf("[ERROR] Unable to create the OpenGL context!\n");
        return;
    }

    init_opengl();
    reshape(width, height);

    init_camera(&(app->camera));
    init_scene(&(app->scene));

    app->is_running = true;
}

void init_opengl()
{
    glShadeModel(GL_SMOOTH);

    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);

    glClearColor(0.1, 0.1, 0.1, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);

    glClearDepth(1.0);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void reshape(GLsizei width, GLsizei height)
{
    int x, y, w, h;
    double ratio;

    ratio = (double)width / height;
    if (ratio > VIEWPORT_RATIO)
    {
        w = (int)((double)height * VIEWPORT_RATIO);
        h = height;
        x = (width - w) / 2;
        y = 0;
    }
    else
    {
        w = width;
        h = (int)((double)width / VIEWPORT_RATIO);
        x = 0;
        y = (height - h) / 2;
    }

    glViewport(x, y, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(
        -.08, .08,
        -.06, .06,
        .1, 100);
}

void handle_app_events(App *app)
{
    SDL_Event event;
    static bool is_mouse_down = false;
    static int mouse_x = 0;
    static int mouse_y = 0;
    int x;
    int y;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_ESCAPE:
                app->is_running = false;
                break;
            case SDL_SCANCODE_W:
                set_camera_speed(&(app->camera), 1);
                break;
            case SDL_SCANCODE_S:
                set_camera_speed(&(app->camera), -1);
                break;
            case SDL_SCANCODE_A:
                set_camera_side_speed(&(app->camera), 1);
                break;
            case SDL_SCANCODE_D:
                set_camera_side_speed(&(app->camera), -1);
                break;
            case SDL_SCANCODE_UP:
                move_active_model(&(app->scene), 0, 1);
                break;
            case SDL_SCANCODE_DOWN:
                move_active_model(&(app->scene), 0, -1);
                break;
            case SDL_SCANCODE_LEFT:
                move_active_model(&(app->scene), -1, 0);
                break;
            case SDL_SCANCODE_RIGHT:
                move_active_model(&(app->scene), 1, 0);
                break;
            case SDL_SCANCODE_TAB:
                if (app->scene.model_count > 0)
                {
                    app->scene.active_model = (app->scene.active_model + 1) % app->scene.model_count;
                }
                break;
            case SDL_SCANCODE_R:
                init_scene(&(app->scene));
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_S:
                set_camera_speed(&(app->camera), 0);
                break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_D:
                set_camera_side_speed(&(app->camera), 0);
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            is_mouse_down = true;
            break;
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&x, &y);
            if (is_mouse_down)
            {
                rotate_camera(&(app->camera), mouse_x - x, mouse_y - y);
            }
            mouse_x = x;
            mouse_y = y;
            break;
        case SDL_MOUSEBUTTONUP:
            is_mouse_down = false;
            break;
        case SDL_QUIT:
            app->is_running = false;
            break;
        default:
            break;
        }
    }
}

void update_app(App *app)
{
    double current_time;
    double elapsed_time;

    current_time = (double)SDL_GetTicks() / 1000;
    elapsed_time = current_time - app->uptime;
    app->uptime = current_time;

    // Set camera target to follow the active cube
    vec3 cube_pos = app->scene.instances[app->scene.active_model].position;

    // Start with a wider view for the first 2 seconds
    float distance_factor = (app->uptime < 2.0) ? 1.8f : 1.0f;

    app->camera.target_position.x = cube_pos.x;
    app->camera.target_position.y = cube_pos.y - 5.0f * distance_factor;
    app->camera.target_position.z = cube_pos.z + 3.0f * distance_factor;

    update_camera(&(app->camera), elapsed_time);
    update_scene(&(app->scene), elapsed_time);
}

static void draw_collectible_hud(const Scene *scene, SDL_Window *window)
{
    int width = 800;
    int height = 600;
    SDL_GetWindowSize(window, &width, &height);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, (double)width, 0.0, (double)height, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    int total = scene->total_collectibles;
    int collected = scene->collected_count;
    for (int i = 0; i < total; ++i)
    {
        float x = 10.0f + i * 30.0f;
        float y = (float)height - 30.0f;
        if (i < collected)
        {
            glColor3f(0.0f, 1.0f, 0.0f);
        }
        else
        {
            glColor3f(1.0f, 1.0f, 0.0f);
        }
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + 24.0f, y);
        glVertex2f(x + 24.0f, y - 24.0f);
        glVertex2f(x, y - 24.0f);
        glEnd();
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void render_app(App *app)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    set_view(&(app->camera));
    render_scene(&(app->scene));
    glPopMatrix();

    int remaining = app->scene.total_collectibles - app->scene.collected_count;
    char title[80];
    if (remaining > 0)
    {
        sprintf(title, "Cube! - Collectibles: %d/%d (%d left)", app->scene.collected_count, app->scene.total_collectibles, remaining);
    }
    else
    {
        sprintf(title, "Cube! - Collectibles: %d/%d - all collected!", app->scene.collected_count, app->scene.total_collectibles);
    }
    SDL_SetWindowTitle(app->window, title);

    draw_collectible_hud(&(app->scene), app->window);

    SDL_GL_SwapWindow(app->window);
}

void destroy_app(App *app)
{
    if (app->gl_context != NULL)
    {
        SDL_GL_DeleteContext(app->gl_context);
    }

    if (app->window != NULL)
    {
        SDL_DestroyWindow(app->window);
    }

    SDL_Quit();
}
