#include "ModelTexture.h"


ModelTexture::~ModelTexture() {}
ModelTexture::ModelTexture(int texID) {
	this->textureID = texID;
}

bool ModelTexture::getTransparency() {
	return this->hasTransparency;
}

bool ModelTexture::getFakeLighting() {
	return this->useFakeLighting;
}

void ModelTexture::setHasTransparency(bool val) {
	this->hasTransparency = val;
}

int ModelTexture::getID() {
	return this->textureID;
}

float ModelTexture::getReflectivity() {
	return this->reflectivity;
}

float ModelTexture::getShineDamper() {
	return this->shineDamper;
}

void ModelTexture::setReflectivity(float num) {
	this->reflectivity = num;
}

void ModelTexture::setShineDamper(float num) {
	this->shineDamper = num;
}

void ModelTexture::setUseFakeLighting(bool val) {
	this->useFakeLighting = val;
}