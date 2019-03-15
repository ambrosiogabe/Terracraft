#pragma once

#include "../Shaders/StaticShader.h"
#include "EntityRenderer.h"
#include <map> 
#include <vector>
#include "../Models/TexturedModel.h"
#include "../Entities/Entity.h"
#include "../Entities/Light.h"
#include "../Entities/Camera.h"
#include "TerrainRenderer.h"
#include "../Shaders/TerrainShader.h"

class MasterRenderer
{
public:
	MasterRenderer(int width, int height);
	~MasterRenderer();

	void cleanUp();
	void render(std::vector<Light*> lights, Camera* camera);
	void processEntity(Entity* entity);
	void processTerrain(Terrain* terrain);
	void prepare();

	static void enableCulling();
	static void disableCulling();

private:
	int SCREEN_WIDTH, SCREEN_HEIGHT;

	StaticShader* staticShader = new StaticShader();
	TerrainShader* terrainShader = new TerrainShader();
	EntityRenderer* entityRenderer = nullptr;
	TerrainRenderer* terrainRenderer = nullptr;
	glm::mat4 projectionMatrix;


	std::map<TexturedModel*, std::vector<Entity>> entities;
	std::vector<Terrain> terrains;
	void createProjectionMatrix();
};

