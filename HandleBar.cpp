#include "HandleBar.h"

HandleBar::HandleBar()
{
    pos.x = 0;
    pos.y = 4000;
    pos.z = -400;
    casterAngle = 8;
}

void HandleBar::loadObj()
{
	cout << " loading model " << endl;
	if (objLoader.loadModel("TestModels/HandleBars.obj", model))//returns true if the model is loaded, puts the model in the model parameter
	{
		cout << " model loaded " << endl;

		//if you want to translate the object to the origin of the screen,
		//first calculate the centre of the object, then move all the vertices
		//back so that the centre is on the origin.
		model.calcCentrePoint();
		model.centreOnZero();


		model.calcVertNormalsUsingOctree();  //the method will construct the octree if it hasn't already been created.


		//turn on VBO by setting useVBO to true in threeDmodel.cpp default constructor - only permitted on 8 series cards and higher
		model.initDrawElements();
		model.initVBO(myShader);
		model.deleteVertexFaceData();

	}
	else
	{
		cout << " model failed to load " << endl;
	}
}

void HandleBar::draw()
{
	glUseProgram(myShader->handle());
	//amount += temp;
	//if (amount > 1.0f || amount < -0.5f)
	//	temp = -temp;
	//amount = 0;
	//glUniform1f(glGetUniformLocation(myShader->handle(), "displacement"), amount);

	glm::mat4 trans = glm::translate(glm::mat4(1.0), pos);
	glm::vec3 pivot = glm::vec3(0, 0, 0);
	pivot.y += offset;
	glm::mat4 piv = glm::translate(glm::mat4(1.0), pivot);
	glm::mat4 minPiv = glm::translate(glm::mat4(1.0), -pivot);

	glm::vec3 r = glm::vec3((casterAngle * 3.1415f) / 180, 0, 0);

	glm::quat handlebarOffset = glm::quat(r);
	glm::quat minHandlebarOffest = glm::quat(-r);


	ModelMatrix = trans *
		glm::toMat4(rot) *
		glm::toMat4(st) *
		minPiv * glm::toMat4(lean) * piv *
		glm::toMat4(minHandlebarOffest) * glm::toMat4(steer) * glm::toMat4(handlebarOffset);

	GLuint matLocation = glGetUniformLocation(myShader->handle(), "ProjectionMatrix");
	glUniformMatrix4fv(matLocation, 1, GL_FALSE, &ProjectionMatrix[0][0]);

	glm::mat4 viewingMatrix = Camera::getViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "ViewMatrix"), 1, GL_FALSE, &viewingMatrix[0][0]);

	glUniform4fv(glGetUniformLocation(myShader->handle(), "LightPos"), 1, LightPos);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "light_ambient"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "light_diffuse"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "light_specular"), 1, Light_Specular);

	glUniform4fv(glGetUniformLocation(myShader->handle(), "material_ambient"), 1, Material_Ambient);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "material_diffuse"), 1, Material_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "material_specular"), 1, Material_Specular);
	glUniform1f(glGetUniformLocation(myShader->handle(), "material_shininess"), Material_Shininess);

	//DRAW THE MODEL
	ModelViewMatrix = viewingMatrix * ModelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);


	glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(myShader->handle(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

	model.drawElementsUsingVBO(myShader);
}

void HandleBar::projection(glm::mat4 matrix)
{
	ProjectionMatrix = matrix;
}

void HandleBar::shader(Shader* shader)
{
	myShader = shader;
}

glm::vec3 HandleBar::getPos()
{
	return pos;
}

glm::quat HandleBar::getRot()
{
	return rot;
}

glm::quat HandleBar::getRo()
{
	return st;
}

void HandleBar::update(glm::quat l, glm::quat s, glm::quat ro, glm::quat so, glm::vec3 p)
{
	lean = l;
	steer = s;
	st = so;
	rot = ro;

	pos = p;
}
