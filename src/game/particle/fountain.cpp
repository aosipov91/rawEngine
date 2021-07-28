#include "fountain.h"

#include <cmath>
#include "src/math/random.h"
#include "src/math/constant.h"


namespace game {
    namespace particle {

        Fountain::Fountain()
            : FountainEmitter()
        {}
        Fountain::~Fountain() = default;


        void Fountain::initParticle(CParticle& out)
        {
            out.mPosition = glm::vec3(0.0f);
            out.mDirection = glm::vec3(math::RandomNumber(mSpawnDir1.x, mSpawnDir2.x),
                                       math::RandomNumber(mSpawnDir1.y, mSpawnDir2.y),
                                       math::RandomNumber(mSpawnDir1.z, mSpawnDir2.z));

            out.mColor = glm::vec4(math::RandomNumber(mColor1.x, mColor2.x),
                                       math::RandomNumber(mColor1.y, mColor2.y),
                                       math::RandomNumber(mColor1.z, mColor2.z),
                                        math::RandomNumber(mColor1.w, mColor2.w));
            out.mSize = .3f;
            out.mAge = mTime;
            out.mLifeTime = 4.0f;
        }


    }
}

