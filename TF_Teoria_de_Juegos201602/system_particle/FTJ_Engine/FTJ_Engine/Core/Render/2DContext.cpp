/***********************************************
* Filename:  		2DContext.cpp
* Date:      		10/7/15
* Mod. Date: 		10/7/15
* Mod. Initials:	DKR
* Author:    		Alexis Sotelo, Credits to David Rasmussen
* Purpose:   		A render context to render
2D objects
************************************************/
#include "../FTJ_Console.h"

#include "2DContext.h"
#include "RenderComponent.h"
#include "SpriteRenderer.h"

#include "..\..\Assets\Shaders\CompiledShaders\Default2D_VS.csh"
#include "..\..\Assets\Shaders\CompiledShaders\Default2D_GS.csh"
#include "..\..\Assets\Shaders\CompiledShaders\Default2D_PS.csh"

#include "Mesh.h"
#include "../Asset/AssetManager.h"
#include "FTJ_RenderManager.h"

using namespace DirectX;

namespace FTJ
{


	C2DContext::C2DContext() {}

	C2DContext::~C2DContext()
	{
		Shutdown();
	}

	void C2DContext::Initialize(CRenderManager* _pRenderer,

		ID3D11Device*			pDevice,
		D3D11_VIEWPORT*			pViewport,
		ID3D11DeviceContext*	m_pContext,

		ID3D11DepthStencilView*	pDSV,
		ID3D11RenderTargetView*	pRTV)
	{
		CBaseContext::Initialize(_pRenderer, pDevice, pViewport, m_pContext, pDSV, pRTV);

		// Create Shaders
		if (!DXCall(m_pDevice->CreateVertexShader(Default2D_VS, sizeof(Default2D_VS), nullptr, &m_pDefault_VS)))
			Log("Failed to create 2D Vertex Shader");
		DXName(m_pDefault_VS, "2D Vertex Shader");

		if (!DXCall(m_pDevice->CreateGeometryShader(Default2D_GS, sizeof(Default2D_GS), nullptr, &m_pDefault_GS)))
			Log("Failed to create 2D Geometry Shader");
		DXName(m_pDefault_GS, "2D Geometry Shader");

		if (!DXCall(m_pDevice->CreatePixelShader(Default2D_PS, sizeof(Default2D_PS), nullptr, &m_pDefault_PS)))
			Log("Failed to create 2D Pixel Shader");
		DXName(m_pDefault_PS, "2D Pixel Shader");

		// Declare Input Layout Description
		D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		UINT numElementsInLayout = ARRAYSIZE(InputLayoutDesc);

		// Create the Input Layout with its Description
		if (!DXCall(m_pDevice->CreateInputLayout(InputLayoutDesc, 1, Default2D_VS, sizeof(Default2D_VS), &m_pInputLayout)))
			Log("Failed creating 2D Input Layout");
		DXName(m_pInputLayout, "2D Input Layout");

		// Make the objects constant buffer
		D3D11_BUFFER_DESC constantBufferDescription;
		ZeroMemory(&constantBufferDescription, sizeof(D3D11_BUFFER_DESC));
		constantBufferDescription.ByteWidth = sizeof(T2DInfo);
		constantBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
		constantBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		if (!DXCall(m_pDevice->CreateBuffer(&constantBufferDescription, nullptr, &m_pHUDConstantBuffer)))
			Log("Failed creating HUD Constant Buffer");

		DXName(m_pHUDConstantBuffer, "HUD Constant Buffer");

		D3D11_BLEND_DESC blendStateDescription;
		ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));
		blendStateDescription.RenderTarget[0].BlendEnable = TRUE;

		blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		if (!DXCall(m_pDevice->CreateBlendState(&blendStateDescription, &m_pBlendState)))
			Log("Failed creating Blend State");

		DXName(m_pBlendState, "Transparent 2D Blend State");
	}

	void C2DContext::Render(CRenderComponent* _cRenderComponent)
	{
		//if (_cRenderComponent->GetContextType() == eContextType::Object2D)
		{
			CSpriteRenderer* pSprite = dynamic_cast<CSpriteRenderer*>(_cRenderComponent);

			ID3D11Buffer *GSConstantBuffers[] = { m_pHUDConstantBuffer };
			UINT numConstantBuffers = ARRAYSIZE(GSConstantBuffers);

			m_pContext->GSSetConstantBuffers(0, numConstantBuffers, GSConstantBuffers);

			// Set the Shaders
			m_pContext->VSSetShader(m_pDefault_VS, nullptr, 0);
			m_pContext->GSSetShader(m_pDefault_GS, nullptr, 0);
			m_pContext->PSSetShader(m_pDefault_PS, nullptr, 0);

			// Set the object's vertex buffer
			//std::string textureID = _cRenderComponent->GetTexture();
			ID3D11Buffer *VertexBuffers[] = { pSprite->GetVertexBuffer() };
			ID3D11ShaderResourceView *SRVs[] = { pSprite->GetTexture()->m_pSRV };
			UINT	numVertexBuffers = ARRAYSIZE(VertexBuffers),
				numSRVs = ARRAYSIZE(SRVs),
				pStrides[] = { sizeof(MeshVertex) },
				pOffsets[] = { 0 };
			m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			m_pContext->IASetVertexBuffers(0, numVertexBuffers, VertexBuffers, pStrides, pOffsets);
			//m_pDeferredContext->IASetIndexBuffer(pSprite->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

			float			Blend[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			m_pContext->OMSetBlendState(m_pBlendState, Blend, 0xffffffff);

			m_pContext->PSSetShaderResources(0, numSRVs, SRVs);

			ID3D11SamplerState* SamplerStates[] = { m_pRenderManager->GetClampSampler() };
			UINT	numSamplers = ARRAYSIZE(SamplerStates);
			m_pContext->PSSetSamplers(0, numSamplers, SamplerStates);

			// Draw
			m_pContext->Draw(1, 0);
		}
	}

	void C2DContext::RemapBuffers(CRenderComponent* _pComponent)
	{
		CSpriteRenderer* pSprite = dynamic_cast<CSpriteRenderer*>(_pComponent);

		D3D11_MAPPED_SUBRESOURCE mapSubresource;
		ZeroMemory(&mapSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));
		m_pContext->Map(m_pHUDConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapSubresource);
		memcpy(mapSubresource.pData, &(pSprite->Get2DInfo().tInfo), sizeof(T2DInfo));
		m_pContext->Unmap(m_pHUDConstantBuffer, 0);
	}

	void C2DContext::Shutdown()
	{

		CBaseContext::Shutdown();
		//SAFE_RELEASE(m_pGeometryShader);
		SAFE_RELEASE(m_pHUDConstantBuffer);
		SAFE_RELEASE(m_pBlendState);

	}
}