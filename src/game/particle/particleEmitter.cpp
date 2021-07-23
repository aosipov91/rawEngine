#include "particleEmitter.h"
#include <stdio.h>
#include <string.h>
#include "src/core/timer.h"


namespace game {
namespace particle {

ParticleEmitter::ParticleEmitter()
	: count(0)
	, mTime(0.0f)
	, mTimePerParticle(0.0025f)
        , mInitParticles(false)
        , shaderProgram(nullptr)
        , tex(nullptr)
{

    create();
    createBuffer();
    printf("%lu\n", sizeof(Particle));
    printf("%lu\n", sizeof(Particle_T));
}

ParticleEmitter::~ParticleEmitter()
{
    mAliveParticles = nullptr;
    mDeadParticles = nullptr;
    if (shaderProgram)
    {
        delete shaderProgram;
    }
    if (tex)
    {
        delete tex;
    }
}


void ParticleEmitter::createBuffer()
{
        shaderProgram = new renderer::Shader(
                        "../data/shaders/particles/point_vertex.glsl",
                        "../data/shaders/particles/point_fragment.glsl",
                        "../data/shaders/particles/point_geom.glsl");

    shaderProgram->Bind();
    tex = new renderer::Texture("../data/textures/torch.dds");
    renderer::Uniform<int>::Set(glGetUniformLocation(shaderProgram->GetHandle(), "SpriteTex"), 0);
    //renderer::Uniform<float>::Set(glGetUniformLocation(shaderProgram->GetHandle(), "Size2"), 0.5f);
    glUniform1f(glGetUniformLocation(shaderProgram->GetHandle(), "Size2"), 5.f);
    //renderer::Uniform<float>::Set(glGetUniformLocation(shaderProgram->GetHandle(), "gTime"), get_running_time(timer));
    glUniform1f(glGetUniformLocation(shaderProgram->GetHandle(), "gTime"), mTime);
    //renderer::Uniform<vec3>::Set(glGetUniformLocation(shaderProgram->GetHandle(), "gAccel"), vec3(0.0f, 0.9f, 0.0f));
		renderer::shader_uniform_3f(shaderProgram->GetHandle(), "gAccel", 0.0f, 0.9f, 0.0f);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(Particle_T), 0, GL_STREAM_DRAW);
    float *ptr = 0;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle_T), (GLvoid*)(ptr+=0)); // position
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle_T), (GLvoid*)(ptr+=3)); // vel
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle_T), (GLvoid*)(ptr+=3)); // size
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle_T), (GLvoid*)(ptr+=1)); // time
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle_T), (GLvoid*)(ptr+=1)); // lifeTime
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Particle_T), (GLvoid*)(ptr+=1)); // mass
    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(Particle_T), (GLvoid*)(ptr+=1)); // color
    glEnableVertexAttribArray( 0 );
    glEnableVertexAttribArray( 1 );
    glEnableVertexAttribArray( 2 );
    glEnableVertexAttribArray( 3 );
    glEnableVertexAttribArray( 4 );
    glEnableVertexAttribArray( 5 );
    glEnableVertexAttribArray( 6 );

    glBindVertexArray(0);
}

void ParticleEmitter::create()
{
    memset(mParticles, 0, sizeof(Particle));

    mDeadParticles = &mParticles[0];
    mAliveParticles = nullptr;
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        mParticles[i].next = &mParticles[i+1];
        mParticles[i].lifeTime = -1.0f;
        mParticles[i].initialTime = 0.0f;
    }
    mParticles[MAX_PARTICLES-1].next = nullptr;

    mInitParticles = true;
}


void ParticleEmitter::insert()
{
    Particle *particle;
    if (!mDeadParticles) {
        return;
    }
    particle = mDeadParticles;
    mDeadParticles = particle->next;
    particle->next = mAliveParticles;
    mAliveParticles = particle;

    initParticle(particle);
    count++;
}


void ParticleEmitter::update(float deltaTime)
{
    Particle* p, *next;
    Particle* active, *tail;

    mTime += deltaTime;

    if (!mInitParticles)
        create();

    active = nullptr;
    tail = nullptr;
    for (p = mAliveParticles; p ; p = next)
    {
        next = p->next;
        // Is the particle dead?
        if( (mTime - p->initialTime) > p->lifeTime)
        {
            p->next = mDeadParticles;
            mDeadParticles = p;
            //p->initialColor = vec3();
						p->initialTime = 0.0f;
						p->lifeTime = -1.0f;
            count--;
            continue;
        }

        p->next = nullptr;
        if (!tail) {
            active = tail = p;
        } else { 
            tail->next = p;
            tail = p;
        }
    }
    mAliveParticles = active;

    // A negative or zero mTimePerParticle value denotes
    // not to emit any particles.
    if( mTimePerParticle > 0.0f )
    {
            // Emit particles.
            static float timeAccum = 0.0f;
            timeAccum += deltaTime;
            while( timeAccum >= mTimePerParticle )
            {
                    insert();
                    timeAccum -= mTimePerParticle;
            }
    }
}


void ParticleEmitter::render()
{
    shaderProgram->Bind();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE );

    renderer::Uniform<vec3>::Set(glGetUniformLocation(shaderProgram->GetHandle(), "eyePos"), vec3(0.0f, 0.0f, 30.0f));
    tex->Set(glGetUniformLocation(shaderProgram->GetHandle(), "SpriteTex"), 0);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    float* pData = static_cast<float*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
    for (Particle* p = mAliveParticles; p; p = p->next)
    {
        *(pData++)=p->initialPos.x;
        *(pData++)=p->initialPos.y;
        *(pData++)=p->initialPos.z;
        *(pData++)=p->initialVelocity.x;
        *(pData++)=p->initialVelocity.y;
        *(pData++)=p->initialVelocity.z;
        *(pData++)=p->initialSize;
				*(pData++)=p->initialTime;
        *(pData++)=p->lifeTime;
        *(pData++)=p->mass;
        *(pData++)=p->initialColor.x;
        *(pData++)=p->initialColor.y;
        *(pData++)=p->initialColor.z;

    }
    glUnmapBuffer(GL_ARRAY_BUFFER);

    glDrawArrays(GL_POINTS, 0, count);
    glBindVertexArray(0);

    glDisable(GL_BLEND);

}

void ParticleEmitter::setProj(mat4& proj)
{
    shaderProgram->Bind();
    //renderer::Uniform<mat4>::Set(glGetUniformLocation(shaderProgram->GetHandle(), "ProjectionMatrix"), proj);
    renderer::shader_uniform_mat4(shaderProgram->GetHandle(), "ProjectionMatrix", &proj.v[0]);
}
void ParticleEmitter::setView(mat4& view)
{
    shaderProgram->Bind();
    //renderer::Uniform<mat4>::Set(glGetUniformLocation(shaderProgram->GetHandle(), "ModelViewMatrix"), view);
    renderer::shader_uniform_mat4(shaderProgram->GetHandle(), "ModelViewMatrix", &view.v[0]);
}


}
}
