#include "RenderComponent.h"
#include "FTJ_RenderManager.h"
#include "../Entity/GameObject.h"

namespace FTJ
{

	CRenderComponent::CRenderComponent(CGameObject* _gameObject, eDepth _depth) : 
		IComponent(_gameObject),
		m_eDepth(_depth)
	{
		m_pTransform = m_pGameObject->GetTransform();

		SetTexture(L"Default.dds");
	}


	CRenderComponent::~CRenderComponent()
	{
	}

	void CRenderComponent::SetTexture(std::wstring _textureName)
	{
		FTJ::Texture* _texture = CRenderManager::GetInstance()->FindTexture(_textureName);
		if (_texture)
			m_pTexture = _texture;
		else
			m_pTexture = CRenderManager::GetInstance()->FindTexture(L"Default.dds");
	}
}