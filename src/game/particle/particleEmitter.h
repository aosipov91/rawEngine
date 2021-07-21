#ifndef PARTICLE_EMITTER_H_
#define PARTICLE_EMITTER_H_


#include "src/math/vec3.h"
#include "particle.h"

namespace game {
namespace particle {

class ParticleEmitter final
{
public:
	ParticleEmitter();
	~ParticleEmitter();

	enum {
		MAX_PARTICLES = 10	
	};

	void create();
	void add(vec3 pos, vec3 velocity, float size, float time, float life, float mass, vec3 color);
	void update(float deltaTime);
	void render();
	void handleSmoke(float deltaTime);

	Particle* mParticles;
	Particle* mAliveParticles[MAX_PARTICLES];
	Particle* mDeadParticles[MAX_PARTICLES];
	unsigned int count;
	float mTime; // it's internal time
	float mTimePerParticle;
};

}
}

#endif
