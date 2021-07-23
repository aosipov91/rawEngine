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

	mParticles.resize(MAX_PARTICLES);
	mAliveParticles.resize(MAX_PARTICLES);
	mDeadParticles.resize(MAX_PARTICLES);

	for(int i = 0; i < MAX_PARTICLES; ++i)
	{
		mParticles[i].lifeTime = -1.0f;	
		mParticles[i].initialTime = 0.0f;
	}

    createBuffer();
}

ParticleEmitter::~ParticleEmitter()
{
    if (shaderProgram)
    {
        delete shaderProgram;
    }
    if (tex)
    {
        delete tex;
    }
}

void ParticleEmitter::addParticle()
{
		if (mDeadParticles.size() > 0)
		{
				Particle* p= mDeadParticles.back();	
				initParticle(*p);

				mDeadParticles.pop_back();
				mAliveParticles.push_back(p);
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
    //renderer::Uniform<int>::Set(glGetUniformLocation(shaderProgram->GetHandle(), "SpriteTex"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram->GetHandle(), "SpriteTex"), 0);
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
    shaderProgram->Bind();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE );

    glUniform1f(glGetUniformLocation(shaderProgram->GetHandle(), "gTime"), mTime);
		renderer::shader_uniform_3f(shaderProgram->GetHandle(), "eyePos", 0.0f, 0.0f, 20.0f);
    tex->Set(glGetUniformLocation(shaderProgram->GetHandle(), "SpriteTex"), 0);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    float* pData = static_cast<float*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
    for (int i = 0; i < mAliveParticles.size(); ++i)
    {
        *(pData++)=mAliveParticles[i]->initialPos.x;
        *(pData++)=mAliveParticles[i]->initialPos.y;
        *(pData++)=mAliveParticles[i]->initialPos.z;
        *(pData++)=mAliveParticles[i]->initialVelocity.x;
        *(pData++)=mAliveParticles[i]->initialVelocity.y;
        *(pData++)=mAliveParticles[i]->initialVelocity.z;
        *(pData++)=mAliveParticles[i]->initialSize;
				*(pData++)=mAliveParticles[i]->initialTime;
        *(pData++)=mAliveParticles[i]->lifeTime;
        *(pData++)=mAliveParticles[i]->mass;
        *(pData++)=mAliveParticles[i]->initialColor.x;
        *(pData++)=mAliveParticles[i]->initialColor.y;
        *(pData++)=mAliveParticles[i]->initialColor.z;

    }
    glUnmapBuffer(GL_ARRAY_BUFFER);

    glDrawArrays(GL_POINTS, 0, mAliveParticles.size());
    glBindVertexArray(0);

    glDisable(GL_BLEND);

}

void ParticleEmitter::setProj(glm::mat4& proj)
{
    shaderProgram->Bind();
    //renderer::Uniform<mat4>::Set(glGetUniformLocation(shaderProgram->GetHandle(), "ProjectionMatrix"), proj);
    renderer::shader_uniform_mat4(shaderProgram->GetHandle(), "ProjectionMatrix", glm::value_ptr(proj));
}
void ParticleEmitter::setView(glm::mat4& view)
{
    shaderProgram->Bind();
    //renderer::Uniform<mat4>::Set(glGetUniformLocation(shaderProgram->GetHandle(), "ModelViewMatrix"), view);
    renderer::shader_uniform_mat4(shaderProgram->GetHandle(), "ModelViewMatrix", glm::value_ptr(view));
}


}
}
