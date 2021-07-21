#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "src/math/vec3.h"

namespace game {
namespace particle {

class Particle
{
public:
	Particle()
		: prev(nullptr)
		, next(nullptr)
		, initialPos()
		, initialVelocity()
		, initialSize(0.0f)
		, initialTime(0.0f)
		, lifeTime(0.0f)
		, mass(0.0f)
		, initialColor()
		, alive(false)
	{
	}

	~Particle()
	{
		delete next;	
		next = nullptr;
		prev = nullptr;
	}

	vec3 initialPos;
	vec3 initialVelocity;
	float initialSize;
	float initialTime;
	float lifeTime;
	float mass;
	vec3 initialColor;
	bool alive;

	Particle* prev;
	Particle* next;

};

}
}

#endif
