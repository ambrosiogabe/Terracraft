#include <iostream>
#include "../Utils/Time.h"
#include "Loader.h"
#include "../Guis/GuiRenderer.h"
#include "../Guis/GuiTexture.h"
#include <vector>
#include "EntityRenderer.h"
#include "../Models/RawModel.h"
#include "../Shaders/StaticShader.h"
#include "../Textures/ModelTexture.h"
#include "../Models/TexturedModel.h"
#include "../Entities/Entity.h"
#include "../Utils/InputHandler.h"
#include "../Utils/FileHandler.h"
#include <stdlib.h>
#include "../Entities/Camera.h"
#include "../Entities/Player.h"
#include "../Textures/TerrainTexture.h"
#include "../Textures/TerrainTexturePack.h"
#include "MasterRenderer.h"
#include "DisplayManager.h"

static const unsigned int WIDTH = 1920;
static const unsigned int HEIGHT = 1080;
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

// This value is assigned in the initialization function
// it controls how long we wait between renders.
static const double MS_PER_UPDATE = 0.03; // 30 FPS
static const double MS_FACTOR_PER_UPDATE = 200000;

DisplayManager::DisplayManager() {}

GLFWwindow* DisplayManager::createDisplay() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "3D Game Engine", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	InputHandler::init();

	return window;
}

void DisplayManager::updateDisplay(GLFWwindow* display) {
	double previous = Time::getCurrentTime();
	double lag = 0.0;

	glfwMakeContextCurrent(display);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}
	glfwSwapInterval(1);

	Loader* loader = new Loader();

	/* TERRAIN TEXTURE STUFF ---------------------------------------*/

	TerrainTexture* backgroundTexture = new TerrainTexture(loader->loadTexture("res/textures/block/grass_block_top.png"));
	TerrainTexture* rTexture = new TerrainTexture(loader->loadTexture("res/textures/block/dirt.png"));
	TerrainTexture* gTexture = new TerrainTexture(loader->loadTexture("res/textures/block/sand.png"));
	TerrainTexture* bTexture = new TerrainTexture(loader->loadTexture("res/textures/block/stone_bricks.png"));
	TerrainTexture* blendMap = new TerrainTexture(loader->loadTexture("res/blendMap.png"));

	TerrainTexturePack* texturePack = new TerrainTexturePack(backgroundTexture, rTexture, gTexture, bTexture);

	/* ------------------------------------------------------------- */

	RawModel* treeModel = FileHandler::loadOBJ("res/tree.obj", loader);
	RawModel* fernModel = FileHandler::loadOBJ("res/fern.obj", loader);
	RawModel* rLampModel = FileHandler::loadOBJ("res/lamp.obj", loader);

	ModelTexture* texture = new ModelTexture(loader->loadTexture("res/tree.png"));;
	ModelTexture* fernTexture = new ModelTexture(loader->loadTexture("res/fern.png"));
	ModelTexture* lampTexture = new ModelTexture(loader->loadTexture("res/lamp.png"));

	fernTexture->setHasTransparency(true);
	fernTexture->setUseFakeLighting(true);

	TexturedModel* texturedTree = new TexturedModel(treeModel, texture);
	TexturedModel* tFernModel = new TexturedModel(fernModel, fernTexture);
	TexturedModel* tLampModel = new TexturedModel(rLampModel, lampTexture);
	texturedTree->getTexture()->setReflectivity(1);
	texturedTree->getTexture()->setShineDamper(10);

	Entity** trees = new Entity*[100];
	Entity** ferns = new Entity*[100];

	RawModel* rawPlayerModel = FileHandler::loadOBJ("res/bunny.obj", loader);
	ModelTexture* playerTexture = new ModelTexture(loader->loadTexture("res/stallTexture.png"));
	TexturedModel* texturedPlayer = new TexturedModel(rawPlayerModel, playerTexture);
	Player* player = new Player(texturedPlayer, *(new glm::vec3(0, 0, -30)), 0, 0, 0, 0.6);
	player->setEyeLevel(3.5);

	Terrain* terrain0 = new Terrain(0, -1, loader, texturePack, blendMap, "res/heightmap.png");
	Terrain* terrain2 = new Terrain(0, 0, loader, texturePack, blendMap, "res/heightmap.png");
	Terrain* terrain3 = new Terrain(-1, -1, loader, texturePack, blendMap, "res/heightmap.png");
	Terrain* terrain4 = new Terrain(-1, 0, loader, texturePack, blendMap, "res/heightmap.png");

	std::vector<GuiTexture*> guis = *(new std::vector<GuiTexture*>());
	GuiTexture* gui = new GuiTexture(loader->loadTexture("res/socuwan.png"), *(new glm::vec2(-0.70f, 0.95f)), *(new glm::vec2(0.17f, 0.23f)));
	guis.push_back(gui);

	GuiRenderer* guiRenderer = new GuiRenderer(loader);

	for (int i = 0; i < 100; i++) {
		float randX = -1 * (rand() % 300) + 150;
		float randZ = -1 * (rand() % 300) + 150;
		float height = terrain0->getHeightOfTerrain(randX, randZ);

		Entity* tree = new Entity(texturedTree, *(new glm::vec3(randX, height, randZ)), 0, 0, 0, 5.5);
		trees[i] = tree;

		randX = -1 * (rand() % 300) + 150;
		randZ = -1 * (rand() % 300) + 150;
		height = terrain0->getHeightOfTerrain(randX, randZ);
		Entity* eFern = new Entity(tFernModel, *(new glm::vec3(randX, height, randZ)), 0, 0, 0, 1.0);
		ferns[i] = eFern;
	}

	Camera* camera = new Camera(player);

	Light* light = new Light(*(new glm::vec3(0.0, 1000.0, -7000.0)), *(new glm::vec3(0.4, 0.4, 0.4)));
	Entity* lamp1 = new Entity(tLampModel, *(new glm::vec3(55, 0, -15)), 0, 0, 0, 1);
	Light* light2 = new Light(*(new glm::vec3(55.0, 14.0, -15.0)), *(new glm::vec3(0.0, 0.0, 2.0)), *(new glm::vec3(1.0, 0.01, 0.002)));
	Entity* lamp2 = new Entity(tLampModel, *(new glm::vec3(25, 0, 0)), 0, 0, 0, 1);
	Light* light3 = new Light(*(new glm::vec3(25.0, 14.0, 0)), *(new glm::vec3(2.0, 0.0, 0.0)), *(new glm::vec3(1.0, 0.01, 0.002)));
	std::vector<Light*>* lights = new std::vector<Light*>;
	lights->push_back(light);
	lights->push_back(light2);
	lights->push_back(light3);
	MasterRenderer* renderer = new MasterRenderer(WIDTH, HEIGHT);

	while (!glfwWindowShouldClose(display)) {
		double current = Time::getCurrentTime();
		double elapsed = current - previous;
		previous = current;
		lag += elapsed;

		// processInput();

		while (lag >= MS_PER_UPDATE) {
			// update();
			camera->move();
			player->move(lag / MS_FACTOR_PER_UPDATE, terrain0);
			lag -= MS_PER_UPDATE;
		}

		// render
		// ------
		renderer->processEntity(lamp1);
		renderer->processEntity(lamp2);
		renderer->processEntity(player);
		renderer->processTerrain(terrain0);
		renderer->processTerrain(terrain2);
		renderer->processTerrain(terrain3);
		renderer->processTerrain(terrain4);

		for (int i = 0; i < 100; i++) {
			renderer->processEntity(trees[i]);
			renderer->processEntity(ferns[i]);
		}
		renderer->render(*lights, camera);
		guiRenderer->render(guis);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(display);
		glfwPollEvents();
	}

	guiRenderer->cleanUp();
	renderer->cleanUp();
	loader->cleanUp();
}

void DisplayManager::closeDisplay(GLFWwindow* display) {
	glfwDestroyWindow(display);
	glfwTerminate();
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (action == GLFW_PRESS) InputHandler::setKeyPressed(key);
	if (action == GLFW_RELEASE) InputHandler::setKeyReleased(key);
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	InputHandler::setMouseX(xpos);
	InputHandler::setMouseY(ypos);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	InputHandler::setMouseScroll(yoffset);
	std::cout << yoffset << std::endl;
}