#include "ParticleContext.h"
#include "../FTJ_Console.h"

#include "..\..\Assets\Shaders\CompiledShaders\DefaultParticle_VS.csh"
#include "..\..\Assets\Shaders\CompiledShaders\DefaultParticle_PS.csh"
#include "RenderComponent.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "../Asset/AssetManager.h"
#include "FTJ_RenderManager.h"

namespace FTJ
{
	CParticleContext::CParticleContext(){}

	CParticleContext::~CParticleContext()
	{
		Shutdown();
	}

	void CParticleContext::Initialize(CRenderManager* _pRenderer,

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
		if (!DXCall(m_pDevice->CreateVertexShader(DefaultParticle_VS, sizeof(DefaultParticle_VS), nullptr, &m_pDefault_VS)))
			Log("Failed to create Particle Vertex Shader");
		DXName(m_pDefault_VS, "Particle Vertex Shader");

		if (!DXCall(m_pDevice->CreatePixelShader(DefaultParticle_PS, sizeof(DefaultParticle_PS), nullptr, &m_pDefault_PS)))
			Log("Failed to create Particle Pixel Shader");
		DXName(m_pDefault_VS, "Particle Pixel Shader");

		// Declare Input Layout Description
		D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMALS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		UINT numElementsInLayout = ARRAYSIZE(InputLayoutDesc);

		// Create the Input Layout with its Description
		if (!DXCall(m_pDevice->CreateInputLayout(InputLayoutDesc, numElementsInLayout, DefaultParticle_VS, sizeof(DefaultParticle_VS), &m_pInputLayout)))
			Log("Failed creating Particle Input Layout");
		DXName(m_pInputLayout, "Particle Input Layout");

		// Make the objects constant buffer
		D3D11_BUFFER_DESC constantBufferDescription;
		ZeroMemory(&constantBufferDescription, sizeof(D3D11_BUFFER_DESC));
		constantBufferDescription.ByteWidth = sizeof(T3DVSConstantBuffer);
		constantBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
		constantBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		if (!DXCall(m_pDevice->CreateBuffer(&constantBufferDescription, nullptr, &m_pObjectConstantBuffer)))
			Log("Failed creating Particle Object Constant Buffer");


		if (!DXCall(m_pDevice->CreateBuffer(&constantBufferDescription, nullptr, &m_pPSDirectionalLightsCB)))
			Log("Failed creating Directional Light Constant Buffer");
		DXName(m_pPSDirectionalLightsCB, "Directional Light Constant Buffer");
	}

	void CParticleContext::Render(CRenderComponent* _pRenderComponent)
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

	void CParticleContext::Shutdown()
	{
		CBaseContext::Shutdown();
		SAFE_RELEASE(m_pBlendState);
		//SAFE_RELEASE(m_pPSDirectionalLightsCB);
	}

	void CParticleContext::RemapBuffers(CRenderComponent* _pComponent)
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