#include "../Utils/InputHandler.h"
#include "../Utils/Math.h"
#include <iostream>
#include "Player.h"


Player::~Player() {}
Player::Player(TexturedModel* model, glm::vec3 pos, float rx, float ry, float rz, float scale) : Entity(model, pos, rx, ry, rz, scale) {}

void Player::move(float delta, Terrain* terrain) {
	checkInputs();
	float distance = delta * currentSpeed[0];
	float distance2 = delta * currentSpeed[1];

	float dx = distance * sin(Math::toRadians(Entity::getRy())) - distance2 * cos(Math::toRadians(Entity::getRy()));
	float dz = distance * cos(Math::toRadians(Entity::getRy())) + distance2 * sin(Math::toRadians(Entity::getRy()));
	Entity::increasePosition(dx, 0, dz);

	//speedDy += GRAVITY * delta;
	float terrainHeight = terrain->getHeightOfTerrain(Entity::getPos().x, Entity::getPos().z);
	Entity::increasePosition(0, speedDy * delta, 0);
	/*if (Entity::getPos().y < terrainHeight) {
		speedDy = 0;
		glm::vec3 curPos = Entity::getPos();
		Entity::setPos(*(new glm::vec3(curPos.x, terrainHeight, curPos.z)));
		this->onGround = true;
	}*/
}

void Player::jump() {
	if (onGround) {
		this->speedDy = JUMP_FORCE;
		this->onGround = false;
	}
}

void Player::checkInputs() {
	if (InputHandler::isKeyDown(GLFW_KEY_W)) {
		this->currentSpeed[0] = RUN_SPEED;
	} else if (InputHandler::isKeyDown(GLFW_KEY_S)) {
		this->currentSpeed[0] = -RUN_SPEED;
	} else {
		this->currentSpeed[0] = 0;
	}

	float tmp = InputHandler::getMouseDx();
	if (tmp != 0.0) {
		float tmpRy = this->getRy();
		this->setRy(tmpRy - (tmp * 0.3f));
	}

	if (InputHandler::isKeyDown(GLFW_KEY_D)) {
		this->currentSpeed[1] = RUN_SPEED;
	} else if (InputHandler::isKeyDown(GLFW_KEY_A)) {
		this->currentSpeed[1] = -RUN_SPEED;
	} else {
		this->currentSpeed[1] = 0;
	}

	if (InputHandler::isKeyDown(GLFW_KEY_SPACE)) {
		this->speedDy = RUN_SPEED;
		//jump();
	} else if (InputHandler::isKeyDown(GLFW_KEY_LEFT_SHIFT)) {
		this->speedDy = -RUN_SPEED;
	} else {
		this->speedDy = 0;
	}
}

void Player::setEyeLevel(float level) {
	this->eyeLevel = level;
}

float Player::getEyeLevel() {
	return this->eyeLevel;
}