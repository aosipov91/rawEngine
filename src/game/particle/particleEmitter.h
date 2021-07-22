#ifndef PARTICLE_EMITTER_H_
#define PARTICLE_EMITTER_H_


#include "src/math/vec3.h"
#include "src/math/mat4.h"
#include "particle.h"
#include "src/renderer/opengl/openGLExtensions.h"
#include "src/renderer/texture.h"
#include "src/renderer/shader.h"
#include "src/renderer/uniform.h"

namespace game {
namespace particle {

class ParticleEmitter
{
public:
	ParticleEmitter();
	virtual ~ParticleEmitter();

	enum {
		MAX_PARTICLES = 1500
	};

	void create();
        void insert();
	void update(float deltaTime);
	void render();
        void createBuffer();
        void setProj(mat4& proj);
        void setView(mat4& view);

        virtual void initParticle(Particle* out) = 0; 

	Particle mParticles[MAX_PARTICLES];
	Particle mTestParticles[MAX_PARTICLES];
	Particle* mAliveParticles;
	Particle* mDeadParticles;

        bool mInitParticles;

	unsigned int count;
	float mTime; // it's internal time
	float mTimePerParticle;

        GLuint vao;
        GLuint vbo;
        GLuint ebo;
        renderer::Texture *tex;
        renderer::Shader* shaderProgram;
};

}
}

#endif
