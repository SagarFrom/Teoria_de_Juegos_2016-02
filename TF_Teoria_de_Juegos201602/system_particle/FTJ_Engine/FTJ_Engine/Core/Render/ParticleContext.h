#ifndef CPARTICLE_CONTEXT_H
#define CPARTICLE_CONTEXT_H

#include "BaseContext.h"

namespace FTJ
{

	class CRenderComponent;

	class CParticleContext : public CBaseContext
	{

	private:

		// Members
		ID3D11BlendState*		m_pBlendState;
		ID3D11Buffer*			m_pSceneCBuffer;
		ID3D11Buffer*			m_pPSDirectionalLightsCB;

		// Helper Functions 

	public:
		// Constructors
		CParticleContext();

		// Destructors
		virtual ~CParticleContext();

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