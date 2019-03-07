#pragma once
#include "ShaderProgram.h"
#include "../Entities/Camera.h"
#include "../Entities/Light.h"
#include <vector>

#define MAX_LIGHTS 4

class TerrainShader : public ShaderProgram
{
public:
	TerrainShader(void);
	~TerrainShader();
	void loadTransformationMatrix(glm::mat4 matrix);
	void loadProjectionMatrix(glm::mat4 matrix);
	void loadViewMatrix(Camera* camera);
	void loadLights(std::vector<Light*> lights);
	void loadShineVariables(float damper, float reflectivity);
	void loadSkyColor(float r, float g, float b);
	void loadTextureUnits();

private:
	int location_transformationMatrix;
	int location_projectionMatrix;
	int location_viewMatrix;
	int location_lightPosition[MAX_LIGHTS];
	int location_lightColor[MAX_LIGHTS];
	int location_attenuation[MAX_LIGHTS];
	int location_reflectivity;
	int location_shineDamper;
	int location_skyColor;
	int location_backgroundTexture;
	int location_rTexture;
	int location_gTexture;
	int location_bTexture;
	int location_blendMap;

protected:
	virtual void bindAttributes();
	virtual void getAllUniformLocations();
};

