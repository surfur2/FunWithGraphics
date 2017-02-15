#include "Camera.h"
#include "DXCore.h"



Camera::Camera(XMFLOAT3 newPos, float newMoveSpeed)
{
	pos = newPos;
	up = XMFLOAT3(0, 1, 0);
	dir = XMFLOAT3(0, 0, 1);
	xRotation = 0;
	yRotation = 0;
	moveSpeed = newMoveSpeed;
	rotationSpeed = .003;
}


Camera::~Camera()
{
}

void Camera::RotateX(int additionalRotation)
{
	xRotation += (additionalRotation * rotationSpeed);

	if (xRotation > XM_2PI)
		xRotation -= XM_2PI;

	if (xRotation < -XM_2PI)
		xRotation += XM_2PI;
}

void Camera::RotateY(int additionalRotation)
{
	yRotation += (additionalRotation * rotationSpeed);

	if (yRotation > XM_2PI)
		yRotation -= XM_2PI;

	if (yRotation < -XM_2PI)
		yRotation += XM_2PI;
}

void Camera::Update(float deltaTime)
{
	XMVECTOR upVec = XMVectorSet(up.x, up.y, up.z, 0);
	XMVECTOR dirVec = XMVectorSet(dir.x, dir.y, dir.z, 0);
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(xRotation, yRotation, 0);
	dirVec = XMVector3Normalize(XMVector3Transform(dirVec, rotation));

	if (GetAsyncKeyState('W') & 0x8000)
	{
		float x = XMVectorGetX(dirVec) * (moveSpeed * deltaTime);
		float y = XMVectorGetY(dirVec) * (moveSpeed * deltaTime);
		float z = XMVectorGetZ(dirVec) * (moveSpeed * deltaTime);
		pos.x += x;
		pos.y += y;
		pos.z += z;
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		float x = XMVectorGetX(dirVec) * (moveSpeed * deltaTime);
		float y = XMVectorGetY(dirVec) * (moveSpeed * deltaTime);
		float z = XMVectorGetZ(dirVec) * (moveSpeed * deltaTime);
		pos.x -= x;
		pos.y -= y;
		pos.z -= z;
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		XMVECTOR crossMovement = XMVector3Cross(upVec, dirVec);
		float x = XMVectorGetX(crossMovement) * (moveSpeed * deltaTime);
		float y = XMVectorGetY(crossMovement) * (moveSpeed * deltaTime);
		float z = XMVectorGetZ(crossMovement) * (moveSpeed * deltaTime);
		pos.x -= x;
		pos.y -= y;
		pos.z -= z;
	}

	if (GetAsyncKeyState('D') & 0x8000)
	{
		XMVECTOR crossMovement = XMVector3Cross(upVec, dirVec);
		float x = XMVectorGetX(crossMovement) * (moveSpeed * deltaTime);
		float y = XMVectorGetY(crossMovement) * (moveSpeed * deltaTime);
		float z = XMVectorGetZ(crossMovement) * (moveSpeed * deltaTime);
		pos.x += x;
		pos.y += y;
		pos.z += z;
	}

	if (GetAsyncKeyState('X') & 0x8000)
	{
		pos.y += (moveSpeed * deltaTime);
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		pos.y -= (moveSpeed * deltaTime);
	}
	// Create the View matrix
	// - In an actual game, recreate this matrix every time the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction vector along which to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	XMVECTOR posVec = XMVectorSet(pos.x, pos.y, pos.z, 0);
	
	XMMATRIX V = XMMatrixLookToLH(
		posVec,     // The position of the "camera"
		dirVec,     // Direction the camera is looking
		upVec);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!
}
