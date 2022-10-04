#include <gl\glew.h>
#include <MeGlWindow.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <assert.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <QtGui\qkeyevent>

#include "ShaderParser.h"

GLuint traingleVbo, triangleEbo, triangleVao;
unsigned int triangleProgram;

glm::vec3 blueOffset(-0.5f, 0.5f, 0.0f);
glm::vec3 redOffset(0.5f, -0.5f, 0.0f);
float moveSpeed = 0.1f;

unsigned int transformLoc, colorLoc;

void genBuffers()
{
	glGenVertexArrays(1, &triangleVao);
	glGenBuffers(1, &traingleVbo);
	//glGenBuffers(1, &triangleEbo);

}

void bindBuffersAndSetAttribute()
{
	glBindVertexArray(triangleVao);

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

	glBindBuffer(GL_ARRAY_BUFFER, traingleVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts),
		verts, GL_STATIC_DRAW);

	GLushort indices[] = { 0,1,2, 2,3,0, 4,5,6 };

	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(verts), sizeof(indices), &indices);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, traingleVbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
		indices, GL_STATIC_DRAW);


	// set vao attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (char*)(sizeof(float) * 2));
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 3, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(GL_UNSIGNED_SHORT) * 9, (char*)sizeof(verts));
}


void sendDataToOpenGL()
{
	genBuffers();
	bindBuffersAndSetAttribute();
}

void installShaders()
{
	ShaderParser::ShaderProgramSource source = ShaderParser::ParseShader("triangle.shader");

	GLuint vertexShaderID = ShaderParser::CompileShader(GL_VERTEX_SHADER, source.VertexSource);
	GLuint fragmentShaderID = ShaderParser::CompileShader(GL_FRAGMENT_SHADER, source.FragmentSource);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	triangleProgram = glCreateProgram();
	glAttachShader(triangleProgram, vertexShaderID);
	glAttachShader(triangleProgram, fragmentShaderID);
	glLinkProgram(triangleProgram);
	glValidateProgram(triangleProgram);

	transformLoc = glGetUniformLocation(triangleProgram, "transform");
	colorLoc = glGetUniformLocation(triangleProgram, "color");
}

void MeGlWindow::initializeGL()
{
	glewInit();
	sendDataToOpenGL();
	installShaders();
}

void MeGlWindow::keyPressEvent(QKeyEvent* e)
{
	switch (e->key())
	{
	case Qt::Key::Key_W:
		blueOffset += glm::vec3(0.0f, moveSpeed, 0.0f);
		break;
	case Qt::Key::Key_A:
		blueOffset += glm::vec3(-moveSpeed, 0.0f, 0.0f);
		break;
	case Qt::Key::Key_S:
		blueOffset += glm::vec3(0.0f, -moveSpeed, 0.0f);
		break;
	case Qt::Key::Key_D:
		blueOffset += glm::vec3(moveSpeed, 0.0f, 0.0f);
		break;
	case Qt::Key::Key_Up:
		redOffset += glm::vec3(0.0f, moveSpeed, 0.0f);
		break;
	case Qt::Key::Key_Left:
		redOffset += glm::vec3(-moveSpeed, 0.0f, 0.0f);
		break;
	case Qt::Key::Key_Down:
		redOffset += glm::vec3(0.0f, -moveSpeed, 0.0f);
		break;
	case Qt::Key::Key_Right:
		redOffset += glm::vec3(moveSpeed, 0.0f, 0.0f);
		break;
	}

	repaint();
}

void MeGlWindow::paintGL()
{
	glViewport(0, 0, width(), height());

	glClear(GL_COLOR_BUFFER_BIT);

	// first triangle
	// --------------

	// create transformations
	glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	transform = glm::translate(transform, blueOffset);
	transform = transform * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
	
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	glm::vec3 inputColor(0.1f, 0.5f, 0.2f);
	glUniform3f(colorLoc, inputColor[0], inputColor[1], inputColor[2]);

	glUseProgram(triangleProgram);
	glBindVertexArray(triangleVao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);

	// second triangle
	// ---------------
	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, redOffset);
	transform = transform * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
	inputColor = glm::vec3(0.5f, 0.1f, 0.2f);
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	glUniform3f(colorLoc, inputColor[0], inputColor[1], inputColor[2]);
	glUseProgram(triangleProgram);

	glBindVertexArray(triangleVao);
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLvoid*)6);
	glDrawArrays(GL_TRIANGLES, 1, 3);

	// another shape
	// -------------
	transform = glm::mat4(1.0f);
	transform = transform * glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 0.8f, 0.8f));
	inputColor = glm::vec3(0.5f, 0.2f, 0.8f);
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	glUniform3f(colorLoc, inputColor[0], inputColor[1], inputColor[2]);
	glUseProgram(triangleProgram);

	glBindVertexArray(triangleVao);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	glDrawArrays(GL_TRIANGLES, 4, 3);
}

