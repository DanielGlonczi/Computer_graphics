#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdbool.h>

void CreateTriangle()
{
glBegin(GL_TRIANGLES);

glColor3f(0.0f, 1.0f, 0.0f);
glVertex2f(0.0, 0.5);
glColor3f(1.0f, 0.0f, 1.0f);
glVertex2f(0.0, 0.0);
glColor3f(0.0f, 0.0f, 1.0f);
glVertex2f(0.5, 0.0);

glColor3f(0.0f, 1.0f, 1.0f);
glVertex2f(0.0, 0.5);

glVertex2f(0.5, 0.5);

glVertex2f(0.5, 0.0);

glEnd();
}

void CreateGrid()
{
    glBegin(GL_LINES);

    glColor3f(0.3,0.3,0.3);

    for(float i=-1.0; i<1.0; i=i+0.1)
    {

        glVertex2f(i,1.0);
        glVertex2f(i,-1.0);

        glVertex2f(1.0,i);
        glVertex2f(-1.0,i);
    }

    glEnd();
}

int main(int argc, char* argv[])
{
    SDL_Window* window;
    SDL_GLContext context;
    SDL_Event event;

    bool running=true;

    float xPos=0.0;
    float yPos=0.0;
    float zPos=0.0;

    bool grid=false;
    bool triangle=false;

    if(SDL_Init(SDL_INIT_EVERYTHING)!=0)
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return -1;
    }

    window=SDL_CreateWindow(
        "Test Window",
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_OPENGL
    );

    if (!window)
    {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        return -1;
    }

    context=SDL_GL_CreateContext(window);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    if (!context)
    {
        printf("SDL_CreateContext failed: %s\n", SDL_GetError());
        return -1;
    }
    printf("Press 'g' to toggle grid\n");
    printf("Press 'h' to toggle shape\n");
    printf("Use arrow keys to move the shape\n");
    printf("Use 'a' and 'd' to rotate the shape\n");
    printf("Press 'r' to rotate the grid\n");
    while(running)
    {
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }

            if (event.type==SDL_KEYDOWN)
            {
                if(event.key.keysym.sym==SDLK_UP)
                {
                    yPos+=0.1;
                }
                if(event.key.keysym.sym==SDLK_DOWN)
                {
                    yPos-=0.1;
                }
                if(event.key.keysym.sym==SDLK_LEFT)
                {
                    xPos-=0.1;
                }
                if(event.key.keysym.sym==SDLK_RIGHT)
                {
                    xPos+=0.1;
                }

                if(event.key.keysym.sym==SDLK_a)
                {
                    zPos+=1.0;
                }
                if(event.key.keysym.sym==SDLK_d)
                {
                    zPos-=1.0;
                }

                if(event.key.keysym.sym==SDLK_r)
                {
                    glRotatef(5,0,0,1);
                }

                if(event.key.keysym.sym==SDLK_g)
                {
                    grid=true;
                    
                }

                if(event.key.keysym.sym==SDLK_h)
                {
                    triangle=true;
                    
                }
                
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);

        if (grid)
        {
            CreateGrid();
        }
        
    
        
    glPushMatrix();
    glRotatef(zPos,0,0,1);
    glTranslatef(xPos,yPos,0.0);
        if (triangle)
        {
            CreateTriangle();
        }
        
    
    glPopMatrix();

        
        SDL_GL_SwapWindow(window);
    }

    

    
    SDL_DestroyWindow(window);
    SDL_Quit();

return 0;
}