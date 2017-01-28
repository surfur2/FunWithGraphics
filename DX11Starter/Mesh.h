#include "Vertex.h"
#include <d3d11.h>

#pragma once
class Mesh
{
public:
	Mesh(Vertex* verticies, int numVerticies, int* indicies, int numIndicies, ID3D11Device* device);
	~Mesh();
	ID3D11Buffer* GetVertexBuffer() { return vertexBuffer; };
	ID3D11Buffer* GetIndexBuffer() { return indexBuffer; };
	int GetIndexCount() { return numIndicies; };

private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int numIndicies;
};

