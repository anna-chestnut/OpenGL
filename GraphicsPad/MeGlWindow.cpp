#include <gl\glew.h>
#include <MeGlWindow.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <assert.h>
#include <iostream>

extern const char* vertexShaderCode;
extern const char* fragmentShaderCode;

struct ShaderProgramSource
{

	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{

	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{

		if (line.find("#shader") != std::string::npos)
		{

			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{

			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{

	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "verttex" : "fragment")
			<< " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

void sendDataToOpenGL()
{
	GLfloat verts[] =
	{
		-0.1f, +0.1f,
		+0.8f, +0.2f, +0.0f,
		-0.2f, +0.3f,
		+0.8f, +0.2f, +0.0f,
		-0.25f, +0.1f,
		+0.8f, +0.2f, +0.0f,

		+0.5f, +0.5f,
		+0.2f, +0.1f, +0.7f,
		-0.5f, +0.5f,
		+0.2f, +0.1f, +0.7f,
		-0.5f, -0.5f,
		+0.2f, +0.1f, +0.7f,
		+0.5f, -0.5f,
		+0.2f, +0.1f, +0.7f,

	};
	GLuint myBufferID;
	glGenBuffers(1, &myBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, myBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts),
		verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (char*)(sizeof(float) * 2));

	GLushort indices[] = { 0,1,2, 2,3,0, 4,5,6 };
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
		indices, GL_STATIC_DRAW);
}

void installShaders()
{
	ShaderProgramSource source = ParseShader("triangle.shader");

	GLuint vertexShaderID = CompileShader(GL_VERTEX_SHADER, source.VertexSource);//glCreateShader(GL_VERTEX_SHADER);//
	GLuint fragmentShaderID = CompileShader(GL_FRAGMENT_SHADER, source.FragmentSource);//glCreateShader(GL_FRAGMENT_SHADER);//C

	/*const char* adapter[1];
	adapter[0] = vertexShaderCode;
	glShaderSource(vertexShaderID, 1, adapter, 0);
	adapter[0] = fragmentShaderCode;
	glShaderSource(fragmentShaderID, 1, adapter, 0);*/

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);
	glValidateProgram(programID);

	glUseProgram(programID);
}

void MeGlWindow::initializeGL()
{
	glewInit();
	sendDataToOpenGL();
	installShaders();
}

void MeGlWindow::paintGL()
{
	glViewport(0, 0, width(), height());
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}

