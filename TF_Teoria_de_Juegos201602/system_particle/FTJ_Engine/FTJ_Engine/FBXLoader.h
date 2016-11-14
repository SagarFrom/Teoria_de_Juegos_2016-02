#ifndef FBXLoader
#define FBXLoader

#include <vector>

class Vertex
{
public:
	float position[3];
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

#endif 