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

	__declspec(align(16)) class CTransform : public FTJ::IComponent
	{
		XMVECTOR m_v3Scale;
		XMMATRIX m_Transform;//stores position + rotation

	public:
		CTransform(CGameObject *_owner);
		~CTransform();

		void SetPosition(float x, float y, float z);
		void SetPosition(XMVECTOR& _newPosition);
		void ScaleUniform(float _scale);
		//void SetRotation(btQuaternion& _newRotation);

		void SetTransform(XMMATRIX& _m){ m_Transform = _m; }

		//btQuaternion GetRotation() const { return m_Transform.getRotation(); }
		//void Multiply(XMMATRIX& _m);

		XMVECTOR GetForward() const;
		XMVECTOR GetUp() const;
		XMVECTOR GetRight() const;

		void Translate(float _x, float _y, float _z);
		void Translate(XMVECTOR& _translation);

		DirectX::XMMATRIX CalculateWorldMatrix();
		void RotateAxis(RotationAxis axis, float _fRadians);
		void CTransform::Rotate(XMVECTOR _vector, float _fRadians);

		XMVECTOR GetPosition() const;// { return m_Transform.getOrigin(); }

		void LookAt(XMVECTOR _targetPosition);

		void* operator new(size_t i)
		{
			return _mm_malloc(i, 16);
		}

			void operator delete(void* p)
		{
			_mm_free(p);
		}

	};
}