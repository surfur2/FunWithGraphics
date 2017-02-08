#include "GameEntity.h"
#include "Mesh.h"
#include <DirectXMath.h>



GameEntity::GameEntity(Mesh* entityMesh)
{
	myMesh = entityMesh;

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
