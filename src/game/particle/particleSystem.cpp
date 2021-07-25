#include "particleSystem.h"

#include "src/renderer/shader.h"
#include "src/util/ddsLoader.h"

#include "src/core/timer.h"

#include <algorithm>

namespace game {
namespace particle {

ParticleSystem::ParticleSystem()
    : shaderProgram(0)
    , texture(0)
    , LastUsedParticle(0)
{

}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::draw(float dt)
{

}

void ParticleSystem::SortParticles(){
}

int ParticleSystem::FindUnusedParticle(){


	return 0; // All particles are taken, override the first one
}


}

}
