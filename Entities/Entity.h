#pragma once

#include "../Models/TexturedModel.h"
#include "glm/glm.hpp"
#include <glm/vec3.hpp>

class Entity
{
public:
	Entity(TexturedModel* model, glm::vec3 pos, float rx, float ry, float rz, float scale);
	TexturedModel* getModel();
	glm::vec3 getPos();
	float getRx();
	float getRy();
	float getRz();
	float getScale();
	void setModel(TexturedModel* model);
	void setPos(glm::vec3 pos);
	void setRx(float rx);
	void setRy(float ry);
	void setRz(float rz);
	void setScale(float scale);

	void increasePosition(float dx, float dy, float dz);
	void increaseRotation(float rx, float ry, float rz);

	~Entity();

private:
	TexturedModel* model;
	glm::vec3 position;
	float rotX, rotY, rotZ;
	float scale;
};

