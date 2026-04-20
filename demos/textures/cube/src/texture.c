#include "texture.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

GLuint load_texture(const char* filename)
{
    SDL_Surface* surface;
    GLuint texture_name;

    surface = IMG_Load(filename);
    if (surface == NULL) {
        printf("[ERROR] Failed to load texture '%s': %s\n", filename, IMG_GetError());
        return 0;
    }

    glGenTextures(1, &texture_name);
    glBindTexture(GL_TEXTURE_2D, texture_name);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    SDL_FreeSurface(surface);
    return texture_name;
}

GLuint create_color_texture(GLubyte red, GLubyte green, GLubyte blue)
{
    const int size = 64;
    Pixel* pixels = malloc(size * size * sizeof(Pixel));
    if (pixels == NULL) {
        return 0;
    }

    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            GLubyte factor = ((x / 8) % 2) ^ ((y / 8) % 2) ? 220 : 120;
            int index = y * size + x;
            pixels[index][0] = (GLubyte)((red * factor) / 255);
            pixels[index][1] = (GLubyte)((green * factor) / 255);
            pixels[index][2] = (GLubyte)((blue * factor) / 255);
        }
    }

    GLuint texture_name;
    glGenTextures(1, &texture_name);
    glBindTexture(GL_TEXTURE_2D, texture_name);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    free(pixels);
    return texture_name;
}
