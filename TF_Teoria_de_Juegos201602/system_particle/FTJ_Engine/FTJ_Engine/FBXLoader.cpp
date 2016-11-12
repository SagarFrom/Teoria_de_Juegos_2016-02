#include "FBXLoader.h"

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

void Read_ModelBinary(char* _filepath, Model** _ppModel)
{

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
	string filename = "C:\\Users\\Diego\\Documents\\GitHub\\Teoria_de_Juegos_2016-02\\TF_Teoria_de_Juegos201602\\FBXLoader\\FBXLoader\\FBXLoader\\Assets\\" + _filename + ".fbx";

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
		//nuevoM->vIndices[i] = _pMesh

	}
	int indices = _pMesh->GetPolygonVertexCount();
	nuevoM->vIndices = vector<unsigned int>(indices);
	for (size_t i = 0; i < indices; i++)
	{
		nuevoM->vIndices[i] = _pMesh->GetPolygonVertexIndex(i);
	}

	_model->vMeshes.push_back(nuevoM);
}