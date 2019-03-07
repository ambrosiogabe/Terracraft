#pragma once
#include "ShaderProgram.h"
#include "../Entities/Camera.h"
#include "../Entities/Light.h"
#include <vector>

#define MAX_LIGHTS 4
// to print glm::mat4
#define GLM_ENABLE_EXPERIMENTAL 
#include "glm/gtx/string_cast.hpp"

class StaticShader : public ShaderProgram
{
public:
	StaticShader(void);
	~StaticShader();
	void loadTransformationMatrix(glm::mat4 matrix);
	void loadProjectionMatrix(glm::mat4 matrix);
	void loadViewMatrix(Camera* camera);
	void loadLights(std::vector<Light*> lights);
	void loadFakeLighting(bool val);
	void loadShineVariables(float damper, float reflectivity);
	void loadSkyColor(float r, float g, float b);

private:
	int location_transformationMatrix;
	int location_projectionMatrix;
	int location_viewMatrix;
	int location_lightPosition[MAX_LIGHTS];
	int location_lightColor[MAX_LIGHTS];
	int location_attenuation[MAX_LIGHTS];
	int location_reflectivity;
	int location_shineDamper;
	int location_useFakeLighting;
	int location_skyColor;

protected:
	virtual void bindAttributes();
	virtual void getAllUniformLocations();
};

