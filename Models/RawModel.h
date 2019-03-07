#pragma once
class RawModel
{
public:
	RawModel(int vaoID, int vertexCount);
	~RawModel();
	int getVaoID();
	int getVertexCount();

private:
	int vaoID;
	int vertexCount;
};

