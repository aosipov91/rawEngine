#include "shader.h"

namespace renderer {



GLuint load_program(const char *path1, const char *path2, const char *path3)
{
    GLuint shader1 = load_shader(GL_VERTEX_SHADER, path1);
    GLuint shader2 = load_shader(GL_FRAGMENT_SHADER, path2);
    GLuint shader3 = load_shader(GL_GEOMETRY_SHADER, path3);
    GLuint program = make_program(shader1, shader2, shader3);
    return program;
}


GLuint load_program(const char *path1, const char *path2)
{
    GLuint shader1 = load_shader(GL_VERTEX_SHADER, path1);
    GLuint shader2 = load_shader(GL_FRAGMENT_SHADER, path2);
    GLuint program = make_program(shader1, shader2);
    return program;
}

void unload_shader_program(GLuint* id)
{
    glDeleteProgram(*id);
}

char *load_file(const char *path) {
    #if defined(_WIN32)
    FILE *file;
    fopen_s(&file, path, "rb");

    #elif defined(__linux__)
    FILE *file = fopen(path, "rb");
    #endif
    if (!file) {
	fprintf(stderr, "fopen %s failed: %d %s\n", path, errno, strerror(errno));
	exit(1);
    }
    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    rewind(file);
    char *data = (char*)calloc(length + 1, sizeof(char));
    fread(data, 1, length, file);
    fclose(file);
    return data;
}

GLuint make_shader(GLenum type, const char *source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
	GLint length;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
	GLchar *info = (GLchar*)calloc(length, sizeof(GLchar));
	glGetShaderInfoLog(shader, length, NULL, info);
	fprintf(stderr, "glCompileShader failed:\n%s\n %s", info, source);

	free(info);
    }
    return shader;
}

GLuint load_shader(GLenum type, const char *path) {
    char *data = load_file(path);
    GLuint result = make_shader(type, data);
    free(data);
    return result;
}

GLuint make_program(GLuint shader1, GLuint shader2, GLuint shader3) {
    GLuint program = glCreateProgram();
    glAttachShader(program, shader1);
    glAttachShader(program, shader2);
    glAttachShader(program, shader3);
    glLinkProgram(program);
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
	GLint length;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
	GLchar *info = (GLchar*)calloc(length, sizeof(GLchar));
	glGetProgramInfoLog(program, length, NULL, info);
	fprintf(stderr, "glLinkProgram failed: %s\n", info);
	free(info);
    }
    glDetachShader(program, shader1);
    glDetachShader(program, shader2);
    glDetachShader(program, shader3);
    glDeleteShader(shader1);
    glDeleteShader(shader2);
    glDeleteShader(shader3);
    return program;
}

GLuint make_program(GLuint shader1, GLuint shader2) {
    GLuint program = glCreateProgram();
    glAttachShader(program, shader1);
    glAttachShader(program, shader2);
    glLinkProgram(program);
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
	GLint length;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
	GLchar *info = (GLchar*)calloc(length, sizeof(GLchar));
	glGetProgramInfoLog(program, length, NULL, info);
	fprintf(stderr, "glLinkProgram failed: %s\n", info);
	free(info);
    }
    glDetachShader(program, shader1);
    glDetachShader(program, shader2);
    glDeleteShader(shader1);
    glDeleteShader(shader2);
    return program;
}


void shader_uniform_1i(GLuint program, const char* name, const int value)
{
    // glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, name), value);
    // glUseProgram(0);
}

void shader_uniform_1f(GLuint program, const char* name, const int value)
{
    // glUseProgram(program);
    glUniform1f(glGetUniformLocation(program, name), value);
    // glUseProgram(0);
}

void shader_uniform_3f(GLuint program, const char* name, float x, float y, float z)
{
    const float data[3] = {x, y, z};
    GLint loc = glGetUniformLocation(program, name);
    if (loc == -1) {
        fprintf(stderr, "glGetUniformLocation failed, programId = %u, uniforName = %s\n", program, name);
    }

    glUniform3fv(loc, 1, (GLfloat*)data);
}

void shader_uniform_mat4(GLuint program, const char* name, const float* data)
{
    // glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, (GLfloat*)data);
    // glUseProgram(0);
}

Shader::Shader()
    : mHandle(0)
{

}
Shader::~Shader()
{

}
Shader::Shader(const char *path1, const char *path2)
{
    mHandle = load_program(path1, path2);    
}
Shader::Shader(const char *path1, const char *path2, const char *path3)
{
    mHandle = load_program(path1, path2, path3);    
}

}
