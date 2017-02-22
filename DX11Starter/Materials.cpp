#include "Materials.h"



Materials::Materials(SimpleVertexShader* newVertShader, SimplePixelShader* newPixShader)
{
	vertexShader = newVertShader;
	pixelShader = newPixShader;
}


Materials::~Materials()
{
	
}
