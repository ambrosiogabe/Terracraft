#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "../External_Resources/stb_image.h"
#include "Loader.h"



Loader::Loader() {}
Loader::~Loader() {}

RawModel* Loader::loadToVao(float* positions, int positionsLength, int* indices, int indicesLength, float* textureCoords, int textureCoordsLength, float* normals, int normalsLength) {
	unsigned int vaoID = this->createVao();
	this->bindIndicesBuffer(indices, indicesLength);
	this->storeDataInAttributeList(0, 3, positions, positionsLength);
	this->storeDataInAttributeList(1, 2, textureCoords, textureCoordsLength);
	this->storeDataInAttributeList(2, 3, normals, normalsLength);
	this->unbindVao();
	return new RawModel(vaoID, indicesLength);
}

RawModel* Loader::loadToVao(float* positions, int positionsLength) {
	unsigned int vaoID = this->createVao();
	this->storeDataInAttributeList(0, 2, positions, positionsLength);
	this->unbindVao();
	return new RawModel(vaoID, positionsLength / 2);
}

unsigned char* Loader::imageBuffer(char* filename, int& width, int& height, int& bpp) {

	unsigned char* image = stbi_load(filename, &width, &height, &bpp, STBI_rgb_alpha);

	if (image == nullptr) {
		std::cout << stbi_failure_reason() << std::endl;
		std::cout << "Failed to load image texture: " << filename << std::endl;
		system("pause");
		exit(-1);
	}

	if (bpp == 3) {
		stbi_image_free(image);
		image = stbi_load(filename, &width, &height, &bpp, STBI_rgb);
	}

	return image;
}

void Loader::freeImage(unsigned char* image) {
	stbi_image_free(image);
}

int Loader::loadTexture(char* filename) {
	int width, height, bpp;
	unsigned char* image = stbi_load(filename, &width, &height, &bpp, STBI_rgb_alpha);

	if (image == nullptr) {
		std::cout << stbi_failure_reason() << std::endl;
		std::cout << "Failed to load image texture: " << filename << std::endl;
		system("pause");
		exit(-1);
	}

	if (bpp == 3) {
		stbi_image_free(image);
		image = stbi_load(filename, &width, &height, &bpp, STBI_rgb);
	}

	GLuint texID;
	glGenTextures(1, &texID);
	textures.push_back(texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Uncomment if image doesn't look right
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if (bpp == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	else if (bpp == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	else {
		std::cout << "Unrecognized image line 43 Loader.cpp" << std::endl;
		system("pause");
		exit(-1);
	}

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);
	glBindTexture(GL_TEXTURE_2D, 0);

	// I may need to add this to the clean up function
	stbi_image_free(image);
	return texID;
}

unsigned int Loader::createVao() {
	unsigned int vaoID;
	glGenVertexArrays(1, &vaoID);
	vaos.push_back(vaoID);
	glBindVertexArray(vaoID);

	return vaoID;
}

// This guy takes care of the vertex position data
void Loader::storeDataInAttributeList(unsigned int attributeNumber, int coordinateSize, float* data, int dataSize) {
	unsigned int vboID;
	glGenBuffers(1, &vboID);
	vbos.push_back(vboID);

	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, dataSize * sizeof(float), data, GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// This guy takes care of the triangle indices data structure
void Loader::bindIndicesBuffer(int* data, int dataSize) {
	unsigned int vboID;
	glGenBuffers(1, &vboID);
	vbos.push_back(vboID);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize * sizeof(int), data, GL_STATIC_DRAW);
}

void Loader::unbindVao() {
	glBindVertexArray(0);
}

void Loader::cleanUp() {
	for (unsigned int vao : vaos) {
		glDeleteVertexArrays(1, &vao);
	}

	for (unsigned int vbo : vbos) {
		glDeleteBuffers(1, &vbo);
	}

	for (unsigned int tex : textures) {
		glDeleteTextures(1, &tex);
	}
}