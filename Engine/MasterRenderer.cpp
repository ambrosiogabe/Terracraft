#include <iostream>
#include "MasterRenderer.h"

#define PI 3.14159265
#define FOV 70
#define NEAR_PLANE 0.1f
#define FAR_PLANE 1000.f

float RED = 135 / 255.0;
float GREEN = 136 / 255.0;
float BLUE = 156 / 255.0;

float toRadians(float num) {
	return (num * PI) / 180.0;
}

MasterRenderer::~MasterRenderer() {}
MasterRenderer::MasterRenderer(int width, int height) {
	MasterRenderer::enableCulling();
	glEnable(GL_DEPTH_TEST);

	this->SCREEN_WIDTH = width;
	this->SCREEN_HEIGHT = height;
	this->createProjectionMatrix();

	this->entityRenderer = new EntityRenderer(this->staticShader, this->projectionMatrix);
	this->terrainRenderer = new TerrainRenderer(this->terrainShader, this->projectionMatrix);
}

void MasterRenderer::render(std::vector<Light*> lights, Camera* camera) {
	prepare();

	staticShader->start();
	staticShader->loadSkyColor(RED, GREEN, BLUE);
	staticShader->loadLights(lights);
	staticShader->loadViewMatrix(camera);
	entityRenderer->render(entities);
	staticShader->stop();

	terrainShader->start();
	terrainShader->loadSkyColor(RED, GREEN, BLUE);
	terrainShader->loadLights(lights);
	terrainShader->loadViewMatrix(camera);
	terrainRenderer->render(terrains);
	terrainShader->stop();

	terrains->clear();
	entities->clear();
}

void MasterRenderer::processTerrain(Terrain* terrain) {
	this->terrains->push_back(terrain);
}

void MasterRenderer::processEntity(Entity* entity) {
	TexturedModel* model = entity->getModel();
	bool inMap = entities->count(model);
	if (inMap) {
		std::vector<Entity*>* batch = entities->at(model);
		batch->push_back(entity);
	} else {
		std::vector<Entity*>* batch = new std::vector<Entity*>();
		batch->push_back(entity);
		entities->insert( std::pair<TexturedModel*, std::vector<Entity*>*>(model, batch) );
	}
}

void MasterRenderer::createProjectionMatrix() {
	float aspectRatio = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
	float y_scale = (float)((1.f / tan(toRadians(FOV / 2.f))) * aspectRatio);
	float x_scale = y_scale / aspectRatio;
	float frustum_length = FAR_PLANE - NEAR_PLANE;

	projectionMatrix = *(new glm::mat4());
	projectionMatrix[0][0] = x_scale;
	projectionMatrix[1][1] = y_scale;
	projectionMatrix[2][2] = -((FAR_PLANE + NEAR_PLANE) / frustum_length);
	projectionMatrix[2][3] = -1.f;
	projectionMatrix[3][2] = -((2 * NEAR_PLANE * FAR_PLANE) / frustum_length);
	projectionMatrix[3][3] = 0.f;
}

void MasterRenderer::prepare() {
	glClearColor(RED, GREEN, BLUE, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MasterRenderer::cleanUp() {
	staticShader->cleanUp();
	terrainShader->cleanUp();
}

void MasterRenderer::enableCulling() {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void MasterRenderer::disableCulling() {
	glDisable(GL_CULL_FACE);
}