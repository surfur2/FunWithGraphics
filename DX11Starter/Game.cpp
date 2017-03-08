#include "Game.h"
#include "Vertex.h"
#include "WICTextureLoader.h"

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore( 
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	// Initialize fields
	meshOne = 0;
	meshTwo = 0;
	meshObject = 0;
	myMaterial = 0;
	metalMat = 0;
	myCamera = 0;
	pixelShader = 0;
	vertexShader = 0;
	radSRV = 0;
	mtlSRV = 0;
	sampler = 0;

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Delete all our meshes for the game. We delete these here instead of in entities so that we do not
	// have to keep track of the number of references per Entity. Different entites will share meshes
	delete meshOne;
	delete meshTwo;
	delete meshObject;
	delete meshCube;

	// Delete all out gameEntities
	for (int i = 0; i < numberGameEntities; i++)
		delete gameEntities[i];

	// Delete the camera
	delete myCamera;

	//Release texture resources
	radSRV->Release();
	mtlSRV->Release();
	sampler->Release();

	// Delete my material. We delete these here instead of in entities so that we do not
	// have to keep track of the number of references per Entity. Different entites will share materials.
	delete myMaterial;
	delete metalMat;

	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Create Lights
	dirLightOne.AmbientColor = XMFLOAT4(.1f, .1f, .1f, 1.0f);
	dirLightOne.DiffuseColor = XMFLOAT4(.5f, .5f, 1, 1);
	dirLightOne.Direction = XMFLOAT3(1, -1, 0);

	dirLightTwo.AmbientColor = XMFLOAT4(.1f, .1f, .1f, 1.0f);
	dirLightTwo.DiffuseColor = XMFLOAT4(1, 1, 1, 1);
	dirLightTwo.Direction = XMFLOAT3(1, -1, 0);

	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateMatrices();
	CreateBasicGeometry();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	if (!vertexShader->LoadShaderFile(L"Debug/VertexShader.cso"))
		vertexShader->LoadShaderFile(L"VertexShader.cso");		

	pixelShader = new SimplePixelShader(device, context);
	if (!pixelShader->LoadShaderFile(L"Debug/PixelShader.cso"))
		pixelShader->LoadShaderFile(L"PixelShader.cso");

	CreateWICTextureFromFile(device, context, L"Assets/Textures/rad.png", 0, &radSRV);
	CreateWICTextureFromFile(device, context, L"Assets/Textures/mtl.png", 0, &mtlSRV);

	// Create a sampler state that defines the sampling
	// options for any and all textures we use

	// First, create a description
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.MaxAnisotropy = 16;


	// Now, create the sampler from the description
	device->CreateSamplerState(&sampDesc, &sampler);

	myMaterial = new Materials(vertexShader, pixelShader, radSRV, sampler);
	metalMat = new Materials(vertexShader, pixelShader, mtlSRV, sampler);
	// You'll notice that the code above attempts to load each
	// compiled shader file (.cso) from two different relative paths.

	// This is because the "working directory" (where relative paths begin)
	// will be different during the following two scenarios:
	//  - Debugging in VS: The "Project Directory" (where your .cpp files are) 
	//  - Run .exe directly: The "Output Directory" (where the .exe & .cso files are)

	// Checking both paths is the easiest way to ensure both 
	// scenarios work correctly, although others exist
}



// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
	// Create the camera which has a view matrix
	myCamera = new Camera(XMFLOAT3(0, 0, -5), 2, width, height);
}


// --------------------------------------------------------
// Creates the geometry we're going to draw
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	//XMFLOAT4 red	= XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	//XMFLOAT4 green	= XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	//XMFLOAT4 blue	= XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	// Default normals and uvs for testing purposes.
	XMFLOAT3 defaultNormal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	XMFLOAT2 defaultUV = XMFLOAT2(0.0f, 0.0f);

	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	Vertex verticesOne[] = 
	{
		{ XMFLOAT3(+0.0f, +1.0f, +0.0f), defaultNormal, defaultUV },
		{ XMFLOAT3(+1.5f, -1.0f, +0.0f), defaultNormal, defaultUV },
		{ XMFLOAT3(-1.5f, -1.0f, +0.0f), defaultNormal, defaultUV },

	};

	// Setup for Mesh Two

	Vertex verticesTwo[] =
	{
		{ XMFLOAT3(+0.0f, +0.87f, +0.0f), defaultNormal, defaultUV },
		{ XMFLOAT3(+0.5f, -0.0f, +0.0f), defaultNormal, defaultUV },
		{ XMFLOAT3(-0.5f, -0.0f, +0.0f), defaultNormal, defaultUV },
		{ XMFLOAT3(-0.0f, -0.87f, +0.0f), defaultNormal, defaultUV },
	};

	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...
	int indicesOne[] = { 0, 1, 2 };
	int indicesTwo[] = { 2, 0, 1, 2, 1, 3 };

	// Create our meshes for the game
	meshOne = new Mesh(verticesOne, 3, indicesOne, 3, device);
	meshTwo = new Mesh(verticesTwo, 4, indicesTwo, 6, device);
	meshObject = new Mesh("cone.obj", device);
	meshCube = new Mesh("cube.obj", device);

	// Create game entities with the new meshes and individual world matricies.
	//gameEntities.push_back(new GameEntity(meshOne, myMaterial));
	//gameEntities.push_back(new GameEntity(meshOne, myMaterial));
	//gameEntities.push_back(new GameEntity(meshTwo, myMaterial));
	gameEntities.push_back(new GameEntity(meshObject, myMaterial));
	GameEntity* temp = new GameEntity(meshCube, metalMat);
	temp->SetPosition(XMFLOAT3(4, 0 , 0));
	gameEntities.push_back(temp);
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	// Resize the projection matrix in our Camera
	myCamera->Resize(width, height);

}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	float sinTime = (sin(totalTime * 3) + 2.0f) / 10.0f;
	float speed = DirectX::XM_PI / 5.0;

	// Update the world matricies of all our game objects every frame.
	/*gameEntities[0]->SetAngleFromOrigin(speed * deltaTime);
	gameEntities[0]->SetScale(sinTime);
	gameEntities[0]->SetRotationZ(totalTime);
	gameEntities[0]->SetTranslation(2 * cos(gameEntities[0]->GetAngleFromOrigin()),2 * sin(gameEntities[0]->GetAngleFromOrigin()));

	gameEntities[1]->SetAngleFromOrigin(speed * deltaTime);
	gameEntities[1]->SetScale(sinTime);
	gameEntities[1]->SetRotationZ(-totalTime);
	gameEntities[1]->SetTranslation(2* cos(gameEntities[1]->GetAngleFromOrigin() + DirectX::XM_PI), 2 * sin (gameEntities[1]->GetAngleFromOrigin() + DirectX::XM_PI));*/

	// Update the view matrix every frame.
	myCamera->Update(deltaTime);
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = {0.4f, 0.6f, 0.75f, 0.0f};

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	
	for (int i = 0; i < numberGameEntities; i++)
	{
		// Calculate the new worldMatrix for entity
		gameEntities[i]->CalculateWorldMatrix();

		// Set all material data for each game entity
		gameEntities[i]->PrepareMaterials(myCamera->GetViewMatrix(), myCamera->GetProjectionMatrix(), dirLightOne, dirLightTwo);

		// Custom draw method that will set the mesh verticies for us
		gameEntities[i]->Draw(context);
	}

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	if (buttonState && 0x0002)
	{
		myCamera->RotateX(y - prevMousePos.y);
		myCamera->RotateY(x - prevMousePos.x);
	}
	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion