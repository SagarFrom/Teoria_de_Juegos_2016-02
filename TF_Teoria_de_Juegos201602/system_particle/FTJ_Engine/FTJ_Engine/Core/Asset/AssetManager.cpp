#include "AssetManager.h"
#include "../FTJ_Console.h"

#include "DDSTextureLoader.h"
#include "TinyXML\tinyxml.h"

#include "../Render/Mesh.h"

#include <fstream>

#include <iostream>
#include <time.h>

#include <fbxsdk.h>
#include <fbxsdk\core\fbxmanager.h>
#include <fbxsdk\fileio\fbxiosettings.h>
#include "../../FBXLoader.h"

#define TEXTURE_PATH L"Assets\\Textures\\"
#define MODEL_PATH "Assets\\Models\\"

namespace FTJ
{
	CAssetManager::CAssetManager() {}
	CAssetManager::~CAssetManager() {}
	void LoadNode(FbxNode* _node, Model* _model);
	void Write_ModelBinary(std::string _filepath, const Model* _pModel);


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

	void Load_FBXMesh(FbxMesh* _pMesh, Model* _model)
	{
		int puntos = _pMesh->GetControlPointsCount();
		if (puntos == 0) return;
		Mesh *nuevoM = new Mesh();
		nuevoM->vVertices = vector<Vertex>(puntos);
		auto Puntos = _pMesh->GetControlPoints();
		for (size_t i = 0; i < puntos; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				nuevoM->vVertices[i].position[j] = Puntos[i][j];
			}

		}
		int indices = _pMesh->GetPolygonVertexCount();
		nuevoM->vIndices = vector<unsigned int>(indices);
		for (size_t i = 0; i < indices; i++)
		{
			nuevoM->vIndices[i] = _pMesh->GetPolygonVertexIndex(i);
		}

		_model->vMeshes.push_back(nuevoM);



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

	bool CAssetManager::Load_FBXModel(FbxManager* manager, FbxImporter* importer, std::string _filename, Model** _ppModel)
	{
		//string filename = "C:\\Users\\Diego\\Documents\\GitHub\\Teoria_de_Juegos_2016-02\\TF_Teoria_de_Juegos201602\\FBXLoader\\FBXLoader\\FBXLoader\\Assets\\" + _filename + ".fbx";
		//string filePath = "Assets/Models/ModelNames.xml";

		if (importer->Initialize(_filename.c_str(), -1, manager->GetIOSettings()) == false)
		{
			string temp = importer->GetStatus().GetErrorString();
			return false;
		}

		FbxScene* scene = FbxScene::Create(manager, "");
		if (scene == nullptr)
		{
			return false;
		}
		importer->Import(scene);
		importer->Destroy();

		Model* pModel = 0;

		FbxNode* rootNode = scene->GetRootNode();
		if (rootNode)
		{
			pModel = *_ppModel = new Model();

			unsigned int numChildren = rootNode->GetChildCount();
			for (unsigned int c = 0; c < numChildren; c++)
			{
				LoadNode(rootNode->GetChild(c), pModel);
			}
		}
		else
		{
			return false;
		}

		Write_ModelBinary(_filename + ".bin", pModel);

		TiXmlDocument document;


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

		_filename = MODEL_PATH;

		for (int i = 0; i < meshAmount; i++)
		{
			threadData.nameKey = modelNames[i];
			threadData.path = (_filename + modelNames[i]).c_str();
			threads.push_back(thread(LoadMeshAsync, &threadData));
			threads[i].join();
		}


		return true;
	}

	void Write_ModelBinary(std::string _filepath, const Model* _pModel)
	{
		ofstream arch(_filepath, ios::binary);
		if (!arch.is_open()){
			return;
		}
		int cuantosMesh = _pModel->vMeshes.size();

		//TODO
		arch.write((char*)&cuantosMesh, sizeof(int));
		for (size_t i = 0; i < cuantosMesh; i++)
		{
			int cuantosVrt = _pModel->vMeshes[i]->vVertices.size();
			int cuantosIdx = _pModel->vMeshes[i]->vIndices.size();
			arch.write((char*)&cuantosVrt, sizeof(int));
			arch.write((char*)_pModel->vMeshes[i]->vVertices.data(), sizeof(Vertex)*cuantosVrt);
			arch.write((char*)&cuantosIdx, sizeof(int));
			arch.write((char*)_pModel->vMeshes[i]->vIndices.data(), sizeof(unsigned int)*cuantosIdx);

		}
		arch.close();

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

	void LoadNode(FbxNode* _node, Model* _model)
	{
		if (_node->GetNodeAttribute() != nullptr && _node->GetNodeAttribute()->GetAttributeType() == fbxsdk_2015_1::FbxNodeAttribute::EType::eMesh)
		{
			Load_FBXMesh(_node->GetMesh(), _model);
			//LoadTextureFBX(_node->GetMesh(), _model);
		}

		unsigned int numChildren = _node->GetChildCount();
		for (unsigned int c = 0; c < numChildren; c++)
		{
			LoadNode(_node->GetChild(c), _model);
		}
	}
}