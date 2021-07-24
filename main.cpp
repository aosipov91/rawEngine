#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "src/core/App.h"
#include "src/core/timer.h"

#include "src/renderer/data/mesh.h"
#include "src/renderer/uniform.h"
#include "src/renderer/renderer.h"

#include "src/game/particle/fireRing.h"
#include "src/game/camera.h"
#include "src/game/player.h"
#include "src/game/entity.h"

#include "src/util/ddsLoader.h"

#include "src/scene/loadLevel.h"


namespace {

constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;



class PSystem : public core::Application
{
private:
    float deltaTime;
    glm::mat4 proj;
    glm::mat4 view;
    glm::mat4 model;
    game::particle::FireRing *particleEmitter;
    Camera* camera{};
    game::Player* player{};
    renderer::Renderer* renderer;
    renderer::Shader* levelShaderProgram;
public:
    PSystem()
        : core::Application()
        , deltaTime(0.0f)
        , view()
        , proj()
        , model()
        , particleEmitter(nullptr)
        , renderer(nullptr)
        , levelShaderProgram(nullptr)
    {


    }
    bool init() {
        float aspect = (float)WIDTH/(float)HEIGHT;
        camera = new Camera(aspect);
        camera->pos.z = 20.0f;
        player = new game::Player();

        levelShaderProgram = new renderer::Shader("../data/shaders/basic_vertex.glsl", "../data/shaders/basic_fragment.glsl");
        renderer = new renderer::Renderer();
        LoadCollidableGeometry("../data/env.lvl");
        for (int i = 0; i < entityCount; i++)
        {
            renderer->drawIndexed(entities[i]->obj.f_vertices, entities[i]->obj.nVertices, entities[i]->obj.indices, entities[i]->obj.nIndices);
        }

        particleEmitter = new game::particle::FireRing();
        particleEmitter->setWindowHeight((float)HEIGHT);
        particleEmitter->createBuffer();



        return true;
    }
    void render() final
    {
        glClearColor(.4f, 0.4f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        levelShaderProgram->Bind();
        renderer::shader_uniform_mat4(levelShaderProgram->GetHandle(), "uViewProjM", (const float*)&camera->mViewProj);
        for (int i = 0; i < entityCount; i++)
        {
            renderer::shader_uniform_mat4(levelShaderProgram->GetHandle(), "uModelM", (const float*)&entities[i]->obj.matrix);
            draw_mesh(renderer->batch[i]);
        }
        particleEmitter->render();
    }


    void update(float deltaTime)
    {
        particleEmitter->update(deltaTime);
        particleEmitter->setProj(camera->mProj);
        particleEmitter->setView(camera->mView);
        particleEmitter->setEyePos(camera->pos);

        player->rot.y = mouseDelta.y * 0.001f;
        player->rot.x = mouseDelta.x * 0.001f;
        player->rot.x = glm::clamp(player->rot.x, -glm::half_pi<float>(), +glm::half_pi<float>());

        player->input = 0;
        if (isKeyPressed(KEY_A))
        {
            player->input |= player->LEFT;
        }
        if (isKeyPressed(KEY_D))
        {
            player->input |= player->RIGHT;
        }
        if (isKeyPressed(KEY_W))
        {
            player->input |= player->UP;
        }
        if (isKeyPressed(KEY_S))
        {
            player->input |= player->DOWN;
        }
        player->update(deltaTime);
        camera->pos = player->pos;
        camera->rot = player->rot;
        camera->update();
    }

    void idle() final
    {
        auto d = (float)get_delta_time(timer);
        while (d > 0.0f) {
            deltaTime = fmin(d, 1.0f / 60.0f);
            update(deltaTime);
            d -= deltaTime;
        }
    }
    
    ~PSystem() override
    {
        delete particleEmitter;
        delete camera;
        delete player;
        delete levelShaderProgram;
        delete renderer;
        ClearLevel();
    }
};

}

int main(int argc,char **argv)
{
    auto *glApp = new PSystem();
    
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

    glApp->init();

    glApp->main();
    
    //Engine::clear();
    delete glApp;

    return EXIT_SUCCESS;
}
