#pragma once

#include "shaders/Shader.h"   // include shader header file, this is not part of OpenGL
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"
#include "3DStruct\threeDModel.h"
#include "Obj\OBJLoader.h"
#include "glm\gtx\quaternion.hpp"
#include "Camera.h"

class HandleBar
{
public:
	HandleBar();
	void loadObj();
	void draw();
	void projection(glm::mat4 matrix);
	void shader(Shader* shader);
	glm::vec3 getPos();
	glm::quat getRot();
	glm::quat getRo();
	void update(glm::quat l, glm::quat s, glm::quat ro, glm::quat so, glm::vec3 p);

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
	glm::quat lean;
	glm::quat steer;
	glm::quat rotate;
	glm::quat rot;
	glm::quat st;

	float offset = 3900;
	float casterAngle;
};

