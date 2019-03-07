#include <iostream>
#include <fstream>
#include <string> 
#include "../External_Resources/lodepng.h"
#include <math.h>
#include "FileHandler.h"


void FileHandler::skipLine(char*& c) {
	while (*c != '\n' && *c != '\0') ++(c);
	++(c);
}

void FileHandler::skipWhiteSpace(char*& c) {
	while (*c == ' ' || *c == '\t') ++(c);
}

char FileHandler::consume(char*& c) {
	char tmp = *c;
	++c;
	return tmp;
}

float FileHandler::readF(char*& c) {
	std::vector<char> string;
	while (*c != ' ' && *c != '\0' && *c != '\n') {
		string.push_back(*c);
		++c;
	}
	std::string number(string.begin(), string.end());

	if (number.size() == 0) {
		std::cout << "ERROR: Cannot parse float of size 0." << std::endl;
		system("pause");
		exit(-1);
	}
	return stof(number);
}

int FileHandler::readI(char*& c) {
	std::vector<char> string;
	while (*c != '/' && *c != ' ' && *c != '\0' && *c != '\n') {
		string.push_back(*c);
		++c;
	}
	if (*c == '/') ++c;
	std::string number(string.begin(), string.end());

	if (number.size() == 0) {
		std::cout << "ERROR: Cannot parse int of size 0." << std::endl;
		system("pause");
		exit(-1);
	}
	return stoi(number);
}

glm::ivec3* FileHandler::readVector3i(char*& c) {
	glm::ivec3* vert = new glm::ivec3();
	vert->x = FileHandler::readI(c);
	vert->y = FileHandler::readI(c);
	vert->z = FileHandler::readI(c);
	return vert;
}

glm::vec3* FileHandler::readVector3f(char*& c) {
	glm::vec3* vert = new glm::vec3();
	vert->x = FileHandler::readF(c);
	FileHandler::skipWhiteSpace(c);
	vert->y = FileHandler::readF(c);
	FileHandler::skipWhiteSpace(c);
	vert->z = FileHandler::readF(c);
	return vert;
}

glm::vec2* FileHandler::readVector2f(char*& c) {
	glm::vec2* vert = new glm::vec2();
	vert->x = FileHandler::readF(c);
	FileHandler::skipWhiteSpace(c);
	vert->y = FileHandler::readF(c);
	return vert;
}

void FileHandler::processVertex(glm::ivec3* vertexData, std::vector<int>& indices, std::vector<glm::vec2> textures, std::vector<glm::vec3> normals, float* textureArray, float* normalArray) {
	int curTriangle = vertexData->x - 1;
	indices.push_back(curTriangle);

	glm::vec2 currentTex = textures.at(vertexData->y - 1);
	glm::vec3 currentNormal = normals.at(vertexData->z - 1);

	textureArray[curTriangle * 2] = currentTex.x;
	textureArray[curTriangle * 2 + 1] = 1 - currentTex.y;

	normalArray[curTriangle * 3] = currentNormal.x;
	normalArray[curTriangle * 3 + 1] = currentNormal.y;
	normalArray[curTriangle * 3 + 2] = currentNormal.z;
}

bool FileHandler::checkForBin(char* filename) {
	std::string fullFile("bin/");
	fullFile += filename + std::string(".bin");
	std::ifstream f(fullFile.c_str());
	return f.good();
}

RawModel* FileHandler::loadBinOBJ(char* filename, Loader* loader) {
	std::ifstream inFile;
	inFile.open((std::string("bin/") + filename + std::string(".bin")), std::ios::in | std::ios::binary);

	// Initialize size variables
	float _vertSize, _indSize, _texSize, _normSize;

	// Read the sizes into the variables
	inFile.read((char*)&_vertSize, sizeof(float));
	inFile.read((char*)&_indSize, sizeof(float));
	inFile.read((char*)&_texSize, sizeof(float));
	inFile.read((char*)&_normSize, sizeof(float));

	// Dynamically create the arrays to temporarily hold the data
	float* vertArray = new float[_vertSize];
	int* indArray = new int[_indSize];
	float* texArray = new float[_texSize];
	float* normArray = new float[_normSize];

	// Load the data into the arrays
	inFile.read((char*)vertArray, _vertSize);
	inFile.read((char*)indArray, _indSize);
	inFile.read((char*)texArray, _texSize);
	inFile.read((char*)normArray, _normSize);

	// Create the model
	RawModel* model = loader->loadToVao(vertArray, _vertSize / sizeof(float), indArray, _indSize / sizeof(int), texArray, _texSize / sizeof(float), normArray, _normSize / sizeof(float));

	// Deallocate memory
	delete[] vertArray;
	delete[] indArray;
	delete[] texArray;
	delete[] normArray;

	return model;
}

