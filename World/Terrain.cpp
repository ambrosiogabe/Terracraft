#include <iostream>
#include <glm/glm.hpp>
#include <math.h>
#include "../Utils/Math.h"
#include <glm/vec2.hpp>
#include "Terrain.h"

Terrain::~Terrain() {
	for (int i = 0; i < VERTEX_COUNT; i++) {
		delete[] heights[i];
	}
	delete[] heights;
}

Terrain::Terrain(int gridX, int gridZ, Loader* loader, TerrainTexturePack* texturePack, TerrainTexture* blendMap, char* heightmap) {
	this->texturePack = texturePack;
	this->blendMap = blendMap;
	this->x = gridX * SIZE;
	this->z = gridZ * SIZE;
	this->model = generateTerrain(loader, heightmap);
}

float Terrain::getHeightOfTerrain(float worldX, float worldZ) {
	float terrainX = worldX - this->x;
	float terrainZ = worldZ - this->z;
	float gridSquareSize = ((float)SIZE / (VERTEX_COUNT - 1));
	int gridX = (int)floor(terrainX / gridSquareSize);
	int gridZ = (int)floor(terrainZ / gridSquareSize);
	if (gridX >= VERTEX_COUNT - 1 || gridZ >= VERTEX_COUNT - 1 || gridX < 0 || gridZ < 0) {
		return 0;
	}

	float xCoord = (fmodf(terrainX, gridSquareSize)) / gridSquareSize;
	float zCoord = (fmodf(terrainZ, gridSquareSize)) / gridSquareSize;
	float answer;

	if (xCoord <= 1 - z) {
		answer = Math::barryCentric(
			*(new glm::vec3(0, heights[gridX][gridZ], 0)),
			*(new glm::vec3(1, heights[gridX + 1][gridZ], 0)),
			*(new glm::vec3(0, heights[gridX][gridZ + 1], 1)),
			*(new glm::vec2(xCoord, zCoord))
		);
	} else {
		answer = Math::barryCentric(
			*(new glm::vec3(1, heights[gridX + 1][gridZ], 0)),
			*(new glm::vec3(1, heights[gridX + 1][gridZ + 1], 1)),
			*(new glm::vec3(0, heights[gridX][gridZ + 1], 1)),
			*(new glm::vec2(xCoord, zCoord))
		);
	}
	return answer;
}

RawModel* Terrain::generateTerrain(Loader* loader, char* filename) {
	int width, height, bpp;
	unsigned char* image = loader->imageBuffer(filename, width, height, bpp);

	VERTEX_COUNT = height;

	heights = new float*[VERTEX_COUNT];
	for (int i = 0; i < VERTEX_COUNT; i++) {
		heights[i] = new float[VERTEX_COUNT];
	}

	int count = VERTEX_COUNT * VERTEX_COUNT;
	int vertsLength = count * 3;
	int textureLength = count * 2;
	int indicesLength = 6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1);

	float* verts = new float[vertsLength];
	float* normals = new float[vertsLength];
	float* textureCoords = new float[textureLength];
	int* indices = new int[indicesLength];
	int vertexPointer = 0;
	for (int i = 0; i < VERTEX_COUNT; i++) {
		for (int j = 0; j < VERTEX_COUNT; j++) {
			// Create vertices
			verts[vertexPointer * 3] = (float)j / ((float)VERTEX_COUNT - 1) * SIZE;
			float tHeight = getHeight(j, i, image, width, height, bpp);
			heights[j][i] = tHeight;
			verts[vertexPointer * 3 + 1] = tHeight;
			verts[vertexPointer * 3 + 2] = (float)i / ((float)VERTEX_COUNT - 1) * SIZE;

			// Create normals
			glm::vec3 normal = calculateNormal(j, i, image, width, height, bpp);
			normals[vertexPointer * 3] = normal.x;
			normals[vertexPointer * 3 + 1] = normal.y;
			normals[vertexPointer * 3 + 2] = normal.z;

			// Create Texture Coordinates
			textureCoords[vertexPointer * 2] = (float)j / ((float)VERTEX_COUNT - 1);
			textureCoords[vertexPointer * 2 + 1] = (float)i / ((float)VERTEX_COUNT - 1);
			vertexPointer++;
		}
	}
	int pointer = 0;
	for (int gz = 0; gz < VERTEX_COUNT - 1; gz++) {
		for (int gx = 0; gx < VERTEX_COUNT - 1; gx++) {
			int topLeft = (gz * VERTEX_COUNT) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
			int bottomRight = bottomLeft + 1;
			indices[pointer++] = topLeft;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = topRight;
			indices[pointer++] = topRight;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = bottomRight;
		}
	}
	RawModel* model = loader->loadToVao(verts, vertsLength, indices, indicesLength, textureCoords, textureLength, normals, vertsLength);

	delete[] verts;
	delete[] indices;
	delete[] textureCoords;
	delete[] normals;
	loader->freeImage(image);

	return model;
}

float Terrain::getX() {
	return this->x;
}

float Terrain::getZ() {
	return this->z;
}

RawModel* Terrain::getModel() {
	return this->model;
}

TerrainTexturePack* Terrain::getTexturePack() {
	return this->texturePack;
}

TerrainTexture* Terrain::getBlendMap() {
	return this->blendMap;
}

glm::vec3 Terrain::calculateNormal(int x, int y, unsigned char* data, int width, int height, int bpp) {
	float heightL = getHeight(x - 1, y, data, width, height, bpp);
	float heightR = getHeight(x + 1, y, data, width, height, bpp);
	float heightU = getHeight(x, y + 1, data, width, height, bpp);
	float heightD = getHeight(x, y - 1, data, width, height, bpp);
	glm::vec3 normal = *(new glm::vec3(heightL - heightR, 2.f, heightD - heightU));
	return glm::normalize(normal);
}

float Terrain::getHeight(int x, int y, unsigned char* data, int width, int height, int bpp) {
	if (x < 0 || x >= height || y < 0 || y >= width) {
		return 0;
	}

	unsigned char* pixelOffset = data + (x + width * y) * bpp;
	int r = pixelOffset[0];
	int g = pixelOffset[1];
	int b = pixelOffset[2];

	float newHeight = r;
	newHeight -= (MAX_PIXEL_COLOR / 2.f);
	newHeight /= (MAX_PIXEL_COLOR / 2.f);
	newHeight *= MAX_HEIGHT;
	return newHeight;
}