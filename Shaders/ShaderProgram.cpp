#include <fstream>
#include <iostream>
#include <vector>
#include "ShaderProgram.h"

// Make sure to call constructor, then call bind attributes in child constructor
// then call init();
// then call getAllUniformLocations(); from child class
ShaderProgram::ShaderProgram(char * vertexFile, char * fragmentFile) {
	if (vertexFile == nullptr || fragmentFile == nullptr || vertexFile == "" || fragmentFile == "") {
		std::cout << "ERROR: Null pointers not allowed here...";
		exit(-1);
	}

	vertexShaderID = loadShader(vertexFile, GL_VERTEX_SHADER);
	fragmentShaderID = loadShader(fragmentFile, GL_FRAGMENT_SHADER);

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
}

void ShaderProgram::init() {
	glLinkProgram(programID);
	glValidateProgram(programID);
}

int ShaderProgram::getUniformLocation(char* uniformName) {
	return glGetUniformLocation(this->programID, uniformName);
}

void ShaderProgram::loadFloat(int location, float value) {
	glUniform1f(location, value);
}

void ShaderProgram::loadInt(int location, int value) {
	glUniform1i(location, value);
}

void ShaderProgram::loadVector(int location, glm::vec3 vector) {
	glUniform3f(location, vector.x, vector.y, vector.z);
}

void ShaderProgram::LoadMatrix(int location, glm::mat4 matrix) {
	glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

void ShaderProgram::loadBoolean(int location, bool value) {
	float toLoad = 0.f;
	if (value) toLoad = 1.f;
	glUniform1f(location, toLoad);
}

void ShaderProgram::start() {
	glUseProgram(programID);
}

void ShaderProgram::stop() {
	glUseProgram(0);
}

void ShaderProgram::cleanUp() {
	stop();
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	glDeleteProgram(programID);
}

int ShaderProgram::loadShader(char * file, GLuint type) {
	std::ifstream in(file);
	std::string contents(
		(std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>()
	);
	const GLchar* source = (const GLchar *)contents.c_str();

	// GLuint and unsigned int are equivalent
	GLuint shaderID = glCreateShader(type);
	glShaderSource(shaderID, 1, &source, 0);
	glCompileShader(shaderID);

	int isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, &infoLog[0]);
		glDeleteShader(shaderID);
		for (auto const &i : infoLog) {
			std::cout << i << std::endl;
		}
		std::cout << source << std::endl;
		std::cout << "Could not compile shader" << std::endl;
		exit(-1);
	}

	std::cout << "Loaded shader: " << file << " successfully." << std::endl;
	return shaderID;
}

void ShaderProgram::bindAttribute(GLuint attribute, char * variableName){
	glBindAttribLocation(programID, attribute, variableName);
}
