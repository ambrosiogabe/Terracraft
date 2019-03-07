#include "RawModel.h"



RawModel::RawModel(int vaoID, int vertexCount) {
	this->vaoID = vaoID;
	this->vertexCount = vertexCount;
}

int RawModel::getVaoID() {
	return this->vaoID;
}

int RawModel::getVertexCount() {
	return this->vertexCount;
}

RawModel::~RawModel() {
	// Method for on destroy
}
