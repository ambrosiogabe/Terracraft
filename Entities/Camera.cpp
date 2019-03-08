#include "../Utils/InputHandler.h"
#include "../Utils/Math.h"
#include "Camera.h"

#define SPEED 0.0008f;

Camera::~Camera() {}
Camera::Camera(Player* player) {
	this->player = player;
	calculatePosition();
}

void Camera::move() {
	if (InputHandler::getKeyPress(GLFW_KEY_F5)) {
		toggleView();
	}

	calculateZoom();
	calculatePitch();

	calculatePosition();
	this->yaw = 180 - player->getRy();
}

void Camera::calculatePosition() {
	float dx = cos(Math::toRadians(pitch));
	float dy = sin(Math::toRadians(pitch));
	if (view == 0) {
		dx *= distanceFromPlayer;
		dy *= distanceFromPlayer;
	} else if (view == 1) {
		dx *= -0.01f;
		dy *= -0.01f;
	}

	float newY = player->getPos().y + dy + player->getEyeLevel();
	float newX = player->getPos().x - (dx * sin(Math::toRadians(player->getRy())));
	float newZ = player->getPos().z - (dx * cos(Math::toRadians(player->getRy())));
	this->position[0] = newX;
	//if (newY > 0.0) this->position[1] = newY;
	this->position[1] = newY;
	this->position[2] = newZ;
}

void Camera::toggleView() {
	this->view += 1;
	if (this->view == 2) {
		this->view = 0;
	}
}

void Camera::calculateZoom() {
	float zoomLevel = InputHandler::getMouseScroll();
	if (distanceFromPlayer - zoomLevel > 15 && distanceFromPlayer - zoomLevel < 31)
		distanceFromPlayer -= zoomLevel;
}

void Camera::calculatePitch() {
	float pitchChange = InputHandler::getMouseDy() * 0.1f;
	pitch += pitchChange;
}

glm::vec3 Camera::getPos() {
	return this->position;
}

float Camera::getPitch() {
	return this->pitch;
}

float Camera::getYaw() {
	return this->yaw;
}

float Camera::getRoll() {
	return this->roll;
}
