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
#include "src/game/particle/fireRing.h"

#include "src/util/ddsLoader.h"


namespace {

constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;



class PSystem : public core::Application
{
private:
    float deltaTime;
    mat4 proj;
    mat4 view;
    mat4 model;
    game::particle::FireRing *particleEmitter;
public:
    PSystem()
        : core::Application()
        , deltaTime(0.0f)
        , view()
        , proj()
        , model()
        , particleEmitter(nullptr)
    {


    }
    bool init() {

        view = lookAt(vec3(0.0f, 0.0f, 30.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
        proj = perspective(90.0f, (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);

        particleEmitter = new game::particle::FireRing();
        particleEmitter->setProj(proj);
        particleEmitter->setView(view);


        return true;
    }
    void render()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        particleEmitter->render();
    }

    void idle()
    {
        float d = (float)get_delta_time(timer);
        while (d > 0.0f) {
            deltaTime = fmin(d, 1.0f / 60.0f);
            d -= deltaTime;
        }
        particleEmitter->update(deltaTime);


    }
    
    ~PSystem()
    {
                if (particleEmitter)
                {
                    delete particleEmitter;
                }
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
