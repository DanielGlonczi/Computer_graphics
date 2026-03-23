#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

float angleY = 0.0f;
float angleZ = 0.0f;

void drawCube()
{
    glBegin(GL_QUADS);

    // front
    glColor4f(1,0,0,0.2);
    glVertex3f(-0.5,-0.5,0.5);
    glVertex3f(0.5,-0.5,0.5);
    glVertex3f(0.5,0.5,0.5);
    glVertex3f(-0.5,0.5,0.5);

    // back
    glColor4f(0,1,0,0.2);
    glVertex3f(-0.5,-0.5,-0.5);
    glVertex3f(-0.5,0.5,-0.5);
    glVertex3f(0.5,0.5,-0.5);
    glVertex3f(0.5,-0.5,-0.5);

    // left
    glColor4f(0,0,1,0.2);
    glVertex3f(-0.5,-0.5,-0.5);
    glVertex3f(-0.5,-0.5,0.5);
    glVertex3f(-0.5,0.5,0.5);
    glVertex3f(-0.5,0.5,-0.5);

    // right
    glColor4f(1,1,0,0.2);
    glVertex3f(0.5,-0.5,-0.5);
    glVertex3f(0.5,0.5,-0.5);
    glVertex3f(0.5,0.5,0.5);
    glVertex3f(0.5,-0.5,0.5);

    // top
    glColor3f(0,1,1);
    glVertex3f(-0.5,0.5,-0.5);
    glVertex3f(-0.5,0.5,0.5);
    glVertex3f(0.5,0.5,0.5);
    glVertex3f(0.5,0.5,-0.5);

    // bottom
    glColor3f(1,0,1);
    glVertex3f(-0.5,-0.5,-0.5);
    glVertex3f(0.5,-0.5,-0.5);
    glVertex3f(0.5,-0.5,0.5);
    glVertex3f(-0.5,-0.5,0.5);

    glEnd();
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "3D Cube",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_OPENGL
    );

    SDL_GLContext context = SDL_GL_CreateContext(window);

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0/600.0, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);

    int running = 1;

    while(running)
    {
        SDL_Event event;

        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                running = 0;

            if(event.type == SDL_KEYDOWN)
            {
                if(event.key.keysym.sym == SDLK_LEFT)
                    angleY -= 5;

                if(event.key.keysym.sym == SDLK_RIGHT)
                    angleY += 5;

                if(event.key.keysym.sym == SDLK_DOWN)
                    angleZ -= 5;

                if(event.key.keysym.sym == SDLK_UP)
                    angleZ += 5;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();

        glTranslatef(0,0,-3);

        glRotatef(angleY,0,1,0);
        glRotatef(angleZ,0,0,1);

        drawCube();

        SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}