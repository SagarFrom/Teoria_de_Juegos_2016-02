#include <iostream>
#include <fstream>
#include <time.h>
#include <string>
#include <fbxsdk.h>
#include <fbxsdk\core\fbxmanager.h>
#include <fbxsdk\fileio\fbxiosettings.h>

using namespace std;
int main() {

	FbxManager *manager = FbxManager::Create();

	FbxScene *scene = FbxScene::Create(manager, "");


	FbxImporter *importer = FbxImporter::Create(manager, "");

	string ruta = "Assets/Dragon.fbx";

	importer->Initialize(ruta.c_str(), -1);

	importer->Import(scene);

	FbxExporter* exporter = FbxExporter::Create(manager, "");

	string path = "Assets/Dragon.obj";

	exporter->Initialize(path.c_str(), -1);

	exporter->Export(scene);

}