/***********************************************
* Filename:  		2DContext.h
* Date:      		10/7/15
* Mod. Date: 		10/7/15
* Mod. Initials:	DKR
* Author:    		David Rasmussen
* Purpose:   		A render context to render
					2D objects
************************************************/

#ifndef C2D_CONTEXT_H
#define C2D_CONTEXT_H

#include "BaseContext.h"

//using namespace DirectX;

namespace FTJ
{
class CSpriteRenderer;

	class C2DContext : public CBaseContext
	{

	private:
		ID3D11BlendState*		m_pBlendState;
		// Members
		ID3D11GeometryShader*	m_pDefault_GS;
		ID3D11Buffer*			m_pHUDConstantBuffer;

	public:
		// Constructors
		C2DContext();

		// Destructors
		virtual ~C2DContext();

		// Operators

		// Accessors

		// Mutators

		// Override Methods
		virtual void Initialize(CRenderManager* _pRenderer,

			ID3D11Device*			pDevice,
			D3D11_VIEWPORT*			pViewport,
			ID3D11DeviceContext*	m_pContext,

			ID3D11DepthStencilView*	pDSV,
			ID3D11RenderTargetView*	pRTV) override;

		virtual void Shutdown() override;

		virtual void Render(CRenderComponent* _pComponent) override;
		virtual void RemapBuffers(CRenderComponent* _pRenderComponent) override;

	};

}
#endif
