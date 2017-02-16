#include <DirectXMath.h>
#pragma once

using namespace DirectX;
class Camera
{
public:
	Camera(XMFLOAT3 pos, float moveSpeed, int width, int height);
	~Camera();
	void RotateY(int pixels);
	void RotateX(int pixels);
	void Update(float deltaTime);
	void Resize(int width, int height);
	inline XMFLOAT4X4 GetViewMatrix() { return viewMatrix; };
	inline XMFLOAT4X4 GetProjectionMatrix() { return projectionMatrix; };
private:
	XMFLOAT3 pos;
	XMFLOAT3 dir;
	XMFLOAT3 up;
	XMFLOAT4X4 viewMatrix;
	// The matrices to go from model space to screen space
	XMFLOAT4X4 projectionMatrix;
	float xRotation;
	float yRotation;
	float moveSpeed;
	float rotationSpeed;
};

