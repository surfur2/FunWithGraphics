#include "Materials.h"



Materials::Materials(SimpleVertexShader* newVertShader, SimplePixelShader* newPixShader, ID3D11ShaderResourceView* srvPtr, ID3D11SamplerState* smplPtr)
{
	vertexShader = newVertShader;
	pixelShader = newPixShader;
	srv = srvPtr;
	sampler = smplPtr;
}


Materials::~Materials()
{
	
}
