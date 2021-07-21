#include "particleEmitter.h"


namespace game {
namespace particle {

ParticleEmitter::ParticleEmitter()
	: mParticles(nullptr)
	, count(0)
	, mTime(0.0f)
	, mTimePerParticle(0.0f)
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
	particle->initialTime = time;
	particle->lifeTime = life;
	particle->mass = mass;
	particle->initialColor = color;
}

void ParticleEmitter::update(float deltaTime)
{
	mTime += deltaTime;

	Particle *particle = mParticles;

	int index = 0;
	// Loop through all particles
	while(particle != nullptr)
	{
		// Remember next particle
		Particle *nextParticle = particle->next;

		// set flag to remove from list
		bool removeFromList = false;

		if ( (mTime - particle->initialTime) > particle->lifeTime)
		{
			removeFromList = true;
			// add to alive buffer
			mDeadParticles[index] = particle;
		}
		else
		{
			mAliveParticles[index] = particle;
		}

		// remove particle from list if flagged
		if (removeFromList)
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

		if (mTimePerParticle > 0.0f)
		{
			static float timeAccum = 0.0f;	
			timeAccum += deltaTime;
			while(timeAccum >= mTimePerParticle)
			{
				addParticle();	
				timeAccum -= mTimePerParticle;
			}
		}

		count++;
		index++;

		// go to next particle
		particle = nextParticle;
	}
}

void ParticleEmitter::addParticle()
{
	
	add(vec3(0.0f, 4.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f),
						10.0f, 0.5f, 4.0f, 2.0f, vec3(1.0f, 0.0f, 0.0f));
}

void ParticleEmitter::render()
{

}



}
}
