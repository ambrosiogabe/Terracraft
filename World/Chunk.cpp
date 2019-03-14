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
				if (y == 16) {
					chunkData[x][y][z] = grass;
				} else if (y <= 16) {
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
	std::vector<glm::vec3*> verts;
	for (int i = 0; i < 8; i++) {
		verts.emplace_back(new glm::vec3(0, 0, 0));
	}

	for (int x = 0; x < CHUNK_WIDTH; x++) {
		for (int y = 0; y < CHUNK_HEIGHT; y++) {
			for (int z = 0; z < CHUNK_WIDTH; z++) {
				if (chunkData[x][y][z]->id == 0) continue;

				verts[0]->x = chunkWorldX + x - 0.5; verts[0]->y = y - 0.5; verts[0]->z = chunkWorldZ + z + 0.5;
				verts[1]->x = chunkWorldX + x + 0.5; verts[1]->y = y - 0.5; verts[1]->z = chunkWorldZ + z + 0.5;
				verts[2]->x = chunkWorldX + x + 0.5; verts[2]->y = y - 0.5; verts[2]->z = chunkWorldZ + z - 0.5;
				verts[3]->x = chunkWorldX + x - 0.5; verts[3]->y = y - 0.5; verts[3]->z = chunkWorldZ + z - 0.5;
				verts[4]->x = chunkWorldX + x - 0.5; verts[4]->y = y + 0.5; verts[4]->z = chunkWorldZ + z + 0.5;
				verts[5]->x = chunkWorldX + x + 0.5; verts[5]->y = y + 0.5; verts[5]->z = chunkWorldZ + z + 0.5;
				verts[6]->x = chunkWorldX + x + 0.5; verts[6]->y = y + 0.5; verts[6]->z = chunkWorldZ + z - 0.5;
				verts[7]->x = chunkWorldX + x - 0.5; verts[7]->y = y + 0.5; verts[7]->z = chunkWorldZ + z - 0.5;

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
	RawModel* rawModel = loader->loadToVao(vertices.get_pointer(), vertices.length(), indices.get_pointer(), indices.length(), textureCoords.get_pointer(), textureCoords.length(), normals.get_pointer(), normals.length());
	return new TexturedModel(rawModel, modelTexture);
}

void Chunk::addFace(MArray<float>* vertices, MArray<int>* indices, MArray<float>* textureCoords, MArray<float>* normals, std::vector<glm::vec3*>* verts, int face, std::vector<TexturePointer*>* blockTextures, int x, int y, int z) {
	int startTriangle = vertices->length() / 3;
	BlockTexture* texture = nullptr;

	switch (face) {
	case 0:
		addVertex((*verts)[4], vertices);
		addVertex((*verts)[5], vertices);
		addVertex((*verts)[1], vertices);
		addVertex((*verts)[0], vertices);
		texture = blockTextures->at(chunkData[x][y][z]->id)->tex_right;

		for (int i = 0; i < 4; i++) {
			normals->push_back(0);
			normals->push_back(0);
			normals->push_back(1);
		}
		break;
	case 1:
		addVertex((*verts)[6], vertices);
		addVertex((*verts)[7], vertices);
		addVertex((*verts)[3], vertices);
		addVertex((*verts)[2], vertices);
		texture = blockTextures->at(chunkData[x][y][z]->id)->tex_left;

		for (int i = 0; i < 4; i++) {
			normals->push_back(0);
			normals->push_back(0);
			normals->push_back(-1);
		}
		break;
	case 2:
		addVertex((*verts)[7], vertices);
		addVertex((*verts)[6], vertices);
		addVertex((*verts)[5], vertices);
		addVertex((*verts)[4], vertices);
		texture = blockTextures->at(chunkData[x][y][z]->id)->tex_top;

		for (int i = 0; i < 4; i++) {
			normals->push_back(0);
			normals->push_back(1);
			normals->push_back(0);
		}
		break;
	case 3:
		addVertex((*verts)[0], vertices);
		addVertex((*verts)[1], vertices);
		addVertex((*verts)[2], vertices);
		addVertex((*verts)[3], vertices);
		texture = blockTextures->at(chunkData[x][y][z]->id)->tex_bottom;

		for (int i = 0; i < 4; i++) {
			normals->push_back(0);
			normals->push_back(-1);
			normals->push_back(0);
		}
		break;
	case 4:
		addVertex((*verts)[5], vertices);
		addVertex((*verts)[6], vertices);
		addVertex((*verts)[2], vertices);
		addVertex((*verts)[1], vertices);
		texture = blockTextures->at(chunkData[x][y][z]->id)->tex_forward;

		for (int i = 0; i < 4; i++) {
			normals->push_back(1);
			normals->push_back(0);
			normals->push_back(0);
		}
		break;
	case 5:
		addVertex((*verts)[7], vertices);
		addVertex((*verts)[4], vertices);
		addVertex((*verts)[0], vertices);
		addVertex((*verts)[3], vertices);
		texture = blockTextures->at(chunkData[x][y][z]->id)->tex_back;

		for (int i = 0; i < 4; i++) {
			normals->push_back(-1);
			normals->push_back(0);
			normals->push_back(0);
		}
		break;
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

void Chunk::addVertex(glm::vec3* vertex, MArray<float>* vertices) {
	vertices->push_back(vertex->x);
	vertices->push_back(vertex->y);
	vertices->push_back(vertex->z);
}
