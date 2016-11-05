#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include <thread>
#include <mutex>

#include <unordered_map>
#include <string>

using namespace DirectX;
using namespace std;

namespace FTJ
{
	typedef unsigned int uint;

	class CMesh;

	class Texture {
	public:
		ID3D11ShaderResourceView* m_pSRV = NULL;
	};

	struct ThreadMeshData
	{
		std::string nameKey;//mesh name.extension

		std::string path;
		ID3D11Device* device;
		std::mutex* mutex;

		std::unordered_map<string, CMesh*>* Meshes;
	};

	struct ThreadTextureData
	{
		std::wstring nameKey;//texture name.extension

		std::wstring path;
		ID3D11Device* device;
		std::mutex* mutex;

		std::unordered_map<wstring, Texture>* Textures;
	};

	class CAssetManager
	{
		ID3D11Device*	m_pDevice;
		std::mutex mutex;

		std::unordered_map<string, CMesh*> m_Meshes;
		std::unordered_map<wstring, Texture> m_Textures;

		void LoadModels();
	public:
		~CAssetManager();
		CAssetManager();

		void Initialize(ID3D11Device*	_device);
		void Terminate();


		CMesh* GetMesh(std::string _meshName);
		Texture* GetTexture(std::wstring _textureName);

	private:
	};

}