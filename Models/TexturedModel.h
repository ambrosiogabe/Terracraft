#pragma once

#include "RawModel.h"
#include "../Textures/ModelTexture.h"

class TexturedModel
{
public:
	TexturedModel(RawModel* rawModel, ModelTexture* texture);
	~TexturedModel();
	RawModel* getRawModel();
	ModelTexture* getTexture();

private:
	RawModel* rawModel;
	ModelTexture* texture;
};

