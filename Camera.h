#pragma once
#include <iostream>
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"
static class Camera
{
public:
	Camera();
	static void cameraView(glm::vec3 pos, glm::quat rot, glm::quat ro);
	static glm::mat4 getViewMatrix();
	static void changeCamera();

private:
	static glm::mat4 viewMatrix;
	static int camNum;
};

