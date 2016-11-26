#include "CameraBehavior.h"
#include "../../Core/Input/InputManager.h"

#include "../../Core/Entity/GameObject.h"

#include "../../Core/FTJ_Console.h"
#include "../../Core/Log/Log.h"

using namespace FTJ;

CameraBehavior::CameraBehavior(FTJ::CGameObject* _go) : FTJ::IBehavior(_go)
{
	OEAddComponent(_go, this);

	CInputManager::GetInstance()->ActivateKey(VK_LBUTTON);

	translationMat = XMMatrixTranslationFromVector(m_pTransform->GetPosition());
	rotationMat = XMMatrixIdentity();

	XMMATRIX& m = XMMatrixMultiply(rotationMat, translationMat);
	m_pTransform->SetTransform(m);
}

CameraBehavior::~CameraBehavior()
{
}

/*virtual*/ void CameraBehavior::Update(const float fDeltaTime)
{
	auto mouseInfo = CInputManager::GetInstance()->GetMouseInput();

	if (true == CInputManager::GetInstance()->GetKeyDown(VK_LBUTTON))
	{
		float deltaX = (float)mouseInfo.x - (float)mouseInfo.prevX;
		float deltaY = (float)mouseInfo.y - (float)mouseInfo.prevY;

		const float MOUSE_SENSITIVITY = 12.f;

		translationMat = XMMatrixTranslationFromVector(m_pTransform->GetPosition());

		if (deltaY != 0 || deltaX != 0)
		{
			if (abs(deltaX) > abs(deltaY))
			{
				XMVECTOR axis = { 0, 1, 0, 1 };

				float angleX = -deltaX * MOUSE_SENSITIVITY* fDeltaTime;
				rotationMat = XMMatrixMultiply(rotationMat, XMMatrixRotationNormal(axis, angleX));
			}
			else if (abs(deltaY) > abs(deltaX))
			{
				XMVECTOR axis = { 1, 0, 0, 1 };

				float angleY = -deltaY * MOUSE_SENSITIVITY* fDeltaTime;
				rotationMat = XMMatrixMultiply(XMMatrixRotationNormal(axis, angleY), rotationMat);
			}

			XMMATRIX& m = XMMatrixMultiply(rotationMat, translationMat);
			m_pTransform->SetTransform(m);
		}
	}
}
