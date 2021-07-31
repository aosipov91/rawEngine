#include "particleEmitter.h"
#include "src/libTexture/libTexture.h"
#include "src/math/random.h"

namespace game {
namespace particle {

ParticleEmitter::ParticleEmitter()
    : count(0)
    , mTime(0.0f)
    , mTimePerParticle(0.0025f)
    , mViewportHeight(0.0f)
    , vao(0)
    , vbo(0)
{
    mParticles.resize(MAX_PARTICLES);
    mAliveParticles.resize(MAX_PARTICLES);
    mDeadParticles.resize(MAX_PARTICLES);

    for(int i = 0; i < MAX_PARTICLES; ++i)
    {
        mParticles[i].lifeTime = -1.0f;
        mParticles[i].initialTime = 0.0f;
    }


}

ParticleEmitter::~ParticleEmitter()
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vbo);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);
}

void ParticleEmitter::addParticle()
{
        if (!mDeadParticles.empty())
        {
            RealParticle* p= mDeadParticles.back();
            initParticle(*p);

            mDeadParticles.pop_back();
            mAliveParticles.push_back(p);
        }
}


void ParticleEmitter::createBuffer()
{
    program = renderer::load_program(
                        "../data/shaders/particles/point_vertex.glsl",
                        "../data/shaders/particles/point_fragment.glsl",
                        "../data/shaders/particles/point_geom.glsl");

    glUseProgram(program);

    ptTexture = createTexture2D(true, "../data/textures/torch.dds");
    //renderer::Uniform<int>::Set(glGetUniformLocation(shaderProgram->GetHandle(), "SpriteTex"), 0);
    glUniform1i(glGetUniformLocation(program, "SpriteTex"), 0);
    //renderer::Uniform<float>::Set(glGetUniformLocation(shaderProgram->GetHandle(), "Size2"), 0.5f);
    //renderer::Uniform<float>::Set(glGetUniformLocation(shaderProgram->GetHandle(), "gTime"), get_running_time(timer));
    glUniform1f(glGetUniformLocation(program, "gTime"), mTime);
    //renderer::Uniform<vec3>::Set(glGetUniformLocation(shaderProgram->GetHandle(), "gAccel"), vec3(0.0f, 0.9f, 0.0f));
    renderer::shader_uniform_3f(program, "gAccel", 0.0f, 0.9f, 0.0f);
    glUniform1f(glGetUniformLocation(program, "glViewportHeight"), mViewportHeight);


    GLintptr vertex_position_offset = 0;
    GLintptr vertex_velocity_offset = 3 * sizeof(float);
    GLintptr vertex_size_offset = 6 * sizeof(float);
    GLintptr vertex_time_offset = 7 * sizeof(float);
    GLintptr vertex_life_offset = 8 * sizeof(float);
    GLintptr vertex_mass_offset = 9 * sizeof(float);
    GLintptr vertex_color_offset = 10 * sizeof(float);


    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(RealParticle), nullptr, GL_STREAM_DRAW);
    float *ptr = nullptr;
    glVertexAttribPointer(POSITION_SLOT, 3, GL_FLOAT, GL_FALSE, sizeof(RealParticle), (GLvoid*)vertex_position_offset);
    glVertexAttribPointer(VELOCITY_SLOT, 3, GL_FLOAT, GL_FALSE, sizeof(RealParticle), (void*)vertex_velocity_offset);
    glVertexAttribPointer(SIZE_SLOT, 1, GL_FLOAT, GL_FALSE, sizeof(RealParticle), (void*)vertex_size_offset);
    glVertexAttribPointer(TIME_SLOT, 1, GL_FLOAT, GL_FALSE, sizeof(RealParticle), (void*)vertex_time_offset);
    glVertexAttribPointer(LIFE_SLOT, 1, GL_FLOAT, GL_FALSE, sizeof(RealParticle), (void*)vertex_life_offset);
    glVertexAttribPointer(MASS_SLOT, 1, GL_FLOAT, GL_FALSE, sizeof(RealParticle), (void*)vertex_mass_offset);
    glVertexAttribPointer(COLOR_SLOT, 4, GL_FLOAT, GL_FALSE, sizeof(RealParticle), (void*)vertex_color_offset);
    glEnableVertexAttribArray( POSITION_SLOT );
    glEnableVertexAttribArray( VELOCITY_SLOT );
    glEnableVertexAttribArray( SIZE_SLOT );
    glEnableVertexAttribArray( TIME_SLOT );
    glEnableVertexAttribArray( LIFE_SLOT );
    glEnableVertexAttribArray( MASS_SLOT );
    glEnableVertexAttribArray( COLOR_SLOT );
    glBindVertexArray(0);
}

