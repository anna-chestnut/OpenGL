#include <gl\glew.h>
#include <iostream>
#include <fstream>

#include <MeGlWindow.h>
#include <QtGui\qkeyevent>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Primitives\Vertex.h>
#include <Primitives\ShapeGenerator.h>
#include "ShaderParser.h"
#include "Camera.h"

using namespace std;
using glm::vec3;
using glm::mat4;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera;
glm::vec3 lightPos(0, 3, 0);


unsigned int passTroughProgram;
unsigned int passTroughMvp;

// cube data
// ---------
unsigned int cubeProgram;
float rotateDegree = 20.0f;
GLint modelLoc, viewLoc, projectionLoc;
float xoffset, yoffset;
float moveSpeed = 0.2f;
bool drawCube = true;

// triangle data
// -------------
unsigned int triangleProgram;
unsigned int triangleVao, cubeVao;
unsigned int transformLoc, colorLoc;

// plane data
// ----------

GLuint vertexBufferID, indexBufferID;
GLuint numIndices;
GLsizeiptr sizeVertices, sizeIndices;
unsigned int planeVao, planeProgram;
unsigned int mvpLoc, mvLoc, planeModelLoc, planeLightPosLoc, planeLightColor, planeViewPos;

// sphere data
// -----------
unsigned int sphereVao, sphereVbo, sphereEbo;
GLuint sphereNumIndices;
GLsizeiptr sphereSizeVertices, sphereSizeIndices;
unsigned int sphereProgram;
unsigned int sphereMvpLoc, sphereMvLoc, sphereModelLoc, sphereLightPosLoc, sphereLightColor, sphereViewPos;


// cube data
// ---------

GLuint cubeVbo, cubeEbo;
GLuint cubeNumIndices;
GLsizeiptr cubeSizeVertices, cubeSizeIndices;


// arrow data
// ---------

GLuint arrowVao, arrowVbo, arrowEbo;
GLuint arrowNumIndices;
GLsizeiptr arrowSizeVertices, arrowSizeIndices;

glm::vec3 planePos(0.0f, -0.0f, -0.0f);

glm::vec3 cubePositions[] = {
	   glm::vec3(13.0f,  -10.0f,  -12.0f),
	   glm::vec3(2.0f,  -5.0f, -15.0f),
	   glm::vec3(-1.5f, -12.2f, -12.5f),
	   glm::vec3(-3.8f, -2.0f, -12.3f),
	   glm::vec3(2.4f, -0.4f, -3.5f),
	   glm::vec3(-1.7f,  3.0f, -7.5f),
	   glm::vec3(1.3f, -2.0f, -2.5f),
	   glm::vec3(2.5f,  2.0f, -2.5f),
	   glm::vec3(1.5f,  0.2f, -1.5f),
	   glm::vec3(-1.3f,  1.0f, -1.5f),


	   glm::vec3(-2.5f,  1.7f, -1.8f),
	   glm::vec3(-3.0f,  -1.7f, -2.8f)
};

glm::vec3 spherePositions[] = {
	   glm::vec3(-5.0f,  4.0f,  -8.0f),
	   glm::vec3(-12.0f,  -5.0f, -5.0f),
	   glm::vec3(-1.5f, 12.2f, -10.5f)
};

glm::vec3 arrowPositions[] = {
	   glm::vec3(5.0f,  -4.0f,  -2.0f),
	   glm::vec3(-4.0f,  -2.0f, -10.0f),
	   glm::vec3(-1.5f, 4.2f, -7.5f)
};

// triangle data
// -------------
GLfloat verts[] =
{

	+0.5f, +0.5f,
	+0.2f, +0.1f, +0.7f,
	-0.5f, +0.5f,
	+0.2f, +0.1f, +0.7f,
	-0.5f, -0.5f,
	+0.2f, +0.1f, +0.7f,
	+0.5f, -0.5f,
	+0.2f, +0.1f, +0.7f,

	0.1f, +0.1f,
	+0.8f, +0.2f, +0.0f,
	-0.2f, +0.3f,
	+0.8f, +0.2f, +0.0f,
	-0.25f, +0.1f,
	+0.8f, +0.2f, +0.0f,


};

GLushort indices[] = { 0,1,2, 2,3,0, 4,5,6 };


#define PR_DEBUG

#ifdef PR_DEBUG
#define ASSERT(x) \
  if (!(x))       \
  __debugbreak()
