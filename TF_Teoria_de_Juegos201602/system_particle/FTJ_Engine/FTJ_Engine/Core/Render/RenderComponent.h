#pragma once
#include "../Entity/IComponent.h"

#include "FTJ_ShaderDefinitions.h"

#include "atlbase.h"

namespace FTJ
{
	using namespace ShaderDefinitions;

	enum RENDER_CONTEXT { CONTEXT_2D, CONTEXT_3D, CONTEXT_PARTICLE};

	class CRenderManager;

	class CMesh;
	class Texture;

	class CTransform;

	__declspec(align(16)) class CRenderComponent : public IComponent
	{
	protected:
		eDepth		m_eDepth;
		Texture*	m_pTexture = nullptr;

		CTransform* m_pTransform;

	public:
		CRenderComponent(CGameObject* _gameObject, eDepth _depth);
		~CRenderComponent();

		CTransform* GetTransform() const { return m_pTransform; }

		//Rendering
		eDepth		GetDepth() const { return m_eDepth; }
		Texture*	GetTexture() const { return m_pTexture; }

		void SetTexture(std::wstring _textureName);

		virtual RENDER_CONTEXT GetRenderContext() const = 0;

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