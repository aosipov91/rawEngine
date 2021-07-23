#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "src/core/App.h"
#include "src/core/timer.h"

#include "src/renderer/data/mesh.h"
#include "src/renderer/uniform.h"

#include "src/game/particle/fireRing.h"
#include "src/game/camera.h"
#include "src/game/player.h"

#include "src/util/ddsLoader.h"


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
        float aspect = (float)WIDTH/(float)HEIGHT;
		camera = new Camera(aspect);
		camera->pos.z = 20.0f;
    	player = new game::Player();
        particleEmitter = new game::particle::FireRing();
        particleEmitter->setWindowHeight((float)HEIGHT);
        particleEmitter->createBuffer();
        return true;
    }
    void render() final
    {
        glClearColor(.4f, 0.4f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        particleEmitter->render();
    }


    void update(float deltaTime)
    {
        particleEmitter->update(deltaTime);
		particleEmitter->setProj(camera->mProj);
		particleEmitter->setView(camera->mView);

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