#define GLCall(x) \
  GLClearError(); \
  x;              \
  ASSERT(GLLogCall(#x, __FILE__, __LINE__));
#else
#define GLCall(x) x
#endif

static void GLClearError();
static bool GLLogCall(const char* function, const char* file, int line);

void sendDataToOpenGL()
{

	glGenVertexArrays(1, &triangleVao);
	glGenVertexArrays(1, &cubeVao);
	glGenVertexArrays(1, &planeVao);
	glGenVertexArrays(1, &sphereVao);
	glGenVertexArrays(1, &arrowVao);


	// Cube Data
	// =========

	ShapeData cube = ShapeGenerator::makeCube();

	glGenBuffers(1, &cubeVbo);
	glGenBuffers(1, &cubeEbo);

	glBindVertexArray(cubeVao);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVbo);
	glBufferData(GL_ARRAY_BUFFER, cube.vertexBufferSize(), cube.vertices, GL_STATIC_DRAW);

	// set vao attributes
	// vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	// color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 3));
	// normal
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 6));

	// set indices data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube.indexBufferSize(), cube.indices, GL_STATIC_DRAW);

	cubeNumIndices = cube.numIndices;
	cubeSizeVertices = cube.vertexBufferSize();
	cubeSizeIndices = cube.indexBufferSize();
	cube.cleanup();

	//// Triangle Data 
	//// =============
	//glBindVertexArray(triangleVao);

	//// set vertices data
	//GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID)); 
	//GLCall(glBufferSubData(GL_ARRAY_BUFFER, sizeVertices + sizeIndices, sizeof(verts), &verts));

	//// set indices data
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBufferID);
	//glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeVertices + sizeIndices + sizeof(verts), sizeof(indices), &indices);

	//// set vao attribute
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (char*)(sizeVertices + sizeIndices));
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (char*)(sizeVertices + sizeIndices + sizeof(float) * 2));


	// Plane Data
	// ==========

	ShapeData plane = ShapeGenerator::makePlane(2);

	glGenBuffers(1, &vertexBufferID);
	glGenBuffers(1, &indexBufferID);

	glBindVertexArray(planeVao);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, plane.vertexBufferSize(), plane.vertices, GL_STATIC_DRAW);

	// set vao attributes
	// vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	// color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 3));
	// normal
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 6));

	// set indices data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, plane.indexBufferSize(), plane.indices, GL_STATIC_DRAW);

	numIndices = plane.numIndices;
	sizeVertices = plane.vertexBufferSize();
	sizeIndices = plane.indexBufferSize();
	plane.cleanup();


	// Sphere data
	// ===========

	ShapeData sphere = ShapeGenerator::makeSphere(160);

	glGenBuffers(1, &sphereVbo);
	glGenBuffers(1, &sphereEbo);

	glBindVertexArray(sphereVao);

	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo);
	glBufferData(GL_ARRAY_BUFFER, sphere.vertexBufferSize(), sphere.vertices, GL_STATIC_DRAW);

	// set vao attributes
	// vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	// color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 3));
	// normal
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 6));

	// set indices data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.indexBufferSize(), sphere.indices, GL_STATIC_DRAW);

	sphereNumIndices = sphere.numIndices;
	sphereSizeVertices = sphere.vertexBufferSize();
	sphereSizeIndices = sphere.indexBufferSize();
	sphere.cleanup();

	// Arrow data
	// ===========

	ShapeData arrow = ShapeGenerator::makeArrow();

	glGenBuffers(1, &arrowVbo);
	glGenBuffers(1, &arrowEbo);

	glBindVertexArray(arrowVao);

	glBindBuffer(GL_ARRAY_BUFFER, arrowVbo);
	glBufferData(GL_ARRAY_BUFFER, arrow.vertexBufferSize(), arrow.vertices, GL_STATIC_DRAW);

	// set vao attributes
	// vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	// color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 3));
	// normal
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (char*)(sizeof(float) * 6));

	// set indices data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, arrowEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, arrow.indexBufferSize(), arrow.indices, GL_STATIC_DRAW);

	arrowNumIndices = arrow.numIndices;
	arrowSizeVertices = arrow.vertexBufferSize();
	arrowSizeIndices = arrow.indexBufferSize();
	arrow.cleanup();
	

}

void MeGlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());
	glClearColor(+1.0f, +0.75f, 0.8f, 1.0f);

	glUseProgram(planeProgram);
	glBindVertexArray(planeVao);

	mat4 model = glm::translate(mat4(), planePos);
	model = model * glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 3.0f, 3.0f));

	
	mat4 viewToProjectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.1f, 20.0f);
	mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

	mat4 modelToProjectionMatrix = worldToProjectionMatrix * model;
	//mat4 planeModelToWorldMatrix = viewToProjectionMatrix * model;

	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(planeModelLoc, 1, GL_FALSE, &model[0][0]);

	glUniform3fv(planeLightPosLoc, 1, &lightPos[0]);
	vec3 camPos = camera.getPosition();
	glUniform3fv(planeViewPos, 1, &camPos[0]);
	vec3 lightColor = vec3(1.0f, 0.0f, 0.0f);
	glUniform3fv(planeLightColor, 1, &lightColor[0]);

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);

	// Draw cubes
	// ----------
	model = glm::translate(glm::mat4(1.0f), vec3(3.0f, 1.0f, 5.0f)) *
		glm::rotate(model, -30.0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(model, -45.0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.2f, 0.3f));

	modelToProjectionMatrix = worldToProjectionMatrix * model;

	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(planeModelLoc, 1, GL_FALSE, &model[0][0]);

	glBindVertexArray(cubeVao);
	glDrawElements(GL_TRIANGLES, cubeNumIndices, GL_UNSIGNED_SHORT, 0);

	model = glm::translate(mat4(), vec3(-3.0f, 1.0f, 2.0f));
	model = glm::rotate(model, glm::radians(104.0f), glm::vec3(1.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(40.0f), glm::vec3(1.0f, 0.0f, 1.0f));
	model = model * glm::scale(glm::mat4(1.0f), glm::vec3(0.24f, 0.5f, 0.37f));

	modelToProjectionMatrix = worldToProjectionMatrix * model;

	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	//glUniformMatrix4fv(mvLoc, 1, GL_FALSE, &planeModelToWorldMatrix[0][0]);
	glUniformMatrix4fv(planeModelLoc, 1, GL_FALSE, &model[0][0]);

	//glBindVertexArray(cubeVao);
	glDrawElements(GL_TRIANGLES, cubeNumIndices, GL_UNSIGNED_SHORT, 0);

	for (unsigned int i = 0; i < 12; i++)
	{
		// calculate the model matrix for each object and pass it to shader before drawing
		// translation * rotation * scale
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
		float scaleValue = (i + 1) * 0.03f;
		model = model * glm::scale(glm::mat4(1.0f), glm::vec3(scaleValue, scaleValue, scaleValue));

		modelToProjectionMatrix = worldToProjectionMatrix * model;
		//planeModelToWorldMatrix = viewToProjectionMatrix * model;

		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
		//glUniformMatrix4fv(mvLoc, 1, GL_FALSE, &planeModelToWorldMatrix[0][0]);
		glUniformMatrix4fv(planeModelLoc, 1, GL_FALSE, &model[0][0]);

		glDrawElements(GL_TRIANGLES, cubeNumIndices, GL_UNSIGNED_SHORT, 0);
	}

	

	// Draw Sphere
	// -----------

	glBindVertexArray(sphereVao);

	for (unsigned int i = 0; i < 3; i++)
	{

		model = glm::translate(mat4(), spherePositions[i]);
		float scaleValue = (i + 5) * 0.2f;
		model = model * glm::scale(glm::mat4(1.0f), glm::vec3(scaleValue, scaleValue, scaleValue));

		modelToProjectionMatrix = worldToProjectionMatrix * model;

		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
		glUniformMatrix4fv(planeModelLoc, 1, GL_FALSE, &model[0][0]);

		glDrawElements(GL_TRIANGLES, sphereNumIndices, GL_UNSIGNED_SHORT, 0);

	}

	// Draw arrow
	// -----------

	glBindVertexArray(arrowVao);

	for (unsigned int i = 0; i < 3; i++)
	{

		model = glm::translate(mat4(), arrowPositions[i]);
		float scaleValue = (i + 5) * 0.2f;
		model = model * glm::scale(glm::mat4(1.0f), glm::vec3(scaleValue, scaleValue, scaleValue));

		modelToProjectionMatrix = worldToProjectionMatrix * model;

		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
		glUniformMatrix4fv(planeModelLoc, 1, GL_FALSE, &model[0][0]);

		glDrawElements(GL_TRIANGLES, arrowNumIndices, GL_UNSIGNED_SHORT, 0);

	}
	
	// Pass through shader to draw a cube

	glBindVertexArray(cubeVao);
	mat4 cubeModelToWorldMatrix =
		glm::translate(mat4(), lightPos) *
		glm::scale(glm::mat4(1.0f), vec3(0.1f, 0.1f, 0.1f));
	modelToProjectionMatrix = worldToProjectionMatrix * cubeModelToWorldMatrix;
	glUseProgram(passTroughProgram);
	glUniformMatrix4fv(passTroughMvp, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, cubeNumIndices, GL_UNSIGNED_SHORT, 0);

	//glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, (GLvoid*)sizeVertices);

	//// Draw cube
	//// =========

	//// tell opengl which program and vao to use to render
	//glUseProgram(cubeProgram);
	//glBindVertexArray(cubeVao);

	//mat4 model = glm::translate(mat4(), cubePos);
	//model = glm::rotate(model, glm::radians(xoffset), glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(yoffset), glm::vec3(1.0f, 0.0f, 0.0f));

	//glm::mat4 view = glm::mat4(1.0f);
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	//mat4 projection = glm::perspective(45.0f, ((float)width()) / height(), 0.1f, 100.0f);


	//if (drawCube) {
	//	model = model * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	//}
	//else {
	//	model = model * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.001f));
	//}

	//// set the value in shader
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
	//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

	//glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, (GLvoid*)sizeVertices);

	//// Draw multiple cubes
	//// -------------------

	//for (unsigned int i = 0; i < 12; i++)
	//{
	//	// calculate the model matrix for each object and pass it to shader before drawing
	//	// translation * rotation * scale
	//	model = glm::mat4(1.0f);
	//	model = glm::translate(model, cubePositions[i]);
	//	float angle = 20.0f * i;
	//	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
	//	float scaleValue = (i + 1) * 0.03f;
	//	model = model * glm::scale(glm::mat4(1.0f), glm::vec3(scaleValue, scaleValue, scaleValue));
	//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

	//	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, (GLvoid*)sizeVertices);

	//}
	//
	//// Draw traingle
	//// =============

	//// tell opengl which program and vao to use to render
	//glUseProgram(triangleProgram);
	//glBindVertexArray(triangleVao);

	//// create transformations -> translation * rotation * scale
	//glm::mat4 transform = glm::mat4(1.0f);
	//transform = glm::translate(transform, vec3(0.9f, -0.9f, 0.0f));
	//transform = transform * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
	//glm::vec3 inputColor(0.1f, 0.2f, 0.2f);

	//// set the value in shader
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	//glUniform3f(colorLoc, inputColor[0], inputColor[1], inputColor[2]);

	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLvoid*)(sizeVertices + sizeIndices + 140));

	//// second triangle
	//// ---------------
	//transform = glm::mat4(1.0f);
	//transform = glm::translate(transform, vec3(-0.9f, 0.8f, 0.0f));
	//transform = transform * glm::scale(glm::mat4(1.0f), glm::vec3(0.15f, 0.25f, 0.5f));
	//inputColor = glm::vec3(0.5f, 0.1f, 0.2f);
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	//glUniform3f(colorLoc, inputColor[0], inputColor[1], inputColor[2]);
	//glUseProgram(triangleProgram);

	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLvoid*)(sizeVertices + sizeIndices + 146));

	//// third triangle
	//// ---------------
	//transform = glm::mat4(1.0f);
	//transform = glm::translate(transform, vec3(-1.0f, -1.0f, 0.0f));
	//transform = transform * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
	//inputColor = glm::vec3(0.18f, 0.78f, 0.28f);
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	//glUniform3f(colorLoc, inputColor[0], inputColor[1], inputColor[2]);
	//glUseProgram(triangleProgram);

	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLvoid*)(sizeVertices + sizeIndices + 146));

	//// first rectangle
	//// -------------
	//transform = glm::mat4(1.0f);
	//transform = glm::translate(transform, vec3(1.0f, 1.0f, 0.0f));
	//transform = transform * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
	//inputColor = glm::vec3(0.1f, 0.5f, 0.7f);
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	//glUniform3f(colorLoc, inputColor[0], inputColor[1], inputColor[2]);
	//glUseProgram(triangleProgram);

	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (GLvoid*)(sizeVertices + sizeIndices + 140));
}

