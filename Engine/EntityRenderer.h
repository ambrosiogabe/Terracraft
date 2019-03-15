#pragma once

#include <glm/mat4x4.hpp>
#include "../Entities/Entity.h"
#include "../Shaders/StaticShader.h"
#include "../Models/TexturedModel.h"
#include <map>
#include <vector>

class EntityRenderer
{
public:
	EntityRenderer(StaticShader* shader, glm::mat4 projectionMatrix);
	~EntityRenderer();

	void render(std::map<TexturedModel*, std::vector<Entity>>* entities);

private:
	void prepareTexturedModel(TexturedModel* model);
	void unbindTexturedModel();
	void prepareInstance(Entity* entity);

	StaticShader* shader = nullptr;
};

