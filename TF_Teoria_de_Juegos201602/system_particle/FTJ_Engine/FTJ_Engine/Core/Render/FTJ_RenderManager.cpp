#include "FTJ_RenderManager.h"

//ASSETS
#include "Mesh.h"
#include "../Asset/AssetManager.h"

//Entity RELATED
#include "../Entity/EntityManager.h"
#include "../Entity/GameObject.h"
#include "../Entity/Transform.h"

#include "RenderComponent.h"
#include "Camera.h"

#include "../FTJ_Console.h"

namespace FTJ
{

	CRenderManager* CRenderManager::m_pInstance = nullptr;

	CRenderManager* CRenderManager::GetInstance()
	{
		if (m_pInstance == nullptr)
			m_pInstance = new CRenderManager;
		return m_pInstance;
	}

	CRenderManager::CRenderManager(){}
	CRenderManager::~CRenderManager(){}

	void CRenderManager::Initialize(HWND hWnd, int _windowWidth, int _windowHeight)
	{
		m_nWindowWidth	= _windowWidth;
		m_nWindowHeight = _windowHeight;

		D3D11_BUFFER_DESC bufferDesc;
		D3D11_SUBRESOURCE_DATA pInitial;

#pragma region INIT_DEVICE_SWAPCHAIN
		// TODO: PART 1 STEP 3a
		const D3D_FEATURE_LEVEL Levels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};

		D3D_FEATURE_LEVEL level;
		DXGI_SWAP_CHAIN_DESC desc;
		memset(&desc, 0, sizeof(DXGI_SWAP_CHAIN_DESC));
		desc.OutputWindow = hWnd;
		desc.Windowed = true;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		desc.BufferDesc.Width = _windowWidth;
		desc.BufferDesc.Height = _windowHeight;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferCount = 1;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		// TODO: PART 1 STEP 3b
		HRESULT hr = D3D11CreateDeviceAndSwapChain(
			NULL, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG,
			Levels, 3, D3D11_SDK_VERSION, &desc, &swapchain, &device, &level, &context);

		// TODO: PART 1 STEP 4
		ID3D11Texture2D* pBuffer;
		hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);

		hr = device->CreateRenderTargetView(pBuffer, NULL, &RTV);
		pBuffer->Release();//**

		viewports = new D3D11_VIEWPORT[1];
		viewports[0].TopLeftX =
		viewports[0].TopLeftY = 0;
		viewports[0].Width = (float)_windowWidth;
		viewports[0].Height = (float)_windowHeight;
		viewports[0].MinDepth = 0;
		viewports[0].MaxDepth = 1;

		context->RSSetViewports(1, &viewports[0]);

#pragma endregion

		//CONSTANT BUFFER SCENE/ CAMERA
#if 1
		ShaderDefinitions::SCENE_BUFFER tSceneBufferData;

		float nearPlane = 0.1f;
		float farPlane = 500.f;
		float FOV = 65.f / 180.f* 3.1416f;
		float aspectRatio = (float)_windowWidth / _windowHeight;

		float YScale = 1 / (tan(0.6f * FOV));
		float XScale = YScale * aspectRatio;

		tSceneBufferData.projMatrix = 
			XMMatrixPerspectiveFovLH(FOV, aspectRatio, nearPlane, farPlane);

		memset(&bufferDesc, 0, sizeof(D3D11_BUFFER_DESC));
		bufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;// D3D11_USAGE::D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//NULL;
		bufferDesc.ByteWidth = sizeof(ShaderDefinitions::SCENE_BUFFER);
		bufferDesc.StructureByteStride = 0;

		memset(&pInitial, 0, sizeof(D3D11_SUBRESOURCE_DATA));
		pInitial.pSysMem = &tSceneBufferData;
		pInitial.SysMemPitch = 0; //sizeof(SEND_TO_VRAM);
		pInitial.SysMemSlicePitch = 0;// 1;

		hr = device->CreateBuffer(&bufferDesc, &pInitial, &m_pSceneCBuffer);

#endif

