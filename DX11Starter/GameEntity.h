#pragma once

#include <DirectXMath.h>
#include "Mesh.h"

class GameEntity
{
public:
	GameEntity(Mesh* entityMesh);
	~GameEntity();

	DirectX::XMFLOAT4X4 GetMatrix();
	void SetMatrix(DirectX::XMMATRIX& newWorldMatrix);

	DirectX::XMFLOAT3 GetPosition();
	void SetPosition(DirectX::XMFLOAT3 newPosition);
	
	DirectX::XMFLOAT3 GetScalar();
	void SetScalar(DirectX::XMFLOAT3 newScalar);

	DirectX::XMFLOAT3 GetRotation();
	void SetRotation(DirectX::XMFLOAT3 newRotation);

	void Draw(ID3D11DeviceContext*	context);

	void SetScale(float scale);
	
	void SetRotationZ(float z);

	void SetTranslation(float x, float y);

	void SetAngleFromOrigin(float angle);
	float GetAngleFromOrigin();

	void CalculateWorldMatrix();

private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scalar;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT4X4 worldMatrix;
	Mesh* myMesh;
	float angleFromOrigin;
};

