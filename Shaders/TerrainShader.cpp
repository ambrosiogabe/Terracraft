#include "../Utils/Math.h"
#include <iostream>
#include <sstream>
#include "TerrainShader.h"

#define VERTEX_FILE "Shaders/Vertex_Shaders/terrainVertexShader.glsl"
#define FRAGMENT_FILE "Shaders/Fragment_Shaders/terrainFragmentShader.glsl"

TerrainShader::TerrainShader(void) : ShaderProgram(VERTEX_FILE, FRAGMENT_FILE) {
	bindAttributes();
	ShaderProgram::init();
	getAllUniformLocations();
}

TerrainShader::~TerrainShader() {}

void TerrainShader::bindAttributes() {
	this->bindAttribute(0, "position");
	this->bindAttribute(1, "textureCoordinates");
	this->bindAttribute(2, "normal");
}

void TerrainShader::getAllUniformLocations() {
	location_transformationMatrix = ShaderProgram::getUniformLocation("transformationMatrix");
	location_projectionMatrix = ShaderProgram::getUniformLocation("projectionMatrix");
	location_viewMatrix = ShaderProgram::getUniformLocation("viewMatrix");
	location_reflectivity = ShaderProgram::getUniformLocation("reflectivity");
	location_shineDamper = ShaderProgram::getUniformLocation("shineDamper");
	location_skyColor = ShaderProgram::getUniformLocation("skyColor");
	location_backgroundTexture = ShaderProgram::getUniformLocation("backgroundTexture");
	location_rTexture = ShaderProgram::getUniformLocation("rTexture");
	location_bTexture = ShaderProgram::getUniformLocation("bTexture");
	location_gTexture = ShaderProgram::getUniformLocation("gTexture");
	location_blendMap = ShaderProgram::getUniformLocation("blendMap");

	for (int i = 0; i < MAX_LIGHTS; i++) {
		std::stringstream str;
		str << "lightPosition[" << i << "]";
		std::stringstream str2;
		str2 << "lightColor[" << i << "]";
		std::stringstream str3;
		str3 << "attenuation[" << i << "]";

		location_lightColor[i] = ShaderProgram::getUniformLocation((char*)(str2.str().c_str()));
		location_lightPosition[i] = ShaderProgram::getUniformLocation((char*)(str.str().c_str()));
		location_attenuation[i] = ShaderProgram::getUniformLocation((char*)(str3.str().c_str()));
	}
}

void TerrainShader::loadTextureUnits() {
	ShaderProgram::loadInt(location_backgroundTexture, 0);
	ShaderProgram::loadInt(location_rTexture, 1);
	ShaderProgram::loadInt(location_gTexture, 2);
	ShaderProgram::loadInt(location_bTexture, 3);
	ShaderProgram::loadInt(location_blendMap, 4);
}

void TerrainShader::loadShineVariables(float damper, float reflectivity) {
	ShaderProgram::loadFloat(location_shineDamper, damper);
	ShaderProgram::loadFloat(location_reflectivity, reflectivity);
}

void TerrainShader::loadSkyColor(float r, float g, float b) {
	ShaderProgram::loadVector(location_skyColor, glm::vec3(r, g, b));
}

void TerrainShader::loadTransformationMatrix(glm::mat4 matrix) {
	ShaderProgram::LoadMatrix(location_transformationMatrix, matrix);
}

void TerrainShader::loadProjectionMatrix(glm::mat4 projection) {
	ShaderProgram::LoadMatrix(location_projectionMatrix, projection);
}

void TerrainShader::loadViewMatrix(Camera* camera) {
	glm::mat4 viewMatrix = Math::createViewMatrix(camera);
	ShaderProgram::LoadMatrix(location_viewMatrix, viewMatrix);
}

void TerrainShader::loadLights(std::vector<Light*> lights) {
	for (int i = 0; i < MAX_LIGHTS; i++) {
		if (i < lights.size()) {
			Light* light = lights.at(i);
			ShaderProgram::loadVector(location_lightPosition[i], light->getPosition());
			ShaderProgram::loadVector(location_lightColor[i], light->getColor());
			ShaderProgram::loadVector(location_attenuation[i], light->getAttenuation());
		}
		else {
			ShaderProgram::loadVector(location_lightPosition[i], glm::vec3(0.0, 0.0, 0.0));
			ShaderProgram::loadVector(location_lightColor[i], glm::vec3(0.0, 0.0, 0.0));
			ShaderProgram::loadVector(location_attenuation[i], glm::vec3(1.0, 0.0, 0.0));
		}
	}
}