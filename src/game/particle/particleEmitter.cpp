#include "particleEmitter.h"
#include <stdio.h>


namespace game {
namespace particle {

ParticleEmitter::ParticleEmitter()
	: mParticles(nullptr)
	, count(0)
	, mTime(0.0f)
	, mTimePerParticle(0.0025f)
{

}

ParticleEmitter::~ParticleEmitter()
{
	if (mParticles)
	{
		delete mParticles;
		mParticles = nullptr;
	}
}

void ParticleEmitter::create()
{
}

void ParticleEmitter::add(vec3 pos, vec3 velocity, float size, float time, float life, float mass, vec3 color)
{
	Particle *particle = new Particle();	
	particle->prev = nullptr;
	if((particle->next = mParticles))
	{
		mParticles->prev = particle;	
	}
	mParticles = particle;

	particle->initialPos = pos;
	particle->initialVelocity = velocity;
	particle->initialSize = size;
	particle->initialTime = mTime;
	particle->lifeTime = life;
	particle->mass = mass;
	particle->initialColor = color;
	particle->alive = true;
}

void ParticleEmitter::update(float deltaTime)
{

	mTime += deltaTime;

	Particle *particle = mParticles;

	// Loop through all particles
	while(particle != nullptr)
	{
		// Remember next particle
		Particle *nextParticle = particle->next;

		// set flag to remove from list
		bool removeFromList = false;

		if (particle->lifeTime)
		{
			if( (mTime - particle->initialTime > particle->lifeTime))
			{
				removeFromList = true;	
			}	
			else
			{
				particle->lifeTime = 0.0;	
			}
		}


		// remove particle from list if flagged
		if (removeFromList == true)
		{
			if (particle->prev)
			{
				particle->prev->next = particle->next;	
			}	
			else
			{
				mParticles = particle->next;	
			}

			if (particle->next)
			{
				particle->next->prev = particle->prev;	
			}

			particle->prev = nullptr;
			particle->next = nullptr;
			delete particle;
		}


		count++;

		// go to next particle
		particle = nextParticle;
	}
}

void ParticleEmitter::handleSmoke(float deltaTime)
{
	if (mTimePerParticle > 0.0f)
	{
		static float timeAccum = 0.0f;
		timeAccum += deltaTime;
		while (timeAccum >= mTimePerParticle)
		{
			add(vec3(0.0f, 4.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f),
						mTime, 2.5f, .5f, 2.0f, vec3(1.0f, 0.0f, 0.0f));
			timeAccum -= mTimePerParticle;
		}
	}
}

void ParticleEmitter::render()
{

}



}
}
