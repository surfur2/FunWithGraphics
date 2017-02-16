#include "SimpleShader.h"

#pragma once
class Materials
{
public:
	Materials(SimpleVertexShader* newVertShader, SimplePixelShader* newPixShader);
	~Materials();
	inline SimpleVertexShader* GetVertexShader() { return vertexShader; };
	inline SimplePixelShader* GetPixelShader() { return pixelShader; };
private:
	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
};

