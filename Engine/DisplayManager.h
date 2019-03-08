#pragma once

#include <vector>
#include "../Utils/FileHandler.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class DisplayManager
{
public:
	DisplayManager();

	static GLFWwindow* createDisplay();
	static void updateDisplay(GLFWwindow* display, std::vector<BlockTexture*>* blockTextures);
	static void closeDisplay(GLFWwindow* display);
};

