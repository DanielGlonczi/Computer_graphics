#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdbool.h>

void drawRect(float x, float y, float w, float h)
{
    glBegin(GL_QUADS);

    glVertex2f(x-w, y-h);
    glVertex2f(x+w, y-h);
    glVertex2f(x+w, y+h);
    glVertex2f(x-w, y+h);

    glEnd();
}

int main(int argc, char* argv[])
{
    SDL_Window* window;
    SDL_Event event;
    SDL_GLContext context;
    bool running = true;

    float Xvel = 0.0002f;
    float Yvel = 0.0002f;

    float leftPadY = 0.0f;
    float rightPadY = 0.0f;

    float ballX = 0.0f;
    float ballY = 0.0f;

    float ballHeight = 0.03f;
    float ballWidth = 0.03f;

    float padHeight = 0.3f;
    float padWidth = 0.03f;

    Uint32 lastTime = SDL_GetTicks();

    SDL_Init(SDL_INIT_EVERYTHING);

    window=SDL_CreateWindow(
        "Simple Pong",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_OPENGL
    );

    SDL_GL_CreateContext(window);

    glClearColor(0,0,0,1);

    while (running)
    {
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }

            if (event.type==SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_w)
                {
                    leftPadY += 0.05f;
                }

                if (event.key.keysym.sym == SDLK_s)
                {
                    leftPadY -= 0.05f;
                }
                
                if (event.key.keysym.sym == SDLK_UP)
                {
                    rightPadY += 0.05f;
                }

                if (event.key.keysym.sym == SDLK_DOWN)
                {
                    rightPadY -= 0.05f;
                }

                if (leftPadY+padHeight > 1)
                {
                    leftPadY = 1 - padHeight;
                }
                
                if (leftPadY-padHeight < -1)
                {
                    leftPadY = -1 + padHeight;
                }

                if (rightPadY+padHeight > 1)
                {
                    rightPadY = 1 - padHeight;
                }
                
                if (rightPadY-padHeight < -1)
                {
                    rightPadY = -1 + padHeight;
                }

               

                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }
                
                
                
                
            }

            
            
        }


        ballX += Xvel;
        ballY += Yvel;

        if (ballY>1-ballHeight)
        {
            Yvel = -Yvel;
            ballY = 1-ballHeight;
        }
        if (ballY<-1+ballHeight)
        {
            Yvel = -Yvel;
            ballY = -1+ballHeight;
        }

        if(ballX > 0.9f -padWidth &&
           ballY < rightPadY + padHeight &&
           ballY > rightPadY - padHeight)
        {
            Xvel = -Xvel;
            ballX = 0.9f-ballWidth;
        }

        if(ballX < -0.9f +padWidth &&
           ballY < leftPadY + padHeight &&
           ballY > leftPadY - padHeight)
        {
            Xvel = -Xvel;
            ballX = -0.9f+ballWidth;
        }
        
        if (ballX > 1 || ballX < -1)
        {
            ballX = 0.0f;
            ballY = 0.0f;
        }
        


        

        glClear(GL_COLOR_BUFFER_BIT);

        drawRect(-0.9, leftPadY, padWidth, padHeight);
        drawRect(0.9, rightPadY, padWidth, padHeight);
        drawRect(ballX, ballY, ballWidth, ballHeight);

        SDL_GL_SwapWindow(window);
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}