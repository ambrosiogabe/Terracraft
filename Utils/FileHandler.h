#pragma once

#include <vector>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "../Models/RawModel.h"
#include "../Engine/Loader.h"

struct BlockTexture {
	int id;
	float x1;
	float x2;
	float y1;
	float y2;
};

class FileHandler
{
public:
	static RawModel* loadOBJ(char* filename, Loader* loader);
	static RawModel* loadBinOBJ(char* filename, Loader* loader);

	static void skipLine(char*& c);
	static void skipWhiteSpace(char*& c);
	static float readF(char*& c);
	static int readI(char*& c);
	static glm::vec3* readVector3f(char*& c);
	static glm::vec2* readVector2f(char*& c);
	static glm::ivec3* readVector3i(char*& c);
	static char consume(char*& c);
	static void processVertex(glm::ivec3* vertexData, std::vector<int>& indices, std::vector<glm::vec2> textures, std::vector<glm::vec3> normals, float* textureArray, float* normalArray);
	static std::vector<BlockTexture*>* createTextureAtlas(std::vector<char*> files);

	static bool checkForBin(char* filename);
};

