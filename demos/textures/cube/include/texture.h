#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl.h>

typedef GLubyte Pixel[3];

/**
 * Load texture from file and returns with the texture name.
 */
GLuint load_texture(const char* filename);

/**
 * Create a simple solid/checked texture from a base color.
 */
GLuint create_color_texture(GLubyte red, GLubyte green, GLubyte blue);

#endif /* TEXTURE_H */
