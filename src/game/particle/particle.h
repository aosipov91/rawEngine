#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <glm/vec3.hpp>

namespace game {
namespace particle {

struct Particle_T
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
