#include "../Models/RawModel.h"
#include <vector>
#include "Chunk.h"

Chunk::~Chunk() {}
Chunk::Chunk(int chunkX, int chunkZ) {
	this->chunkX = chunkX;
	this->chunkZ = chunkZ;
	this->chunkWorldX = chunkX * 16;
	this->chunkWorldZ = chunkZ * 16;
	this->name = std::string("chunk_") + std::to_string(chunkX) + std::string("_") + std::to_string(chunkZ);

	Block* empty = new Block();
	empty->id = 0;
	Block* dirt = new Block();
	dirt->id = 2;
	Block* grass = new Block();
	grass->id = 1;
	Block* wood = new Block();
	wood->id = 2;

	for (int x = 0; x < CHUNK_WIDTH; x++) {
		for (int z = 0; z < CHUNK_WIDTH; z++) {
			for (int y = 0; y < CHUNK_HEIGHT; y++) {
				if (y == 40) {
					chunkData[x][y][z] = grass;
				} else if (y <= 40) {
					chunkData[x][y][z] = dirt;
				} else {
					chunkData[x][y][z] = empty;
				}
			}
		}
	}
	status = 0;
}

void Chunk::generateModel(std::vector<TexturePointer*>* blockTextures) {
	int triangleCounter = 0;
	for (int x = 0; x < CHUNK_WIDTH; x++) {
		for (int y = 0; y < CHUNK_HEIGHT; y++) {
			for (int z = 0; z < CHUNK_WIDTH; z++) {
				if (chunkData[x][y][z]->id == 0) continue;

				std::vector<glm::vec3*> verts;
				verts.push_back(new glm::vec3(chunkWorldX + x - 0.5, y - 0.5, chunkWorldZ + z + 0.5));
				verts.push_back(new glm::vec3(chunkWorldX + x + 0.5, y - 0.5, chunkWorldZ + z + 0.5));
				verts.push_back(new glm::vec3(chunkWorldX + x + 0.5, y - 0.5, chunkWorldZ + z - 0.5));
				verts.push_back(new glm::vec3(chunkWorldX + x - 0.5, y - 0.5, chunkWorldZ + z - 0.5));
				verts.push_back(new glm::vec3(chunkWorldX + x - 0.5, y + 0.5, chunkWorldZ + z + 0.5));
				verts.push_back(new glm::vec3(chunkWorldX + x + 0.5, y + 0.5, chunkWorldZ + z + 0.5));
				verts.push_back(new glm::vec3(chunkWorldX + x + 0.5, y + 0.5, chunkWorldZ + z - 0.5));
				verts.push_back(new glm::vec3(chunkWorldX + x - 0.5, y + 0.5, chunkWorldZ + z - 0.5));

				// 0 - Right 
				// 1 - Left
				// 2 - Top
				// 3 - Bottom
				// 4 - Forward
				// 5 - Back
				int faces[] = { 0, 0, 0, 0, 0, 0 };
				if (z == CHUNK_WIDTH - 1 && chunkData[x][y][z]->id != 0) faces[0] = 1;
				else if (z < CHUNK_WIDTH - 1) faces[0] = chunkData[x][y][z + 1]->id == 0;
				if (z == 0 && chunkData[x][y][z]->id != 0) faces[1] = 1;
				else if (z > 0) faces[1] = chunkData[x][y][z - 1]->id == 0;
				if (x == CHUNK_WIDTH - 1 && chunkData[x][y][z]->id != 0) faces[4] = 1;
				else if (x < CHUNK_WIDTH - 1) faces[4] = chunkData[x + 1][y][z]->id == 0;
				if (x == 0 && chunkData[x][y][z]->id != 0) faces[5] = 1;
				else if (x > 0) faces[5] = chunkData[x - 1][y][z]->id == 0;
				if (y == CHUNK_HEIGHT - 1 && chunkData[x][y][z]->id != 0) faces[2] = 1;
				else if (y < CHUNK_HEIGHT - 1) faces[2] = chunkData[x][y + 1][z]->id == 0;
				if (y == 0 && chunkData[x][y][z]->id != 0) faces[3] = 1;
				else if (y > 0) faces[3] = chunkData[x][y - 1][z]->id == 0;

				for (int i = 0; i < 6; i++) {
					if (faces[i]) {
						addFace(
							&vertices,
							&indices,
							&textureCoords,
							&normals,
							&verts,
							i,
							blockTextures,
							x,
							y,
							z
						);
					}
				}
			}
		}
	}
}