void installShadersParser()
{
	// Load cube shader
	// ================
	ShaderParser::ShaderProgramSource source = ShaderParser::ParseShader("cube.shader");

	GLuint vertexShaderID = ShaderParser::CompileShader(GL_VERTEX_SHADER, source.VertexSource);
	GLuint fragmentShaderID = ShaderParser::CompileShader(GL_FRAGMENT_SHADER, source.FragmentSource);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	// attach to cube shader to cubeProgram
	cubeProgram = glCreateProgram();
	glAttachShader(cubeProgram, vertexShaderID);
	glAttachShader(cubeProgram, fragmentShaderID);
	glLinkProgram(cubeProgram);
	glValidateProgram(cubeProgram);

	// find the location of uniform variable in shader	
	modelLoc = glGetUniformLocation(cubeProgram, "model");
	viewLoc = glGetUniformLocation(cubeProgram, "view");
	projectionLoc = glGetUniformLocation(cubeProgram, "projection");

	// Load pass through shader
	// ========================
	source = ShaderParser::ParseShader("passThrough.shader");

	vertexShaderID = ShaderParser::CompileShader(GL_VERTEX_SHADER, source.VertexSource);
	fragmentShaderID = ShaderParser::CompileShader(GL_FRAGMENT_SHADER, source.FragmentSource);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	// attach to triangle shader to triangleProgram
	passTroughProgram = glCreateProgram();
	glAttachShader(passTroughProgram, vertexShaderID);
	glAttachShader(passTroughProgram, fragmentShaderID);
	glLinkProgram(passTroughProgram);
	glValidateProgram(passTroughProgram);

	// find the location of uniform variable in shader
	passTroughMvp = glGetUniformLocation(passTroughProgram, "mvpMatrix");
	//colorLoc = glGetUniformLocation(triangleProgram, "color");

	// Load plane shader
	// ================
	source = ShaderParser::ParseShader("plane.shader");

	vertexShaderID = ShaderParser::CompileShader(GL_VERTEX_SHADER, source.VertexSource);
	fragmentShaderID = ShaderParser::CompileShader(GL_FRAGMENT_SHADER, source.FragmentSource);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	// attach to cube shader to cubeProgram
	planeProgram = glCreateProgram();
	glAttachShader(planeProgram, vertexShaderID);
	glAttachShader(planeProgram, fragmentShaderID);
	glLinkProgram(planeProgram);
	glValidateProgram(planeProgram);

	// find the location of uniform variable in shader	
	mvpLoc = glGetUniformLocation(planeProgram, "mvpMatrix");
	//mvLoc = glGetUniformLocation(planeProgram, "mvMatrix");
	planeModelLoc = glGetUniformLocation(planeProgram, "model");
	planeLightPosLoc = glGetUniformLocation(planeProgram, "lightPos");
	planeLightColor = glGetUniformLocation(planeProgram, "lightColor");
	planeViewPos = glGetUniformLocation(planeProgram, "viewPos"); 
}

