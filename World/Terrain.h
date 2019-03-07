#pragma once

#include "../Models/RawModel.h"
#include "../Textures/TerrainTexturePack.h"
#include "glm/vec3.hpp"
#include "../Textures/TerrainTexture.h"
#include "../Engine/Loader.h"

class Terrain
{
public:
	Terrain(int gridX, int gridZ, Loader* loader, TerrainTexturePack* texturePack, TerrainTexture* blendMap, char* heightmap);
	~Terrain();

	float getHeightOfTerrain(float worldX, float worldZ);
	float getX();
	float getZ();
	RawModel* getModel();
	TerrainTexturePack* getTexturePack();
	TerrainTexture* getBlendMap();

private:
	const float SIZE = 800;
	int VERTEX_COUNT = 128;
	const float MAX_HEIGHT = 40;
	const int MAX_PIXEL_COLOR = 255;

	float getHeight(int x, int y, unsigned char* data, int width, int height, int bpp);
	glm::vec3 calculateNormal(int x, int y, unsigned char* data, int width, int height, int bpp);

	float** heights;
	float x, z;
	RawModel* model = nullptr;
	TerrainTexturePack* texturePack = nullptr;
	TerrainTexture* blendMap = nullptr;

	RawModel* generateTerrain(Loader* loader, char* filename);
};

