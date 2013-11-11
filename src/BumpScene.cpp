#include "BumpScene.h"
#include "utils.h"

BumpScene::BumpScene(ShaderCollection* shader):AbstractScene(shader){
}

BumpScene::~BumpScene() {
}

void BumpScene::Render() {
	ShaderProgram* program = getShader("bumpShader");

	glm::mat4& projMat = *getProjectionMatrix();
	glm::mat4& worldMat = *getWorldMatrix();
	Model* model = getCurrentModel();

	//
	// Standard rendering
	//
	program->use();

	glm::mat4x4 WM = (*pWorld);
	WM *= (*pRotation);
	program->uniformMatrix4f("ProjectionMatrix", glm::value_ptr(projMat));
	program->uniformMatrix4f("ModelViewMatrix",  glm::value_ptr(WM));
	program->uniformMatrix4f("ViewMatrix", glm::value_ptr(*pView));

	// shader setup
	// layer attributs
	program->uniform1i("diffuseSampler", 0);

	//program->uniform1i("substance", 1);
	//program->uniform1i("fresnell", 2);
	program->uniform1i("normalSampler", 3);

	glm::vec4 eyeLight = glm::vec4(getLightPosition(), 1.0f);
	program->uniform4fv("light",  glm::value_ptr(eyeLight)); //light setup

	//glm::mat4 gNormalMatrix = glm::mat4(worldMat);
	//program->uniformMatrix4f("NormalMatrix",  glm::value_ptr(gNormalMatrix));

	model->PrepareTextures();
	model->Render();

	program->disable();

	glMatrixMode(GL_PROJECTION);
    glLoadMatrixf((const GLfloat*)&projMat[0]);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf((const GLfloat*)&WM[0]);
	
	model->RenderDebug();
}