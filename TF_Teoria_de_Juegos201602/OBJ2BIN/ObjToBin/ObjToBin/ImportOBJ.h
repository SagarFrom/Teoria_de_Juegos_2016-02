#pragma once
#include <vector>
#include <fstream>
#include <DirectXMath.h>

using namespace DirectX;
using namespace std;

typedef unsigned int uint;

#ifndef MeshVertex
typedef struct MeshVertex
{
	XMFLOAT3 pos;
	XMFLOAT3 uv;
	XMFLOAT3 normal;
}MeshVertex;
#endif

class Mesh
{
public:
	std::vector< MeshVertex > vertices;
	std::vector< uint > indices;
};

bool LoadOBJ(const char* path, Mesh* _pMesh)
{
	std::vector<MeshVertex> & vertices = _pMesh->vertices;

	std::vector< unsigned int > uvIndices, normalIndices;
	std::vector<XMFLOAT3> temp_vertices;
	std::vector<XMFLOAT3> temp_uvs;
	std::vector<XMFLOAT3> temp_normals;

	FILE *file;
	fopen_s(&file, path, "r");
	if (file == NULL){
		printf("Fail: to open the file !\n");
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
