
#include "cameraclass.h"

using namespace DirectX;
CameraClass::CameraClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}


void CameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}


void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}


DirectX::XMFLOAT3 CameraClass::GetPosition()
{
	return DirectX::XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}


DirectX::XMFLOAT3 CameraClass::GetRotation()
{
	return DirectX::XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}


void CameraClass::Render()
{
	DirectX::XMFLOAT3 up, position, lookAt;
	float yaw, pitch, roll;
	DirectX::XMMATRIX rotationMatrix;

	
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	DirectX::XMFLOAT3 angles{ -pitch, -yaw, -roll };
	DirectX::XMVECTOR vector = DirectX::XMLoadFloat3(&angles);

	rotationMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(vector);

	XMVECTOR _lookat = XMLoadFloat3(&lookAt), _up = XMLoadFloat3(&up), _pos = XMLoadFloat3(&position);
	_lookat = XMVector3TransformCoord(_lookat, rotationMatrix);
	_lookat = XMVector3TransformCoord(_lookat, rotationMatrix);
	_up = XMVector3TransformCoord(_up, rotationMatrix);


	_lookat = _pos + _lookat;

	
	m_viewMatrix = XMMatrixLookAtLH(_pos, _lookat, _up);

	return;
}


void CameraClass::GetViewMatrix(DirectX::XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}