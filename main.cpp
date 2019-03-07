#include <GLFW/glfw3.h>
#include "Engine/DisplayManager.h"
#include "Utils/FileHandler.h"
#include <iostream>

int main()
{
	std::vector<char*>* files = new std::vector<char*>();
	files->push_back("res/textures/block/grass_block_side.png");
	files->push_back("res/textures/block/grass_block_top.png");
	files->push_back("res/textures/block/dirt.png");
	files->push_back("res/textures/block/stone.png");
	files->push_back("res/textures/block/sand.png");
	files->push_back("res/textures/block/oak_planks.png");
	files->push_back("res/textures/block/gravel.png");
	files->push_back("res/textures/block/blue_stained_glass.png");
	files->push_back("res/textures/block/bedrock.png");
	files->push_back("res/textures/block/stone_bricks.png");
	files->push_back("res/textures/block/red_stained_glass.png");
	FileHandler::createTextureAtlas(*files);
	
	GLFWwindow* display = DisplayManager::createDisplay();
	DisplayManager::updateDisplay(display);
	DisplayManager::closeDisplay(display);
	

	system("pause");
	return 0;
}
