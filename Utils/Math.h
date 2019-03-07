#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "../Entities/Camera.h"

class Math
{
public:
	Math();
	~Math();
	static glm::mat4* createTransformationMatrix(glm::vec3 translation, float rx, float ry, float rz, float scale);
	static glm::mat4* createTransformationMatrix(glm::vec2 translation, glm::vec2 scale);
	static glm::mat4* createViewMatrix(Camera* camera);
	static float toRadians(float degrees);
	static float barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);
};

