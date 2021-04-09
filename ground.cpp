#include "ground.h"

ground::ground()
{
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	edge = 100000;
}

ground::ground(float x, float y, float z)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;
	edge = 100000;
}

void ground::loadObj()
{
	cout << " loading model " << endl;
	if (objLoader.loadModel("TestModels/ground.obj", model))//returns true if the model is loaded, puts the model in the model parameter
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

void ground::draw()
{
	glUseProgram(myShader->handle());
	/*amount += temp;
	if (amount > 1.0f || amount < -0.5f)
		temp = -temp;
	amount = 0;
	glUniform1f(glGetUniformLocation(myShader->handle(), "displacement"), amount);*/

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

	glm::mat4 trans = glm::translate(glm::mat4(1.0), pos);
	ModelMatrix = trans;
	//DRAW THE MODEL
	ModelViewMatrix = viewingMatrix * ModelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);


	glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(myShader->handle(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

	model.drawElementsUsingVBO(myShader);
}

void ground::projection(glm::mat4 matrix)
{
	ProjectionMatrix = matrix;
}

void ground::shader(Shader* shader)
{
	myShader = shader;
}

void ground::setPos(glm::vec3 p)
{
	pos = p;
}

float ground::getEdge()
{
	return edge;
}

glm::vec3 ground::getPos()
{
	return pos;
}
