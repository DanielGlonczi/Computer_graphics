/*
 * edge_cube.c
 *
 * A simple EDGE-style cube puzzle demo in one file.
 * The cube rolls around its bottom edge, and if it moves off the platform,
 * it falls and resets.
 *
 * Compile on Windows with FreeGLUT:
 *   gcc edge_cube.c -lfreeglut -lopengl32 -lglu32 -lm -o edge_cube.exe
 */

#include <GL/glut.h>
#include <stdbool.h>
#include <stdio.h>

#define TILE_SIZE 1.0f
#define CUBE_SIZE 1.0f
#define ANIM_DURATION 0.18f
#define FALL_SPEED  4.0f
#define RESET_DELAY 1000

static const int LEVEL_ROWS = 8;
static const int LEVEL_COLS = 8;
static const int level[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0}
};

static const int start_x = 1;
static const int start_z = 1;

static float cube_x = 1.0f;
static float cube_y = 0.5f;
static float cube_z = 1.0f;
static bool is_animating = false;
static bool will_fall_after_move = false;
static bool is_falling = false;
static int move_dx = 0;
static int move_dz = 0;
static float animation_angle = 0.0f;
static int target_x = 1;
static int target_z = 1;
static float fall_velocity = 0.0f;

static void reset_game(void);
static void start_cube_move(int dx, int dz);
static void update_scene(float delta_seconds);
static void draw_tile(int x, int z);
static void draw_cube(void);

static void set_material(const GLfloat ambient[4], const GLfloat diffuse[4], const GLfloat specular[4], GLfloat shininess)
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

