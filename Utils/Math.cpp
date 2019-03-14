#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Math.h"

#define PI 3.14159265

Math::Math() {}


Math::~Math() {}

float Math::barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos) {
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

glm::mat4* Math::createTransformationMatrix(glm::vec2 translation, glm::vec2 scale) {
	glm::mat4 matrix = glm::mat4(1.0f);
	matrix = glm::translate(matrix, glm::vec3(translation.x, translation.y, 0.f));
	matrix = glm::scale(matrix, glm::vec3(scale.x, scale.y, 1.f));
	return &matrix;
}

glm::mat4* Math::createTransformationMatrix(glm::vec3 translation, float rx, float ry, float rz, float scale) {
	glm::mat4* matrix = new glm::mat4(1.0f);
	*matrix = glm::translate(*matrix, translation);
	*matrix = glm::rotate(*matrix, glm::radians(rx), *(new glm::vec3(1.0f, 0.0f, 0.0f)) );
	*matrix = glm::rotate(*matrix, glm::radians(ry), *(new glm::vec3(0.0f, 1.0f, 0.0f)) );
	*matrix = glm::rotate(*matrix, glm::radians(rz), *(new glm::vec3(0.0f, 0.0f, 1.0f)) );
	*matrix = glm::scale(*matrix, *(new glm::vec3(scale, scale, scale)) );
	return matrix;
}

glm::mat4* Math::createViewMatrix(Camera* camera) {
	glm::mat4* matrix = new glm::mat4(1.0f);
	*matrix = glm::rotate(*matrix, glm::radians(camera->getPitch()), *(new glm::vec3(1.0f, 0.0f, 0.0f)));
	*matrix = glm::rotate(*matrix, glm::radians(camera->getYaw()), *(new glm::vec3(0.0f, 1.0f, 0.0f)));
	glm::vec3 cameraPos = camera->getPos();
	glm::vec3 negCameraPos = *(new glm::vec3(-cameraPos.x, -cameraPos.y, -cameraPos.z));
	*matrix = glm::translate(*matrix, negCameraPos);
	return matrix;
}

float Math::toRadians(float degrees) {
	return (degrees * PI) / 180.0;
}