void MeGlWindow::initializeGL()
{
	glewInit();
	glEnable(GL_DEPTH_TEST);
	sendDataToOpenGL();
	installShadersParser();
}


void MeGlWindow::mouseMoveEvent(QMouseEvent* e)
{
	setFocus();
	camera.mouseUpdate(glm::vec2(e->x(), e->y()));
	repaint();
}

void MeGlWindow::keyPressEvent(QKeyEvent* e)
{
	switch (e->key())
	{
	case Qt::Key::Key_W:
		//planePos += glm::vec3(0.0f, moveSpeed, 0.0f);
		camera.moveUp();
		break;
	case Qt::Key::Key_A:
		//camera.moveUp();
		camera.strafeLeft();
		break;
	case Qt::Key::Key_S:
		//planePos += glm::vec3(0.0f, -moveSpeed, 0.0f);
		camera.moveDown();
		break;
	case Qt::Key::Key_D:
		//planePos += glm::vec3(moveSpeed, 0.0f, 0.0f);
		//camera.moveDown();
		camera.strafeRight();
		break;
	case Qt::Key::Key_R:
		//planePos += glm::vec3(moveSpeed, 0.0f, 0.0f);
		//camera.moveDown();
		camera.moveForward();
		break;
	case Qt::Key::Key_F:
		//planePos += glm::vec3(moveSpeed, 0.0f, 0.0f);
		//camera.moveDown();
		camera.moveBackward();
		break;
	case Qt::Key::Key_Up:
		//yoffset += rotateDegree;
		lightPos += glm::vec3(0.0f, 0.0f, moveSpeed);
		break;
	case Qt::Key::Key_Left:
		//xoffset -= rotateDegree;
		lightPos -= glm::vec3(moveSpeed, 0.0f, 0.0f);
		break;
	case Qt::Key::Key_Down:
		//yoffset -= rotateDegree;
		lightPos -= glm::vec3(0.0f, 0.0f, moveSpeed);
		break;
	case Qt::Key::Key_Right:
		//xoffset += rotateDegree;
		lightPos += glm::vec3(moveSpeed, 0.0f, 0.0f);
		break;
	case Qt::Key::Key_Space:
		drawCube = !drawCube;
		break;
	}

	repaint();
}

static void GLClearError()
{

	while (glGetError() != GL_NO_ERROR)
		;
}

static bool GLLogCall(const char* function, const char* file, int line)
{

	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ")" << function << " " << file << ":" << line << std::endl;
		return false;
	}

	return true;
}