static void init_lighting(void)
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    const GLfloat ambient_light[] = {0.18f, 0.18f, 0.18f, 1.0f};
    const GLfloat diffuse_light[] = {0.95f, 0.95f, 0.95f, 1.0f};
    const GLfloat specular_light[] = {0.75f, 0.75f, 0.75f, 1.0f};
    const GLfloat position[] = {3.0f, 7.0f, 6.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

static void init_gl(void)
{
    glClearColor(0.08f, 0.12f, 0.20f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    init_lighting();
}

static void reshape(int width, int height)
{
    if (height == 0) {
        height = 1;
    }
    float aspect = (float)width / (float)height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, aspect, 0.1, 40.0);
    glMatrixMode(GL_MODELVIEW);
}

static void draw_floor(void)
{
    for (int z = 0; z < LEVEL_ROWS; ++z) {
        for (int x = 0; x < LEVEL_COLS; ++x) {
            if (level[z][x] == 1) {
                draw_tile(x, z);
            }
        }
    }
}

static void draw_tile(int x, int z)
{
    glPushMatrix();
    glTranslatef((float)x, 0.0f, (float)z);
    glTranslatef(0.5f * TILE_SIZE, 0.0f, 0.5f * TILE_SIZE);
    glScalef(TILE_SIZE, 0.1f, TILE_SIZE);

    const GLfloat ambient[] = {0.10f, 0.35f, 0.30f, 1.0f};
    const GLfloat diffuse[] = {0.15f, 0.60f, 0.55f, 1.0f};
    const GLfloat specular[] = {0.15f, 0.15f, 0.15f, 1.0f};
    set_material(ambient, diffuse, specular, 16.0f);

    glutSolidCube(1.0f);
    glPopMatrix();
}

static void draw_cube(void)
{
    glPushMatrix();

    if (is_animating) {
        float local_pivot_x = 0.0f;
        float local_pivot_y = -0.5f;
        float local_pivot_z = 0.0f;
        float axis_x = 0.0f;
        float axis_z = 0.0f;
        float rotation_angle = animation_angle;

        if (move_dx != 0) {
            local_pivot_x = 0.5f * move_dx;
            axis_z = 1.0f;
            rotation_angle *= (move_dx > 0 ? -1.0f : 1.0f);
        } else if (move_dz != 0) {
            local_pivot_z = 0.5f * move_dz;
            axis_x = 1.0f;
            rotation_angle *= (move_dz > 0 ? 1.0f : -1.0f);
        }

        glTranslatef(cube_x, cube_y, cube_z);
        glTranslatef(local_pivot_x, local_pivot_y, local_pivot_z);
        glRotatef(rotation_angle, axis_x, 0.0f, axis_z);
        glTranslatef(-local_pivot_x, -local_pivot_y, -local_pivot_z);
    } else {
        glTranslatef(cube_x, cube_y, cube_z);
    }

    const GLfloat ambient[] = {0.20f, 0.12f, 0.25f, 1.0f};
    const GLfloat diffuse[] = {0.80f, 0.35f, 0.90f, 1.0f};
    const GLfloat specular[] = {0.75f, 0.75f, 0.85f, 1.0f};
    set_material(ambient, diffuse, specular, 48.0f);

    glutSolidCube(CUBE_SIZE);
    glPopMatrix();
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(3.5f, 7.0f, 10.0f,  3.5f, 0.0f, 3.5f,  0.0f, 1.0f, 0.0f);

    draw_floor();
    draw_cube();

    glutSwapBuffers();
}

static void keyboard_special(int key, int x, int y)
{
    (void)x;
    (void)y;

    if (is_animating || is_falling) {
        return;
    }

    if (key == GLUT_KEY_RIGHT) {
        start_cube_move(1, 0);
    } else if (key == GLUT_KEY_LEFT) {
        start_cube_move(-1, 0);
    } else if (key == GLUT_KEY_UP) {
        start_cube_move(0, -1);
    } else if (key == GLUT_KEY_DOWN) {
        start_cube_move(0, 1);
    }
}

static void start_cube_move(int dx, int dz)
{
    if (is_animating || is_falling) {
        return;
    }

    int next_x = (int)(cube_x + dx + 0.5f * (dx > 0 ? 1 : -1));
    int next_z = (int)(cube_z + dz + 0.5f * (dz > 0 ? 1 : -1));

    /* Ensure next position is computed as an integer grid cell. */
    next_x = (int)(cube_x + dx);
    next_z = (int)(cube_z + dz);

    bool valid = false;
    if (next_x >= 0 && next_x < LEVEL_COLS && next_z >= 0 && next_z < LEVEL_ROWS) {
        valid = (level[next_z][next_x] == 1);
    }

    move_dx = dx;
    move_dz = dz;
    target_x = next_x;
    target_z = next_z;
    animation_angle = 0.0f;
    is_animating = true;
    will_fall_after_move = !valid;
}

static void reset_game(void)
{
    cube_x = (float)start_x;
    cube_y = 0.5f;
    cube_z = (float)start_z;
    is_animating = false;
    will_fall_after_move = false;
    is_falling = false;
    animation_angle = 0.0f;
    fall_velocity = 0.0f;
}

static void update_scene(float delta_seconds)
{
    if (is_animating) {
        animation_angle += 90.0f * delta_seconds / ANIM_DURATION;
        if (animation_angle >= 90.0f) {
            animation_angle = 90.0f;
            is_animating = false;
            cube_x = (float)target_x;
            cube_z = (float)target_z;
            cube_y = 0.5f;

            if (will_fall_after_move) {
                is_falling = true;
                fall_velocity = 0.0f;
            }
        }
    }

    if (is_falling) {
        fall_velocity += 9.8f * delta_seconds;
        cube_y -= fall_velocity * delta_seconds;
        if (cube_y < -6.0f) {
            reset_game();
        }
    }
}

static void timer(int value)
{
    (void)value;

    static int last_time = 0;
    int current_time = glutGet(GLUT_ELAPSED_TIME);
    float delta_seconds = (current_time - last_time) * 0.001f;
    last_time = current_time;

    if (last_time == 0) {
        last_time = current_time;
        delta_seconds = 0.0f;
    }

    update_scene(delta_seconds);
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(960, 720);
    glutCreateWindow("EDGE-style Rolling Cube");

    init_gl();
    reset_game();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard_special);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();

    return 0;
}
