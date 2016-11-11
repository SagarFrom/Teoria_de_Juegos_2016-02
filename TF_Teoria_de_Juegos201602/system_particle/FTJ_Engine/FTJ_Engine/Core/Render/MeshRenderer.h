#pragma once
#include "RenderComponent.h"

#include <vector>

namespace FTJ
{
	class CGameObject;

	class MeshRenderer :
		public CRenderComponent
	{
		T3DContextInfo			m_t3DContextInfo;

	protected:
		CMesh*		m_pMesh = nullptr;

		UINT			m_nVertices;
	public:
		MeshRenderer(CGameObject* _gameObject, std::string _modelName, eDepth _depth = Playable, bool _transparent = false);

		~MeshRenderer();

		inline T3DContextInfo& Get3DInfo() { return m_t3DContextInfo; }

		void SetMesh(std::string _meshName);

		CMesh*		GetMesh() const { return m_pMesh; }

		virtual RENDER_CONTEXT GetRenderContext() const { return RENDER_CONTEXT::CONTEXT_3D; }

	};
}
