#pragma once

#include "DXCore.h"
#include "Mesh.h"
#include "GameEntity.h"
#include "Camera.h"
#include "Lights.h"
#include "SimpleShader.h"
#include "Materials.h"
#include <DirectXMath.h>
#include <vector>

using namespace std;

class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);
private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void CreateMatrices();
	void CreateBasicGeometry();

	// Mesh containers for buffer values
	Mesh* meshOne;
	Mesh* meshTwo;
	Mesh* meshObject;
	Mesh* meshCube;

	// Make a few GameEntities
	int numberGameEntities = 2;
	vector<GameEntity*> gameEntities;

	// Make a new Camera
	Camera* myCamera;

	// Shaders for game
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	// Lights for game
	DirectionalLight dirLightOne;
	DirectionalLight dirLightTwo;

	// Textures for the game
	ID3D11ShaderResourceView* radSRV;
	ID3D11ShaderResourceView* mtlSRV;
	ID3D11SamplerState* sampler;

	//Material for all current stuff
	Materials* myMaterial;
	Materials* metalMat;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
};

