#pragma once
#include <string>
#include <GL/glew.h>
#include <vector>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Texture.h"

#define GLCheckError() (glGetError() == GL_NO_ERROR)
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)

class BasicMesh
{
public:
	BasicMesh(){}
	~BasicMesh(){}
	bool LoadMesh(const std::string& Filename);
	void Render();
	void Render(unsigned int NumInstances, const Matrix4* WVPMats, const Matrix4* WorldMats);

private:

	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	
	void CountVerticesAndIndices(const aiScene* pScene, unsigned int& NumVertices, unsigned int& NumIndices);

	void ReserveSpace(unsigned int NumVertices, unsigned int NumIndices);

	void InitAllMeshes(const aiScene* pScene);

	void InitSingleMesh(const aiMesh* paiMesh);

	bool InitMaterials(const aiScene* pScene, const std::string& Filename);

	void PopulateBuffers();

#define INVALID_MATERIAL 0xFFFFFFFF
	
	enum BUFFER_TYPE {
		INDEX_BUFFER = 0,
		POS_VB       = 1,
		TEXCOORD_VB  = 2,
		NORMAL_VB    = 3,
		WVP_MAT_VB   = 4,
		WORLD_MAT_VB = 5,
		NUM_BUFFERS  = 6
	};
	
	GLuint m_VAO = 0;
	GLuint m_Buffers[NUM_BUFFERS] = { 0 };

	struct BasicMeshEntry {
		BasicMeshEntry()
		{
			NumIndices = 0;
			BaseVertex = 0;
			BaseIndex = 0;
			MaterialIndex = INVALID_MATERIAL;
		}

		unsigned int NumIndices;
		unsigned int BaseVertex;
		unsigned int BaseIndex;
		unsigned int MaterialIndex;
	};

	std::vector<BasicMeshEntry> m_Meshes;
	std::vector<Texture*> m_Textures;

	// Temporary space for vertex stuff before we load them into the GPU
	std::vector<Vector3> m_Positions;
	std::vector<Vector3> m_Normals;
	std::vector<Vector2> m_TexCoords;
	std::vector<unsigned int> m_Indices;
};
