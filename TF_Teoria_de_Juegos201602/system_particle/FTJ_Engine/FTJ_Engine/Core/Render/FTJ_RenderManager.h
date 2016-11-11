#pragma once

#include "FTJ_ShaderDefinitions.h"

#include <unordered_map>
#include <vector>
#include <cstring>

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

#include "2DContext.h"
#include "3DContext.h"

#include "../GameScenes/IGameScene.h"

namespace FTJ
{
	//assets
	class CAssetManager;
	class CMesh;
	class Texture;

	//components
	class CRenderComponent;
	class CCamera;

	//scene
	//class IGameScene;

	struct TRenderScene
	{
		//skybox*	skybox;
		//std::vector<CRenderComponent*>	m_vBackgroundNodes;
		std::vector<CRenderComponent*>		m_vPlayableNodes;
		//std::vector<CRenderComponent*>		m_vPostProcessNodes;
		std::vector<CRenderComponent*>		m_vHUDNodes;

		CCamera* m_pMainCamera;
		//PxVec4  v4BackBufferColor;
	};

	class CRenderManager
	{

		static CRenderManager* m_pInstance;

		CRenderManager();
		~CRenderManager();

		CAssetManager*				m_pAssetManager;

		std::unordered_map<GAME_SCENE, TRenderScene>	m_mRenderableScenes;


		ID3D11Device*					device;
		ID3D11DeviceContext*			context;
		ID3D11RenderTargetView*			RTV;
		IDXGISwapChain*					swapchain;
		D3D11_VIEWPORT*					viewports;

		ID3D11Texture2D*				depthBuffer;
		ID3D11DepthStencilView*			DSV;

		//CBUFFERS
		ID3D11Buffer*					m_pSceneCBuffer;

		//SAMPLERS
		ID3D11SamplerState		*m_pClampSampler;
		ID3D11SamplerState		*m_pWrapSampler;

		//CONTEXT
		C3DContext		m_3DContext;
		C2DContext		m_2DContext;

		int m_nWindowWidth, m_nWindowHeight;

	public:

		static CRenderManager* GetInstance();

		void Initialize(HWND hWnd, int _windowWidth, int _windowHeight);
		void Terminate();

		void Render(IGameScene* _sceneToRender);

		void RegisterSceneCamera(IGameScene* _pGameScene, CCamera* _pCamera);

		void RegisterRenderableObject(CRenderComponent* _obj, IGameScene* _pGameScene);
		void UnregisterRenderableObject(CRenderComponent* _obj, IGameScene* _pGameScene);

		CMesh* FindMesh(std::string _meshName);
		Texture* FindTexture(std::wstring _textureName);

		inline CAssetManager* const GetAssetManager() { return m_pAssetManager; }
		
		inline ID3D11SamplerState* GetWrapSampler() { return m_pWrapSampler; }
		inline ID3D11SamplerState* GetClampSampler() { return m_pClampSampler; }

		inline int GetWindowWidth() const { return m_nWindowWidth; }
		inline int GetWindowHeight() const { return m_nWindowHeight; }

	private:

		void RenderComponent(CRenderComponent* _pRenderComponent);
	};

}