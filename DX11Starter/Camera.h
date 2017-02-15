#include <DirectXMath.h>
#pragma once

using namespace DirectX;
class Camera
{
public:
	Camera(XMFLOAT3 pos, float moveSpeed);
	~Camera();
	void RotateY(int pixels);
	void RotateX(int pixels);
	void Update(float deltaTime);
	inline XMFLOAT4X4 GetViewMatrix() { return viewMatrix; };

private:
	XMFLOAT3 pos;
	XMFLOAT3 dir;
	XMFLOAT3 up;
	XMFLOAT4X4 viewMatrix;
	float xRotation;
	float yRotation;
	float moveSpeed;
	float rotationSpeed;
};

