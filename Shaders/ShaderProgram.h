#pragma once

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class ShaderProgram
{
public:
	ShaderProgram(char* vertexFile, char* fragmentFile);
	void start();
	void stop();
	void cleanUp();
	void init();

private:
	int programID;
	int vertexShaderID;
	int fragmentShaderID;

	int loadShader(char* file, GLuint type);

protected:
	void loadFloat(int location, float value);
	void loadVector(int location, glm::vec3 vector);
	void loadBoolean(int location, bool value);
	void loadInt(int location, int value);
	void LoadMatrix(int location, glm::mat4 matrix);
	void bindAttribute(GLuint attribute, char* variableName);
	int getUniformLocation(char* uniformName);
	virtual void bindAttributes() = 0;
	virtual void getAllUniformLocations() = 0;
};

