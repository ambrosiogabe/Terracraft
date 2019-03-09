#include <iostream>
#include "World.h"

World::~World() {}
World::World(Player* player, std::vector<BlockTexture*>* blockTextures, Loader* loader, ModelTexture* texture) {
	this->player = player;
	this->blockTextures = blockTextures;
	this->centerChunkX = player->getPos().x;
	this->centerChunkZ = player->getPos().z;
	this->loader = loader;
	this->textureAtlas = texture;
	init();
	building = true;
	t1 = std::thread(&World::generateChunks, this, centerChunkX, centerChunkZ);
	//generateChunks(centerChunkX, centerChunkZ);
	//addNewEntities();
}

void World::update(float delta) {
	float d1 = player->getPos().x - centerChunkX;
	d1 *= d1;
	float d2 = player->getPos().z - centerChunkZ;
	d2 *= d2;
	float distance = sqrt(d1 + d2);
	if (distance > 16 * threshold) {
		if (!building) {
			killThread = false;
			if (t1.joinable()) t1.join();
			building = true;
			t1 = std::thread(&World::generateChunks, this, player->getPos().x, player->getPos().z);
			//generateChunks(player->getPos().x, player->getPos().z);
		} else if (!killThread) {
			killThread = true;
		}
	}

	if (chunks->size() > chunkEntities->size())
		addNewEntities();
}

void World::generateChunks(int x, int z) {
	int chunkX = x / 16;
	int chunkZ = z / 16;
	centerChunkX = chunkX * 16;
	centerChunkZ = chunkZ * 16;
	std::map<std::string, Chunk*>::iterator it;
	for (it = chunks->begin(); it != chunks->end(); it++) {
		Chunk* chunk = it->second;
		float d1 = chunk->chunkX - chunkX;
		d1 *= d1;
		float d2 = chunk->chunkZ - chunkZ;
		d2 *= d2;
		float distance = sqrt(d1 + d2);
		if (distance > chunkRange) {
			chunk->status = 2;
			chunksToRemove.push_back(it->first);
		}
	}

	removeChunks();

	std::vector<std::thread*> helperThreads;
	for (int i = 0; i < chunkRange; i++) {
		for (int j = 0; j < chunkRange; j++) {
			if (killThread) {
				building = false;
				return;
			}

			if (!hasChunk(chunkX + i, chunkZ + j)) {
				waitForHelperThreads(&helperThreads);
				helperThreads.push_back(new std::thread(&World::buildChunk, this, chunkX + i, chunkZ + j));
			}
			if (!hasChunk(chunkX + i, chunkZ - j)) {
				waitForHelperThreads(&helperThreads);
				helperThreads.push_back(new std::thread(&World::buildChunk, this, chunkX + i, chunkZ - j));
			}
			if (!hasChunk(chunkX - i, chunkZ + j)) {
				waitForHelperThreads(&helperThreads);
				helperThreads.push_back(new std::thread(&World::buildChunk, this, chunkX - i, chunkZ + j));
			}
			if (!hasChunk(chunkX - i, chunkZ - j)) {
				waitForHelperThreads(&helperThreads);
				helperThreads.push_back(new std::thread(&World::buildChunk, this, chunkX - i, chunkZ - j));
			}
		}
	}

	for (int i = 0; i < helperThreads.size(); i++) {
		helperThreads.at(i)->join();
	}
	helperThreads.empty();
	building = false;
}

void World::waitForHelperThreads(std::vector<std::thread*>* helperThreads) {
	while (helperThreads->size() >= maxThreads) {
		helperThreads->at(0)->join();
		helperThreads->erase(helperThreads->begin());
	}
}

void World::buildChunk(int x, int z) {
	std::string chunkName = std::string("chunk_") + std::to_string(x) + std::string("_") + std::to_string(z);
	std::map<std::string, Chunk*>::iterator it = chunks->find(chunkName);
	if (it != chunks->end()) {
		it->second->status = 1;
	} else {
		Chunk* chunk = new Chunk(x, z);
		chunk->generateModel(texturePointers);

		std::lock_guard<std::mutex> guard(lock_safety);
		chunks->insert(std::pair<std::string, Chunk*>(chunkName, chunk));
		std::cout << "Adding model: " << chunkName << std::endl;
	}
}

void World::eraseChunk(std::string chunkName) {
	std::map<std::string, Chunk*>::iterator it = chunks->find(chunkName);
	std::map<std::string, Entity*>::iterator entityIt = chunkEntities->find(chunkName);
	if (it != chunks->end()) {
		std::lock_guard<std::mutex> guard(lock_safety);
		it->second->~Chunk();
		chunks->erase(it);
	}

	if (entityIt != chunkEntities->end()) {
		chunkEntities->erase(entityIt);
	}
}

void World::removeChunks() {
	for (int i = 0; i < chunksToRemove.size(); i++) {
		eraseChunk(chunksToRemove.at(i));
	}
	chunksToRemove.empty();
}

void World::addNewEntities() {
	std::map<std::string, Chunk*>::iterator it;
	std::map<std::string, Entity*>::iterator entityIt;
	for (it = chunks->begin(); it != chunks->end(); it++) {
		entityIt = chunkEntities->find(it->first);
		if (entityIt == chunkEntities->end()) {
			std::cout << "Getting entity: " << it->first << std::endl;
			TexturedModel* chunkModel = it->second->getModel(loader, textureAtlas);
			Entity* chunkEntity = new Entity(chunkModel, *(new glm::vec3(0, 0, 0)), 0, 0, 0, 1);
			chunkEntities->insert(std::pair<std::string, Entity*>(it->first, chunkEntity));
		}
	}
}

void World::cleanUp() {
	killThread = true;
}

bool World::hasChunk(int x, int z) {
	std::string chunkName = std::string("chunk_") + std::to_string(x) + std::string("_") + std::to_string(z);
	return chunks->find(chunkName) != chunks->end();
}

void World::init() {
	texturePointers = new std::vector<TexturePointer*>();
	texturePointers->push_back(nullptr);
	TexturePointer* grass = new TexturePointer();
	grass->blockId = 1;
	grass->tex_back = blockTextures->at(0);
	grass->tex_forward = blockTextures->at(0);
	grass->tex_left = blockTextures->at(0);
	grass->tex_right = blockTextures->at(0);
	grass->tex_top = blockTextures->at(1);
	grass->tex_bottom = blockTextures->at(2);
	this->texturePointers->push_back(grass);

	TexturePointer* dirt = new TexturePointer();
	dirt->blockId = 2;
	dirt->tex_back = blockTextures->at(2);
	dirt->tex_forward = blockTextures->at(2);
	dirt->tex_left = blockTextures->at(2);
	dirt->tex_right = blockTextures->at(2);
	dirt->tex_top = blockTextures->at(2);
	dirt->tex_bottom = blockTextures->at(2);
	this->texturePointers->push_back(dirt);
}

