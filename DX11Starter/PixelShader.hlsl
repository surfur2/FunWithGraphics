
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 normal       : NORMAL;
	float2 uv           : TEXCOORD;
	//float4 color		: COLOR;
};

// Struct for defining a directional light
struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

// cbuffer for a directional light
cbuffer externalData : register(b0)
{
	DirectionalLight lightOne;
	DirectionalLight lightTwo;
};

Texture2D diffuseTexture : register(t0);
SamplerState samp : register (s0);

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{

	// Sample a texture for the colors on that texture
	float4 surfaceColor = diffuseTexture.Sample(samp, input.uv);


	// Calculate the lights effects in the scene

	// Normnalize all incoming pixel data, just in case.
	input.normal = normalize(input.normal);

	// Reverse the direction of light direction
	float3 lightOneReverseNormal = normalize(mul(lightOne.Direction , -1.0f));
	float3 lightTwoReverseNormal = normalize(mul(lightTwo.Direction, -1.0f));

	// Calculate the amount of light on a single pixel by taking the between direction to the light and the normal of a pixel.
	// This yields the angle between the two.
	float amountLightOne = saturate(dot(input.normal, lightOneReverseNormal));
	float amountLightTwo = saturate(dot(input.normal, lightTwoReverseNormal));

	// Add all lights together and return the color.
	return (surfaceColor * amountLightOne) + (surfaceColor * amountLightTwo);
}