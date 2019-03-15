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
//#include "../Utils/FileHandler.h"
#include <stdlib.h>
#include <map>
#include <string>
#include "../Entities/Camera.h"
#include "../Entities/Player.h"
#include "../Textures/TerrainTexture.h"
#include "../Textures/TerrainTexturePack.h"
#include "../World/Chunk.h"
#include "../World/World.h"
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
static const double MS_FACTOR_PER_UPDATE = 500000;

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

void DisplayManager::updateDisplay(GLFWwindow* display, std::vector<BlockTexture*>* blockTextures) {
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

	Loader loader = Loader();

	ModelTexture terrainTextureAtlas = ModelTexture(loader.loadTexture("res/texture_atlas.png"));

	RawModel* rawPlayerModel = FileHandler::loadOBJ("res/bunny.obj", &loader);
	ModelTexture* playerTexture = new ModelTexture(loader.loadTexture("res/stallTexture.png"));
	TexturedModel* texturedPlayer = new TexturedModel(rawPlayerModel, playerTexture);
	Player* player = new Player(texturedPlayer, *(new glm::vec3(0, 46, -30)), 0, 0, 0, 0.2);
	player->setEyeLevel(0.5);

	World* world = new World(player, blockTextures, &loader, &terrainTextureAtlas);

	std::vector<GuiTexture*> guis = *(new std::vector<GuiTexture*>());
	GuiTexture* gui = new GuiTexture(loader.loadTexture("res/socuwan.png"), *(new glm::vec2(-0.70f, 0.95f)), *(new glm::vec2(0.17f, 0.23f)));
	guis.push_back(gui);

	GuiRenderer* guiRenderer = new GuiRenderer(&loader);
	Camera* camera = new Camera(player);

	Light* light = new Light(*(new glm::vec3(0.0, 1000.0, -7000.0)), *(new glm::vec3(0.4, 0.4, 0.4)));
	std::vector<Light*>* lights = new std::vector<Light*>;
	lights->push_back(light);
	MasterRenderer* renderer = new MasterRenderer(WIDTH, HEIGHT);

	std::map<std::string, Entity*>::iterator it;
	while (!glfwWindowShouldClose(display)) {
		double current = Time::getCurrentTime();
		double elapsed = current - previous;
		previous = current;
		lag += elapsed;

		// processInput();

		while (lag >= MS_PER_UPDATE) {
			// update();
			camera->move();
			player->move(lag / MS_FACTOR_PER_UPDATE);
			world->update(lag / MS_FACTOR_PER_UPDATE);
			lag -= MS_PER_UPDATE;
		}

		// render
		// ------
		renderer->processEntity(player);
		for (it = world->chunkEntities->begin(); it != world->chunkEntities->end(); it++) {
			Entity* chunk = it->second;
			renderer->processEntity(chunk);
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
	loader.cleanUp();
	world->cleanUp();
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
}