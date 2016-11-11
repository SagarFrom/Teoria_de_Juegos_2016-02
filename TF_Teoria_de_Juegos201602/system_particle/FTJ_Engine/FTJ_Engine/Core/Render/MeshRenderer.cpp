#include "MeshRenderer.h"
#include "Mesh.h"

#include "FTJ_RenderManager.h"
#include "../Asset/AssetManager.h"

#include "../Entity/Transform.h"
#include "../Entity/GameObject.h"

namespace FTJ
{

	MeshRenderer::MeshRenderer(
		CGameObject* _gameObject, std::string _modelName,eDepth _depth, bool _transparent) 
		: CRenderComponent(_gameObject,  _depth)
	{
		OEAddComponent(m_pGameObject, this);

		SetMesh(_modelName);

		m_t3DContextInfo.eDepthFromCamera = _depth;
		m_t3DContextInfo.bIsTransparent = _transparent;
		m_t3DContextInfo.szModelName = _modelName;
		m_t3DContextInfo.tConstantBufferInfo.WorldMatrix = m_pTransform->CalculateWorldMatrix();
	}

	MeshRenderer::~MeshRenderer()
	{
	}


	void MeshRenderer::SetMesh(std::string _meshName)
	{
		FTJ::CMesh* _mesh = CRenderManager::GetInstance()->FindMesh(_meshName);
		if (m_pMesh == NULL && _mesh != NULL)
		{
			//register for the 1st time in Render Array
			CRenderManager::GetInstance()->RegisterRenderableObject(this, m_pGameObject->GetGameScene());
		}
		else
		{
			//if newMesh == NULL, remove from Render Array
			if (_mesh == NULL)
				CRenderManager::GetInstance()->UnregisterRenderableObject(this, m_pGameObject->GetGameScene());
		}
		m_pMesh = _mesh;
	}
}