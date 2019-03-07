#pragma once

#include "../Shaders/TerrainShader.h"
#include "glm/mat4x4.hpp"
#include <vector>
#include "../World/Terrain.h"
#include "../Textures/TerrainTexturePack.h"
#include "../Textures/TerrainTexture.h"

class TerrainRenderer
{
public:
	TerrainRenderer(TerrainShader* shader, glm::mat4 projectionMatrix);
	~TerrainRenderer();

	void render(std::vector<Terrain*>* terrains);

private:
	TerrainShader* shader = nullptr;

	void prepareTerrain(Terrain* terrain);
	void unbindTexturedModel();
	void loadModelMatrix(Terrain* terrain);
	void bindTextures(Terrain* terrain);
};

