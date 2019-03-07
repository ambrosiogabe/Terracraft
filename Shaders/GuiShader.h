#pragma once

#include "ShaderProgram.h"
#include <glm/mat4x4.hpp>

class GuiShader : public ShaderProgram
{
public:
	GuiShader(void);
	~GuiShader();

	void loadTransformationMatrix(glm::mat4 matrix);

private:
	int location_transformationMatrix;

protected:
	virtual void bindAttributes();
	virtual void getAllUniformLocations();
};

