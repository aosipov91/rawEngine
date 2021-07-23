#include "player.h"

namespace game {

Player::Player()
	: input(0)
{

    pos = glm::vec3(0.0f, PLAYER_HEIGHT, 20.0f);
    velocity = glm::vec3();
    rot = glm::vec3();
}

Player::~Player()
{

}

void Player::update(float deltaTime)
{
	glm::mat4 m = glm::mat4(1.0f);
  //input = 0; // reset bitmask
/*
    // change flags of bitmask
    if (GInput->keyStates[KEY_A]) input |= LEFT;
    if (GInput->keyStates[KEY_D]) input |= RIGHT;
    if (GInput->keyStates[KEY_W]) input |= UP;
    if (GInput->keyStates[KEY_S]) input |= DOWN;
    if (GInput->keyStates[KEY_SPACE]) input |= JUMP;
*/
    float fmove = 0.0f, smove = 0.0f;

    isMoving = false;

    if (input & LEFT)
    {
        smove = -1.0f;
        isMoving = true;
    }
    if (input & RIGHT)
    {
        smove = +1.0f;
        isMoving = true;
    }
    if (input & UP) 
    {
        fmove = -1.0f;
        isMoving = true;
    }
    if (input & DOWN)
    {
        fmove = +1.0f;
        isMoving = true;
    }


		m = glm::rotate(m, -rot.x, glm::vec3(1.0, 0.0, 0.0));
		m = glm::rotate(m, -rot.y, glm::vec3(0.0, 1.0, 0.0));

    // view matrix is inverse of camera matrix
    // and axis stored in columns
		glm::vec3 dir   = glm::vec3(m[0][2], m[1][2], m[2][2]);
		glm::vec3 right = glm::vec3(m[0][0], m[1][0], m[2][0]);

    velocity.x = (dir.x * fmove + right.x * smove) * PLAYER_MAX_SPEED;
    velocity.z = (dir.z * fmove + right.z * smove) * PLAYER_MAX_SPEED;

    velocity.y = 0.0f;


		pos = pos + velocity * deltaTime;
}

}
