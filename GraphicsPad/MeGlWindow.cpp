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

// cube data
// ---------
GLuint numIndices;
GLsizeiptr sizeVertices, sizeIndices;
unsigned int cubeProgram;
float rotateDegree = 20.0f;
GLint modelLoc, viewLoc, projectionLoc;
float xoffset, yoffset;
float moveSpeed = 0.2f;
bool drawCube = true;

// triangle data
// -------------
unsigned int triangleProgram;
unsigned int triangleVao, cubeVao, planeVao;
unsigned int transformLoc, colorLoc;

unsigned int planeProgram;
unsigned int mvpLoc, mvLoc, planeModelLoc, planeLightPosLoc, planeLightColor, planeViewPos;



// cube data
// ---------

glm::vec3 planePos(0.0f, -0.0f, -0.0f);

glm::vec3 cubePositions[] = {
	   glm::vec3(3.0f,  0.0f,  -12.0f),
	   glm::vec3(2.0f,  5.0f, -15.0f),
	   glm::vec3(-1.5f, -2.2f, -2.5f),
	   glm::vec3(-3.8f, -2.0f, -12.3f),
	   glm::vec3(2.4f, -0.4f, -3.5f),
	   glm::vec3(-1.7f,  3.0f, -7.5f),
	   glm::vec3(1.3f, -2.0f, -2.5f),
	   glm::vec3(2.5f,  2.0f, -2.5f),
	   glm::vec3(1.5f,  0.2f, -1.5f),
	   glm::vec3(-1.3f,  1.0f, -1.5f),


	   glm::vec3(-2.5f,  1.7f, -1.8f),
	   glm::vec3(-3.0f,  -1.7f, -0.8f)
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


	//// Cube Data
	//// =========

	//ShapeData shape = ShapeGenerator::makeCube();

	//GLuint vertexBufferID;
	//glGenBuffers(1, &vertexBufferID);
	//glBindVertexArray(cubeVao);

	//glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

	//// initalize the whole buffer * need to include the size of triangle data *
	//glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize() + shape.indexBufferSize() + sizeof(verts) + sizeof(indices), shape.vertices, GL_STATIC_DRAW);

	//// set vertices data
	//glBufferSubData(GL_ARRAY_BUFFER, 0, shape.vertexBufferSize(), shape.vertices);

	//// set vao attributes
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) * 3));

	//// set indices data
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBufferID);
	//glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, shape.vertexBufferSize(), shape.indexBufferSize(), shape.indices);

	//numIndices = shape.numIndices;
	//sizeVertices = shape.vertexBufferSize();
	//sizeIndices = shape.indexBufferSize();

	//shape.cleanup();

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

	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);

	glBindVertexArray(planeVao);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

	// initalize the whole buffer * need to include the size of triangle data *
	glBufferData(GL_ARRAY_BUFFER, plane.vertexBufferSize() + plane.indexBufferSize(), plane.vertices, GL_STATIC_DRAW);

	// set vertices data
	glBufferSubData(GL_ARRAY_BUFFER, 0, plane.vertexBufferSize(), plane.vertices);

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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBufferID);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, plane.vertexBufferSize(), plane.indexBufferSize(), plane.indices);

	numIndices = plane.numIndices;
	sizeVertices = plane.vertexBufferSize();
	sizeIndices = plane.indexBufferSize();
	plane.cleanup();

}

void MeGlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());
	//glClearColor(+1.0f, +0.75f, 0.8f, 1.0f);

	glUseProgram(planeProgram);
	glBindVertexArray(planeVao);

	//glUseProgram(programID);
	//GLint ambientLightUniformLocation = glGetUniformLocation(programID, "ambientLight");
	//vec4 ambientLight(0.05f, 0.05f, 0.05f, 1.0f);
	//glUniform4fv(ambientLightUniformLocation, 1, &ambientLight[0]);
	//GLint eyePositionWorldUniformLocation = glGetUniformLocation(programID, "eyePositionWorld");
	//glm::vec3 eyePosition = camera.getPosition();
	//glUniform3fv(eyePositionWorldUniformLocation, 1, &eyePosition[0]);
	//GLint lightPositionUniformLocation = glGetUniformLocation(programID, "lightPositionWorld");
	//glUniform3fv(lightPositionUniformLocation, 1, &lightPositionWorld[0]);

	//fullTransformationUniformLocation = glGetUniformLocation(programID, "modelToProjectionMatrix");
	//modelToWorldMatrixUniformLocation = glGetUniformLocation(programID, "modelToWorldMatrix");

	mat4 model = glm::translate(mat4(), planePos);
	model = model * glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 3.0f, 3.0f));
	//model = glm::rotate(model, glm::radians(xoffset), glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(yoffset), glm::vec3(1.0f, 0.0f, 0.0f));

	mat4 modelToProjectionMatrix;
	mat4 viewToProjectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.1f, 20.0f);
	mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

	modelToProjectionMatrix = worldToProjectionMatrix * model;
	//// set the value in shader
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
	//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
	mat4 planeModelToWorldMatrix = viewToProjectionMatrix * model;

	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, &planeModelToWorldMatrix[0][0]);
	glUniformMatrix4fv(planeModelLoc, 1, GL_FALSE, &model[0][0]);

	glUniform3fv(planeLightPosLoc, 1, &lightPos[0]);
	vec3 camPos = camera.getPosition();
	glUniform3fv(planeViewPos, 1, &camPos[0]);
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	glUniform3fv(planeLightColor, 1, &lightColor[0]);

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, (GLvoid*)sizeVertices);


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

	// Load triangle shader
	// ====================
	source = ShaderParser::ParseShader("triangle.shader");

	vertexShaderID = ShaderParser::CompileShader(GL_VERTEX_SHADER, source.VertexSource);
	fragmentShaderID = ShaderParser::CompileShader(GL_FRAGMENT_SHADER, source.FragmentSource);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	// attach to triangle shader to triangleProgram
	triangleProgram = glCreateProgram();
	glAttachShader(triangleProgram, vertexShaderID);
	glAttachShader(triangleProgram, fragmentShaderID);
	glLinkProgram(triangleProgram);
	glValidateProgram(triangleProgram);

	// find the location of uniform variable in shader
	transformLoc = glGetUniformLocation(triangleProgram, "transform");
	colorLoc = glGetUniformLocation(triangleProgram, "color");

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
	mvLoc = glGetUniformLocation(planeProgram, "mvMatrix");
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
		planePos += glm::vec3(0.0f, moveSpeed, 0.0f);
		break;
	case Qt::Key::Key_A:
		planePos += glm::vec3(-moveSpeed, 0.0f, 0.0f);
		break;
	case Qt::Key::Key_S:
		planePos += glm::vec3(0.0f, -moveSpeed, 0.0f);
		break;
	case Qt::Key::Key_D:
		planePos += glm::vec3(moveSpeed, 0.0f, 0.0f);
		break;
	case Qt::Key::Key_Up:
		yoffset += rotateDegree;
		break;
	case Qt::Key::Key_Left:
		xoffset -= rotateDegree;
		break;
	case Qt::Key::Key_Down:
		yoffset -= rotateDegree;
		break;
	case Qt::Key::Key_Right:
		xoffset += rotateDegree;
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