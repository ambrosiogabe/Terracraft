#include "TexturedModel.h"


TexturedModel::~TexturedModel() {}

TexturedModel::TexturedModel(RawModel* rawModel, ModelTexture* texture) {
	this->rawModel = rawModel;
	this->texture = texture;
}

RawModel* TexturedModel::getRawModel() {
	return this->rawModel;
}

ModelTexture* TexturedModel::getTexture() {
	return this->texture;
}