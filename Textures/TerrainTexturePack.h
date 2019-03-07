#pragma once

#include "TerrainTexture.h"

class TerrainTexturePack
{
public:
	TerrainTexturePack(TerrainTexture* backgroundTexture, TerrainTexture* rTexture, TerrainTexture* gTexture, TerrainTexture* bTexture);
	~TerrainTexturePack();
	TerrainTexture* getBackgroundTexture();
	TerrainTexture* getRTexture();
	TerrainTexture* getGTexture();
	TerrainTexture* getBTexture();

private:
	TerrainTexture* backgroundTexture = nullptr;
	TerrainTexture* rTexture = nullptr;
	TerrainTexture* gTexture = nullptr;
	TerrainTexture* bTexture = nullptr;
};

