#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "src/math/vec3.h"

namespace game {
namespace particle {

struct Particle
{
    vec3 initialPos;
    vec3 initialVelocity;
    float initialSize;
    float initialTime;
    float lifeTime;
    float mass;
    vec3 initialColor;

};

}
}

#endif
