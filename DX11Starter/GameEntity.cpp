#include "GameEntity.h"
#include "Mesh.h"
#include "Lights.h"
#include <DirectXMath.h>

using namespace DirectX;

GameEntity::GameEntity(Mesh* entityMesh, Materials* newMaterial)
{
	myMesh = entityMesh;
	myMaterial = newMaterial;

	DirectX::XMMATRIX W = DirectX::XMMatrixIdentity();
	DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixTranspose(W));

	position = DirectX::XMFLOAT3(+0.0f, +0.0f, +0.0f);
	scalar = DirectX::XMFLOAT3(+1.0f, +1.0f, +1.0f);
	rotation = DirectX::XMFLOAT3(+0.0f, +0.0f, +0.0f);
	angleFromOrigin = 0.0f;
}


GameEntity::~GameEntity()
{
}

// Getters and Setters
DirectX::XMFLOAT4X4 GameEntity::GetMatrix() { return worldMatrix; }
void GameEntity::SetMatrix(DirectX::XMMATRIX& newWorldMatrix) { DirectX::XMStoreFloat4x4(&worldMatrix, newWorldMatrix); }

DirectX::XMFLOAT3 GameEntity::GetPosition() { return position; }
void GameEntity::SetPosition(DirectX::XMFLOAT3 newPosition) { position = newPosition; }

DirectX::XMFLOAT3 GameEntity::GetScalar() { return scalar; }
void GameEntity::SetScalar(DirectX::XMFLOAT3 newScalar) { scalar = newScalar; }

DirectX::XMFLOAT3 GameEntity::GetRotation() { return rotation; }
void GameEntity::SetRotation(DirectX::XMFLOAT3 newRotation) { rotation = newRotation; }


void GameEntity::SetAngleFromOrigin(float angle)
{
	angleFromOrigin += angle;

	if (angleFromOrigin > XM_2PI)
		angleFromOrigin -= XM_2PI;

	if (angleFromOrigin < -XM_2PI)
		angleFromOrigin += XM_2PI;
}

float GameEntity::GetAngleFromOrigin()
{
	return angleFromOrigin;
}

void GameEntity::SetScale(float scale)
{
	SetScalar(DirectX::XMFLOAT3(scale, scale, scale));
}

void GameEntity::SetRotationZ(float z)
{
	SetRotation(DirectX::XMFLOAT3(0.0f, 0.0f, z));
}

void GameEntity::SetTranslation(float x, float y)
{
	SetPosition(DirectX::XMFLOAT3(x, y, position.z));
}

void GameEntity::CalculateWorldMatrix()
{
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(scalar.x, scalar.y, scalar.z);
	DirectX::XMMATRIX localRotation = DirectX::XMMatrixRotationZ(rotation.z);
	DirectX::XMMATRIX localPosition = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

	SetMatrix(DirectX::XMMatrixTranspose(scale * localRotation * localPosition));
}
void GameEntity::Draw(ID3D11DeviceContext*	context)
{
	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11Buffer* temp = myMesh->GetVertexBuffer();
	context->IASetVertexBuffers(0, 1, &temp, &stride, &offset);
	context->IASetIndexBuffer(myMesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	// Finally do the actual drawing
	//  - Do this ONCE PER OBJECT you intend to draw
	//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
	//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
	//     vertices in the currently set VERTEX BUFFER
	context->DrawIndexed(
		myMesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices
}

void GameEntity::PrepareMaterials(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMarix, DirectionalLight& dirLightOne, DirectionalLight& dirLightTwo)
{
	// Send data to shader variables
	//  - Do this ONCE PER OBJECT you're drawing
	//  - This is actually a complex process of copying data to a local buffer
	//    and then copying that entire buffer to the GPU.  
	//  - The "SimpleShader" class handles all of that for you.
	myMaterial->GetVertexShader()->SetMatrix4x4("world", worldMatrix);
	myMaterial->GetVertexShader()->SetMatrix4x4("view", viewMatrix);
	myMaterial->GetVertexShader()->SetMatrix4x4("projection", projectionMarix);

	// Once you've set all of the data you care to change for
	// the next draw call, you need to actually send it to the GPU
	//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
	myMaterial->GetVertexShader()->CopyAllBufferData();
	myMaterial->GetVertexShader()->SetShader();

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	myMaterial->GetPixelShader()->SetData(
		"lightOne",
		&dirLightOne,
		sizeof(DirectionalLight));

	myMaterial->GetPixelShader()->SetData(
		"lightTwo",
		&dirLightTwo,
		sizeof(DirectionalLight));


	myMaterial->GetPixelShader()->CopyAllBufferData();
	myMaterial->GetPixelShader()->SetShader();

}
