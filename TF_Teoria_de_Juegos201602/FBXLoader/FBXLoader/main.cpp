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

bool Read_ModelBinary(char* _filepath, Model** _ppModel)
{
	ifstream archivo(_filepath, ios::binary);
	if (!archivo.is_open()){
		return false;

	}
	Model * pModel = *_ppModel = new Model();
	int CuantosMesh = 0;
	archivo.read((char*)&CuantosMesh, sizeof(int));
	for (int i = 0; i < CuantosMesh; i++)
	{
		int CuantosVrt = 0;
		int CuantosIdx = 0;
		archivo.read((char*)&CuantosVrt, sizeof(int));
		Vertex* vertices = new Vertex[CuantosVrt];
		archivo.read((char*)vertices, sizeof(Vertex)*CuantosVrt);
		Mesh * meshito = new Mesh();
		//meshito->vVertices = vector<Vertex>(&vertices, sizeof(Vertex)*CuantosVrt);
		for (int j = 0; j < CuantosVrt; j++)
		{

			//	Mesh * nMesh = new Mesh();
			//Vertex v = Vertex();
			//archivo.read((char*)&v,sizeof Vertex);
			meshito->vVertices.push_back(vertices[j]);

		}
		archivo.read((char*)&CuantosIdx, sizeof (int));
		unsigned int *indices = new unsigned int[CuantosIdx];
		archivo.read((char*)indices, sizeof(unsigned int)*CuantosIdx);

		for (int j = 0; j < CuantosIdx; j++)
		{
			meshito->vIndices.push_back(indices[j]);
		}
		pModel->vMeshes.push_back(meshito);

	}



	return true;
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
	string filename = "C:\\Users\\Diego\\Documents\\GitHub\\Teoria_de_Juegos_2016-02\\TF_Teoria_de_Juegos201602\\FBXLoader\\FBXLoader\\FBXLoader\\Assets\\" + _filename + ".FBX";

	if (importer->Initialize(filename.c_str(), -1, manager->GetIOSettings()) == false)
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
		return false;
	}

	manager->SetIOSettings(ioSettings);

	FbxImporter* importer = FbxImporter::Create(manager, "");
	if (importer == nullptr)
	{
		return false;
	}

	Model* miModelo;
	Load_FBXModel(manager, importer, "trump_lp_Rig", &miModelo);

	Print_Model(miModelo);

	manager->Destroy();

	getchar();
	return 0;
}