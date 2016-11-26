#include "Camera.h"

#include "../Entity/GameObject.h"
#include "../Entity/Transform.h"

#include "FTJ_RenderManager.h"

namespace FTJ
{

	CCamera::CCamera(CGameObject* _owner) : IComponent(_owner)
	{
		OEAddComponent(m_pGameObject, this);


		float nearPlane = 0.1f;
		float farPlane = 500.f;
		float FOV = 65.f / 180.f* 3.1416f;
		float aspectRatio = 
			(float)CRenderManager::GetInstance()->GetWindowWidth() / 
			CRenderManager::GetInstance()->GetWindowHeight();

		float YScale = 1.f / (tan(0.6f * FOV));
		float XScale = YScale * aspectRatio;

		m_MVP.projMatrix =
			XMMatrixPerspectiveFovLH(FOV, aspectRatio, nearPlane, farPlane);
	}

	CCamera::~CCamera()
	{
	}

	void CCamera::CalculateMVP(XMMATRIX& _rotation, XMMATRIX& _translation)
	{
		m_MVP.viewMatrix = XMMatrixMultiply(_rotation, _translation);
		XMVECTOR determinant = XMMatrixDeterminant(m_MVP.viewMatrix);
		m_MVP.viewMatrix = XMMatrixInverse(&determinant, m_MVP.viewMatrix);
	}

	void CCamera::CalculateMVP()
	{
		//XMMATRIX _translation = XMMatrixTranslation(
		//	GetPosition().m128_f32[0],
		//	GetPosition().y(),
		//	GetPosition().z());

		//XMVECTOR quat = m_Transform.getRotation().get128();
		//XMMATRIX _rotation = XMMatrixRotationQuaternion(quat);


#if 0
		XMVECTOR position = GetGameObject()->GetTransform()->GetPosition();
		XMMATRIX translation = XMMatrixTranslation(position.m128_f32[0], position.m128_f32[1], position.m128_f32[2]);

		CalculateMVP(XMMatrixIdentity(),
			translation);
#else
		m_MVP.viewMatrix = GetGameObject()->GetTransform()->CalculateWorldMatrix();
		XMVECTOR determinant = XMMatrixDeterminant(m_MVP.viewMatrix);
		m_MVP.viewMatrix = XMMatrixInverse(&determinant, m_MVP.viewMatrix);
#endif
	}
}