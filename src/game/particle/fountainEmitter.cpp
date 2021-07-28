#include "fountainEmitter.h"
#include "src/libTexture/libTexture.h"
#include "src/math/random.h"

namespace game {
    namespace particle {

        FountainEmitter::FountainEmitter()
                : count(0)
                , mTime(0.0f)
                , mTimePerParticle(0.0025f)
                , shaderProgram(nullptr)
                , mViewportHeight(0.0f)
                , vao(0)
                , vbo(0)
                , mSpawnDir1(-1.0f, -1.0f, -1.0f)
                , mSpawnDir2(1.0f, 1.0f, 1.0f)
                , mColor1(.4f, .2f, 6.0f, .9f)
                , mColor2(.6f, .5f, .2f, .1f)
                , mMinEmitRate(10.0f)
                , mMaxEmitRate(30.f)
        {
            mParticles.resize(MAX_PARTICLES);
            mAliveParticles.resize(MAX_PARTICLES);
            mDeadParticles.resize(MAX_PARTICLES);

            for(int i = 0; i < MAX_PARTICLES; ++i)
            {
                mParticles[i].mLifeTime = -1.0f;
                mParticles[i].mAge = 0.0f;
            }


        }

        FountainEmitter::~FountainEmitter()
        {
            delete shaderProgram;
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDeleteBuffers(1, &vbo);
            glBindVertexArray(0);
            glDeleteVertexArrays(1, &vao);
        }

        void FountainEmitter::addParticle()
        {
            if (!mDeadParticles.empty())
            {
                CParticle* p= mDeadParticles.back();
                initParticle(*p);

                mDeadParticles.pop_back();
                mAliveParticles.push_back(p);
            }
        }


        void FountainEmitter::createBuffer()
        {
            shaderProgram = new renderer::Shader(
                    "../data/shaders/particles/fountain/fountain_vertex.glsl",
                    "../data/shaders/particles/fountain/fountain_fragment.glsl",
                    "../data/shaders/particles/fountain/fountain_geom.glsl");

            shaderProgram->Bind();

            ptTexture = createTexture2D(true, "../data/textures/smoke.dds");
            glUniform1i(glGetUniformLocation(shaderProgram->GetHandle(), "SpriteTex"), 0);
            glUniform1f(glGetUniformLocation(shaderProgram->GetHandle(), "gTime"), mTime);
            glUniform3f(glGetUniformLocation(shaderProgram->GetHandle(), "gAccel"), 0.0f, 0.9f, 0.0f);
            glUniform1f(glGetUniformLocation(shaderProgram->GetHandle(), "glViewportHeight"), mViewportHeight);


            GLintptr vertex_position_offset = 0;
            GLintptr vertex_velocity_offset = 3 * sizeof(float);
            GLintptr vertex_size_offset = 6 * sizeof(float);
            GLintptr vertex_time_offset = 7 * sizeof(float);
            GLintptr vertex_life_offset = 8 * sizeof(float);
            GLintptr vertex_color_offset = 9 * sizeof(float);

            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(RealParticleFountain), nullptr, GL_STREAM_DRAW);
            float *ptr = nullptr;
            glVertexAttribPointer(POSITION_SLOT, 3, GL_FLOAT, GL_FALSE, sizeof(RealParticleFountain), (GLvoid*)vertex_position_offset);
            glVertexAttribPointer(VELOCITY_SLOT, 3, GL_FLOAT, GL_FALSE, sizeof(RealParticleFountain), (GLvoid*)vertex_velocity_offset);
            glVertexAttribPointer(SIZE_SLOT,     1, GL_FLOAT, GL_FALSE, sizeof(RealParticleFountain), (GLvoid*)vertex_size_offset);
            glVertexAttribPointer(TIME_SLOT,     1, GL_FLOAT, GL_FALSE, sizeof(RealParticleFountain), (GLvoid*)vertex_time_offset);
            glVertexAttribPointer(LIFE_SLOT,     1, GL_FLOAT, GL_FALSE, sizeof(RealParticleFountain), (GLvoid*)vertex_life_offset);
            glVertexAttribPointer(COLOR_SLOT,    4, GL_FLOAT, GL_FALSE, sizeof(RealParticleFountain), (GLvoid*)vertex_color_offset);
            //glVertexAttribPointer(MASS_SLOT, 1, GL_FLOAT, GL_FALSE, sizeof(RealParticle), (void*)(ptr+=1));
            glEnableVertexAttribArray( POSITION_SLOT );
            glEnableVertexAttribArray( VELOCITY_SLOT );
            glEnableVertexAttribArray( SIZE_SLOT );
            glEnableVertexAttribArray( TIME_SLOT );
            glEnableVertexAttribArray( LIFE_SLOT );
            glEnableVertexAttribArray( COLOR_SLOT );
            //glEnableVertexAttribArray( MASS_SLOT );
            glBindVertexArray(0);
        }

