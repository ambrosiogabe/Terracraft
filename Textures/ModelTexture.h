#pragma once
class ModelTexture
{
public:
	ModelTexture(int texID);
	~ModelTexture();

	int getID();
	float getShineDamper();
	float getReflectivity();
	bool getTransparency();
	bool getFakeLighting();
	void setShineDamper(float num);
	void setReflectivity(float num);
	void setHasTransparency(bool val);
	void setUseFakeLighting(bool val);

private:
	int textureID;
	float shineDamper = 1.0;
	float reflectivity = 0.0;
	bool hasTransparency = false;
	bool useFakeLighting = false;
};

