#include "Vertex.h"
#include <d3d11.h>

#pragma once
// Class that contains the definition for rendering a shape
class Mesh
{
public:
	Mesh(Vertex* verticies, int numVerticies, int* indicies, int numIndicies, ID3D11Device* device);
	Mesh(char* fileinfo, ID3D11Device* device);
	~Mesh();
	ID3D11Buffer* GetVertexBuffer() { return vertexBuffer; };
	ID3D11Buffer* GetIndexBuffer() { return indexBuffer; };
	int GetIndexCount() { return numIndicies; };

private:
	void CreateVertexBuffer(int vertexNumber, Vertex* verticies, ID3D11Device* device);
	void CreateIndexBuffer(int indexNumber, int* indicies, ID3D11Device* device);
	// Holds the verticies for a shape. Defines position adn color
	ID3D11Buffer* vertexBuffer;
	// Holds the order the verticies should be rendered in.
	ID3D11Buffer* indexBuffer;
	// Number indicies to render
	int numIndicies;
};

