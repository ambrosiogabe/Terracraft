#include "../Utils/Math.h"
#include <iostream>
#include <sstream>
#include "StaticShader.h"

#define VERTEX_FILE "Shaders/Vertex_Shaders/vertexShader.glsl"
#define FRAGMENT_FILE "Shaders/Fragment_Shaders/fragmentShader.glsl"

StaticShader::StaticShader(void) : ShaderProgram(VERTEX_FILE, FRAGMENT_FILE) {
	bindAttributes();
	ShaderProgram::init();
	getAllUniformLocations();
}

StaticShader::~StaticShader(){}

void StaticShader::bindAttributes() {
	this->bindAttribute(0, "position");
	this->bindAttribute(1, "textureCoordinates");
	this->bindAttribute(2, "normal");
}

void StaticShader::getAllUniformLocations() {
	location_transformationMatrix = ShaderProgram::getUniformLocation("transformationMatrix");
	location_projectionMatrix = ShaderProgram::getUniformLocation("projectionMatrix");
	location_viewMatrix = ShaderProgram::getUniformLocation("viewMatrix");
	location_reflectivity = ShaderProgram::getUniformLocation("reflectivity");
	location_shineDamper = ShaderProgram::getUniformLocation("shineDamper");
	location_useFakeLighting = ShaderProgram::getUniformLocation("useFakeLighting");
	location_skyColor = ShaderProgram::getUniformLocation("skyColor");

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

void StaticShader::loadShineVariables(float damper, float reflectivity) {
	ShaderProgram::loadFloat(location_shineDamper, damper);
	ShaderProgram::loadFloat(location_reflectivity, reflectivity);
}

void StaticShader::loadSkyColor(float r, float g, float b) {
	ShaderProgram::loadVector(location_skyColor, glm::vec3(r, g, b));
}

void StaticShader::loadFakeLighting(bool val) {
	ShaderProgram::loadBoolean(location_useFakeLighting, val);
}

void StaticShader::loadTransformationMatrix(glm::mat4 matrix) {
	ShaderProgram::LoadMatrix(location_transformationMatrix, matrix);
}

void StaticShader::loadProjectionMatrix(glm::mat4 projection) {
	ShaderProgram::LoadMatrix(location_projectionMatrix, projection);
}

void StaticShader::loadViewMatrix(Camera* camera) {
	glm::mat4 viewMatrix = Math::createViewMatrix(camera);
	ShaderProgram::LoadMatrix(location_viewMatrix, viewMatrix);
}

void StaticShader::loadLights(std::vector<Light*> lights) {
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