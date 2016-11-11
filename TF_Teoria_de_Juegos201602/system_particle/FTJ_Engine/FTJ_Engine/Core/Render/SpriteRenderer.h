#pragma once
#include "RenderComponent.h"

#include <d3d11.h>

namespace FTJ
{
	class CSpriteRenderer :
		public CRenderComponent
	{
		CComPtr<ID3D11Buffer>	m_pVertexBuffer;
		T2DContextInfo			m_t2DContextInfo;

	public:
		CSpriteRenderer(CGameObject* _gameObject, eDepth _depth);
		~CSpriteRenderer();

		inline ID3D11Buffer* GetVertexBuffer() { return m_pVertexBuffer.p; }
		inline T2DContextInfo& Get2DInfo() { return m_t2DContextInfo; }

		virtual RENDER_CONTEXT GetRenderContext() const { return RENDER_CONTEXT::CONTEXT_2D; }

	};
}