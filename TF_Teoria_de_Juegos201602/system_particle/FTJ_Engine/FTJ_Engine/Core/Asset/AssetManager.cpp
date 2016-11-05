#include "AssetManager.h"
#include "../FTJ_Console.h"

#include "DDSTextureLoader.h"
#include "TinyXML\tinyxml.h"

#include "../Render/Mesh.h"

#include <fstream>

#define TEXTURE_PATH L"Assets\\Textures\\"
#define MODEL_PATH "Assets\\Models\\"

namespace FTJ
{
	CAssetManager::CAssetManager() {}
	CAssetManager::~CAssetManager() {}

	void LoadMeshAsync(ThreadMeshData* data)
	{
		data->mutex->lock();

		CMesh* mesh;
		CMesh::LoadMesh(data->path.c_str(), data->device, &mesh);
		if (mesh)
		{
			cout << "Success - loaded: " << data->path << endl;
			(*data->Meshes)[data->nameKey] = mesh;
		}

		data->mutex->unlock();
	}

	void LoadTextureAsync(ThreadTextureData* data)
	{
		data->mutex->lock();

		Texture texture;
		CreateDDSTextureFromFile(data->device, data->path.c_str(), NULL, &texture.m_pSRV);

		if (texture.m_pSRV)
		{
			cout << "Success - loaded: " << data->nameKey.c_str() << endl;
			(*data->Textures)[data->nameKey] = texture;
		}

		data->mutex->unlock();

	}

	void CAssetManager::Initialize(ID3D11Device*	_device)
	{
		m_pDevice = _device;

		//LOAD OBJS - multithreaded
		LoadModels();

		//LOAD TEXTURES - multithreaded
		{
			std::vector<std::thread> threads;
			const wchar_t* textureNames[] =
			{
				L"Default.dds",
				L"concrete_diff.dds",
				L"wall.dds",
				L"skybox.dds"
			};

			int textureAmount = sizeof(textureNames) / 4;
			ThreadTextureData threadTextureData;
			threadTextureData.device = m_pDevice;
			threadTextureData.Textures = &m_Textures;
			threadTextureData.mutex = &mutex;

			wstring filePath = TEXTURE_PATH;
			for (int i = 0; i < textureAmount; i++)
			{
				threadTextureData.nameKey = textureNames[i];
				threadTextureData.path = (filePath + textureNames[i]);
				threads.push_back(thread(LoadTextureAsync, &threadTextureData));

				if (threads[i].joinable())
					threads[i].join();
			}
		}

		FTJ::Console::Print("Asset Manager Initialized");
	}

	void CAssetManager::LoadModels()
	{
		string filePath = "Assets/Models/ModelNames.xml";

		TiXmlDocument document;
		if (document.LoadFile(filePath.c_str()) == false)
		{
			FTJ::Console::Print("Error loading: ModelNames.xml");
			return;
		}

		std::vector<string> modelNames;

		TiXmlElement* root = document.RootElement();
		TiXmlElement* currentElement = root->FirstChildElement();

		while (currentElement != nullptr)
		{
			modelNames.push_back(currentElement->Attribute("Name"));

			currentElement = currentElement->NextSiblingElement();
		}

		int meshAmount = modelNames.size();
		std::vector<std::thread> threads;

		ThreadMeshData threadData;
		threadData.device = m_pDevice;
		threadData.Meshes = &m_Meshes;
		threadData.mutex = &mutex;

		filePath = MODEL_PATH;
		for (int i = 0; i < meshAmount; i++)
		{
			threadData.nameKey = modelNames[i];
			threadData.path = (filePath + modelNames[i]).c_str();
			threads.push_back(thread(LoadMeshAsync, &threadData));
			threads[i].join();
		}
	}

	void CAssetManager::Terminate()
	{
		//liberamos memoria de Meshes
		{
			//std::unordered_map<string, CMesh*>::iterator iter = m_Meshes.begin();
			auto iter = m_Meshes.begin();
			for (; iter != m_Meshes.end(); ++iter)
			{
				delete iter->second;
				iter->second = nullptr;
			}
			m_Meshes.clear();
		}

		//liberamos memoria de Textures
		{
			auto iter = m_Textures.begin();
			for (; iter != m_Textures.end(); ++iter)
			{
				iter->second.m_pSRV->Release();
				iter->second.m_pSRV = nullptr;
			}
			m_Textures.clear();
		}
	}

	//============ACCESORS

	CMesh* CAssetManager::GetMesh(std::string _meshName) //const
	{
		if (m_Meshes.find(_meshName) == m_Meshes.end())
		{
			return nullptr;
		}

		return m_Meshes[_meshName];
	}

	Texture* CAssetManager::GetTexture(std::wstring _textureName) //const
	{
		if (m_Textures.find(_textureName) == m_Textures.end())
		{
			return nullptr;
		}

		return &m_Textures[_textureName];
	}
}