#include <GLFW/glfw3.h>
#include "Engine/DisplayManager.h"
#include "Utils/FileHandler.h"
#include <iostream>

#include "Utils/MArray.h"
#include "Utils/Time.h"


int main()
{
	/*
	MArray<int>* myVec = new MArray<int>(1000000);
	std::vector<int>* vec = new std::vector<int>(1000000);

	float start = Time::getCurrentTime();
	for (int i = 0; i < 1000000; i++) {
		myVec->push_back(0);
	}
	float end = Time::getCurrentTime();
	std::cout << "My function took: " << end - start << "ms" << std::endl;

	start = Time::getCurrentTime();
	for (int i = 0; i < 1000000; i++) {
		vec->emplace_back(0);
	}
	end = Time::getCurrentTime();
	std::cout << "Standard library implementation took: " << end - start << "ms" << std::endl;
	*/

	std::vector<char*>* files = new std::vector<char*>();
	//files->push_back("res/textures/block/grass_block_side.png");
	//files->push_back("res/textures/block/grass_block_top.png");
	files->push_back("res/textures/block/iron_block.png");
	files->push_back("res/textures/block/iron_block.png");
	files->push_back("res/textures/block/iron_block.png");
	files->push_back("res/textures/block/dirt.png");
	files->push_back("res/textures/block/stone.png");
	files->push_back("res/textures/block/sand.png");
	files->push_back("res/textures/block/oak_planks.png");
	files->push_back("res/textures/block/gravel.png");
	files->push_back("res/textures/block/blue_stained_glass.png");
	files->push_back("res/textures/block/bedrock.png");
	files->push_back("res/textures/block/stone_bricks.png");
	files->push_back("res/textures/block/red_stained_glass.png");
	std::vector<BlockTexture*>* blockTextures = FileHandler::createTextureAtlas(*files);

	GLFWwindow* display = DisplayManager::createDisplay();
	DisplayManager::updateDisplay(display, blockTextures);
	DisplayManager::closeDisplay(display);

	system("pause");
	return 0;
}
