#include "Materials.h"



Materials::Materials(SimpleVertexShader* newVertShader, SimplePixelShader* newPixShader)
{
	vertexShader = newVertShader;
	pixelShader = newPixShader;
}


Materials::~Materials()
{
	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;
}
