#include <iostream>
#include "InputHandler.h"


InputHandler::~InputHandler() {}
InputHandler::InputHandler() {}

int InputHandler::pressedKeys[348];
int InputHandler::keyPresses[348];
float InputHandler::mouseX = 0;
float InputHandler::mouseY = 0;
float InputHandler::mouseDx = 0;
float InputHandler::mouseDy = 0;
float InputHandler::mouseLastX = 0;
float InputHandler::mouseLastY = 0;
float InputHandler::scroll = 0;

void InputHandler::init() { 
	InputHandler::mouseX = InputHandler::getMouseX();
	InputHandler::mouseY = InputHandler::getMouseY();
	InputHandler::setMouseScroll(InputHandler::getMouseScroll());
	for (int i = 0; i < 348; i++) {
		InputHandler::pressedKeys[i] = 0;
		InputHandler::keyPresses[i] = 0;
	}
}

float InputHandler::getMouseScroll() {
	float tmp = InputHandler::scroll;
	InputHandler::scroll = 0;
	return tmp;
}

float InputHandler::getMouseX() {
	return InputHandler::mouseX;
}

float InputHandler::getMouseY() {
	return InputHandler::mouseY;
}

float InputHandler::getMouseDx() {
	float tmp = InputHandler::mouseX - InputHandler::mouseLastX;
	InputHandler::mouseLastX = InputHandler::mouseX;
	return tmp;
}

float InputHandler::getMouseDy() {
	float tmp = InputHandler::mouseY - InputHandler::mouseLastY;
	InputHandler::mouseLastY = InputHandler::mouseY;
	return tmp;
}

bool InputHandler::isKeyDown(int key) {
	return InputHandler::pressedKeys[key] == 1;
}

bool InputHandler::getKeyPress(int key) {
	if (InputHandler::keyPresses[key] == 1) {
		InputHandler::keyPresses[key] = 0;
		return true;
	}
	return false;
}

void InputHandler::setKeyPressed(int key) {
	InputHandler::pressedKeys[key] = 1;
	InputHandler::keyPresses[key] = 1;
}

void InputHandler::setKeyReleased(int key) {
	InputHandler::pressedKeys[key] = 0;
	InputHandler::keyPresses[key] = 0;
}

void InputHandler::setMouseX(float x) {
	InputHandler::mouseLastX = InputHandler::mouseX;
	InputHandler::mouseX = x;
}

void InputHandler::setMouseY(float y) {
	InputHandler::mouseLastY = InputHandler::mouseY;
	InputHandler::mouseY = y;
}

void InputHandler::setMouseScroll(float scroll) {
	InputHandler::scroll = scroll;
}