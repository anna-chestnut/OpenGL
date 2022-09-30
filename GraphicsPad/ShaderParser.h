#pragma once
#include <string>

class ShaderParser
{
public:
	struct ShaderProgramSource
	{

		std::string VertexSource;
		std::string FragmentSource;
	};
	static ShaderProgramSource ParseShader(const std::string& filepath);
	static unsigned int CompileShader(unsigned int type, const std::string& source);

};
