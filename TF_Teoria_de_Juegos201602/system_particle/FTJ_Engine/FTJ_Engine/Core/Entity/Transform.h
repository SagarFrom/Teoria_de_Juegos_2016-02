#pragma once
#include "IComponent.h"

#include <DirectXMath.h>

using namespace DirectX;

namespace FTJ
{
	enum RotationAxis
	{
		X_AXIS, Y_AXIS, Z_AXIS
	};

	class CTransform : public FTJ::IComponent
	{
		XMVECTOR m_v3Scale;
		XMMATRIX m_Transform;//stores position

	public:
		CTransform(CGameObject *_owner);
		~CTransform();

		void SetPosition(float x, float y, float z);
		void SetPosition(XMVECTOR& _newPosition);
		void ScaleUniform(float _scale);
		//void SetRotation(btQuaternion& _newRotation);

		//btQuaternion GetRotation() const { return m_Transform.getRotation(); }

		XMVECTOR GetForward() const;
		XMVECTOR GetRight() const;

		void Translate(float _x, float _y, float _z);
		void Translate(XMVECTOR& _translation);

		DirectX::XMMATRIX CalculateWorldMatrix();
		//void RotateAxis(RotationAxis axis, float _fRadians);

		XMVECTOR GetPosition() const;// { return m_Transform.getOrigin(); }
	};
}