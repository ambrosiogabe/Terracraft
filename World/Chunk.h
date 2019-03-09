#pragma once

#include <glm/vec3.hpp>
#include "../Engine/Loader.h"
#include "../Models/TexturedModel.h"
#include "../Utils/FileHandler.h"
#include <iostream>
#include <string>

#define CHUNK_HEIGHT 256
#define CHUNK_WIDTH 16

class Chunk {

public:
	Chunk(int x, int z);
	~Chunk();

	void generateModel(std::vector<TexturePointer*>* blockTextures);
	TexturedModel* getModel(Loader* loader, ModelTexture* modelTexture);
	int chunkX = 0, chunkZ = 0;
	std::string name = *(new std::string());
	int status = 0;

private:
	Block* chunkData[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_WIDTH];
	Block* blockData;
	int chunkWorldX = 0, chunkWorldZ = 0;

	std::vector<float> vertices;
	std::vector<int> indices;
	std::vector<float> textureCoords;
	std::vector<float> normals;
	
	void addVertex(glm::vec3* vertex, std::vector<float>* vertices);

	void addFace(
		std::vector<float>* vertices,
		std::vector<int>* indices,
		std::vector<float>* textureCoords,
		std::vector<float>* normals,

		std::vector<glm::vec3*>* verts,
		int face,
		std::vector<TexturePointer*>* blockTextures,
		int x,
		int y,
		int z
	);
};