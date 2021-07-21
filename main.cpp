#include <stdio.h>
#include <stdlib.h>
#include <cmath>

#include "src/core/App.h"
#include "src/core/timer.h"

#include "src/renderer/shader.h"
#include "src/renderer/data/mesh.h"
#include "src/renderer/texture.h"
#include "src/renderer/uniform.h"

#include "src/game/particle/particleEmitter.h"

#include "src/util/ddsLoader.h"


namespace {

constexpr int WIDTH = 600;
constexpr int HEIGHT = 600;

class PSystem : public core::Application
{
private:
    float deltaTime;
	renderer::Shader* shaderProgram;
    Mesh* mesh;
    mat4 proj;
    mat4 view;
    mat4 model;
		renderer::Texture *tex;
		game::particle::ParticleEmitter* particleEmitter;
public:
    PSystem()
        : core::Application()
        , deltaTime(0.0f)
        , mesh(nullptr)
        , tex(0)
        , view()
        , proj()
        , model()
        , shaderProgram(nullptr)
				, particleEmitter(nullptr)
    {


    }
    bool init() {

				view = lookAt(vec3(0.0f, 0.0f, 4.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
				proj = perspective(90.0f, (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
				shaderProgram = new renderer::Shader("../data/shaders/trig_vertex.glsl", "../data/shaders/trig_fragment.glsl");
				shaderProgram->Bind();
				tex = new renderer::Texture("../data/textures/torch.dds");
				renderer::Uniform<int>::Set(shaderProgram->GetUniform("Sampler0"), 0);
				renderer::Uniform<mat4>::Set(shaderProgram->GetUniform("proj"), proj);
				renderer::Uniform<mat4>::Set(shaderProgram->GetUniform("view"), view);
				float vertices[] = {
					-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,	
					-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
					1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
					1.0f, -1.0f, 0.0f, 1.0f, 0.0f
				};
				unsigned int indices[] = {0, 1, 2, 0, 2, 3};

				mesh = create_mesh(vertices, 4, indices,6);
				particleEmitter = new game::particle::ParticleEmitter();
				particleEmitter->add(vec3(0.0f, 4.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f),
						10.0f, 0.5f, 4.0f, 2.0f, vec3(1.0f, 0.0f, 0.0f));
				particleEmitter->add(vec3(0.0f, 4.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f),
						10.0f, 0.5f, 4.0f, 2.0f, vec3(1.0f, 0.0f, 0.0f));
				particleEmitter->add(vec3(0.0f, 4.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f),
						10.0f, 0.5f, 4.0f, 2.0f, vec3(1.0f, 0.0f, 0.0f));
				particleEmitter->add(vec3(0.0f, 4.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f),
						10.0f, 0.5f, 4.0f, 2.0f, vec3(1.0f, 0.0f, 0.0f));
				particleEmitter->add(vec3(0.0f, 4.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f),
						10.0f, 0.5f, 4.0f, 2.0f, vec3(1.0f, 0.0f, 0.0f));
				particleEmitter->add(vec3(0.0f, 4.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f),
						10.0f, 0.5f, 4.0f, 2.0f, vec3(1.0f, 0.0f, 0.0f));
				particleEmitter->update(deltaTime);
				printf("%d\n", particleEmitter->count);
        return true;
    }
    void render()
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
				shaderProgram->Bind();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tex->GetHandle());
				draw_mesh(mesh);
        //glEnable(GL_DEPTH_TEST);
    }

    void idle()
    {
        float d = (float)get_delta_time(timer);
        while (d > 0.0f) {
            deltaTime = fmin(d, 1.0f / 60.0f);
            d -= deltaTime;
        }

    }
    
    ~PSystem()
    {
		destroy_mesh(mesh);
		delete tex;
		delete shaderProgram;
		delete particleEmitter;
    }
};

}

int main(int argc,char **argv)
{
    PSystem *glApp = new PSystem();
    
    int w = WIDTH;
    int h = HEIGHT;
    bool fs = false;
    for(int i = 1; i < argc; i++)
    {
        if(!strcmp(argv[i],"-fs"))
        {
            fs = true;
        }
        else if(!strcmp(argv[i],"-w"))
        {
            sscanf(argv[++i],"%d",&w);
        }
        else if(!strcmp(argv[i],"-h"))
        {
            sscanf(argv[++i],"%d",&h);
        }
    }
    
    if(!glApp->setVideoMode(w,h,fs))
    {
        return EXIT_FAILURE;
    }
    
    glApp->setTitle("3D Engine demo v0.1");
    
    if(!glApp->init()) {
            delete glApp;
            return EXIT_FAILURE;
    }
    
    glApp->main();
    
    //Engine::clear();
    delete glApp;

    return EXIT_SUCCESS;
}
