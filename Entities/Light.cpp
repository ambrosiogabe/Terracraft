#include "Light.h"



Light::~Light() {}
Light::Light(glm::vec3 position, glm::vec3 color) {
	this->position = position;
	this->color = color;
}

Light::Light(glm::vec3 position, glm::vec3 color, glm::vec3 attenuation) {
	this->position = position;
	this->color = color;
	this->attenuation = attenuation;
}

glm::vec3 Light::getAttenuation() {
	return this->attenuation;
}

glm::vec3 Light::getPosition() {
	return this->position;
}

glm::vec3 Light::getColor() {
	return this->color;
}

void Light::setPosition(glm::vec3 pos) {
	this->position = pos;
}

void Light::setColor(glm::vec3 color) {
	this->color = color;
}
