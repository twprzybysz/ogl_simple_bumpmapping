#pragma once

#include <vector>
#include "gl_3_3.h"

#include <assimp\Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Submodel {

public:
	Submodel(aiMesh* paiMesh);
	void Render();
	void RenderDebug();
	~Submodel();

private:
	GLuint m_PosBuffer;
	GLuint m_NormalBuffer;
	GLuint m_TexPosBuffer;

	GLuint m_TangentBuffer;
	GLuint m_BiTangentBuffer;

	GLuint m_Index;
	int m_faceCount;

	std::vector<float> m_Vert;
	std::vector<float> m_Norm;
	std::vector<float> m_Text;

	std::vector<float> m_Tangent;
	std::vector<float> m_BiTangent;

	std::vector<int> m_Ind;
};