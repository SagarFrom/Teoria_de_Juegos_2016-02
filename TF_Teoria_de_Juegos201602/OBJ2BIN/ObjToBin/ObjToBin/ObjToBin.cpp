// ========== TAREA ACADEMICA 1 ==========
// Autor:	Marcos Alexis Sotelo
//
// =======================================

#include "ImportOBJ.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

//
// Imprime la informacion correspondiente al Mesh
// Nota:
//  Utilizar el siguiente formato para esta funcion y los archivos binarios
//  -numero de Vertices
//  -Listar todos los Vertices
//  -numero de Indices
//  -Listar todos los Indices
//
void PrintMesh(Mesh& _mesh)
{

	std::cout << "Numero de Vertices: " << _mesh.vertices.size() << "\n" << endl;

	//Listar todo los vertices 
	for (int i = 0; i < _mesh.vertices.size(); i++)
	{

		std::cout << "vec[" << i << "]" << "\n" << "pos: \n" << endl;
		std::cout << _mesh.vertices.at(i).pos.x << ", " << _mesh.vertices.at(i).pos.y << ", " << _mesh.vertices.at(i).pos.z << "\n"<< endl;
		std::cout << "normal: \n" <<endl;
		std::cout << _mesh.vertices.at(i).normal.x << ", " << _mesh.vertices.at(i).normal.y << ", " << _mesh.vertices.at(i).normal.z << "\n" << endl;
		std::cout << "uv: \n" << endl;
		std::cout << _mesh.vertices.at(i).uv.x << ", " << _mesh.vertices.at(i).uv.y << ", " << _mesh.vertices.at(i).uv.z << "\n" << endl;
	
	}

	std::cout << "\n";
	std::cout << "Numero de Indices: " << _mesh.indices.size() << endl;

	//Listar todo los indices 

}

//
// Escribe un archivo binario con la informacion del Mesh
// en la direccion ingresada
//
bool WriteMeshBinary(Mesh& _mesh, const char* _filePath)
{
	bool writed = false;

		std::fstream file;
		file.open(_filePath, ios::binary | ios::out);

		int Nvertices = _mesh.vertices.size();

		XMFLOAT3 pos;
		XMFLOAT3 uv;
		XMFLOAT3 normal;
		
		file.write((char*) &Nvertices, sizeof(Nvertices));

		if (file.is_open())
		{
			for (int i = 0; i < Nvertices; i++)
			{
				pos = _mesh.vertices[i].pos;
				file.write((char*) &pos, sizeof(pos));

				uv = _mesh.vertices[i].uv;
				file.write((char*)&uv, sizeof(uv));

				normal = _mesh.vertices[i].normal;
				file.write((char*)&normal, sizeof(normal));

			}

			int Nindices = _mesh.indices.size();
			file.write((char*) &Nindices, sizeof(Nindices));
			int x;

			for (int i = 0; i < Nindices; i++)
			{
				x = _mesh.indices[i];
				file.write((char*) &x, sizeof(x));
			}

			file.close();
			writed = true;

		}


	return writed;
}

//
// Lee el archivo binario en la ruta ingresada
// y lo traduce al Mesh enviado
//
bool ReadMeshBinary(Mesh* _outMesh, const char* _filePath)
{
	bool readed = false;

	std::fstream file;
	file.open(_filePath, ios::binary | ios::in);

	int Nvertices;


	if (file.is_open())
	{
		file.read((char*) &Nvertices, sizeof(Nvertices));

		for (int i = 0; i < Nvertices; i++)
		{
			XMFLOAT3 pos;
			file.read((char*) &pos, sizeof(pos));

			XMFLOAT3 uv;
			file.read((char*) &uv, sizeof(uv));

			XMFLOAT3 normal;
			file.read((char*) &normal, sizeof(normal));

			MeshVertex V;
			V.pos = pos;
			V.uv = uv;
			V.normal = normal;
			_outMesh->vertices.push_back(V);

		}

		int Nindices;
		file.read((char*) &Nindices, sizeof(Nindices));

		for (int i = 0; i < Nindices; i++)
		{
			uint x;

			file.read((char*) &x, sizeof(x));
			_outMesh->indices.push_back(x);
		}

		file.close();
		readed = true;

	}


	return readed;
}

int main(array<System::String ^> ^args)
{
	Mesh myMesh;

	std::string filePath = "Assets\\Models\\";

	string filenamePath = filePath + "sphere.obj";
	LoadOBJ(filenamePath.c_str(), &myMesh);

	//PrintMesh(myMesh);

	//BINARIO
	std::cout << endl << "===========WriteMeshBinary===========" << endl << endl;

	filenamePath = filePath + "sphere.bin";

	if (WriteMeshBinary(myMesh, filenamePath.c_str()))
		std::cout << "EXITO: " << filenamePath << " escrito con exito" << endl;
	else
		std::cout << "ERROR: " << filenamePath << " no pudo ser escrito" << endl;

	std::cout << endl << "===========ReadMeshBinary===========" << endl << endl;

	myMesh.vertices.clear();
	myMesh.indices.clear();
	if (ReadMeshBinary(&myMesh, filenamePath.c_str()))
		std::cout << "EXITO: " << filenamePath << " leido con exito" << endl;
	else
		std::cout << "ERROR: " << filenamePath << " no pudo ser leido" << endl;

	//PrintMesh(myMesh);

	getchar();
	return 0;
}
