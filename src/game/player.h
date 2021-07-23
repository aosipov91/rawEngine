#ifndef PLAYER_H_
#define PLAYER_H_

#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace game {

constexpr float PLAYER_MAX_SPEED = 16.0f;
constexpr float PLAYER_WALK_SPEED = 16.0f;
constexpr float PLAYER_HEIGHT	= 1.75f;

class Player
{
public:
	Player();
	~Player();
	void update(float deltaTime);

	int input;

    glm::vec3 pos;                   // position of player
    glm::vec3 rot;                   // angle of rotation
    glm::vec3 velocity;

    glm::vec3 gunPos;		// position of gun relative to player
    glm::vec3 cameraForward;		// forwards vector of player
    glm::vec3 cameraSide;		        // sideway vector of player (used for strafing)
    glm::vec3 cameraUp;			// up vector of player
    glm::vec3 cameraLook;

    float gunWalkBob;
    float gunWalkBobAmount;

		glm::mat4 gunMat;
    bool isMoving;

    float shootTime;

		enum
		{
			LEFT    = 1,
			RIGHT   = 2,
			UP      = 4,
			DOWN    = 8,
			JUMP    = 16,
			FIRE_A  = 32,
			FIRE_B  = 64,
		};
};

}

#endif
