#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <glm/vec3.hpp>

namespace game {
namespace particle {

class CParticle
{
public:
  CParticle() { initialSize = 1.0f; lifeTime = 1.0f; age = 0.0f; mass = 0.0f; }
  virtual ~CParticle() { }

    glm::vec3 initialPos;
    glm::vec3 initialVelocity;
    float initialSize;
    float age; // is age of particle
    float lifeTime; // is time of life
    float mass;
    glm::vec3 initialColor;

  // this function is inline not because it's small, but because it's only called
  // in one place (CParticleEmitter.Update()).  This way we can also dodge the
  // performance hit associated with calling a function many times (remember, we're
  // in a for loop in CParticleEmitter.Update()).

  // your needs may vary.  For example, if you can afford the speed hit, it might
  // be cool to make this a virtual function.
  inline bool Update(float fTimeDelta)
  {
    // age the particle
		age += fTimeDelta;

    // if this particle's age is greater than it's lifetime, it dies.
    if (age >= lifeTime) {
      return(false); // dead!
    }

		// move particle
		initialPos += initialVelocity * fTimeDelta;

    return(true); // particle stays alive
  }
};


struct RealParticle
{
    glm::vec3 initialPos;
    glm::vec3 initialVelocity;
    float initialSize;
    float initialTime;
    float lifeTime;
    float mass;
    glm::vec3 initialColor;
};

struct Particle
{
	glm::vec3 initialPos;
	glm::vec3 initialVelocity;
    float initialSize;
    float initialTime;
    float lifeTime;
    float mass;
    glm::vec3 initialColor;
    bool alive;

    Particle* next;


};

}
}

#endif
