#pragma once

#include <GLFW/glfw3.h>

class InputHandler
{
public:
	InputHandler();
	~InputHandler();
	static bool getKeyPress(int key);
	static bool isKeyDown(int key);
	static float getMouseDx();
	static float getMouseDy();
	static float getMouseX();
	static float getMouseY();
	static float getMouseScroll();
	static void init();

	static void setKeyPressed(int key);
	static void setKeyReleased(int key);
	static void setMouseX(float x);
	static void setMouseY(float y);
	static void setMouseScroll(float scroll);

	static int pressedKeys[348];
	static int keyPresses[348];
	static float mouseX;
	static float mouseY;
	static float mouseDx;
	static float mouseDy;
	static float mouseLastX;
	static float mouseLastY;
	static float scroll;
};

