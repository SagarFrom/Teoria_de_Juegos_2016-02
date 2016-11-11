#include "Geometry.h"

#include <iostream>
#include <fstream>
#include <time.h>

#include <fbxsdk.h>
#include <fbxsdk\core\fbxmanager.h>
#include <fbxsdk\fileio\fbxiosettings.h>

using namespace std;

void Load_FBXMesh(FbxMesh* _pMesh, Model* _model);

//Escribe un archivo .bin con la informacion de una mesh 3D
void Write_ModelBinary(std::string _filepath, const Model* _pModel)
{	

}

void Read_ModelBinary(char* _filepath, Model** _ppModel)
{

}

//Imprime contenido de un Modelo
void Print_Model(const Model* _pMesh)
{
	
	cout << _pMesh;

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

//Lee el archivo FBX y actua segun los tipos de nodos que encuentra
bool Load_FBXModel(FbxManager* manager, FbxImporter* importer, std::string _filename, Model** _ppModel)
{
	string filename = "D:\\FBXLoader\\FBXLoader\\FBXLoader\\Assets\\" + _filename + ".fbx";
	if (importer->Initialize(filename.c_str(), -1, manager->GetIOSettings()) == false)
	{
		string temp = importer->GetStatus().GetErrorString();
		//Log("Failed to initialize FbxImporter");
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

	return true;
}

void Load_FBXMesh(FbxMesh* _pMesh, Model* _model)
{

}

int main()
{
	FbxManager * manager = FbxManager::Create();

	if (manager == nullptr)
	{
		//Log("Failed to create FbxManager");
		return false;
	}

	FbxIOSettings* ioSettings = FbxIOSettings::Create(manager, "");
	if (ioSettings == nullptr)
	{
		//Log("Failed to create FbxIOSettings");
		return false;
	}

	manager->SetIOSettings(ioSettings);

	FbxImporter* importer = FbxImporter::Create(manager, "");
	if (importer == nullptr)
	{
		//Log("Failed to create FbxImporter");
		return false;
	}

	Model* miModelo;
	Load_FBXModel(manager, importer, "star", &miModelo);

	Print_Model(miModelo);

	manager->Destroy();

	getchar();
	return 0;
}