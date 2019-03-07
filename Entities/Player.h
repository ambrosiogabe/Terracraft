#pragma once
#include "glm/vec2.hpp"
#include "../World/Terrain.h"
#include "Entity.h"

class Player : public Entity
{
public:
	Player(TexturedModel* model, glm::vec3 pos, float rx, float ry, float rz, float scale);
	~Player();
	void move(float delta, Terrain* terrain);

	void setEyeLevel(float level);
	float getEyeLevel();

private:
	const float RUN_SPEED = 20;
	const float GRAVITY = -100;
	const float JUMP_FORCE = 30;

	const float TERRAIN_HEIGHT = 0;

	glm::vec2 currentSpeed = *(new glm::vec2(0.0, 0.0));
	float eyeLevel = 0.0;
	float speedDy = 0;
	bool onGround = true;

	void checkInputs();
	void jump();
};

