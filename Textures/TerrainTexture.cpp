#include "TerrainTexture.h"


TerrainTexture::~TerrainTexture() {}
TerrainTexture::TerrainTexture(int id) {
	this->id = id;
}

int TerrainTexture::getId() {
	return this->id;
}

void TerrainTexture::setId(int id) {
	this->id = id;
}