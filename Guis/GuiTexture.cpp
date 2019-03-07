#include "GuiTexture.h"


GuiTexture::~GuiTexture() {}
GuiTexture::GuiTexture(int texture, glm::vec2 position, glm::vec2 scale) {
	this->texture = texture;
	this->position = position;
	this->scale = scale;
}

int GuiTexture::getTexture() {
	return this->texture;
}

glm::vec2 GuiTexture::getPosition() {
	return this->position;
}

glm::vec2 GuiTexture::getScale() {
	return this->scale;
}