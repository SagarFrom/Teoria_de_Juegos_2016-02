/***********************************************
* Filename:  		CBaseContext.h
* Date:      		10/7/15
* Mod. Date: 		10/7/15
* Author:    		Alexis Sotelo, credits to David Rasmussen
* Purpose:   		An abstract base class to 
					derive all contexts from
************************************************/


#ifndef CBASE_CONTEXT_H
#define CBASE_CONTEXT_H
// Include DirectX11 and link its library
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

// Include for smart com pointers
#include <atlbase.h>

#include <vector>
using namespace std;

namespace FTJ
{

	class CRenderComponent;
	class CAssetManager;
	//class CLightManager;
	class CRenderManager;

	class CBaseContext
	{

	private:


	protected:
		CRenderManager					*m_pRenderManager;

		// Members
		ID3D11Device			*m_pDevice;
		D3D11_VIEWPORT			*m_tViewport;
		ID3D11DeviceContext		*m_pContext;

		ID3D11RenderTargetView	*m_pScreenRTV;
		ID3D11DepthStencilView	*m_pScreenDSV;

		ID3D11Buffer			*m_pObjectConstantBuffer;//world matrix
		//ID3D11DepthStencilState	*m_pDSState;

		//Context specific, but present in all context
		ID3D11InputLayout		*m_pInputLayout;
		ID3D11VertexShader		*m_pDefault_VS;
		ID3D11PixelShader		*m_pDefault_PS;

		// Helper Functions
		//CLightManager* GetLightManager();
		CAssetManager* GetAssetManager();

	public:

		// Constructors
		CBaseContext();

		// Destructors
		virtual ~CBaseContext();

		// Operators

		// Accessors

		// Mutators

		// Methods
		virtual void Initialize(
			CRenderManager* _pRenderer,

			ID3D11Device*			pDevice,
			D3D11_VIEWPORT			*pViewport,
			ID3D11DeviceContext*	m_pContext,

			ID3D11DepthStencilView*	pDSV,
			ID3D11RenderTargetView*	pRTV
		);

		virtual void Shutdown();

		virtual void Render(CRenderComponent* _pRenderComponent) = 0;
		virtual void RemapBuffers(CRenderComponent* _pRenderComponent) = 0;

	};

}
#endif