void ParticleEmitter::update(float deltaTime)
{
    mTime += deltaTime;


    mDeadParticles.resize(0);
    mAliveParticles.resize(0);

    for (int i = 0; i < MAX_PARTICLES; ++i)
    {
        if ((mTime - mParticles[i].initialTime) > mParticles[i].lifeTime)
        {
            mDeadParticles.push_back(&mParticles[i]);
        }
        else
        {
            mAliveParticles.push_back(&mParticles[i]);
        }
    }


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




}


void ParticleEmitter::render()
{
    glUseProgram(program);

    //glDisable(GL_DEPTH_TEST);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_ONE, GL_ONE );

    glUniform1f(glGetUniformLocation(program, "gTime"), mTime);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ptTexture);
    glBindVertexArray(vao);

    int numActiveParticles = 0;
    float* pData = (float*)glMapBufferRange(GL_ARRAY_BUFFER, 0, mAliveParticles.size() * sizeof(RealParticle),  GL_MAP_WRITE_BIT  | GL_MAP_FLUSH_EXPLICIT_BIT );
    glFlushMappedBufferRange(GL_ARRAY_BUFFER, 0, mAliveParticles.size() * sizeof(RealParticle));
    for (auto & mAliveParticle : mAliveParticles)
    {
        *(pData++)=mAliveParticle->initialPos.x;
        *(pData++)=mAliveParticle->initialPos.y;
        *(pData++)=mAliveParticle->initialPos.z;
        *(pData++)=mAliveParticle->initialVelocity.x;
        *(pData++)=mAliveParticle->initialVelocity.y;
        *(pData++)=mAliveParticle->initialVelocity.z;
        *(pData++)=mAliveParticle->initialSize;
        *(pData++)=mAliveParticle->initialTime;
        *(pData++)=mAliveParticle->lifeTime;
        *(pData++)=mAliveParticle->mass;
        *(pData++)=mAliveParticle->initialColor.x;
        *(pData++)=mAliveParticle->initialColor.y;
        *(pData++)=mAliveParticle->initialColor.z;
        *(pData++)=mAliveParticle->initialColor.w;
        numActiveParticles++;
    }
    pData = 0;
    glUnmapBuffer(GL_ARRAY_BUFFER);

    glDrawArrays(GL_POINTS, 0, mAliveParticles.size());
    glBindVertexArray(0);

    //glDisable(GL_BLEND);

}

void ParticleEmitter::setProj(glm::mat4& proj)
{
    //renderer::Uniform<mat4>::Set(glGetUniformLocation(shaderProgram->GetHandle(), "ProjectionMatrix"), proj);
    renderer::shader_uniform_mat4(program, "ProjectionMatrix", glm::value_ptr(proj));
}
void ParticleEmitter::setView(glm::mat4& view)
{
    //renderer::Uniform<mat4>::Set(glGetUniformLocation(shaderProgram->GetHandle(), "ModelViewMatrix"), view);
    renderer::shader_uniform_mat4(program, "ModelViewMatrix", glm::value_ptr(view));
}

void ParticleEmitter::setEyePos(glm::vec3 &eyePos){
    renderer::shader_uniform_3f(program, "eyePos", eyePos.x, eyePos.y, eyePos.z);
}


}
}
