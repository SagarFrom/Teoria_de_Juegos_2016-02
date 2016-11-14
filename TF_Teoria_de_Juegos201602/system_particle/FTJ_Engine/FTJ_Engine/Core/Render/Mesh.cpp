#include "Mesh.h"
#define LOADMESH_BINARY 1

#include "../FTJ_Console.h"
#include <iostream>
#include <fstream>

namespace FTJ
{

	bool CMesh::LoadOBJ(const char* path, CMesh* _pMesh)
	{
		std::vector<MeshVertex> & vertices = _pMesh->vertices;

		std::vector< unsigned int > uvIndices, normalIndices;
		std::vector<XMFLOAT3> temp_vertices;
		std::vector<XMFLOAT3> temp_uvs;
		std::vector<XMFLOAT3> temp_normals;

		FILE *file;
		fopen_s(&file, path, "r");
		if (file == NULL){
			printf("Fail: to open the file: %s\n", path);
			return false;
		}

		while (true)
		{
			char lineHeader[128];
			// read the first word of the line
			int res = fscanf_s(file, "%s", lineHeader, 128);
			if (res == EOF)
				break; // EOF = End Of File. Quit the loop.

			// else : parse lineHeader
			if (strcmp(lineHeader, "v") == 0)
			{
				XMFLOAT3 pos;
				fscanf_s(file, "%f %f %f\n",
					&pos.x, &pos.y, &pos.z);
				temp_vertices.push_back(pos);
			}
			else if (strcmp(lineHeader, "vt") == 0){
				XMFLOAT3 uv;
				uv.z = 1;
				fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
				temp_uvs.push_back(uv);
			}
			else if (strcmp(lineHeader, "vn") == 0){
				XMFLOAT3 normal;
				fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0){
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
					&vertexIndex[0], &uvIndex[0], &normalIndex[0],
					&vertexIndex[1], &uvIndex[1], &normalIndex[1],
					&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9){
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					return false;
				}

				MeshVertex vert;
				vert.pos = temp_vertices[vertexIndex[0] - 1];
				vert.uv = temp_uvs[uvIndex[0] - 1];
				vert.normal = temp_normals[normalIndex[0] - 1];
				vertices.push_back(vert);

				vert.pos = temp_vertices[vertexIndex[1] - 1];
				vert.uv = temp_uvs[uvIndex[1] - 1];
				vert.normal = temp_normals[normalIndex[1] - 1];
				vertices.push_back(vert);

				vert.pos = temp_vertices[vertexIndex[2] - 1];
				vert.uv = temp_uvs[uvIndex[2] - 1];
				vert.normal = temp_normals[normalIndex[2] - 1];
				vertices.push_back(vert);
			}

		}

		_pMesh->indices.clear();
		int nVertices = vertices.size();
		for (int i = 0; i < nVertices; i++)
		{
			_pMesh->indices.push_back(i);
		}

		return true;
	}

	int CMesh::LoadBIN(const char* path, CMesh* _pMesh)
	{
		ifstream archivo(path, ios::binary);
		if (!archivo.is_open()){
			return false;

		}
		int CuantosMesh = 0;
		archivo.read((char*)&CuantosMesh, sizeof(int));
		for (int i = 0; i < CuantosMesh; i++)
		{
			int CuantosVrt = 0;
			int CuantosIdx = 0;
			archivo.read((char*)&CuantosVrt, sizeof(int));
			MeshVertex* vertices = new MeshVertex[CuantosVrt];
			MeshVertex temp;
			float tempf[9];

			for (int i = 0; i < CuantosVrt; i++)
			{
				archivo.read((char*)tempf, sizeof(float)* 9);
				temp.pos = XMFLOAT3(tempf[0], tempf[1], tempf[2]);
				temp.uv = XMFLOAT3(tempf[3], tempf[4], tempf[5]);
				temp.normal = XMFLOAT3(tempf[6], tempf[7], tempf[8]);

				_pMesh->vertices.push_back(temp);

			}

			archivo.read((char*)&CuantosIdx, sizeof (int));
			unsigned int *indices = new unsigned int[CuantosIdx];
			archivo.read((char*)indices, sizeof(unsigned int)*CuantosIdx);

			for (int j = 0; j < CuantosVrt*3; j++)
			{
				_pMesh->indices.push_back(j);

			}


		}



		return 1;
	}

	void SaveBIN(CMesh* _mesh)
	{

	}

	/*static*/ int CMesh::LoadMesh(const char *path, ID3D11Device *device, CMesh** _outMesh)
	{
		*_outMesh = new CMesh();
		CMesh* pMesh = *_outMesh;

		int result;
#if LOADMESH_BINARY
		result = LoadBIN(path, pMesh);
#else
		result = LoadOBJ(path, pMesh);
#endif
		if (!result)
			return result;

		D3D11_BUFFER_DESC bufferDesc;
		memset(&bufferDesc, 0, sizeof(D3D11_BUFFER_DESC));
		bufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = NULL;
		bufferDesc.ByteWidth = sizeof(MeshVertex) * pMesh->vertices.size();
		bufferDesc.StructureByteStride = sizeof(MeshVertex);


		// TODO: PART 2 STEP 3d
		D3D11_SUBRESOURCE_DATA pInitial;
		memset(&pInitial, 0, sizeof(D3D11_SUBRESOURCE_DATA));
		pInitial.pSysMem = (const void*)pMesh->vertices.data();
		pInitial.SysMemPitch = sizeof(MeshVertex);
		pInitial.SysMemSlicePitch = 1;

		int strides = sizeof(MeshVertex);
		int offsets = 0;
		if (!DXCall(device->CreateBuffer(&bufferDesc, &pInitial, &pMesh->vertexbuffer)))
			Log("failed creating 3D Object Vertex Buffer");
		DXName(pMesh->vertexbuffer, "3D Object Vertex Buffer");

		//INDEX BUFFER
		memset(&bufferDesc, 0, sizeof(D3D11_BUFFER_DESC));
		bufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(uint) * pMesh->indices.size();
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		memset(&pInitial, 0, sizeof(D3D11_SUBRESOURCE_DATA));
		pInitial.pSysMem = pMesh->indices.data();
		pInitial.SysMemPitch = 0;
		pInitial.SysMemSlicePitch = 0;

		if (!DXCall(device->CreateBuffer(&bufferDesc, &pInitial, &pMesh->indexBuffer)))
			Log("failed creating 3D Object Index Buffer");
		DXName(pMesh->indexBuffer, "3D Object Index Buffer");

		return 0;
	}
	CMesh::~CMesh()
	{
		indexBuffer->Release();
		vertexbuffer->Release();
	}

	uint CMesh::GetIndicesCount() const
	{
		return indices.size();
	};
	uint CMesh::GetVerticesCount() const
	{
		return vertices.size();
	};
}