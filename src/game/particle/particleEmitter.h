#ifndef PARTICLE_EMITTER_H_
#define PARTICLE_EMITTER_H_


#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "particle.h"
#include "src/renderer/opengl/openGLExtensions.h"
#include "src/renderer/texture.h"
#include "src/renderer/shader.h"

#include <vector>

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

	void update(float deltaTime);
	void render();
        void createBuffer();
        void setProj(glm::mat4& proj);
        void setView(glm::mat4& view);
	void addParticle();

        virtual void initParticle(Particle& out) = 0; 

	std::vector<Particle> mParticles;
	std::vector<Particle*> mAliveParticles;
	std::vector<Particle*> mDeadParticles;

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
