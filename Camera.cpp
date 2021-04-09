#include "Camera.h"

glm::mat4 Camera::viewMatrix = glm::mat4(1.0);
int Camera::camNum = 0;

Camera::Camera()
{
}

void Camera::cameraView(glm::vec3 pos, glm::quat rot, glm::quat ro)
{
	glm::vec3 cameraSet(0, 0, 0);
	glm::vec3 temp(0, 0, 0);
	temp.x = pos.x;
	temp.y = pos.y;
	temp.z = pos.z;

	if (camNum == 0)
	{
		cameraSet = temp + rot * ro * glm::vec3(0, 5000, -20000);

		viewMatrix = glm::lookAt(cameraSet, pos, glm::vec3(0, 1, 0));
	}
	else if (camNum == 1)
	{
		cameraSet = temp + rot * ro * glm::vec3(-20000, 0, 0);

		viewMatrix = glm::lookAt(cameraSet, pos, glm::vec3(0, 1, 0));
	}
	else if (camNum == 2)
	{
		cameraSet = temp + rot * ro * glm::vec3(0, 20000, 0);

		viewMatrix = glm::lookAt(cameraSet, pos, glm::vec3(0, 0, 1));
	}
	else if (camNum == 3)
	{
		cameraSet = temp + rot * ro * glm::vec3(0, 0, 20000);

		viewMatrix = glm::lookAt(cameraSet, pos, glm::vec3(0, 1, 0));
	}
}

glm::mat4 Camera::getViewMatrix()
{
	return viewMatrix;
}

void Camera::changeCamera()
{
	camNum++;
	if (camNum > 3)
	{
		camNum = 0;
	}
}
