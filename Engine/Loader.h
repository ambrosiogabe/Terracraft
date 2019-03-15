#pragma once

#include <glad/glad.h>
#include "../Models/RawModel.h"
#include <vector>

class Loader
{
public:
	Loader();
	~Loader();


	RawModel* loadToVao(float* positions, int positionsLength, int* indices, int indicesLength, float* textureCoords, int textureCoordsLength,
		float* normals, int normalsLength);
	RawModel* loadToVao(float* positions, int positionsLength);

	unsigned char* imageBuffer(char* filename, int& width, int& height, int& bpp);
	void freeImage(unsigned char* image);
	int loadTexture(char* filename);
	void cleanUp();

private:
	unsigned int createVao();
	void storeDataInAttributeList(unsigned int attributeNumber, int coordinateSize, float* data, int dataSize);
	void bindIndicesBuffer(int* data, int dataSize);
	void unbindVao();

	std::vector<unsigned int> vaos;
	std::vector<unsigned int> vbos;
	std::vector<unsigned int> textures;
};
