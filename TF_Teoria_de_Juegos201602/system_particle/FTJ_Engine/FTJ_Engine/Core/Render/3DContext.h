/***********************************************
* Filename:  		3DContext.h
* Date:      		10/7/15
* Mod. Date: 		10/7/15
* Mod. Initials:	DKR
* Author:    		David Rasmussen
* Purpose:   		A render context to render
					3D objects
************************************************/

#ifndef C3D_CONTEXT_H
#define C3D_CONTEXT_H

#include "BaseContext.h"

namespace FTJ
{

	class CRenderComponent;

	class C3DContext : public CBaseContext
	{

	private:

		// Members
		//ID3D11BlendState*		m_pBlendState;
		ID3D11Buffer*			m_pSceneCBuffer;
		ID3D11Buffer*			m_pPSDirectionalLightsCB;

		// Helper Functions 

	public:
		// Constructors
		C3DContext();

		// Destructors
		virtual ~C3DContext();

		// Operators

		// Accessors

		// Mutators

		// Methods
		virtual void Initialize(CRenderManager* _pRenderer,

			ID3D11Device*			_pDevice,
			D3D11_VIEWPORT*			_pViewport,
			ID3D11DeviceContext*	_pContext,

			ID3D11DepthStencilView*	_pDSV,
			ID3D11RenderTargetView*	_pRTV,
			
			ID3D11Buffer*			_pSceneCBuffer);

		virtual void Shutdown() override;

		virtual void Render(CRenderComponent* _pRenderComponent) override;
		virtual void RemapBuffers(CRenderComponent* _pRenderComponent) override;
	};
}
#endif