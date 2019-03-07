#include "GuiShader.h"

#define VERTEX_FILE "Shaders/Vertex_Shaders/guiVertexShader.glsl"
#define FRAGMENT_FILE "Shaders/Fragment_Shaders/guiFragmentShader.glsl"


GuiShader::~GuiShader() {}
GuiShader::GuiShader() : ShaderProgram(VERTEX_FILE, FRAGMENT_FILE) {
	bindAttributes();
	ShaderProgram::init();
	getAllUniformLocations();
}

void GuiShader::getAllUniformLocations() {
	location_transformationMatrix = ShaderProgram::getUniformLocation("transformationMatrix");
}

void GuiShader::bindAttributes() {
	ShaderProgram::bindAttribute(0, "position");
}

void GuiShader::loadTransformationMatrix(glm::mat4 matrix) {
	ShaderProgram::LoadMatrix(location_transformationMatrix, matrix);
}