#include "Submodel.h"
#include "utils.h"

using namespace std;

Submodel::Submodel(aiMesh* paiMesh) {


	for (int j = 0; j < paiMesh->mNumVertices; j++) {
		m_Vert.push_back(paiMesh->mVertices[j].x);
		m_Vert.push_back(paiMesh->mVertices[j].y);
		m_Vert.push_back(paiMesh->mVertices[j].z);

		m_Norm.push_back(paiMesh->mNormals[j].x);
		m_Norm.push_back(paiMesh->mNormals[j].y);
		m_Norm.push_back(paiMesh->mNormals[j].z);

		m_Text.push_back(paiMesh->mTextureCoords[0][j].x);
		m_Text.push_back(paiMesh->mTextureCoords[0][j].y);

		if (paiMesh->HasTangentsAndBitangents()) {

			glm::vec3 vN = glm::vec3(paiMesh->mNormals[j].x,paiMesh->mNormals[j].y, paiMesh->mNormals[j].z);
			glm::vec3 vT = glm::vec3(paiMesh->mTangents[j].x,paiMesh->mTangents[j].y, paiMesh->mTangents[j].z);
			glm::vec3 vBiT = glm::vec3(paiMesh->mBitangents[j].x,paiMesh->mBitangents[j].y, paiMesh->mBitangents[j].z);

			if (glm::dot(glm::cross(vN, vT), vBiT) < 0.0f){
				vT = vT * -1.0f;
			}

			m_Tangent.push_back(vT.x);
			m_Tangent.push_back(vT.y);
			m_Tangent.push_back(vT.z);

			m_BiTangent.push_back(paiMesh->mBitangents[j].x);
			m_BiTangent.push_back(paiMesh->mBitangents[j].y);
			m_BiTangent.push_back(paiMesh->mBitangents[j].z);			
		}
	}

	for (unsigned int k = 0 ; k < paiMesh->mNumFaces ; k++) {
		const aiFace& Face = paiMesh->mFaces[k];
		assert(Face.mNumIndices == 3);
		m_Ind.push_back(Face.mIndices[0]);
		m_Ind.push_back(Face.mIndices[1]);
		m_Ind.push_back(Face.mIndices[2]);
	}

	helpers::ogl::setupBuffer(GL_ARRAY_BUFFER, m_PosBuffer, sizeof(float) * m_Vert.size(), &(m_Vert[0]), GL_STATIC_DRAW);
	helpers::ogl::setupBuffer(GL_ARRAY_BUFFER, m_NormalBuffer, sizeof(float) * m_Norm.size(),&(m_Norm[0]), GL_STATIC_DRAW);
	helpers::ogl::setupBuffer(GL_ARRAY_BUFFER, m_TexPosBuffer, sizeof(float)* m_Text.size(), &(m_Text[0]), GL_STATIC_DRAW);
	helpers::ogl::setupBuffer(GL_ARRAY_BUFFER, m_TangentBuffer, sizeof(float)* m_Tangent.size(), &(m_Tangent[0]), GL_STATIC_DRAW);
	helpers::ogl::setupBuffer(GL_ARRAY_BUFFER, m_BiTangentBuffer, sizeof(float)* m_BiTangent.size(), &(m_BiTangent[0]), GL_STATIC_DRAW);

	helpers::ogl::setupBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Index, sizeof(int)*m_Ind.size(), &(m_Ind[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_faceCount = paiMesh->mNumFaces * 3;
}

void Submodel::Render() {
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1); 
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_PosBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_NormalBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_TexPosBuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_TangentBuffer);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_BiTangentBuffer);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
	

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Index);
	glDrawElements(GL_TRIANGLES, m_faceCount, GL_UNSIGNED_INT, 0);	

	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(3);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

void Submodel::RenderDebug() {
	//glUseProgram(0);
    // normals
    glColor3f(0,0,1);
    glBegin(GL_LINES);
    for (unsigned int i=0; i<m_Vert.size(); i+=3){
		glm::vec3 p = glm::vec3(m_Vert[i],m_Vert[i+1],m_Vert[i+2]);
        glVertex3fv(&p.x);
		glm::vec3 o = glm::normalize(glm::vec3(m_Norm[i],m_Norm[i+1],m_Norm[i+2]));
        p+=o*0.3f;
        glVertex3fv(&p.x);
    }
    glEnd();

	// tangents
	glColor3f(1,0,0);
	glBegin(GL_LINES);
	for (unsigned int i=0; i<m_Vert.size(); i+=3){
		glm::vec3 p = glm::vec3(m_Vert[i],m_Vert[i+1],m_Vert[i+2]);
		glVertex3fv(&p.x);
		glm::vec3 o = glm::normalize(glm::vec3(m_Tangent[i],m_Tangent[i+1],m_Tangent[i+2]));
		p+=o*0.3f;
		glVertex3fv(&p.x);
	}
	glEnd();
	// bitangents
	// tangents
	glColor3f(0,1,0);
	glBegin(GL_LINES);
	for (unsigned int i=0; i<m_Vert.size(); i+=3){
		glm::vec3 p = glm::vec3(m_Vert[i],m_Vert[i+1],m_Vert[i+2]);
		glVertex3fv(&p.x);
		glm::vec3 o = glm::normalize(glm::vec3(m_BiTangent[i],m_BiTangent[i+1],m_BiTangent[i+2]));
		p+=o*0.3f;
		glVertex3fv(&p.x);
	}
	glEnd();
 
}

Submodel::~Submodel() {
	glDeleteBuffers(1,&m_PosBuffer);
	glDeleteBuffers(1,&m_NormalBuffer);
	glDeleteBuffers(1,&m_TexPosBuffer);
	glDeleteBuffers(1,&m_TangentBuffer);
	glDeleteBuffers(1,&m_BiTangentBuffer);
	glDeleteTextures(1, &m_Index);
}