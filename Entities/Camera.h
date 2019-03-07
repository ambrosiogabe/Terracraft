#pragma once
#include "Player.h"
#include <glm/vec3.hpp>

class Camera
{
public:
	Camera(Player* player);
	~Camera();

	float getPitch();
	float getYaw();
	float getRoll();
	void move();
	glm::vec3 getPos();

private:
	void toggleView();
	void calculateZoom();
	void calculatePitch();
	void calculatePosition();

	Player* player = nullptr;
	int view = 0;
	glm::vec3 position;
	glm::vec3 forwardVector;

	float distanceFromPlayer = 30;
	float angleAroundPlayer = 0;
	float pitch = 20, yaw = 0, roll = 0;
};

