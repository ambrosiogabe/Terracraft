#pragma once

#include <vector>
#include "Chunk.h"
#include <thread>
#include <mutex>
#include "../Utils/FileHandler.h"
#include "../Entities/Player.h"
#include "../Entities/Entity.h"
#include <string>
#include <map>

class World
{
public:
	~World();
	World(Player* player, std::vector<BlockTexture*>* blockTextures, Loader* loader, ModelTexture* textureAtlas);
	void update(float delta);
	void cleanUp();

	std::map<std::string, Entity*>* chunkEntities = new std::map<std::string, Entity*>();
	bool building = false;

private:
	Player* player = nullptr;
	Loader* loader = nullptr;
	ModelTexture* textureAtlas = nullptr;

	std::thread t1;
	std::mutex lock_safety;
	unsigned maxThreads = std::thread::hardware_concurrency() - 2;
	void init();
	void waitForHelperThreads(std::vector<std::thread*>* helperThreads);
	void generateChunks(int x, int z);
	void removeChunks();
	void buildChunk(int x, int z);
	void eraseChunk(std::string chunkName);
	void addNewEntities();
	bool hasChunk(int x, int z);
	bool killThread = false;

	std::vector<TexturePointer*>* texturePointers;
	std::vector<BlockTexture*>* blockTextures;
	std::map<std::string, Chunk*>* chunks = new std::map<std::string, Chunk*>();
	std::vector<std::string> chunksToRemove = *(new std::vector<std::string>());

	// Chunk States
	// ============
	// 0 - Draw
	// 1 - Keep
	// 2 - Done

	int chunkRange = 12;
	int threshold = 1;
	int centerChunkX, centerChunkZ;
};