RawModel* FileHandler::loadOBJ(char* filename, Loader* loader) {
	if (FileHandler::checkForBin(filename))
		return FileHandler::loadBinOBJ(filename, loader);


	std::ifstream in(filename);
	std::string contents(
		(std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>()
	);
	GLchar* source = (GLchar *)contents.c_str();

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> textures;
	std::vector<glm::vec3> normals;
	std::vector<int> indices;

	int texSize = 0;
	int normSize = 0;
	
	char* c = &source[0];

	// This loop will stop when it encounters an f
	while (*c != EOF && *c != '\0') {
		char cur = FileHandler::consume(c);
		char next = *(c);

		if (cur != 'f' && cur != 'v') {
			FileHandler::skipLine(c);
		} else if (cur == 'v' && next == ' ') {
			FileHandler::skipWhiteSpace(c);
			vertices.push_back(*(FileHandler::readVector3f(c)));
			FileHandler::skipLine(c);
		} else if (cur == 'v' && next == 't') {
			FileHandler::consume(c);
			FileHandler::skipWhiteSpace(c);
			textures.push_back(*(FileHandler::readVector2f(c)));
			FileHandler::skipLine(c);
		} else if (cur == 'v' && next == 'n') {
			FileHandler::consume(c);
			FileHandler::skipWhiteSpace(c);
			normals.push_back(*(FileHandler::readVector3f(c)));
			FileHandler::skipLine(c);
		} else if (cur == 'f') {
			texSize = vertices.size() * 2;
			normSize = vertices.size() * 3;
			--c;
			// We will continue parsing after we initialize the arrays
			break;
		} else if (cur == '\n' || cur == '\r') {
			++c;
		} else {
			std::cout << "WARNING: Unexpected line beginning '" << cur << next << "' in '" << filename << "'" << std::endl;
			FileHandler::skipLine(c);
		}
	}


	// We will need to deallocate these after we are finished using them
	float* texturesArray;
	texturesArray = new float[texSize];
	float* normalsArray;
	normalsArray = new float[normSize];

	int myIndex = 0;
	while (*c != EOF && *c != '\0') {
		char cur = consume(c);
		char next = *(c);

		if (cur == 'f' && next == ' ') {
			FileHandler::skipWhiteSpace(c);
			glm::ivec3* vert1 = FileHandler::readVector3i(c);
			FileHandler::skipWhiteSpace(c);
			glm::ivec3* vert2 = FileHandler::readVector3i(c);
			FileHandler::skipWhiteSpace(c);
			glm::ivec3* vert3 = FileHandler::readVector3i(c);
			FileHandler::skipLine(c);

			FileHandler::processVertex(vert1, indices, textures, normals, texturesArray, normalsArray);
			FileHandler::processVertex(vert2, indices, textures, normals, texturesArray, normalsArray);
			FileHandler::processVertex(vert3, indices, textures, normals, texturesArray, normalsArray);
		} else if (cur == '\n' || cur == '\r') {
			++c;
		} else {
			std::cout << "WARNING: Unexpected line beginning '" << cur << next << "' in '" << filename << "'" << std::endl;
			FileHandler::skipLine(c);
		}
		myIndex++;
	}
	
	float* verticesArray;
	verticesArray = new float[vertices.size() * 3];
	int* indicesArray;
	indicesArray = new int[indices.size()];

	int index = 0;
	for (int i = 0; i < vertices.size(); i++) {
		glm::vec3 vert = vertices.at(i);
		verticesArray[index++] = vert.x;
		verticesArray[index++] = vert.y;
		verticesArray[index++] = vert.z;
	}

	for (int i = 0; i < indices.size(); i++) {
		indicesArray[i] = indices.at(i);
	}

	std::cout << "Successfully loaded object: " << filename << std::endl;

	RawModel* model = loader->loadToVao(verticesArray, vertices.size() * 3, indicesArray, indices.size(), texturesArray, texSize, normalsArray, normSize);


	// Create binary file real quick
	std::ofstream outFile;
	outFile.open((std::string("bin/") + filename + std::string(".bin")), std::ios::out | std::ios::binary);
	std::cout << (std::string("bin/") + filename + std::string(".bin")).c_str() << std::endl;

	float _vertSize = vertices.size() * 3 * sizeof(float);
	float _indSize = indices.size() * sizeof(int);
	float _texSize = texSize * sizeof(float);
	float _normSize = normSize * sizeof(float);

	// First write the sizes, so that I can initialize quick arrays when reading
	outFile.write((char*)&_vertSize, sizeof(float));
	outFile.write((char*)&_indSize, sizeof(float));
	outFile.write((char*)&_texSize, sizeof(float));
	outFile.write((char*)&_normSize, sizeof(float));

	// Write the arrays to the file next
	outFile.write((char*)verticesArray, _vertSize);
	outFile.write((char*)indicesArray, _indSize);
	outFile.write((char*)texturesArray, _texSize);
	outFile.write((char*)normalsArray, _normSize);
	outFile.close();

	std::cout << "Created cached bin file" << std::endl;

	// Deallocate that memory that was being used
	delete[] texturesArray; texturesArray = NULL;
	delete[] normalsArray; normalsArray = NULL;
	delete[] verticesArray; verticesArray = NULL;
	delete[] indicesArray; indicesArray = NULL;

	return model;
}


std::vector<BlockTexture*>* FileHandler::createTextureAtlas(std::vector<char*> files) {
	std::vector<unsigned char> final_img;
	std::vector<unsigned char> image; 
	unsigned width, height;

	for (int i = 0; i < files.size(); i++) {
		char* file = files.at(i);
		//decode
		unsigned error = lodepng::decode(image, width, height, file);

		//if there's an error, display it
		if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
	}

	int squareSide = ceil(sqrt(files.size()) - .01);

	for (int row = 0; row < squareSide; row++) {
		int firstImage = row * squareSide;

		for (int y = 0; y < height; y++) {
			int yLevel = y * width * 4;
			for (int x = 0; x < width * 4 * squareSide; x++) {
				int curPicX = x / (width * 4);
				int realX = x - (curPicX * width * 4);
				int curImg = firstImage + curPicX;
				int x1 = curImg * width * height * 4;

				if (curImg >= files.size()) 
					final_img.push_back(255);
				else
					final_img.push_back(image.at(realX + x1 + yLevel));
			}
		}	
	}
	int totalSize = width * squareSide * height * squareSide * 4;

	while (final_img.size() < totalSize) {
		final_img.push_back(255);
	}

	unsigned int error = lodepng::encode("res/texture_atlas.png", final_img, width * squareSide, height * squareSide);

	if (error) std::cout << "encoder error" << error << ": " << lodepng_error_text(error) << std::endl;

	float imageWidth = width * squareSide;
	float imageHeight = height * squareSide;
	std::vector<BlockTexture*>* blockTextures = new std::vector<BlockTexture*>();
	for (int i = 0; i < files.size(); i++) {
		BlockTexture* curBlockTexture = new BlockTexture();
		float blockX = i % squareSide;
		curBlockTexture->x1 = blockX * width / imageWidth;

		int blockY = i / (int)squareSide;
		curBlockTexture->y1 = blockY * height / imageHeight;

		float unitWidth = width / imageWidth;
		curBlockTexture->x2 = curBlockTexture->x1 + unitWidth;
		curBlockTexture->y2 = curBlockTexture->y1 + unitWidth;
		curBlockTexture->id = i;
		blockTextures->push_back(curBlockTexture);

		std::cout << "Id: " << curBlockTexture->id << std::endl;
		std::cout << "X1: " << curBlockTexture->x1 << std::endl;
		std::cout << "X2: " << curBlockTexture->x2 << std::endl;
		std::cout << "Y1: " << curBlockTexture->y1 << std::endl;
		std::cout << "Y2: " << curBlockTexture->y2 << std::endl << std::endl;
	}

	return blockTextures;
}