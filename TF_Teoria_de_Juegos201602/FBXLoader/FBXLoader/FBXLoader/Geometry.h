#ifndef GEOMETRY
#define GEOMETRY

#include <vector>

class Vertex
{
public:
	float position[3];
	float uv[3];
	float normal[3];

	//otros
};

class Mesh
{
public:
	std::vector<Vertex> vVertices;
	std::vector<unsigned int> vIndices;
};

class Model
{
public:
	std::vector<Mesh*> vMeshes;
};

struct XMFLOAT3
{
	float x;
	float y;
	float z;

	XMFLOAT3() {}
	XMFLOAT3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	explicit XMFLOAT3(_In_reads_(3) const float *pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}

	XMFLOAT3& operator= (const XMFLOAT3& Float3) { x = Float3.x; y = Float3.y; z = Float3.z; return *this; }
};
#endif 