#pragma once

#include <glm/vec3.hpp>
#include "../Engine/Loader.h"
#include "../Models/TexturedModel.h"
#include "../Utils/FileHandler.h"

#define CHUNK_HEIGHT 256
#define CHUNK_WIDTH 16

struct Block {
	int id;
	int tex_id_top, tex_id_bottom, tex_id_left, tex_id_right, tex_id_forward, tex_id_back;
};

class Chunk {

public:
	Chunk(int x, int z);
	~Chunk();

	TexturedModel* getChunkModel(Loader* loader, std::vector<BlockTexture*>* blockTextures, ModelTexture* modelTexture);

private:
	Block* chunkData[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_WIDTH];
	
	void addVertex(glm::vec3* vertex, std::vector<float>* vertices);
	void addFace(
		std::vector<float>* vertices,
		std::vector<int>* indices,
		std::vector<float>* textureCoords,
		std::vector<float>* normals,

		std::vector<glm::vec3*>* verts,
		int face,
		std::vector<BlockTexture*>* blockTextures,
		int x,
		int y,
		int z
	);
};