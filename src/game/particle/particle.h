#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
namespace game {
namespace particle {

class CParticle
{
public:
  CParticle() { mSize = 1.0f; mLifeTime = 1.0f; mAge = 0.0f; }
  virtual ~CParticle() { }

  float mSize;
  float mLifeTime;
  float mAge;
  glm::vec4 mColor;
  glm::vec3 mColorStep;

  glm::vec3 mPosition;
  glm::vec3 mDirection;

  // this function is inline not because it's small, but because it's only called
  // in one place (CParticleEmitter.Update()).  This way we can also dodge the
  // performance hit associated with calling a function many times (remember, we're
  // in a for loop in CParticleEmitter.Update()).

  // your needs may vary.  For example, if you can afford the speed hit, it might
  // be cool to make this a virtual function.
  inline bool Update(float fTimeDelta)
  {
    // age the particle
		mAge += fTimeDelta;

    // if this particle's age is greater than it's lifetime, it dies.
    if (mAge >= mLifeTime) {
      return(false); // dead!
    }

		// move particle
		mPosition += mDirection * fTimeDelta;

    return(true); // particle stays alive
  }
};
    struct RealParticleFountain
    {
        glm::vec3 initialPos;
        glm::vec3 initialVelocity;
        glm::vec4 color;
        float initialSize;
        float initialTime;
        float lifeTime;
    };

struct RealParticle
{
    glm::vec3 initialPos;
    glm::vec3 initialVelocity;
    float initialSize;
    float initialTime;
    float lifeTime;
    float mass;
    glm::vec4 initialColor;
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
