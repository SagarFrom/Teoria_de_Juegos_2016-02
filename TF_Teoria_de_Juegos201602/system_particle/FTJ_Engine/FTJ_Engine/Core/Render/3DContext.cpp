/***********************************************
* Filename:  		3DContext.cpp
* Date:      		10/7/15
* Mod. Date: 		10/7/15
* Mod. Initials:	DKR
* Purpose:   		A render context to render
3D objects
************************************************/

#include "3DContext.h"
#include "../FTJ_Console.h"

#include "..\..\Assets\Shaders\CompiledShaders\Default3D_VS.csh"
#include "..\..\Assets\Shaders\CompiledShaders\Default3D_PS.csh"
#include "RenderComponent.h"
#include "MeshRenderer.h"

//#include "../ObjectEngine/GameObject.h"
#include "Mesh.h"
#include "../Asset/AssetManager.h"
#include "FTJ_RenderManager.h"

namespace FTJ
{

	C3DContext::C3DContext()
	{

	}

	C3DContext::~C3DContext()
	{
		Shutdown();
	}

	void C3DContext::Initialize(CRenderManager* _pRenderer,

		ID3D11Device*			pDevice,
		D3D11_VIEWPORT*			pViewport,
		ID3D11DeviceContext*	m_pContext,

		ID3D11DepthStencilView*	pDSV,
		ID3D11RenderTargetView*	pRTV,
		ID3D11Buffer*			_pSceneCBuffer)
	{
		CBaseContext::Initialize(_pRenderer, pDevice, pViewport, m_pContext, pDSV, pRTV);

		m_pSceneCBuffer = _pSceneCBuffer;

		// Create Shaders
		if (!DXCall(m_pDevice->CreateVertexShader(Default3D_VS, sizeof(Default3D_VS), nullptr, &m_pDefault_VS)))
			Log("Failed to create 3D Vertex Shader");
		DXName(m_pDefault_VS, "3D Vertex Shader");

		if (!DXCall(m_pDevice->CreatePixelShader(Default3D_PS, sizeof(Default3D_PS), nullptr, &m_pDefault_PS)))
			Log("Failed to create 3D Pixel Shader");
		DXName(m_pDefault_PS, "3D Pixel Shader");

		// Declare Input Layout Description
		D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMALS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		UINT numElementsInLayout = ARRAYSIZE(InputLayoutDesc);

		// Create the Input Layout with its Description
		if (!DXCall(m_pDevice->CreateInputLayout(InputLayoutDesc, numElementsInLayout, Default3D_VS, sizeof(Default3D_VS), &m_pInputLayout)))
			Log("Failed creating 3D Input Layout");
		DXName(m_pInputLayout, "3D Input Layout");

		// Make the objects constant buffer
		D3D11_BUFFER_DESC constantBufferDescription;
		ZeroMemory(&constantBufferDescription, sizeof(D3D11_BUFFER_DESC));
		constantBufferDescription.ByteWidth = sizeof(T3DVSConstantBuffer);
		constantBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
		constantBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		if (!DXCall(m_pDevice->CreateBuffer(&constantBufferDescription, nullptr, &m_pObjectConstantBuffer)))
			Log("Failed creating 3D Object Constant Buffer");


		if (!DXCall(m_pDevice->CreateBuffer(&constantBufferDescription, nullptr, &m_pPSDirectionalLightsCB)))
			Log("Failed creating Directional Light Constant Buffer");
		DXName(m_pPSDirectionalLightsCB, "Directional Light Constant Buffer");
	}

	void C3DContext::Render(CRenderComponent* _pRenderComponent)
	{
		MeshRenderer* pMeshRender = dynamic_cast<MeshRenderer*>(_pRenderComponent);

		RemapBuffers(pMeshRender);

		ID3D11Buffer *VSConstantBuffers[] = { m_pObjectConstantBuffer, m_pSceneCBuffer/*m_pPSDirectionalLightsCB*/ };
		UINT numConstantBuffers = ARRAYSIZE(VSConstantBuffers);

		m_pContext->VSSetConstantBuffers(0, numConstantBuffers, VSConstantBuffers);

		//ID3D11Buffer *PSConstantBuffers[] = {  m_pPSDirectionalLightsCB };
		//UINT numPSConstantBuffers = ARRAYSIZE(PSConstantBuffers);

		//m_pContext->PSSetConstantBuffers(0, numPSConstantBuffers, PSConstantBuffers);

		// Set the Shaders
		m_pContext->VSSetShader(m_pDefault_VS, nullptr, 0);
		m_pContext->GSSetShader(nullptr, nullptr, 0);
		m_pContext->PSSetShader(m_pDefault_PS, nullptr, 0);

		// Set the object's vertex buffer
		//std::string& textureID = _cRenderComponent->GetTextureID();
		ID3D11Buffer *VertexBuffers[] = { pMeshRender->GetMesh()->GetVertexBuffer() };
		UINT	numVertexBuffers = ARRAYSIZE(VertexBuffers),
			pStrides[] = { sizeof(MeshVertex) },
			pOffsets[] = { 0 };
		m_pContext->IASetVertexBuffers(0, numVertexBuffers, VertexBuffers, pStrides, pOffsets);
		m_pContext->IASetIndexBuffer(pMeshRender->GetMesh()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

		m_pContext->IASetInputLayout(m_pInputLayout);
		//{
		//	m_pContext->OMSetDepthStencilState(m_pDSState, 0);
		//	m_pContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
		//}

		ID3D11ShaderResourceView *SRVs[] = { pMeshRender->GetTexture()->m_pSRV };
		m_pContext->PSSetShaderResources(0, ARRAYSIZE(SRVs), SRVs);


		ID3D11SamplerState* SamplerStates[] = { m_pRenderManager->GetClampSampler(), m_pRenderManager->GetWrapSampler() };
		UINT	numSamplers = ARRAYSIZE(SamplerStates);
		m_pContext->PSSetSamplers(0, numSamplers, SamplerStates);

		// Draw
		m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_pContext->DrawIndexed(pMeshRender->GetMesh()->GetVerticesCount(), 0, 0);
	}

	void C3DContext::Shutdown()
	{
		CBaseContext::Shutdown();
		//SAFE_RELEASE(m_pBlendState);
		SAFE_RELEASE(m_pPSDirectionalLightsCB);
	}

	void C3DContext::RemapBuffers(CRenderComponent* _pComponent)
	{
		D3D11_MAPPED_SUBRESOURCE mapSubresource;
		MeshRenderer* pMeshRender = dynamic_cast<MeshRenderer*>(_pComponent);

		ZeroMemory(&mapSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));
		m_pContext->Map(m_pObjectConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapSubresource);
		memcpy(mapSubresource.pData, &pMeshRender->Get3DInfo().tConstantBufferInfo, sizeof(T3DVSConstantBuffer));
		m_pContext->Unmap(m_pObjectConstantBuffer, 0);

		//ZeroMemory(&mapSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));
		//m_pContext->Map(m_pPSDirectionalLightsCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapSubresource);
		//memcpy(mapSubresource.pData, GetLightManager()->GetDirectionalLightBuffer(), sizeof(TDirectionalLightConstantBuffer));
		//m_pContext->Unmap(m_pPSDirectionalLightsCB, 0);
	}
}