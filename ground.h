#pragma once

#include "shaders/Shader.h"   // include shader header file, this is not part of OpenGL
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"
#include "3DStruct\threeDModel.h"
#include "Obj\OBJLoader.h"
#include "glm\gtx\quaternion.hpp"
#include "camera.h"

class ground
{
public:
	ground();
	ground(float x, float y, float z);
	void loadObj();
	void draw();
	void projection(glm::mat4 matrix);
	void shader(Shader* shader);
	void setPos(glm::vec3 p);
	float getEdge();
	glm::vec3 getPos();

protected:

	float amount = 0;
	float temp = 0.002f;

	ThreeDModel model, modelbox;
	OBJLoader objLoader;
	Shader* myShader;
	///END MODEL LOADING

	glm::mat4 ProjectionMatrix; // matrix for the orthographic projection
	glm::mat4 ModelViewMatrix;  // matrix for the modelling and viewing
	glm::mat4 ModelMatrix;

	//Material properties
	float Material_Ambient[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float Material_Diffuse[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float Material_Specular[4] = { 0.9f,0.9f,0.8f,1.0f };
	float Material_Shininess = 50;

	//Light Properties
	float Light_Ambient_And_Diffuse[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float Light_Specular[4] = { 1.0f,1.0f,1.0f,1.0f };
	float LightPos[4] = { 0.0f, 1.0f, 0.0f, 0.0f };

	glm::vec3 pos;
	float edge;
};

