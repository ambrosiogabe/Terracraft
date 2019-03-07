#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class DisplayManager
{
public:
	DisplayManager();

	static GLFWwindow* createDisplay();
	static void updateDisplay(GLFWwindow* display);
	static void closeDisplay(GLFWwindow* display);
};

