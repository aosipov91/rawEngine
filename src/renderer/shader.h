#ifndef SHADER_H_
#define SHADER_H_

#include "src/renderer/opengl/openGLExtensions.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <assert.h>   /* for assert */

namespace renderer
{
    char* load_file(const char *path);
    GLuint make_shader(GLenum type, const char *source);
    GLuint load_shader(GLenum type, const char *path);
    GLuint make_program(GLuint shader1, GLuint shader2);
    GLuint make_program(GLuint shader1, GLuint shader2, GLuint shader3);
    GLuint load_program(const char *path1, const char *path2);
    GLuint load_program(const char *path1, const char *path2, const char *path3);
    void unload_shader_program(GLuint* id);


    void shader_uniform_1i(GLuint program, const char* name, const int value);
    void shader_uniform_1f(GLuint program, const char* name, const int value);
    void shader_uniform_3f(GLuint program, const char* name, float v1, float v2, float v3);
    void shader_uniform_mat4(GLuint program, const char* name, const float* data);


class Shader
{
public:
    Shader();
    ~Shader();
    Shader(const char *path1, const char *path2);
    Shader(const char *path1, const char *path2, const char *path3);
    GLuint GetHandle() {
        return mHandle;
    }
    void Bind()
    {
        glUseProgram(mHandle);
    }
private:
    GLuint mHandle;
};



}

#endif // SHADER_H_
