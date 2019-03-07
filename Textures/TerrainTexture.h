#pragma once
class TerrainTexture
{
public:
	TerrainTexture(int id);
	~TerrainTexture();
	int getId();
	void setId(int id);

private:
	int id;
};

