#include "Transform.h"
#include "GameObject.h"

namespace FTJ
{
	CTransform::CTransform(CGameObject* _owner) : IComponent(_owner)
	{
		OEAddComponent(m_pGameObject, this);

		m_Transform = XMMatrixIdentity();
		
		ScaleUniform(1.f);

		//SetRotation(btQuaternion(0, 0, 0, 1.f));
	}

	CTransform::~CTransform()
	{
	}

	void CTransform::SetPosition(float x, float y, float z)
	{
		XMFLOAT3 pos = { x, y, z };
		SetPosition(XMLoadFloat3(&pos));
	}
	void CTransform::SetPosition(XMVECTOR& _newPosition)
	{
		this->m_Transform.r[3] = _newPosition;
		//m_Transform.setOrigin(_newPosition);
	}

	XMVECTOR CTransform::GetPosition() const// { return m_Transform.getOrigin(); }
	{
		return this->m_Transform.r[3];
	}

	void CTransform::ScaleUniform(float _scale)
	{
		m_v3Scale.m128_f32[0] = _scale;
		m_v3Scale.m128_f32[1] = _scale; 
		m_v3Scale.m128_f32[2] = _scale;

		//this->m_Transform = XMMatrixScaling(_scale, _scale, _scale);
		//m_v3Scale.setValue(_scale, _scale, _scale);
	}

	XMVECTOR CTransform::GetForward() const
	{
		//float worldMatrix[16];
		//m_Transform.getOpenGLMatrix(worldMatrix);
		//
		//btVector3 forward;
		//forward.setValue(worldMatrix[8], worldMatrix[9], worldMatrix[10]);
		return this->m_Transform.r[2];
	}
	XMVECTOR CTransform::GetUp() const
	{
		return this->m_Transform.r[1];
	}

	XMVECTOR CTransform::GetRight() const
	{
		//float worldMatrix[16];
		//m_Transform.getOpenGLMatrix(worldMatrix);

		//btVector3 right;
		//right.setValue(worldMatrix[0], worldMatrix[1], worldMatrix[2]);
		//return right;
		return this->m_Transform.r[0];
	}

	//void CTransform::Multiply(XMMATRIX& _m)
	//{
	//	m_Transform *= _m;
	//}

	//void IEntity::Translate(btVector3& _translation)
	//{
	//	SetPosition(GetPosition() + _translation);
	//}
	//
	void CTransform::RotateAxis(RotationAxis axis, float _fRadians)
	{
		XMVECTOR _axis;
		XMVectorSet(0.f, 0.f, 0.f, 0.f);
		_axis.m128_f32[axis] = 1.f;
		XMMATRIX vRot = XMMatrixRotationAxis(_axis, _fRadians);

		m_Transform *= vRot;
	}

	void CTransform::Rotate(XMVECTOR _vector, float _fRadians)
	{
		XMMATRIX vRot = XMMatrixRotationAxis(_vector, _fRadians);
		m_Transform = XMMatrixMultiply(m_Transform, vRot);
	}

	//FUNC used by render manager for drawing
	DirectX::XMMATRIX CTransform::CalculateWorldMatrix()
	{
		using namespace DirectX;

		return this->m_Transform;

		//btVector3 position = m_Transform.getOrigin();

		//XMMATRIX _translate = XMMatrixTranslation(
		//	position.x(),
		//	position.y(),
		//	position.z());

		//XMVECTOR quat = m_Transform.getRotation().get128();
		//XMMATRIX _rotation = XMMatrixRotationQuaternion(quat);

		//XMMATRIX _scale = XMMatrixScaling(
		//	m_v3Scale.x(),
		//	m_v3Scale.y(),
		//	m_v3Scale.z());

		//return XMMatrixMultiply(_scale, XMMatrixMultiply(_rotation, _translate));
	}

	//void IEntity::SetRotation(btQuaternion& _newRotation)
	//{
	//	m_Transform.setRotation(_newRotation);
	//}

	void CTransform::Translate(float _x, float _y, float _z)
	{
		XMFLOAT3 translation = { _x, _y, _z };
		Translate(XMLoadFloat3(&translation));
	}
	void CTransform::Translate(XMVECTOR& _translation)
	{
		m_Transform.r[3] += _translation;
	}

	//==
	void CTransform::LookAt(XMVECTOR _targetPosition)
	{
		XMVECTOR worldY = { 0, 1, 0, 0 };
		m_Transform = XMMatrixLookAtLH(GetPosition(), _targetPosition, worldY);
	}
}