TexturedModel* Chunk::getModel(Loader* loader, ModelTexture* modelTexture) {
	RawModel* rawModel = loader->loadToVao(&vertices[0], vertices.size(), &indices[0], indices.size(), &textureCoords[0], textureCoords.size(), &normals[0], normals.size());
	return new TexturedModel(rawModel, modelTexture);
}

void Chunk::addFace(std::vector<float>* vertices, std::vector<int>* indices, std::vector<float>* textureCoords, std::vector<float>* normals, std::vector<glm::vec3*>* verts, int face, std::vector<TexturePointer*>* blockTextures, int x, int y, int z) {
	int startTriangle = vertices->size() / 3;
	BlockTexture* texture = nullptr;

	if (face == 0) {
		addVertex(verts->at(4), vertices);
		addVertex(verts->at(5), vertices);
		addVertex(verts->at(1), vertices);
		addVertex(verts->at(0), vertices);
		texture = blockTextures->at(chunkData[x][y][z]->id)->tex_right;

		for (int i = 0; i < 4; i++) {
			normals->push_back(0);
			normals->push_back(0);
			normals->push_back(1);
		}
	} else if (face == 1) {
		addVertex(verts->at(6), vertices);
		addVertex(verts->at(7), vertices);
		addVertex(verts->at(3), vertices);
		addVertex(verts->at(2), vertices);
		texture = blockTextures->at(chunkData[x][y][z]->id)->tex_left;

		for (int i = 0; i < 4; i++) {
			normals->push_back(0);
			normals->push_back(0);
			normals->push_back(-1);
		}
	} else if (face == 2) {
		addVertex(verts->at(7), vertices);
		addVertex(verts->at(6), vertices);
		addVertex(verts->at(5), vertices);
		addVertex(verts->at(4), vertices);
		texture = blockTextures->at(chunkData[x][y][z]->id)->tex_top;

		for (int i = 0; i < 4; i++) {
			normals->push_back(0);
			normals->push_back(1);
			normals->push_back(0);
		}
	} else if (face == 3) {
		addVertex(verts->at(0), vertices);
		addVertex(verts->at(1), vertices);
		addVertex(verts->at(2), vertices);
		addVertex(verts->at(3), vertices);
		texture = blockTextures->at(chunkData[x][y][z]->id)->tex_bottom;

		for (int i = 0; i < 4; i++) {
			normals->push_back(0);
			normals->push_back(-1);
			normals->push_back(0);
		}
	} else if (face == 4) {
		addVertex(verts->at(5), vertices);
		addVertex(verts->at(6), vertices);
		addVertex(verts->at(2), vertices);
		addVertex(verts->at(1), vertices);
		texture = blockTextures->at(chunkData[x][y][z]->id)->tex_forward;

		for (int i = 0; i < 4; i++) {
			normals->push_back(1);
			normals->push_back(0);
			normals->push_back(0);
		}
	} else if (face == 5) {
		addVertex(verts->at(7), vertices);
		addVertex(verts->at(4), vertices);
		addVertex(verts->at(0), vertices);
		addVertex(verts->at(3), vertices);
		texture = blockTextures->at(chunkData[x][y][z]->id)->tex_back;

		for (int i = 0; i < 4; i++) {
			normals->push_back(-1);
			normals->push_back(0);
			normals->push_back(0);
		}
	}
	//top right
	textureCoords->push_back(texture->x2);
	textureCoords->push_back(texture->y1);

	// top left
	textureCoords->push_back(texture->x1);
	textureCoords->push_back(texture->y1);

	// bottom left
	textureCoords->push_back(texture->x1);
	textureCoords->push_back(texture->y2);

	// bottom right
	textureCoords->push_back(texture->x2);
	textureCoords->push_back(texture->y2);

	indices->push_back(startTriangle + 3);
	indices->push_back(startTriangle + 1);
	indices->push_back(startTriangle + 0);
	indices->push_back(startTriangle + 3);
	indices->push_back(startTriangle + 2);
	indices->push_back(startTriangle + 1);
}

void Chunk::addVertex(glm::vec3* vertex, std::vector<float>* vertices) {
	vertices->push_back(vertex->x);
	vertices->push_back(vertex->y);
	vertices->push_back(vertex->z);
}
