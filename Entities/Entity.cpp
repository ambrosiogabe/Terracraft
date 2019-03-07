#include "Entity.h"


Entity::~Entity() {}
Entity::Entity(TexturedModel* model, glm::vec3 pos, float rx, float ry, float rz, float scale) {
	this->model = model;
	this->position = pos;
	this->rotX = rx;
	this->rotY = ry;
	this->rotZ = rz;
	this->scale = scale;
}

void Entity::increasePosition(float dx, float dy, float dz) {
	this->position.x += dx;
	this->position.y += dy;
	this->position.z += dz;
}

void Entity::increaseRotation(float rx, float ry, float rz) {
	this->rotX += rx;
	this->rotY += ry;
	this->rotZ += rz;
}

TexturedModel* Entity::getModel() {
	return this->model;
}

glm::vec3 Entity::getPos() {
	return this->position;
}

float Entity::getRx() {
	return this->rotX;
}

float Entity::getRy() {
	return this->rotY;
}

float Entity::getRz() {
	return this->rotZ;
}

float Entity::getScale() {
	return this->scale;
}

void Entity::setModel(TexturedModel* model) {
	this->model = model;
}

void Entity::setPos(glm::vec3 pos) {
	this->position = pos;
}

void Entity::setRx(float rx) {
	this->rotX = rx;
}

void Entity::setRy(float ry) {
	this->rotY = ry;
}

void Entity::setRz(float rz) {
	this->rotZ = rz;
}

void Entity::setScale(float scale) {
	this->scale = scale;
}