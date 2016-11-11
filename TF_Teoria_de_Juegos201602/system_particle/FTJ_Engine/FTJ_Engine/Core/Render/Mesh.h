#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include <vector>

using namespace DirectX;
using namespace std;

namespace FTJ
{

	typedef unsigned int uint;

#ifndef MeshVertex
	typedef struct MeshVertex
	{
		XMFLOAT3 pos;
		XMFLOAT3 uv;
		XMFLOAT3 normal;
	}MeshVertex;
#endif

	class CMesh
	{
		ID3D11Buffer*	vertexbuffer;
		ID3D11Buffer*	indexBuffer;

		vector<uint>		indices;
		vector<MeshVertex>	vertices;

		CMesh() = default;

		static bool LoadOBJ(const char* path, CMesh* _pMesh);
		static int LoadBIN(const char* path, CMesh* _pMesh);

	public:
		~CMesh();

		static int LoadMesh(const char *path, ID3D11Device *device, CMesh** _outMesh);

		uint GetIndicesCount() const; 
		uint GetVerticesCount() const; 

		ID3D11Buffer* GetVertexBuffer() const { return vertexbuffer; }
		ID3D11Buffer* GetIndexBuffer() const { return indexBuffer; }
	};
}