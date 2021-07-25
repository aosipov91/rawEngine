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

#include "src/scene/loadLevel.h"
#include "src/libTexture/libTexture.h"


namespace {

constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;



class MainApp : public core::Application
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
    renderer::Texture* texture;
    unsigned int ddsTexture1;
    unsigned int ddsTexture2;
    unsigned int ddsTexture3;
    
public:
    MainApp()
        : core::Application()
        , deltaTime(0.0f)
        , view()
        , proj()
        , model()
        , particleEmitter(nullptr)
        , renderer(nullptr)
        , levelShaderProgram(nullptr)
        , texture(nullptr)
    {


    }
    bool init() {
        float aspect = (float)WIDTH/(float)HEIGHT;
        camera = new Camera(aspect);
        camera->pos.z = 20.0f;
        player = new game::Player();

        
        //ddsTexture = createTexture2D(true, "../data/textures/crete_beton_dirt_01.dds");
        //ddsTexture = createTexture2D(false, "../data/textures/tile_walls_pink_01.dds");
        //ddsTexture = createTexture2D(true, "../data/textures/crete_walls_01.dds");

        levelShaderProgram = new renderer::Shader("../data/shaders/basic_vertex.glsl", "../data/shaders/basic_fragment.glsl");
        levelShaderProgram->Bind();
        glUniform1i(glGetUniformLocation(levelShaderProgram->GetHandle(), "Sampler0"), 0);
        renderer = new renderer::Renderer();
        LoadLevelTextured("../data/mesh.geom");

        for (int i = 0; i < entityCount; i++)
        {

            renderer->drawTextured(entities[i]->obj.textured_vertices, entities[i]->obj.nVertices, entities[i]->obj.indices, entities[i]->obj.nIndices);
        }


        //addParticleObject();

        createFBO();

        return true;
    }
    bool createFBO()
    {
        GLuint fbo;
        GLuint depthTexture;
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glGenTextures(1, &depthTexture);
        glBindTexture(GL_TEXTURE_2D, depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, WIDTH, HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        // create render target
        GLuint renderTarget;
        glGenTextures(1, &renderTarget);
        glBindTexture(GL_TEXTURE_2D, renderTarget);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(
                GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr
        );
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTarget, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            printf("Framebuffer not complete\n");
            return false;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return true;
    }
    void render() final
    {
        glClearColor(.4f, 0.4f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glFrontFace(GL_CW);
        glEnable(GL_DEPTH_TEST);
        //glEnable(GL_CULL_FACE);

        levelShaderProgram->Bind();
        renderer::shader_uniform_mat4(levelShaderProgram->GetHandle(), "uViewProjM", (const float*)&camera->mViewProj);

        for (int i = 0; i < entityCount; i++) {
            //entities[i]->obj.diffuseMap->Set(glGetUniformLocation(levelShaderProgram->GetHandle(), "Sampler0"), 0);
            //entities[i]->obj.specularMap->Set(glGetUniformLocation(levelShaderProgram->GetHandle(), "Sampler0"), 0);
            //entities[i]->obj.normalMap->Set(glGetUniformLocation(levelShaderProgram->GetHandle(), "Sampler0"), 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, entities[i]->obj.diffuseMap);
            renderer::shader_uniform_mat4(levelShaderProgram->GetHandle(), "uModelM",
                                          (const float *) &entities[i]->obj.matrix);
            draw_mesh(renderer->batch[i]);
        }

        //particleEmitter->render();
    }

    void addParticleObject()
    {
        particleEmitter = new game::particle::FireRing();
        particleEmitter->setWindowHeight((float)HEIGHT);
        particleEmitter->createBuffer();
    }

    void particleObjectUpdate(float deltaTime)
    {
        particleEmitter->update(deltaTime);
        particleEmitter->setProj(camera->mProj);
        particleEmitter->setView(camera->mView);
        particleEmitter->setEyePos(camera->pos);
    }


    void update(float deltaTime)
    {

        //particleObjectUpdate(deltaTime);
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
    
    ~MainApp() override
    {
        delete particleEmitter;
        delete camera;
        delete player;
        delete levelShaderProgram;
        delete renderer;
        delete texture;
        ClearLevel();
    }
};

}

int main(int argc,char **argv)
{
    auto *mainApp = new MainApp();
    
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
    
    if(!mainApp->setVideoMode(w,h,fs))
    {
        return EXIT_FAILURE;
    }
    
    mainApp->setTitle("3D Engine demo v0.1");

    mainApp->init();

    mainApp->main();
    
    //Engine::clear();
    delete mainApp;

    return EXIT_SUCCESS;
}
