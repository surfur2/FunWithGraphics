#pragma once

#include <DirectXMath.h>
#include "Mesh.h"
#include "Materials.h"
#include "Lights.h"

using namespace DirectX;

class GameEntity
{
public:
	GameEntity(Mesh* entityMesh, Materials* newMaterial);
	~GameEntity();

	XMFLOAT4X4 GetMatrix();
	void SetMatrix(XMMATRIX& newWorldMatrix);

	XMFLOAT3 GetPosition();
	void SetPosition(XMFLOAT3 newPosition);
	
	XMFLOAT3 GetScalar();
	void SetScalar(XMFLOAT3 newScalar);

	XMFLOAT3 GetRotation();
	void SetRotation(XMFLOAT3 newRotation);

	void Draw(ID3D11DeviceContext*	context);

	void SetScale(float scale);
	
	void SetRotationZ(float z);

	void SetTranslation(float x, float y);

	void SetAngleFromOrigin(float angle);
	float GetAngleFromOrigin();

	void CalculateWorldMatrix();

	void PrepareMaterials(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, DirectionalLight& dirLight, DirectionalLight& dirLightTwo);

private:
	XMFLOAT3 position;
	XMFLOAT3 scalar;
	XMFLOAT3 rotation;
	XMFLOAT4X4 worldMatrix;
	Mesh* myMesh;
	Materials* myMaterial;
	float angleFromOrigin;
};

