/***********************************************
* Filename:  		CBaseContext.cpp
* Date:      		10/7/15
* Mod. Date: 		10/7/15
* Mod. Initials:	DKR
* Author:    		David Rasmussen
* Purpose:   		An abstract base class to
derive all contexts from
************************************************/
#include "BaseContext.h"

#include "FTJ_RenderManager.h"
#include "../Asset/AssetManager.h"

#include "RenderComponent.h"

#include "../FTJ_Console.h"

#include "Mesh.h"

namespace FTJ
{


	CBaseContext::CBaseContext()
	{

	}

	CBaseContext::~CBaseContext()
	{
		Shutdown();
	}

	void CBaseContext::Shutdown()
	{
		SAFE_RELEASE(m_pInputLayout);
		SAFE_RELEASE(m_pDefault_VS);
		SAFE_RELEASE(m_pDefault_PS);
	}

	void CBaseContext::Initialize(
		CRenderManager* _pRenderer,

		ID3D11Device*			_pDevice,
		D3D11_VIEWPORT*			_pViewport,
		ID3D11DeviceContext*	_pContext,

		ID3D11DepthStencilView*	_pDSV,
		ID3D11RenderTargetView*	_pRTV)
	{
		m_pRenderManager = _pRenderer;

		m_pDevice = _pDevice;
		m_tViewport = _pViewport;
		m_pScreenDSV = _pDSV;
		m_pScreenRTV = _pRTV;

		m_pContext = _pContext;
	}

	//CLightManager* CBaseContext::GetLightManager()
	//{
	//	return m_pRenderManager->GetLightManager();
	//}

	CAssetManager* CBaseContext::GetAssetManager()
	{
		return m_pRenderManager->GetAssetManager();
	}
}