        void FountainEmitter::update(float deltaTime)
        {
            mTime += deltaTime;


            mDeadParticles.resize(0);
            mAliveParticles.resize(0);

            for (int i = 0; i < MAX_PARTICLES; ++i)
            {
                if ((mTime - mParticles[i].mAge) > mParticles[i].mLifeTime)
                {
                    mDeadParticles.push_back(&mParticles[i]);
                }
                else
                {
                    mAliveParticles.push_back(&mParticles[i]);
                }
            }

/*
            // A negative or zero mTimePerParticle value denotes
            // not to emit any particles.
            if( mTimePerParticle > 0.0f )
            {
                // Emit particles.
                static float timeAccum = 0.0f;
                timeAccum += deltaTime;
                while( timeAccum >= mTimePerParticle )
                {
                    addParticle();
                    timeAccum -= mTimePerParticle;
                }
            }
*/
// create new particles

            float emitRateThisFrame = math::RandomNumber(mMinEmitRate, mMaxEmitRate);
            int numNewParts = emitRateThisFrame * deltaTime;
            mNumNewPartsExcess += (float)(emitRateThisFrame * deltaTime) - numNewParts;
            if (mNumNewPartsExcess > 1.0f)
            {
                numNewParts += (int)mNumNewPartsExcess;
                mNumNewPartsExcess -= (int)mNumNewPartsExcess;
            }

            for(int i = 0; i < numNewParts; i++)
            {
                addParticle();
            }


        }


        void FountainEmitter::render()
        {
            shaderProgram->Bind();

            glDisable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE );

            glUniform1f(glGetUniformLocation(shaderProgram->GetHandle(), "gTime"), mTime);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, ptTexture);
            glBindVertexArray(vao);

            int numActiveParticles = 0;
            auto* pData = static_cast<float*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
            for (auto & mAliveParticle : mAliveParticles)
            {
                *(pData++)=mAliveParticle->mPosition.x;
                *(pData++)=mAliveParticle->mPosition.y;
                *(pData++)=mAliveParticle->mPosition.z;
                *(pData++)=mAliveParticle->mDirection.x;
                *(pData++)=mAliveParticle->mDirection.y;
                *(pData++)=mAliveParticle->mDirection.z;
                *(pData++)=mAliveParticle->mSize;
                *(pData++)=mAliveParticle->mAge;
                *(pData++)=mAliveParticle->mLifeTime;
                *(pData++)=mAliveParticle->mColor.x;
                *(pData++)=mAliveParticle->mColor.y;
                *(pData++)=mAliveParticle->mColor.z;
                *(pData++)=mAliveParticle->mColor.w;
                numActiveParticles++;
            }
            glUnmapBuffer(GL_ARRAY_BUFFER);

            glDrawArrays(GL_POINTS, 0, numActiveParticles);
            glBindVertexArray(0);

            glDisable(GL_BLEND);

        }

        void FountainEmitter::setProj(glm::mat4& proj) const
        {
            //renderer::Uniform<mat4>::Set(glGetUniformLocation(shaderProgram->GetHandle(), "ProjectionMatrix"), proj);
            renderer::shader_uniform_mat4(shaderProgram->GetHandle(), "ProjectionMatrix", glm::value_ptr(proj));
        }
        void FountainEmitter::setView(glm::mat4& view) const
        {
            //renderer::Uniform<mat4>::Set(glGetUniformLocation(shaderProgram->GetHandle(), "ModelViewMatrix"), view);
            renderer::shader_uniform_mat4(shaderProgram->GetHandle(), "ModelViewMatrix", glm::value_ptr(view));
        }

        void FountainEmitter::setEyePos(glm::vec3 &eyePos) const {
            //renderer::shader_uniform_3f(shaderProgram->GetHandle(), "eyePos", eyePos.x, eyePos.y, eyePos.z);
            shaderProgram->Bind();
            glUniform3f(glGetUniformLocation(shaderProgram->GetHandle(), "eyePos"),eyePos.x, eyePos.y, eyePos.z);
        }


    }
}
