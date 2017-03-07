#include "SimpleShader.h"

#pragma once
class Materials
{
public:
	Materials(SimpleVertexShader* newVertShader, SimplePixelShader* newPixShader, ID3D11ShaderResourceView* srvPtr, ID3D11SamplerState* smplPtr);
	~Materials();
	inline SimpleVertexShader* GetVertexShader() { return vertexShader; };
	inline SimplePixelShader* GetPixelShader() { return pixelShader; };
	inline ID3D11ShaderResourceView* GetShaderResourceView() { return srv; };
	inline ID3D11SamplerState* GetSamplerState() { return sampler; };
private:
	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	ID3D11ShaderResourceView* srv;
	ID3D11SamplerState* sampler;

};

