#pragma once

#include <vector>
#include "GuiTexture.h"
#include "../Engine/Loader.h"
#include "../Models/RawModel.h"
#include "../Shaders/GuiShader.h"

class GuiRenderer
{
public:
	GuiRenderer(Loader* loader);
	~GuiRenderer();

	void render(std::vector<GuiTexture*> guis);
	void cleanUp();

private:
	RawModel* quad = nullptr;
	GuiShader* shader = nullptr;
};

