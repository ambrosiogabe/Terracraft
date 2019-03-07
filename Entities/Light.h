#pragma once

#include "glm/vec3.hpp"

class Light
{
public:
	Light(glm::vec3 position, glm::vec3 color);
	Light(glm::vec3 position, glm::vec3 color, glm::vec3 attenuation);
	~Light();

	glm::vec3 getPosition();
	glm::vec3 getColor();
	glm::vec3 getAttenuation();
	void setPosition(glm::vec3 pos);
	void setColor(glm::vec3 color);

private:
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 attenuation = *(new glm::vec3(1, 0, 0));
};

