#ifndef PARTICLE_SYSTEM
#define PARTICLE_SYSTEM


#include "src/renderer/opengl/openGLExtensions.h"
#include "particle.h"

#include "src/math/mat4.h"

namespace game {
namespace particle {

class ParticleSystem
{
public:
    ParticleSystem();
    virtual ~ParticleSystem();

    void draw(float dt);
    int FindUnusedParticle();
    void SortParticles();

    enum
    {
        MAX_PARTICLES = 1500
    };

    GLuint shaderProgram;
    GLuint texture;
    Particle ParticlesContainer[MAX_PARTICLES];
    GLfloat* g_particule_position_size_data;
    GLubyte* g_particule_color_data;

    GLuint billboard_vertex_buffer;
    GLuint particles_buffer;
    int LastUsedParticle;

    vec3 CameraPosition;
    mat4 view;
    mat4 proj;

};

}

}

#endif
