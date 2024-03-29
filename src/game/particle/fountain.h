#ifndef FOUNTAIN_H_
#define FOUNTAIN_H_

#include "fountainEmitter.h"

namespace game {
    namespace particle {

        class Fountain : public FountainEmitter
        {
        public:
            Fountain(glm::vec3 pos);
            ~Fountain() override;
            void initParticle(CParticle& out);
        };

    }
}

#endif