#pragma region DEPTH BUFFER
		D3D11_TEXTURE2D_DESC descDepth;
		memset(&descDepth, 0, sizeof(D3D11_TEXTURE2D_DESC));
		descDepth.Width = _windowWidth;
		descDepth.Height = _windowHeight;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		memset(&descDSV, 0, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		hr = device->CreateTexture2D(&descDepth, NULL, &depthBuffer);
		hr = device->CreateDepthStencilView(depthBuffer, &descDSV, &DSV);

		context->OMSetRenderTargets(1, &RTV, DSV);
#pragma endregion

#pragma region SAMPLER_STATE
		// Make a sampler state for the pixel shader
		D3D11_SAMPLER_DESC SamplerStateDesc;
		ZeroMemory(&SamplerStateDesc, sizeof(D3D11_SAMPLER_DESC));
		SamplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		SamplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerStateDesc.MaxAnisotropy = 1;
		SamplerStateDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS; //D3D11_COMPARISON_NEVER;
		SamplerStateDesc.MaxLOD = D3D11_FLOAT32_MAX;

		if (!DXCall(device->CreateSamplerState(&SamplerStateDesc, &m_pWrapSampler)))
			Log("Failed to create WrapSamplerState");
		DXName(m_pWrapSampler, "WrapSampler");

		ZeroMemory(&SamplerStateDesc, sizeof(D3D11_SAMPLER_DESC));
		SamplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		SamplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		SamplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		SamplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		SamplerStateDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		SamplerStateDesc.MaxLOD = D3D11_FLOAT32_MAX;

		if (!DXCall(device->CreateSamplerState(&SamplerStateDesc, &m_pClampSampler)))
			Log("Failed to create ClampSamplerState");
		DXName(m_pClampSampler, "ClampSampler");
#pragma endregion

		m_pAssetManager = new CAssetManager();
		m_pAssetManager->Initialize(device);

		m_3DContext.Initialize(this, device, viewports, context, DSV, RTV, m_pSceneCBuffer);
		m_2DContext.Initialize(this, device, viewports, context, DSV, RTV);

		m_mRenderableScenes[GAME_SCENE::SCENE_MAIN] = TRenderScene();
	}

	void CRenderManager::Terminate()
	{
		m_pAssetManager->Terminate();
		delete m_pAssetManager;

		m_pSceneCBuffer->Release();

		depthBuffer->Release();
		DSV->Release();
		m_pClampSampler->Release();
		m_pWrapSampler->Release();


		delete[]viewports;

		swapchain->Release();
		RTV->Release();
		context->Release();
		device->Release();
	}

	void CRenderManager::Render(IGameScene* _sceneToRender)
	{
		TRenderScene& renderScene = m_mRenderableScenes[_sceneToRender->GetGameScene()];

		float clearColor[4] = { 0.2f, 0.2f, 0.2f, 1.f };
		context->ClearRenderTargetView(RTV, clearColor);
		context->ClearDepthStencilView(DSV, D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH, 1.0f, 0);

		//map camera
		CCamera* currentCamera = renderScene.m_pMainCamera;
		currentCamera->CalculateMVP();

		D3D11_MAPPED_SUBRESOURCE mapSubresource;
		ZeroMemory(&mapSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));
		context->Map(m_pSceneCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapSubresource);
		memcpy(mapSubresource.pData, currentCamera->GetSceneBuffer() , sizeof(SCENE_BUFFER));
		context->Unmap(m_pSceneCBuffer, 0);
		//end- map camera buffer

		std::vector<CRenderComponent*>& m_vComponentsToRender 
			= renderScene.m_vPlayableNodes;

		int nCompsToRender = m_vComponentsToRender.size();
		for (int i = 0; i < nCompsToRender; i++)
		{
			RenderComponent(m_vComponentsToRender[i]);
		}

		//PRESENT
		swapchain->Present(0, 0);
	}

	void CRenderManager::RenderComponent(CRenderComponent* _pRenderComponent)
	{
		if (_pRenderComponent->IsEnabled() == false)
			return;

		switch (_pRenderComponent->GetRenderContext())
		{
		case RENDER_CONTEXT::CONTEXT_2D:
			m_2DContext.Render(_pRenderComponent);
			break;

		case RENDER_CONTEXT::CONTEXT_3D:
			m_3DContext.Render(_pRenderComponent);
			break;
		}
	}

	void CRenderManager::RegisterSceneCamera(IGameScene* _pGameScene, CCamera* _pCamera)
	{
		m_mRenderableScenes[_pGameScene->GetGameScene()].m_pMainCamera = _pCamera;
	}

	void CRenderManager::RegisterRenderableObject(
		CRenderComponent* _renderComponent, IGameScene* _pGameScene)
	{
		TRenderScene& renderScene = m_mRenderableScenes[_pGameScene->GetGameScene()];

		switch (_renderComponent->GetDepth())
		{
		case eDepth::HUD:
			renderScene.m_vHUDNodes.push_back(_renderComponent);
			break;
		case eDepth::Playable:
			renderScene.m_vPlayableNodes.push_back(_renderComponent);
			break;
		default:
			//Log("ERROR: Unexpected case in CRenderManager::AddRenderNode");
			break;
		}
	}

	void CRenderManager::UnregisterRenderableObject(CRenderComponent* _obj, IGameScene* _pGameScene)
	{
		TRenderScene& renderScene = m_mRenderableScenes[_pGameScene->GetGameScene()];

		//m_vRenderableObjects.erase(_obj);
	}

	CMesh* CRenderManager::FindMesh(std::string _meshName)
	{
		return m_pAssetManager->GetMesh(_meshName);
	}
	Texture* CRenderManager::FindTexture(std::wstring _textureName)
	{
		return m_pAssetManager->GetTexture(_textureName);
	}
}