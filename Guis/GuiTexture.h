#pragma once

#include <glm/vec2.hpp>

class GuiTexture
{
public:
	GuiTexture(int texture, glm::vec2 position, glm::vec2 scale);
	~GuiTexture();

	int getTexture();
	glm::vec2 getPosition();
	glm::vec2 getScale();

private:
	int texture;
	glm::vec2 position;
	glm::vec2 scale;
};

